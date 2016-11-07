/*--------------------------------------------------
OV7670����ԭ��:
��Դ(DOVDD,AVDD)Ϊ2.5V����(HT7333�����1N4148��ѹ),
DVDD����OV7670�ڲ���LDO(1.8V),�ⲿֻ���һֻ����.
��Ƭ��ΪSTM32F103CBT6������HSIʱ��,PLL��Ƶ��64MHz,��ʡȥ�ⲿ����.

����ֻ��Ҫ����Ҷ�ͼ��,OV7670����ΪYUV��ʽ,Y��ǰ,200*100����,
TIM4_CH1�Ĳ����Ƶ����Ϊ2��Ƶ,�Ϳ���ֻ�ɼ�Y�����ȣ�.

TIM3_CH3����XCLK,(16MHz��ӦVSYNCԼ20HZ,ʵ�����Լ8MHZ)
TIM4_CH1���PCLK(4MHz),�½��ز���,�˲�Ϊ1,����Ƶ,����DMA,�ɼ�PA0~PA7������������DMA_Buf.
TIM3_CH4���VSYNC,�½��ز����ж�,����DMA�Ĺ��뿪.
HREF����.

��������:
PA12:			LED
PA0~7:			OV7670_D0~D7
PB0(TIM3_CH3):	OV7670_XCLK
PB1(TIM3_CH4):	OV7670_VSYNC
PB2:			OV7670_RST
PB7(TIM4_CH2):	OV7670_HREF(��ʡ��)
PB6(TIM4_CH1):	OV7670_PCLK
PB10:			OV7670_SIO_C
PB11:			OV7670_SIO_D
PB12:			OV7670_PWDN

����:
	Ϊ�ӿ��ٶ�,SIO_C��SIO_D�ɼ��ⲿ��������

ע��:
	������SCCBʱ��,ò����IIC��������,ָ����ַ������ʱ:
	OV7670:	...IICWByte(add);IICACK(0);IICEnd();IICStart();...
	��ͨIIC:...IICWByte(add);IICACK(0);         IICStart();...

ͼ����
�ϴ�ͼ���в�����(��STEP���)��������MID_Buf���ڵ�ǰ�ɼ���
ͼ���б�����Ѱ�Ҳ�ֵ��С��λ�ã��Ӷ��õ�����������

----------------------------------------------------*/
//--------------------------------------------------
//		ͷ�ļ�
//--------------------------------------------------
//#include "stm32f10x.h"
#include "includes.H"
#define UART1_SendByte(d)	UART_SendByte(USART1,d)

#define LED_H	IOSET(GPIOA->ODR,BIT(12))
#define LED_L	IOCLR(GPIOA->ODR,BIT(12))
#define LED_XOR	IOXOR(GPIOA->ODR,BIT(12))
//#define VSYNC_IN	(GPIOB->IDR&BIT(1))
//����ͷ�Ŀ����
#define CAMWIDTH	80
#define CAMHEIGHT	80

//�в�����Ŀ����
#define MIDWIDTH	(CAMWIDTH/2)
#define MIDHEIGHT	(CAMHEIGHT/2)

//�в������Ͻ�����
#define MIDX	((CAMWIDTH-MIDWIDTH)/2)
#define MIDY	((CAMHEIGHT-MIDHEIGHT)/2)
//ƥ��ʱ�����ؼ�����Ӵ�ɼӿ��ٶ�
#define STEP	5

#define DMA_BUFSIZE (CAMWIDTH*CAMHEIGHT)

U32	DMA_RCnt=0;//��DMAֹͣʱ��ͳ��DMA�ɼ���������
U8 	DMA_Buf[CAMHEIGHT][CAMWIDTH];//�ɼ�������ͷ����
//U8 	MID_Buf[MIDHEIGHT][MIDWIDTH];//�в����������
U8 	MID_Buf[(MIDHEIGHT/STEP)*(MIDWIDTH/STEP)];//�в����������,����STEP����С�в����������

S16	MovIncX=0,MovIncY=0;//X����������Y��������
S16	MovX=0,MovY=0;//X����λ����Y����λ��

