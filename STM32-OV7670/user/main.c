/*--------------------------------------------------
OV7670驱动原理:
电源(DOVDD,AVDD)为2.5V左右(HT7333输出串1N4148降压),
DVDD采用OV7670内部的LDO(1.8V),外部只需接一只电容.
单片机为STM32F103CBT6，采用HSI时钟,PLL倍频到64MHz,可省去外部晶振.

由于只需要处理灰度图像,OV7670配置为YUV格式,Y在前,200*100像素,
TIM4_CH1的捕获分频设置为2分频,就可以只采集Y（亮度）.

TIM3_CH3产生XCLK,(16MHz对应VSYNC约20HZ,实测最低约8MHZ)
TIM4_CH1检测PCLK(4MHz),下降沿捕获,滤波为1,二分频,触发DMA,采集PA0~PA7的数据至数组DMA_Buf.
TIM3_CH4检测VSYNC,下降沿捕获中断,控制DMA的关与开.
HREF忽略.

引脚连接:
PA12:			LED
PA0~7:			OV7670_D0~D7
PB0(TIM3_CH3):	OV7670_XCLK
PB1(TIM3_CH4):	OV7670_VSYNC
PB2:			OV7670_RST
PB7(TIM4_CH2):	OV7670_HREF(可省略)
PB6(TIM4_CH1):	OV7670_PCLK
PB10:			OV7670_SIO_C
PB11:			OV7670_SIO_D
PB12:			OV7670_PWDN

建议:
	为加快速度,SIO_C与SIO_D可加外部上拉电阻

注意:
	看了下SCCB时序,貌似与IIC略有区别,指定地址读数据时:
	OV7670:	...IICWByte(add);IICACK(0);IICEnd();IICStart();...
	普通IIC:...IICWByte(add);IICACK(0);         IICStart();...

图像处理：
上次图像中部区域(按STEP间隔)存入数组MID_Buf，在当前采集的
图像中遍历，寻找差值最小的位置，从而得到座标增量。

----------------------------------------------------*/
//--------------------------------------------------
//		头文件
//--------------------------------------------------
//#include "stm32f10x.h"
#include "includes.H"
#define UART1_SendByte(d)	UART_SendByte(USART1,d)

#define LED_H	IOSET(GPIOA->ODR,BIT(12))
#define LED_L	IOCLR(GPIOA->ODR,BIT(12))
#define LED_XOR	IOXOR(GPIOA->ODR,BIT(12))
//#define VSYNC_IN	(GPIOB->IDR&BIT(1))
//摄像头的宽与高
#define CAMWIDTH	80
#define CAMHEIGHT	80

//中部区域的宽与高
#define MIDWIDTH	(CAMWIDTH/2)
#define MIDHEIGHT	(CAMHEIGHT/2)

//中部的左上角座标
#define MIDX	((CAMWIDTH-MIDWIDTH)/2)
#define MIDY	((CAMHEIGHT-MIDHEIGHT)/2)
//匹配时的像素间隔，加大可加快速度
#define STEP	5

#define DMA_BUFSIZE (CAMWIDTH*CAMHEIGHT)

U32	DMA_RCnt=0;//当DMA停止时，统计DMA采集的数据量
U8 	DMA_Buf[CAMHEIGHT][CAMWIDTH];//采集的摄像头数据
//U8 	MID_Buf[MIDHEIGHT][MIDWIDTH];//中部区域的数据
U8 	MID_Buf[(MIDHEIGHT/STEP)*(MIDWIDTH/STEP)];//中部区域的数据,利用STEP来减小中部区域的消耗

S16	MovIncX=0,MovIncY=0;//X方向增量与Y方向增量
S16	MovX=0,MovY=0;//X方向位置与Y方向位置

enum{
FLAG_BUSY=BIT(0),//收到数据
FLAG_START=BIT(1),//允许接收数据
FLAG_SAVE=BIT(2)
};
U16 mainflag=0;//标志

