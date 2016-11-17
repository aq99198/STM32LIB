

#include "App.h"
#include "Bsp.h"
#include "stm32f10x.h"
#include "misc.h"
/********************** FUNCTION ************************/

//!< 系统启动接口

void JTAG_Set(u8 mode)
{
	u32 temp;
	temp=mode;
	temp<<=25;
	RCC->APB2ENR|=1<<0;//开启辅助时钟
	AFIO->MAPR&=0XF8FFFFFF;//清除MAPR的[26:24]
	AFIO->MAPR|=temp;//设置jtag模式
}

void Power_ON(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_3);
	
	GPIO_SetBits(GPIOB,GPIO_Pin_3);
	

	
}

void LED_ON(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_12);
	
}

void PWM_OFF(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE); 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_SetBits(GPIOE,GPIO_Pin_11);	//close
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_SetBits(GPIOE,GPIO_Pin_12);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_SetBits(GPIOE,GPIO_Pin_13);
	
}





void Delay(__IO u32 nCount)	 //简单的延时函数
{
	for(; nCount != 0; nCount--);
}

int main(void)
{
	
	JTAG_Set(01);
	Power_ON();
	LED_ON();
	PWM_OFF();
	
	Delay(800000);         //等待HSE稳定
	
    BSP_Init();
	
	
	
    CApp * app  = new CApp();
    app->mainloop();

    //!< 系统设置不应该运行到这里，如果运行到这里，需要检查程序是否会有错误
    return 0;
}