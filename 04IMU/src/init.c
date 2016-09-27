#include "board.h"
#include "init.h"

int16_t failsafeCnt = 0;
uint16_t acc_1G = 256; 
int16_t heading, magHold;
void SetSysClock(bool overclock);
void SysInit(void){
	
	struct {
        GPIO_TypeDef *gpio;
        gpio_config_t cfg;
    } gpio_setup[] = {
#ifdef LED0
        {
            .gpio = LED0_GPIO,
            .cfg = { LED0_PIN, Mode_Out_PP, Speed_2MHz }
        },
#endif
#ifdef LED1

        {
            .gpio = LED1_GPIO,
            .cfg = { LED1_PIN, Mode_Out_PP, Speed_2MHz }
        },
#endif
#ifdef BUZZER
        {
            .gpio = BEEP_GPIO,
            .cfg = { BEEP_PIN, Mode_Out_OD, Speed_2MHz }
        },
#endif
#ifdef INVERTER
        {
            .gpio = INV_GPIO,
            .cfg = { INV_PIN, Mode_Out_PP, Speed_2MHz }
        },
#endif
    };
    gpio_config_t gpio;
    int i, gpio_count = sizeof(gpio_setup) / sizeof(gpio_setup[0]);

    // Configure NVIC preempt/priority groups
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    // Turn on clocks for stuff we use
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 | RCC_APB1Periph_TIM3 | RCC_APB1Periph_TIM4, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_TIM1 | RCC_APB2Periph_ADC1 | RCC_APB2Periph_USART1, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    RCC_ClearFlag();

    // Make all GPIO in by default to save power and reduce noise
    gpio.pin = Pin_All;
    gpio.mode = Mode_AIN;
    gpioInit(GPIOA, &gpio);
    gpioInit(GPIOB, &gpio);
    gpioInit(GPIOC, &gpio);

    // Turn off JTAG port 'cause we're using the GPIO for leds
#define AFIO_MAPR_SWJ_CFG_NO_JTAG_SW            (0x2 << 24)
    AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_NO_JTAG_SW;

#ifdef BUZZER
    // Configure gpio
    systemBeepPtr = beepRev4;
    BEEP_OFF;
#endif
    LED0_OFF;
    LED1_OFF;

    // Hack - rev4 and below used opendrain to PNP for buzzer. Rev5 and above use PP to NPN.
    for (i = 0; i < gpio_count; i++) {
        gpioInit(gpio_setup[i].gpio, &gpio_setup[i].cfg);
    }

		
		SetSysClock(false);
	 // system tick , system clock setting shoud font then cycleCounterInit() function
		cycleCounterInit();
		SysTick_Config (SystemCoreClock / 1000);
		
		
}



