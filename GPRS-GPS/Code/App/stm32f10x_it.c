/**
  ******************************************************************************
  * @file    GPIO/IOToggle/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.1.0
  * @date    06/19/2009
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
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

/* Includes ------------------------------------------------------------------*/

#include "board.h"
#include "stm32f10x_it.h"




// u8 TX[52] = "AT+CIPSTART=";
// u16 k;
// u8 Buffer[] = "Setting Ok";
// u8 At_flag;


/** @addtogroup STM32F10x_StdPeriph_Examples
  * @{
  */

/** @addtogroup GPIO_IOToggle
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)    
{    
 	OSIntEnter(); //Tell uC/OS-II that we are starting an ISR.
  OSTimeTick();  /* Call uC/OS-II's OSTimeTick() */
  OSIntExit();  /* Tell uC/OS-II that we are leaving the ISR */
}  

void delay_nms(u32 nCount)
{    
   for(; nCount != 0; nCount--);
}

// for power control
void EXTI0_IRQHandler(void)                 
{
  if(EXTI_GetITStatus(EXTI_Line0) != RESET)
  {
		EXTI_ClearITPendingBit(EXTI_Line0);     
					
			__disable_irq();     
			
			printf("interrput\r\n");
			GPRS_POWER_OFF();
			delay_nms(8000000);
			GPRS_POWER_ON();          //close GPRS
			delay_nms(8000000);
			GPRS_POWER_OFF();
			delay_nms(8000000);
			printf("gprs power off\r\n");
						
			RCC_APB2PeriphResetCmd(0X01FC,DISABLE);	
			
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);	
			PWR_WakeUpPinCmd(ENABLE);              
			PWR_EnterSTANDBYMode();                
  }
}


// for led
void TIM5_IRQHandler(void)   
{
    if(TIM_GetITStatus(TIM5 , TIM_IT_Update) == SET)
    {

        static UINT32 tick = 0;
				tick++;
			
        switch(SystemStatus)
        {
        case SYS_STARTUP: 
        {
           LED0_ON;
        }
        break;
        case SYS_SIM900A_ON:
        {
					if(tick%10==1)
						LED0_TOGGLE;
        }
        break;
        case SYS_CONNECT_SERVER: 
        {  
					if(tick%10<4){
						LED0_TOGGLE
					}else{
						LED0_ON;
					}
        }        
        break;
				
				case SYS_GPS_FIX: 
        {  
					LED0_TOGGLE
        }        
        break;
				
				case SYS_SAVE: 
        {  
        }        
        break;
				
        default:
            // do nothing 
            break;
        }

        TIM_ClearITPendingBit(TIM5 , TIM_IT_Update);
    }
}

void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSV_Handler exception.
  * @param  None
  * @retval None
  */
// void PendSV_Handler(void)
// {
// }

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */


/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
