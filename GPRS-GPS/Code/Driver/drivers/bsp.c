#include "board.h"


void bsp_init(){
	RCC_Configuration();
	led_gpio_init();
	USART_gpio();
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


void led_gpio_init(){
		GPIO_InitTypeDef GPIO_InitStructure;
		GPIO_StructInit(&GPIO_InitStructure);
	
		/* LED PA12*/	
		GPIO_InitStructure.GPIO_Pin = LED0_PIN;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_Init(LED0_GPIO, &GPIO_InitStructure);

}

