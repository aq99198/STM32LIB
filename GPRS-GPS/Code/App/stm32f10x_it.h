/**
  ******************************************************************************
  * @file GPIO/IOToggle/stm32f10x_it.h 
  * @author  MCD Application Team
  * @version V3.1.0
  * @date    06/19/2009
  * @brief   This file contains the headers of the interrupt handlers.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F10x_IT_H
#define __STM32F10x_IT_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"


#define LED_WAIT_MODE        (0)
#define LED_RECORD_MODE      (1)
#define LED_DOWNLOAD_MODE    (2)
#define LED_HARD_ERR_MODE    (3)
#define LED_AUTH_MODE        (4)
#define LED_CONNECTED 			 (5)
#define LED_SEND_GPS				 (6)
#define LED_DISCONNECTED		 (7)



#ifdef __cplusplus
   
    extern "C"{
#else
   
#endif




/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);
void TIM2_IRQHandler(void);  
void TIM3_IRQHandler(void);  
void TIM4_IRQHandler(void);  
void TIM5_IRQHandler(void);
void DMAChannel1_IRQHandler(void);
void DMAChannel2_IRQHandler(void);
void DMA1_Channel4_IRQHandler(void);
void EXTI0_IRQHandler(void);

#ifdef __cplusplus
    }
#endif 


#endif /* __STM32F10x_IT_H */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