void SDA_MIN(void);
void SDA_MO(void);
//--------------------------------------------------
//--------------------------------------------------
//		声明
//--------------------------------------------------

#include "IIC.C"
#include "OV7670.C"
#include "stm32f10x_it.c"

//void RCC_Configuration(void);
U8 uartflag=0;

//--------------------------------------------------
//SIO_D配置为输入
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
//SIO_D配置为输出
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
//	短延时
//--------------------------------------------------
void delay(__IO uint32_t nCount)
{
	for(; nCount != 0; nCount--);
}
//--------------------------------------------------
//	微秒延时
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
//	毫秒延时
//--------------------------------------------------
void delayms(U32 t)
{
	for(;t!=0;t--)
	{
		delayus(1000);
	}
}
//--------------------------------------------------
//计算差值的绝对值
//--------------------------------------------------
__INLINE U8  caldif(U8 v1,U8 v2)
{
	if(v1>v2)return v1-v2;
	return v2-v1;
}
//--------------------------------------------------
//图像处理：用上次图像中部区域去与当前采集图像（从x,y
//开始的区域）求差，寻找差值最小的位置，
//从而得到x增量（MovIncX）与y增量（MovIncY）
//--------------------------------------------------
void PicProcess(void)
{
	U16 i,j,x,y;
	U32 dif,min=0xffffffff;	//差值，最小值
	U16 minx,miny;			//最小值处的左上角座标
	U8	*p1,*p2;			//为加快数组访问速度
	//--------遍历整个当前图像，查找与之前中部图像相差最小的位置	
	for(y=0;y<CAMHEIGHT-MIDHEIGHT;y++)//从上到下
	{
		for(x=0;x<CAMWIDTH-MIDWIDTH;x++)//从左到右
		{		
			dif=0;//误差清0	
			p1=MID_Buf;	//p1指向上次图像中部
			for(i=0;i<MIDHEIGHT;i+=STEP)
			{
				p2=DMA_Buf[y+i]+x;//p2指向行开始
				for(j=0;j<MIDWIDTH;j+=STEP)
				{//计算当前图像中的点与上次图像中部区域的点的差值
					dif+=caldif(*p1,*p2);//误差累加	
					p1++;
					p2+=STEP;
				}
				//dif+=calrow1(DMA_Buf[y+i]+x,MID_Buf[i]);
			}
			if(min>dif)//误差小于当前最小值，则记最小值为当前值，且记下座标
			{
				min=dif;
				minx=x;
				miny=y;
			}
		}
	}
	//------------------------------计算座标增量
	if(min>30000)min=30000;//此句可省，是为了便于串口发送该值
	if(min<15*(MIDHEIGHT/STEP)*(MIDWIDTH/STEP))//允许平均每个点15的误差
	{
		MovIncX=minx-MIDX;//x增量（有符号数）
		MovIncY=miny-MIDY;//y增量（有符号数）
		MovX+=MovIncX;//模拟成鼠标的绝对座标，便于查看
		MovY+=MovIncY;	
		if(MovX<-1000)MovX=-1000;//限幅
		if(MovX>1000)MovX=1000;	
		if(MovY<-1000)MovY=-1000;
		if(MovY>1000)MovY=1000;
	}	
	//------------------------------复制当前中部区域
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
	//------------------------------串口查看误差与座标
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
//二维数组访问速度较低
void PicProcess(void)
{
	U16 i,j,x,y;
	U32 dif,min=0xffffffff;	//差值，最小值
	U16 minx,miny;			//最小值处的左上角座标
	//--------遍历整个当前图像，查找与之前中部图像相差最小的位置	
	for(y=0;y<CAMHEIGHT-MIDHEIGHT;y++)//从上到下
	{
		for(x=0;x<CAMWIDTH-MIDWIDTH;x++)//从左到右
		{		
			dif=0;//误差清0			
			for(i=0;i<MIDHEIGHT;i+=STEP)
			{
				for(j=0;j<MIDWIDTH;j+=STEP)
				{//计算当前图像中的点与上次图像中部区域的点的差值
					dif+=caldif(DMA_Buf[y+i][x+j],MID_Buf[i][j]);//误差累加	
				}
			}
			if(min>dif)//误差小于当前最小值，则记最小值为当前值，且记下座标
			{
				min=dif;
				minx=x;
				miny=y;
			}
		}
	}
	//------------------------------计算座标增量
	if(min>30000)min=30000;//此句可省，是为了便于串口发送该值
	if(min<15*(MIDHEIGHT/STEP)*(MIDWIDTH/STEP))//允许平均每个点15的误差
	{
		MovIncX=minx-MIDX;//x增量
		MovIncY=miny-MIDY;//y增量
		MovX+=MovIncX;//模拟成鼠标的绝对座标，便于查看
		MovY+=MovIncY;	
		if(MovX<-1000)MovX=-1000;//限幅
		if(MovX>1000)MovX=1000;	
		if(MovY<-1000)MovY=-1000;
		if(MovY>1000)MovY=1000;
	}	
	//------------------------------复制当前中部区域
	for(i=0;i<MIDHEIGHT;i+=STEP)
	{
		//memcpy(MID_Buf[i],DMA_Buf[MIDY+i]+MIDX,MIDWIDTH);
		for(j=0;j<MIDWIDTH;j+=STEP)
		{
			MID_Buf[i][j]=DMA_Buf[MIDY+i][MIDX+j];			
		}
	}
	//------------------------------串口查看误差与座标
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
//效果图
void PicProcess_show(void)
{
	U16 i,j,x,y;
	U8	*p1,*p2;			//为加快数组访问速度
			
	for(y=0;y<CAMHEIGHT-MIDHEIGHT;y++)//从上到下
	{
		for(x=0;x<CAMWIDTH-MIDWIDTH;x++)//从左到右
		{		
			p1=MID_Buf;	//p1指向上次图像中部	
			for(i=0;i<MIDHEIGHT;i+=STEP)
			{
				p2=DMA_Buf[y+i]+x;//p2指向行开始
				for(j=0;j<MIDWIDTH;j+=STEP)
				{
					*p1=*p2;
					*p2=0;//绘制匹配点
					p1++;
					p2+=STEP;
				}				
			}	
			p1=MID_Buf;
			p2=DMA_Buf[0];
			//发送图像
			UART1_SendByte(0xaa);//起始1
			UART1_SendByte(0x55);//起始2
			UART1_SendByte(DMA_RCnt>>8);//帧长高字节
			UART1_SendByte(DMA_RCnt);//帧长低字节
			for(i=0;i<DMA_RCnt;i++)
				UART1_SendByte(p2[i]);
			//恢复图像
			for(i=0;i<MIDHEIGHT;i+=STEP)
			{
				p2=DMA_Buf[y+i]+x;//p2指向行开始
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
//		定时器初始化
//TIM3_CH3产生XCLK:10M~48M 
//TIM时钟源=32M,2分频后产生PWM为16M
//3分频后产生PWM为10.6M
//QCIF格式的PCLK为XCLK/4
//--------------------------------------------------
void Timer_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef	TIM_OCInitStructure;		
    TIM_ICInitTypeDef tim_icinit; 
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);//使能RCC时钟	
	TIM_DeInit(TIM3);
	TIM_TimeBaseStructure.TIM_Period= 1;			//自动重装值,为0时不工作
	TIM_TimeBaseStructure.TIM_Prescaler=2-1;		//时钟频率预分频
	//向上计数:0~TIM_Period		向下计数:TIM_Period~0
	TIM_TimeBaseStructure.TIM_CounterMode= TIM_CounterMode_Up;	//向上计数模式
	TIM_TimeBaseStructure.TIM_ClockDivision= TIM_CKD_DIV1;	//时钟分频因子
	//TIM2_TimeBaseInit.TIM_RepetitionCounter= 0x0000;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);	
	//TIM_UpdateDisableConfig(TIM2,DISABLE);//允许更新
	TIM_InternalClockConfig(TIM3);//由内部提供时钟.
	//TIM_PrescalerConfig(TIM2,1,TIM_PSCReloadMode_Immediate);//立即装入预分频
	//TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);//使能更新事件中断	
	//TIM_ITConfig(TIM2,TIM_IT_CC2,ENABLE);//使能TIM2捕获\比较中断		
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//TIM_OCMode_Toggle// 输出比较触发模式  
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//貌似必须设置为输出才行
	TIM_OCInitStructure.TIM_Pulse = 1;//占空时间
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);//TIM3_CH3:产生XCLK
	
	//-------------TIM3_CH4:检测VSYNC
    tim_icinit.TIM_Channel = TIM_Channel_4;   
    tim_icinit.TIM_ICFilter = 0x00;      //硬件滤波      
    tim_icinit.TIM_ICPolarity = TIM_ICPolarity_Falling;//触发捕获的电平       
    tim_icinit.TIM_ICPrescaler= TIM_ICPSC_DIV1;  //每次检测到触发电平都捕获       
    tim_icinit.TIM_ICSelection = TIM_ICSelection_DirectTI;  //通道方向选择
    TIM_ICInit(TIM3,&tim_icinit);  //捕获初始化 
	TIM_ITConfig(TIM3,TIM_IT_CC4,ENABLE);
	
	TIM_Cmd(TIM3,ENABLE);	//使能定时器
	
	//------------------------------TIM4_CH1用于捕获PCLK,且触发DMA	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);//使能RCC时钟	
	TIM_DeInit(TIM4);
	TIM_TimeBaseStructure.TIM_ClockDivision= TIM_CKD_DIV1;	//时钟分频因子
	TIM_TimeBaseStructure.TIM_Prescaler= 1;		//时钟频率预分频
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);	
	TIM_InternalClockConfig(TIM4);//由内部提供时钟.
	
    tim_icinit.TIM_Channel = TIM_Channel_1;   
    tim_icinit.TIM_ICFilter = 0x01;      //硬件滤波,防干扰(要注意,可取1~2)
	//OV7670时序是应上升沿采样数据,但为加快速度,可以在下降沿去触发DMA,要注意使能硬件滤波TIM_ICFilter
    tim_icinit.TIM_ICPolarity =TIM_ICPolarity_Falling; //TIM_ICPolarity_Rising;//触发捕获的电平       
    tim_icinit.TIM_ICPrescaler= TIM_ICPSC_DIV2;//TIM_ICPSC_DIV1;  //每次检测到触发电平都捕获       
    tim_icinit.TIM_ICSelection = TIM_ICSelection_DirectTI;  //通道方向选择
    TIM_ICInit(TIM4,&tim_icinit);  //初始化  
	
    //配置定时器的DMA  
    //TIM_DMAConfig(TIM4,TIM_DMABase_CCR1,TIM_DMABurstLength_1Byte);  
    //产生DMA请求信号
    TIM_DMACmd(TIM4, TIM_DMA_CC1, ENABLE); //PCLK
	
//	ADCEn=1;
}
//--------------------------------------------------
//重置TIM4输入分频，否则一旦PCLK错位,则会一直错下去
//--------------------------------------------------
void TIM4_ResetPsc(void)
{
    TIM_ICInitTypeDef tim_icinit; 
    tim_icinit.TIM_Channel = TIM_Channel_1;   
    tim_icinit.TIM_ICFilter = 0x01;      //硬件滤波,防干扰(要注意,可取1~2)
	//OV7670时序是应上升沿采样数据,但为加快速度,可以在下降沿去触发DMA,要注意使能硬件滤波TIM_ICFilter
    tim_icinit.TIM_ICPolarity =TIM_ICPolarity_Falling; //TIM_ICPolarity_Rising;//触发捕获的电平       
    tim_icinit.TIM_ICPrescaler=TIM_ICPSC_DIV2;  
    tim_icinit.TIM_ICSelection = TIM_ICSelection_DirectTI;  //通道方向选择
    TIM_ICInit(TIM4,&tim_icinit);  //初始化	(输入分频器将被重置)
	/*U16 temp;	
	temp=TIM4->CCER;
	TIM4->CCER &= 0xfffe;// CCER_CC1E_Reset;
	TIM_SetIC1Prescaler(TIM4,TIM_ICPSC_DIV2);//输入2分频
	TIM4->CCER=temp;	
	//重置分频流程:1,关闭捕获;2,分频设为1;3,分频设为需要的值;4,开启捕获
	//直接调用TIM_ICInit也能实现上述4个步骤*/
}

//--------------------------------------------------
//DMA采集GPIOA->IDR的低8位
//--------------------------------------------------
void DMA_init(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//使能RCC时钟
	//-------------------------------------------------------- DMA channel1 configuration
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr =(U32)&(GPIOA->IDR);	//外设地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (U32)DMA_Buf;			//内存地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;				//外设作为来源
	DMA_InitStructure.DMA_BufferSize = DMA_BUFSIZE;					//内存数据区大小（以内存数据宽度为单位？）
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//不使能外设增量
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;			//使能内存增量//DMA_MemoryInc_Disable;
	DMA_InitStructure.DMA_PeripheralDataSize =DMA_PeripheralDataSize_Byte;//_Word;//_HalfWord;//_Byte;//;//外设数据宽度
	DMA_InitStructure.DMA_MemoryDataSize =DMA_MemoryDataSize_Byte;//Word;//HalfWord;//内存数据宽度
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;//普通模式//DMA_Mode_Circular;//循环模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M =DMA_M2M_Disable;	//不使用"存储器到存储器"?
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	// Enable DMA channel1 
	DMA_Cmd(DMA1_Channel1, ENABLE);	 
}
//--------------------------------------------------
//检测到VSYNC的下降沿之后马上停止DMA
//--------------------------------------------------
void DMA_stop(void)
{	
	DMA_Cmd(DMA1_Channel1, DISABLE);
	DMA_RCnt=DMA_BUFSIZE-DMA1_Channel1->CNDTR;//获取收到的数据
	
	//DMA_Cmd(DMA1_Channel1, ENABLE);	
}
//--------------------------------------------------
//检测到VSYNC的下降沿,如果main空闲就可以启动DMA了
//--------------------------------------------------
void DMA_start(void)
{	
	TIM4_ResetPsc();//复位PCLK分频
	DMA_Cmd(DMA1_Channel1, DISABLE);
	DMA1_Channel1->CNDTR=DMA_BUFSIZE;
	DMA_Cmd(DMA1_Channel1, ENABLE);//此时启动会多一个数,需再次启动
	
	DMA_Cmd(DMA1_Channel1, DISABLE);
	DMA1_Channel1->CNDTR=DMA_BUFSIZE;
	DMA_Cmd(DMA1_Channel1, ENABLE);		
}
//--------------------------------------------------
//	IO口初始化
//--------------------------------------------------
void io_init(void)
{/*
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;	//选择要进行配置的端口(D1  D2)
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		//模式:
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//速度
	GPIO_Init(GPIOC, &GPIO_InitStructure);					 
*/
	GPIO_InitTypeDef GPIO_InitStructure;  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC |
	                     RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE , ENABLE); 	
//串口PA9:TX1,PA10:RX1

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	         //USART1 TX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    //复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);		    //A端口 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;	      //LED
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    //推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);		    //A端口
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	         //USART1 RX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   //复用开漏输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);		         //A端口 
	
	GPIO_InitStructure.GPIO_Pin = 0x00ff;//D0~D7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Pin = BIT(2)|BIT(10)|BIT(11)|BIT(12);//RST,SIO_C,SIO_D,PWDN
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    //推挽输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);		    //B端口
	
	GPIO_InitStructure.GPIO_Pin = BIT(0);//XCLK
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);		    //B端口	

	GPIO_InitStructure.GPIO_Pin = BIT(1)|BIT(6)|BIT(7);//VSYNC,PCLK,HREF
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOB, &GPIO_InitStructure);		    //B端口	

	LED_L;
}
//--------------------------------------------------
//串口中断,TIM3中断
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
//	初始化串口
//--------------------------------------------------
void UART_Config(void)
{
	USART_InitTypeDef USART_InitStructure;
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1,ENABLE);	//使能串口时钟
	USART_InitStructure.USART_BaudRate =115200;// 921600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	  
	/* Enable USART1 Receive and Transmit interrupts */
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	//USART_ITConfig(USART1, USART_IT_TXE, ENABLE);//发送寄存器空
	
	/* Enable the USART1 */
	USART_Cmd(USART1, ENABLE);
}