enum{
FLAG_BUSY=BIT(0),//�յ�����
FLAG_START=BIT(1),//�����������
FLAG_SAVE=BIT(2)
};
U16 mainflag=0;//��־

void SDA_MIN(void);
void SDA_MO(void);
//--------------------------------------------------
//--------------------------------------------------
//		����
//--------------------------------------------------

#include "IIC.C"
#include "OV7670.C"
#include "stm32f10x_it.c"

//void RCC_Configuration(void);
U8 uartflag=0;

//--------------------------------------------------
//SIO_D����Ϊ����
//--------------------------------------------------
void SDA_MIN(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = BIT(11);	      //SIO_D
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
}
//--------------------------------------------------
//SIO_D����Ϊ���
//--------------------------------------------------
void SDA_MO(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = BIT(11);	      //SIO_D
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

//--------------------------------------------------
//	����ʱ
//--------------------------------------------------
void delay(__IO uint32_t nCount)
{
	for(; nCount != 0; nCount--);
}
//--------------------------------------------------
//	΢����ʱ
//--------------------------------------------------
void delayus(U32 t)
{
	U32 i;
	for(;t!=0;t--)
	{
		for(i=8;i!=0;i--);
	}
}
//--------------------------------------------------
//	������ʱ
//--------------------------------------------------
void delayms(U32 t)
{
	for(;t!=0;t--)
	{
		delayus(1000);
	}
}
//--------------------------------------------------
//�����ֵ�ľ���ֵ
//--------------------------------------------------
__INLINE U8  caldif(U8 v1,U8 v2)
{
	if(v1>v2)return v1-v2;
	return v2-v1;
}
//--------------------------------------------------
//ͼ�������ϴ�ͼ���в�����ȥ�뵱ǰ�ɼ�ͼ�񣨴�x,y
//��ʼ��������Ѱ�Ҳ�ֵ��С��λ�ã�
//�Ӷ��õ�x������MovIncX����y������MovIncY��
//--------------------------------------------------
void PicProcess(void)
{
	U16 i,j,x,y;
	U32 dif,min=0xffffffff;	//��ֵ����Сֵ
	U16 minx,miny;			//��Сֵ�������Ͻ�����
	U8	*p1,*p2;			//Ϊ�ӿ���������ٶ�
	//--------����������ǰͼ�񣬲�����֮ǰ�в�ͼ�������С��λ��	
	for(y=0;y<CAMHEIGHT-MIDHEIGHT;y++)//���ϵ���
	{
		for(x=0;x<CAMWIDTH-MIDWIDTH;x++)//������
		{		
			dif=0;//�����0	
			p1=MID_Buf;	//p1ָ���ϴ�ͼ���в�
			for(i=0;i<MIDHEIGHT;i+=STEP)
			{
				p2=DMA_Buf[y+i]+x;//p2ָ���п�ʼ
				for(j=0;j<MIDWIDTH;j+=STEP)
				{//���㵱ǰͼ���еĵ����ϴ�ͼ���в�����ĵ�Ĳ�ֵ
					dif+=caldif(*p1,*p2);//����ۼ�	
					p1++;
					p2+=STEP;
				}
				//dif+=calrow1(DMA_Buf[y+i]+x,MID_Buf[i]);
			}
			if(min>dif)//���С�ڵ�ǰ��Сֵ�������СֵΪ��ǰֵ���Ҽ�������
			{
				min=dif;
				minx=x;
				miny=y;
			}
		}
	}
	//------------------------------������������
	if(min>30000)min=30000;//�˾��ʡ����Ϊ�˱��ڴ��ڷ��͸�ֵ
	if(min<15*(MIDHEIGHT/STEP)*(MIDWIDTH/STEP))//����ƽ��ÿ����15�����
	{
		MovIncX=minx-MIDX;//x�������з�������
		MovIncY=miny-MIDY;//y�������з�������
		MovX+=MovIncX;//ģ������ľ������꣬���ڲ鿴
		MovY+=MovIncY;	
		if(MovX<-1000)MovX=-1000;//�޷�
		if(MovX>1000)MovX=1000;	
		if(MovY<-1000)MovY=-1000;
		if(MovY>1000)MovY=1000;
	}	
	//------------------------------���Ƶ�ǰ�в�����
	p1=MID_Buf;	
	for(i=0;i<MIDHEIGHT;i+=STEP)
	{
		//memcpy(MID_Buf[i],DMA_Buf[MIDY+i]+MIDX,MIDWIDTH);
		p2=DMA_Buf[MIDY+i]+MIDX;
		for(j=0;j<MIDWIDTH;j+=STEP)
		{			
			*p1=*p2;
			p1++;
			p2+=STEP;
		}
	}
	//------------------------------���ڲ鿴���������
	UART1_SendByte(0xaa);
	UART1_SendByte(6);	
	UART1_SendByte(min>>8);
	UART1_SendByte(min);			
	UART1_SendByte(MovX>>8);
	UART1_SendByte(MovX);	
	UART1_SendByte(MovY>>8);
	UART1_SendByte(MovY);
}

/*
//��ά��������ٶȽϵ�
void PicProcess(void)
{
	U16 i,j,x,y;
	U32 dif,min=0xffffffff;	//��ֵ����Сֵ
	U16 minx,miny;			//��Сֵ�������Ͻ�����
	//--------����������ǰͼ�񣬲�����֮ǰ�в�ͼ�������С��λ��	
	for(y=0;y<CAMHEIGHT-MIDHEIGHT;y++)//���ϵ���
	{
		for(x=0;x<CAMWIDTH-MIDWIDTH;x++)//������
		{		
			dif=0;//�����0			
			for(i=0;i<MIDHEIGHT;i+=STEP)
			{
				for(j=0;j<MIDWIDTH;j+=STEP)
				{//���㵱ǰͼ���еĵ����ϴ�ͼ���в�����ĵ�Ĳ�ֵ
					dif+=caldif(DMA_Buf[y+i][x+j],MID_Buf[i][j]);//����ۼ�	
				}
			}
			if(min>dif)//���С�ڵ�ǰ��Сֵ�������СֵΪ��ǰֵ���Ҽ�������
			{
				min=dif;
				minx=x;
				miny=y;
			}
		}
	}
	//------------------------------������������
	if(min>30000)min=30000;//�˾��ʡ����Ϊ�˱��ڴ��ڷ��͸�ֵ
	if(min<15*(MIDHEIGHT/STEP)*(MIDWIDTH/STEP))//����ƽ��ÿ����15�����
	{
		MovIncX=minx-MIDX;//x����
		MovIncY=miny-MIDY;//y����
		MovX+=MovIncX;//ģ������ľ������꣬���ڲ鿴
		MovY+=MovIncY;	
		if(MovX<-1000)MovX=-1000;//�޷�
		if(MovX>1000)MovX=1000;	
		if(MovY<-1000)MovY=-1000;
		if(MovY>1000)MovY=1000;
	}	
	//------------------------------���Ƶ�ǰ�в�����
	for(i=0;i<MIDHEIGHT;i+=STEP)
	{
		//memcpy(MID_Buf[i],DMA_Buf[MIDY+i]+MIDX,MIDWIDTH);
		for(j=0;j<MIDWIDTH;j+=STEP)
		{
			MID_Buf[i][j]=DMA_Buf[MIDY+i][MIDX+j];			
		}
	}
	//------------------------------���ڲ鿴���������
	UART1_SendByte(0xaa);
	UART1_SendByte(6);	
	UART1_SendByte(min>>8);
	UART1_SendByte(min);			
	UART1_SendByte(MovX>>8);
	UART1_SendByte(MovX);	
	UART1_SendByte(MovY>>8);
	UART1_SendByte(MovY);
}*/
/*
//Ч��ͼ
void PicProcess_show(void)
{
	U16 i,j,x,y;
	U8	*p1,*p2;			//Ϊ�ӿ���������ٶ�
			
	for(y=0;y<CAMHEIGHT-MIDHEIGHT;y++)//���ϵ���
	{
		for(x=0;x<CAMWIDTH-MIDWIDTH;x++)//������
		{		
			p1=MID_Buf;	//p1ָ���ϴ�ͼ���в�	
			for(i=0;i<MIDHEIGHT;i+=STEP)
			{
				p2=DMA_Buf[y+i]+x;//p2ָ���п�ʼ
				for(j=0;j<MIDWIDTH;j+=STEP)
				{
					*p1=*p2;
					*p2=0;//����ƥ���
					p1++;
					p2+=STEP;
				}				
			}	
			p1=MID_Buf;
			p2=DMA_Buf[0];
			//����ͼ��
			UART1_SendByte(0xaa);//��ʼ1
			UART1_SendByte(0x55);//��ʼ2
			UART1_SendByte(DMA_RCnt>>8);//֡�����ֽ�
			UART1_SendByte(DMA_RCnt);//֡�����ֽ�
			for(i=0;i<DMA_RCnt;i++)
				UART1_SendByte(p2[i]);
			//�ָ�ͼ��
			for(i=0;i<MIDHEIGHT;i+=STEP)
			{
				p2=DMA_Buf[y+i]+x;//p2ָ���п�ʼ
				for(j=0;j<MIDWIDTH;j+=STEP)
				{
					*p2=*p1;
					p1++;
					p2+=STEP;					
				}				
			}				
		}		
	}
}*/

//--------------------------------------------------
//		��ʱ����ʼ��
//TIM3_CH3����XCLK:10M~48M 
//TIMʱ��Դ=32M,2��Ƶ�����PWMΪ16M
//3��Ƶ�����PWMΪ10.6M
//QCIF��ʽ��PCLKΪXCLK/4
//--------------------------------------------------
void Timer_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef	TIM_OCInitStructure;		
    TIM_ICInitTypeDef tim_icinit; 
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);//ʹ��RCCʱ��	
	TIM_DeInit(TIM3);
	TIM_TimeBaseStructure.TIM_Period= 1;			//�Զ���װֵ,Ϊ0ʱ������
	TIM_TimeBaseStructure.TIM_Prescaler=2-1;		//ʱ��Ƶ��Ԥ��Ƶ
	//���ϼ���:0~TIM_Period		���¼���:TIM_Period~0
	TIM_TimeBaseStructure.TIM_CounterMode= TIM_CounterMode_Up;	//���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_ClockDivision= TIM_CKD_DIV1;	//ʱ�ӷ�Ƶ����
	//TIM2_TimeBaseInit.TIM_RepetitionCounter= 0x0000;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);	
	//TIM_UpdateDisableConfig(TIM2,DISABLE);//�������
	TIM_InternalClockConfig(TIM3);//���ڲ��ṩʱ��.
	//TIM_PrescalerConfig(TIM2,1,TIM_PSCReloadMode_Immediate);//����װ��Ԥ��Ƶ
	//TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);//ʹ�ܸ����¼��ж�	
	//TIM_ITConfig(TIM2,TIM_IT_CC2,ENABLE);//ʹ��TIM2����\�Ƚ��ж�		
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//TIM_OCMode_Toggle// ����Ƚϴ���ģʽ  
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//ò�Ʊ�������Ϊ�������
	TIM_OCInitStructure.TIM_Pulse = 1;//ռ��ʱ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);//TIM3_CH3:����XCLK
	
	//-------------TIM3_CH4:���VSYNC
    tim_icinit.TIM_Channel = TIM_Channel_4;   
    tim_icinit.TIM_ICFilter = 0x00;      //Ӳ���˲�      
    tim_icinit.TIM_ICPolarity = TIM_ICPolarity_Falling;//��������ĵ�ƽ       
    tim_icinit.TIM_ICPrescaler= TIM_ICPSC_DIV1;  //ÿ�μ�⵽������ƽ������       
    tim_icinit.TIM_ICSelection = TIM_ICSelection_DirectTI;  //ͨ������ѡ��
    TIM_ICInit(TIM3,&tim_icinit);  //�����ʼ�� 
	TIM_ITConfig(TIM3,TIM_IT_CC4,ENABLE);
	
	TIM_Cmd(TIM3,ENABLE);	//ʹ�ܶ�ʱ��
	
	//------------------------------TIM4_CH1���ڲ���PCLK,�Ҵ���DMA	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);//ʹ��RCCʱ��	
	TIM_DeInit(TIM4);
	TIM_TimeBaseStructure.TIM_ClockDivision= TIM_CKD_DIV1;	//ʱ�ӷ�Ƶ����
	TIM_TimeBaseStructure.TIM_Prescaler= 1;		//ʱ��Ƶ��Ԥ��Ƶ
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);	
	TIM_InternalClockConfig(TIM4);//���ڲ��ṩʱ��.
	
    tim_icinit.TIM_Channel = TIM_Channel_1;   
    tim_icinit.TIM_ICFilter = 0x01;      //Ӳ���˲�,������(Ҫע��,��ȡ1~2)
	//OV7670ʱ����Ӧ�����ز�������,��Ϊ�ӿ��ٶ�,�������½���ȥ����DMA,Ҫע��ʹ��Ӳ���˲�TIM_ICFilter
    tim_icinit.TIM_ICPolarity =TIM_ICPolarity_Falling; //TIM_ICPolarity_Rising;//��������ĵ�ƽ       
    tim_icinit.TIM_ICPrescaler= TIM_ICPSC_DIV2;//TIM_ICPSC_DIV1;  //ÿ�μ�⵽������ƽ������       
    tim_icinit.TIM_ICSelection = TIM_ICSelection_DirectTI;  //ͨ������ѡ��
    TIM_ICInit(TIM4,&tim_icinit);  //��ʼ��  
	
    //���ö�ʱ����DMA  
    //TIM_DMAConfig(TIM4,TIM_DMABase_CCR1,TIM_DMABurstLength_1Byte);  
    //����DMA�����ź�
    TIM_DMACmd(TIM4, TIM_DMA_CC1, ENABLE); //PCLK
	
