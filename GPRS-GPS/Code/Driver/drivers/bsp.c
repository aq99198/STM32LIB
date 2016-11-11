#include "board.h"




void bsp_init(){
	
	NVIC_SetVectorTable(NVIC_VectTab_FLASH,0x20000);
	
	RCC_Configuration();
	
	led_gpio_init();
	
	WakeupPin_Config();
	
	GPRS_POWER_INIT();
	
	USART_gpio();
	
	USART1_Init();
	USART2_Init();
	UART5_Init();
	
	TIM5_init();
	
	ExitSleepMode_SIM900A();
}


void RCC_Configuration_HSI(void)  
{  
    RCC_DeInit();//??? RCC?????????  
  
    RCC_HSICmd(ENABLE);//??HSI    
    while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET);//??HSI????  
  
    //FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);  
    //FLASH_SetLatency(FLASH_Latency_2);  
     
    RCC_HCLKConfig(RCC_SYSCLK_Div1);     
    RCC_PCLK1Config(RCC_HCLK_Div2);  
    RCC_PCLK2Config(RCC_HCLK_Div1);  
      
    //?? PLL ????????  
    RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_2);//?????? PLL,???????:ENABLE??DISABLE   
    RCC_PLLCmd(ENABLE);//??PLL???????,????????  
    //????? RCC ??????? ??PLL?????  
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);  
  
    //??????(SYSCLK) ??PLL??????  
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);//?????????   
    //??PLL?????????????  
    //  0x00:HSI ??????   
    //  0x04:HSE??????   
    //  0x08:PLL??????    
    while(RCC_GetSYSCLKSource() != 0x08);//??????????????,RCC_SYSCLKSource_PLL  
  
}  



void RCC_Configuration(void)
{																
	/* Setup the microcontroller system. Initialize the Embedded Flash Interface,  
	initialize the PLL and update the SystemFrequency variable. */
	SystemInit();
	
	//RCC_Configuration_HSI();
	
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


void GPRS_POWER_INIT(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPRS_POWER_OFF();
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




