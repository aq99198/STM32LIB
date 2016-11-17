#ifndef __BSP_H    
#define __BSP_H    

#ifdef __cplusplus
    extern "C" {
#endif

#include "JacTypeDef.h"

#define KEY_POWER_CLK         RCC_APB2Periph_GPIOA
#define KEY_POWER_GPIO        GPIOA
#define KEY_POEWR_PIN         GPIO_Pin_0
#define KEY_POWER_SOURCE      GPIO_PortSourceGPIOA
#define KEY_POWER_PIN_SOURCE  GPIO_PinSource0
#define KEY_POEWR_EXTI        EXTI_Line0
#define KEY_POWER_IRQChannel  EXTI0_IRQn
			
			
void SysTick_init(void);
void BSP_Init(void);
void RCC_Configuration(void);
void GPIO_Configuration(void);
void USART_Configuration(void);
void TIM_Configuration(void);  
UINT32 GetSystemResetFlag(void);
void WakeupPin_Config(void);


#ifdef __cplusplus
    }
#endif


   
#endif // __BSP_H 