//	ADCEn=1;
}
//--------------------------------------------------
//����TIM4�����Ƶ������һ��PCLK��λ,���һֱ����ȥ
//--------------------------------------------------
void TIM4_ResetPsc(void)
{
    TIM_ICInitTypeDef tim_icinit; 
    tim_icinit.TIM_Channel = TIM_Channel_1;   
    tim_icinit.TIM_ICFilter = 0x01;      //Ӳ���˲�,������(Ҫע��,��ȡ1~2)
	//OV7670ʱ����Ӧ�����ز�������,��Ϊ�ӿ��ٶ�,�������½���ȥ����DMA,Ҫע��ʹ��Ӳ���˲�TIM_ICFilter
    tim_icinit.TIM_ICPolarity =TIM_ICPolarity_Falling; //TIM_ICPolarity_Rising;//��������ĵ�ƽ       
    tim_icinit.TIM_ICPrescaler=TIM_ICPSC_DIV2;  
    tim_icinit.TIM_ICSelection = TIM_ICSelection_DirectTI;  //ͨ������ѡ��
    TIM_ICInit(TIM4,&tim_icinit);  //��ʼ��	(�����Ƶ����������)
	/*U16 temp;	
	temp=TIM4->CCER;
	TIM4->CCER &= 0xfffe;// CCER_CC1E_Reset;
	TIM_SetIC1Prescaler(TIM4,TIM_ICPSC_DIV2);//����2��Ƶ
	TIM4->CCER=temp;	
	//���÷�Ƶ����:1,�رղ���;2,��Ƶ��Ϊ1;3,��Ƶ��Ϊ��Ҫ��ֵ;4,��������
	//ֱ�ӵ���TIM_ICInitҲ��ʵ������4������*/
}

