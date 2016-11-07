#ifndef __BSP_H    
#define __BSP_H    

#ifdef __cplusplus
    extern "C" {
#endif

			void GPRS_POWER_INIT(void);
void bsp_init(void);
void TIM5_init(void);
void led_gpio_init(void);
void RCC_Configuration(void);
void ExitSleepMode_SIM900A(void);
void WakeupPin_Config(void);
			
			
#ifdef __cplusplus
    }
#endif
		
#endif // __BSP_H 
