#include "board.h"


void bsp_init(){
	
	RCC_Configuration();
	
	led_gpio_init();
	
	USART_gpio();
	
	USART1_Init();
	USART2_Init();
	UART5_Init();
	
	TIM5_init();
	
	ExitSleepMode_SIM900A();
}


void RCC_Configuration(void)
{																
	/* Setup the microcontroller system. Initialize the Embedded Flash Interface,  
	initialize the PLL and update the SystemFrequency variable. */
	SystemInit();
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	//RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
	
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


/* PB4 Logical Low */
void ExitSleepMode_SIM900A(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_StructInit(&GPIO_InitStructure);
	
	/* Power on for external crystal oscillator. */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIOB->BRR = GPIO_Pin_4;
}

void TIM5_init(void){
		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
	
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
    TIM_DeInit(TIM5);
    TIM_TimeBaseStructure.TIM_Prescaler = 7200 - 1 ; // 72M -> 10,000,10K
    TIM_TimeBaseStructure.TIM_Period = 10000 / 10 - 1; //-> 1000 tick, 100ms outtime-> 0.1s ,10Hz
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//向上计数
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
    TIM_ARRPreloadConfig(TIM5, ENABLE);

    TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
    TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);


    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    TIM_Cmd(TIM5, ENABLE);
}