//--------------------------------------------------
//DMA�ɼ�GPIOA->IDR�ĵ�8λ
//--------------------------------------------------
void DMA_init(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//ʹ��RCCʱ��
	//-------------------------------------------------------- DMA channel1 configuration
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr =(U32)&(GPIOA->IDR);	//�����ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (U32)DMA_Buf;			//�ڴ��ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;				//������Ϊ��Դ
	DMA_InitStructure.DMA_BufferSize = DMA_BUFSIZE;					//�ڴ���������С�����ڴ����ݿ��Ϊ��λ����
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//��ʹ����������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;			//ʹ���ڴ�����//DMA_MemoryInc_Disable;
	DMA_InitStructure.DMA_PeripheralDataSize =DMA_PeripheralDataSize_Byte;//_Word;//_HalfWord;//_Byte;//;//�������ݿ��
	DMA_InitStructure.DMA_MemoryDataSize =DMA_MemoryDataSize_Byte;//Word;//HalfWord;//�ڴ����ݿ��
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;//��ͨģʽ//DMA_Mode_Circular;//ѭ��ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M =DMA_M2M_Disable;	//��ʹ��"�洢�����洢��"?
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	// Enable DMA channel1 
	DMA_Cmd(DMA1_Channel1, ENABLE);	 
}
//--------------------------------------------------
//��⵽VSYNC���½���֮������ֹͣDMA
//--------------------------------------------------
void DMA_stop(void)
{	
	DMA_Cmd(DMA1_Channel1, DISABLE);
	DMA_RCnt=DMA_BUFSIZE-DMA1_Channel1->CNDTR;//��ȡ�յ�������
	
	//DMA_Cmd(DMA1_Channel1, ENABLE);	
}
//--------------------------------------------------
//��⵽VSYNC���½���,���main���оͿ�������DMA��
//--------------------------------------------------
void DMA_start(void)
{	
	TIM4_ResetPsc();//��λPCLK��Ƶ
	DMA_Cmd(DMA1_Channel1, DISABLE);
	DMA1_Channel1->CNDTR=DMA_BUFSIZE;
	DMA_Cmd(DMA1_Channel1, ENABLE);//��ʱ�������һ����,���ٴ�����
	
	DMA_Cmd(DMA1_Channel1, DISABLE);
	DMA1_Channel1->CNDTR=DMA_BUFSIZE;
	DMA_Cmd(DMA1_Channel1, ENABLE);		
}
//--------------------------------------------------
//	IO�ڳ�ʼ��
//--------------------------------------------------
void io_init(void)
{/*
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;	//ѡ��Ҫ�������õĶ˿�(D1  D2)
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		//ģʽ:
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//�ٶ�
	GPIO_Init(GPIOC, &GPIO_InitStructure);					 
*/
	GPIO_InitTypeDef GPIO_InitStructure;  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC |
	                     RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE , ENABLE); 	