//--------------------------------------------------
//	串口发送字节
//--------------------------------------------------
void UART_SendByte(USART_TypeDef* USARTx, U8 Data)
{
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE)==RESET);	//等待"发送寄存器空"
	USART_SendData(USARTx, Data);
}
//--------------------------------------------------
//	串口发送字符串
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
//	串口格式化输出
//--------------------------------------------------
int UART_Printf(USART_TypeDef* USARTx,const U8 *fmt, ...)
{
	U8 printf_buf[1024];	
	va_list args;
	int printed;	
	va_start(args, fmt);//args 指向 fmt 后面
	printed = GUI_VsPrintf(printf_buf, fmt, args);
	va_end(args);	
	//puts(printf_buf);
	UART_SendStr(USARTx,printf_buf);
	return printed;
}
//--------------------------------------------------
//使用HSI:8/2*16=64MHZ
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
	RCC_HCLKConfig(RCC_SYSCLK_Div1); //AHB时钟为系统时钟SYSCLK  
	RCC_PCLK2Config(RCC_HCLK_Div1);  //APB1时钟为HCLK/2，其中HCLK为AHB时钟  
	RCC_PCLK1Config(RCC_HCLK_Div2);  //APB2时钟为HCLK  
	//设置 PLL 时钟源及倍频系数  
	RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_16);  //8/2*16=64MHZ
	//使能或者失能 PLL,这个参数可以取：ENABLE或者DISABLE  
	RCC_PLLCmd(ENABLE);//如果PLL被用于系统时钟,那么它不能被失能  
	//等待指定的 RCC 标志位设置成功 等待PLL初始化成功  
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
	{
	
	}
	//设置系统时钟（SYSCLK） 设置PLL为系统时钟源  
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
	//等待PLL成功用作于系统时钟的时钟源     
	// 0x00：HSI 作为系统时钟  
	// 0x04：HSE作为系统时钟  
	// 0x08：PLL作为系统时钟  
	while(RCC_GetSYSCLKSource() != 0x08)
	{
	
	}  
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |
	//RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
}

//--------------------------------------------------
//	初始化所有的设备
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
//	主函数
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
		//UART_SendByte(USART1,IICDEV_RByte(0x1C));//厂商识别字节7F
		//UART_SendByte(USART1,IICDEV_RByte(0x1D));//厂商识别字节A2
		//delayms(500);
		if(mainflag&FLAG_BUSY)
		{				
			/*
			//发送图像
			UART1_SendByte(0xaa);//起始1
			UART1_SendByte(0x55);//起始2
			UART1_SendByte(DMA_RCnt>>8);//帧长高字节
			UART1_SendByte(DMA_RCnt);//帧长低字节
			for(i=0;i<DMA_RCnt;i++)
				UART1_SendByte(p[i]);
			*/
			PicProcess();//图像处理
			IOCLR(mainflag,FLAG_BUSY);
			IOSET(mainflag,FLAG_START);//允许接收													
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
