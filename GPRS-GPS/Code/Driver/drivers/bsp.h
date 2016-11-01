#ifndef __BSP_H    
#define __BSP_H    

#ifdef __cplusplus
    extern "C" {
#endif

void bsp_init(void);
void led_gpio_init(void);
void RCC_Configuration(void);
void ExitSleepMode_SIM900A(void);
			
			
#ifdef __cplusplus
    }
#endif
		
#endif // __BSP_H 