//����PA9:TX1,PA10:RX1

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	         //USART1 TX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    //�����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);		    //A�˿� 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;	      //LED
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    //�������
	GPIO_Init(GPIOA, &GPIO_InitStructure);		    //A�˿�
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	         //USART1 RX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   //���ÿ�©����
	GPIO_Init(GPIOA, &GPIO_InitStructure);		         //A�˿� 
	
	GPIO_InitStructure.GPIO_Pin = 0x00ff;//D0~D7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Pin = BIT(2)|BIT(10)|BIT(11)|BIT(12);//RST,SIO_C,SIO_D,PWDN
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    //�������
	GPIO_Init(GPIOB, &GPIO_InitStructure);		    //B�˿�
	
	GPIO_InitStructure.GPIO_Pin = BIT(0);//XCLK
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//�����������
	GPIO_Init(GPIOB, &GPIO_InitStructure);		    //B�˿�	

	GPIO_InitStructure.GPIO_Pin = BIT(1)|BIT(6)|BIT(7);//VSYNC,PCLK,HREF
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOB, &GPIO_InitStructure);		    //B�˿�	

	LED_L;
}
//--------------------------------------------------
//�����ж�,TIM3�ж�
//--------------------------------------------------
void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* Configure the NVIC Preemption Priority Bits */  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	
	/* Enable the USART1 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
//--------------------------------------------------
//	��ʼ������
//--------------------------------------------------
void UART_Config(void)
{
	USART_InitTypeDef USART_InitStructure;
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1,ENABLE);	//ʹ�ܴ���ʱ��
	USART_InitStructure.USART_BaudRate =115200;// 921600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	  
	/* Enable USART1 Receive and Transmit interrupts */
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	//USART_ITConfig(USART1, USART_IT_TXE, ENABLE);//���ͼĴ�����
	
	/* Enable the USART1 */
	USART_Cmd(USART1, ENABLE);
}

