 
#include "stm32f10x.h"
#include "stm32f10x_flash.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_tim.h"
#include "misc.h"
#include "BSP.h"
#include "usart.h"
#include "stm32f10x_iwdg.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_pwr.h"

void TIM_Configuration(void);
     
/*   
* 函数名：BSP_Init   
* 描述  ：时钟初始化、硬件初始化   
* 输入  ：无   
* 输出  ：无   
*/   
void BSP_Init(void)    
{  
	
	
	
	RCC_Configuration();
	
	GPIO_Configuration();

	USART_Configuration();
	
	WakeupPin_Config();

	//FLASH_Unlock(); 
	 
} 
 
void RCC_Configuration(void)
{																
	/* Setup the microcontroller system. Initialize the Embedded Flash Interface,  
	initialize the PLL and update the SystemFrequency variable. */
	SystemInit();
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
	
	/* Enable GPIOA and USART1 clocks */
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | 
							RCC_APB2Periph_GPIOB |
							RCC_APB2Periph_GPIOC | 
							RCC_APB2Periph_GPIOD |
							RCC_APB2Periph_GPIOE,
							ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5	, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2	, ENABLE);
}

void USART_Configuration(void)
{
	USART1_Init();/* For Printf */
// 	USART3_Init();
 	USART2_Init();
	UART5_Init();/* For SIM900A */
}

void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_StructInit(&GPIO_InitStructure);
	
	/* Configure USART2 Tx (PA2) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Configure USART2 Rx (PA3) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* USART1 SerialConsole Tx(PA9) Rx(PA10)*/
	/* Configure USART1 Tx (PA9) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/* Configure USART1 Rx (PA10) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

// 	/* Configure USART3 Tx (PB10) as alternate function push-pull */
// 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
// 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
// 	GPIO_Init(GPIOB, &GPIO_InitStructure);

// 	/* Configure USART3 Rx (PB11) as input floating */
// 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
// 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
// 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/* UART5 SIM900A Tx(PC12) Rx(PD2)*/
	/* Configure UART5 Tx (PC12) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	/* Configure UART5 Rx (PD2) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
}


void WakeupPin_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(KEY_POWER_CLK| RCC_APB2Periph_AFIO,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = KEY_POEWR_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	         // 下拉输入
  GPIO_Init(KEY_POWER_GPIO, &GPIO_InitStructure);
	
	GPIO_EXTILineConfig(KEY_POWER_SOURCE, KEY_POWER_PIN_SOURCE);
	
  EXTI_InitStructure.EXTI_Line = KEY_POEWR_EXTI;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //上升沿触发
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel = KEY_POWER_IRQChannel;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}