//--------------------------------------------------
//	���ڷ����ֽ�
//--------------------------------------------------
void UART_SendByte(USART_TypeDef* USARTx, U8 Data)
{
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE)==RESET);	//�ȴ�"���ͼĴ�����"
	USART_SendData(USARTx, Data);
}
//--------------------------------------------------
//	���ڷ����ַ���
//--------------------------------------------------
void UART_SendStr(USART_TypeDef* USARTx, U8* s)
{
	while(*s)
	{
		UART_SendByte(USARTx, *s);
		s++;
	}
}
//--------------------------------------------------
//	���ڸ�ʽ�����
//--------------------------------------------------
int UART_Printf(USART_TypeDef* USARTx,const U8 *fmt, ...)
{
	U8 printf_buf[1024];	
	va_list args;
	int printed;	
	va_start(args, fmt);//args ָ�� fmt ����
	printed = GUI_VsPrintf(printf_buf, fmt, args);
	va_end(args);	
	//puts(printf_buf);
	UART_SendStr(USARTx,printf_buf);
	return printed;
}
//--------------------------------------------------
//ʹ��HSI:8/2*16=64MHZ
//--------------------------------------------------
void RCC_Configuration(void)
{
	RCC_DeInit();
	
	RCC_HSICmd(ENABLE);
	
	while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET)
	{
	
	} 
 
	FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);  
	FLASH_SetLatency(FLASH_Latency_2);  
	RCC_HCLKConfig(RCC_SYSCLK_Div1); //AHBʱ��Ϊϵͳʱ��SYSCLK  
	RCC_PCLK2Config(RCC_HCLK_Div1);  //APB1ʱ��ΪHCLK/2������HCLKΪAHBʱ��  
	RCC_PCLK1Config(RCC_HCLK_Div2);  //APB2ʱ��ΪHCLK  
	//���� PLL ʱ��Դ����Ƶϵ��  
	RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_16);  //8/2*16=64MHZ
	//ʹ�ܻ���ʧ�� PLL,�����������ȡ��ENABLE����DISABLE  
	RCC_PLLCmd(ENABLE);//���PLL������ϵͳʱ��,��ô�����ܱ�ʧ��  
	//�ȴ�ָ���� RCC ��־λ���óɹ� �ȴ�PLL��ʼ���ɹ�  
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
	{
	
	}
	//����ϵͳʱ�ӣ�SYSCLK�� ����PLLΪϵͳʱ��Դ  
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
	//�ȴ�PLL�ɹ�������ϵͳʱ�ӵ�ʱ��Դ     
	// 0x00��HSI ��Ϊϵͳʱ��  
	// 0x04��HSE��Ϊϵͳʱ��  
	// 0x08��PLL��Ϊϵͳʱ��  
	while(RCC_GetSYSCLKSource() != 0x08)
	{
	
	}  
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |
	//RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
}

//--------------------------------------------------
//	��ʼ�����е��豸
//--------------------------------------------------
void init_devices(void)
{
	RCC_Configuration();
	//SystemInit();	
	io_init();
	UART_Config();
	Timer_Init();
	DMA_init();
	NVIC_Configuration();
	OV7670_init();
}
//--------------------------------------------------
//	������
//--------------------------------------------------
int main(void)
{
	//U16 i;
	//U8 *p=(U8*)DMA_Buf;
	init_devices();		
	//UART_SendStr(USART1,"Hello world!\r\n");
	//UART_Printf(USART1,"abc%cdefg%9.8dh\r\n",123,54321);	
	while(1)
	{
		//UART1_SendByte(0xaa);		
		//UART_SendByte(USART1,IICDEV_RByte(0x1C));//����ʶ���ֽ�7F
		//UART_SendByte(USART1,IICDEV_RByte(0x1D));//����ʶ���ֽ�A2
		//delayms(500);
		if(mainflag&FLAG_BUSY)
		{				
			/*
			//����ͼ��
			UART1_SendByte(0xaa);//��ʼ1
			UART1_SendByte(0x55);//��ʼ2
			UART1_SendByte(DMA_RCnt>>8);//֡�����ֽ�
			UART1_SendByte(DMA_RCnt);//֡�����ֽ�
			for(i=0;i<DMA_RCnt;i++)
				UART1_SendByte(p[i]);
			*/
			PicProcess();//ͼ����
			IOCLR(mainflag,FLAG_BUSY);
			IOSET(mainflag,FLAG_START);//�������													
			LED_XOR;
		}		
	}
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* Infinite loop */
  while (1){}
}
#endif
