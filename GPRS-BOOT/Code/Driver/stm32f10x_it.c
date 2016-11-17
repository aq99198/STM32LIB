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
#include "stm32f10x_it.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_pwr.h"
#include "JacTypeDef.h"

Ring_BufferTypeDef UART1_Buffer;
Ring_BufferTypeDef UART2_Buffer;
Ring_BufferTypeDef UART3_Buffer;
Ring_BufferTypeDef UART5_Buffer;

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
volatile INT32  tickDecrease;
volatile UINT32 systick;
void SysTick_Handler(void)    
{    
  --tickDecrease; 
  ++systick;
}  





// for power control
void EXTI0_IRQHandler(void)                 
{
  if(EXTI_GetITStatus(EXTI_Line0) != RESET)
  {
		EXTI_ClearITPendingBit(EXTI_Line0);     
					
			__disable_irq();     
			
			//PRINT("interrput\r\n");
	
			//PRINT("gprs power off\r\n");
						
			RCC_APB2PeriphResetCmd(0X01FC,DISABLE);	

			
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);	
			PWR_WakeUpPinCmd(ENABLE);
			//PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);//STOP??		
			PWR_EnterSTANDBYMode();                
  }
}














void USART1_IRQHandler(void)
{
	 /*中断接收*/

  if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)//reveive data inuterrupt
  {
      UART1_Buffer.buffer[UART1_Buffer.end++]= USART_ReceiveData(USART1);
//	  UART5_send_byte(USART_ReceiveData(USART1));
      UART1_Buffer.end &= 0xFF;
      TIM2->CNT = 0;
      TIM2->CR1 |= TIM_CR1_CEN;//TIM_Cmd(TIM2, ENABLE);
      USART_ClearITPendingBit(USART1,USART_IT_RXNE);
      
  } /*  end of if */


}
void USART2_IRQHandler(void)
{
	 if(USART_GetITStatus(USART2, USART_IT_RXNE) == SET)//判断是不是接收中断
	 {
		UART2_Buffer.buffer[UART2_Buffer.end++]= USART_ReceiveData(USART2);
		UART2_Buffer.end &= 0xFF;
    TIM3->CNT = 0;
    TIM3->CR1 |= TIM_CR1_CEN;//neable
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
	 }
}

void USART3_IRQHandler(void)
{
 /*中断接收*/
   if(USART_GetITStatus(USART3, USART_IT_RXNE) == SET)//判断是不是接收中断
 	 {
			UART3_Buffer.buffer[UART3_Buffer.end++]= USART_ReceiveData(USART3);
			UART3_Buffer.end &= 0xFF;
			TIM4->CNT = 0;
			TIM4->CR1 |= TIM_CR1_CEN;//neable
			USART_ClearITPendingBit(USART3,USART_IT_RXNE);
 	}
	
		//开启CR3,bit0的EIE: Error interrupt enable, 处理USART_IT_ERR,USART_IT_ORE_ER,USART_IT_NE,USART_IT_FE   错误
		if(USART_GetFlagStatus(USART3, USART_FLAG_ORE) != RESET)
		{//同  @arg USART_IT_ORE_ER : OverRun Error interrupt if the EIE bit is set
				unsigned char ushTemp = USART_ReceiveData(USART3); //取出来扔掉
				USART_ClearFlag(USART3, USART_FLAG_ORE);
		}

		if(USART_GetFlagStatus(USART3, USART_FLAG_NE) != RESET)
		{//同  @arg USART_IT_NE     : Noise Error interrupt
				USART_ClearFlag(USART3, USART_FLAG_NE);
		}


		if(USART_GetFlagStatus(USART3, USART_FLAG_FE) != RESET)
		{//同   @arg USART_IT_FE     : Framing Error interrupt
				USART_ClearFlag(USART3, USART_FLAG_FE);
		}

		if(USART_GetFlagStatus(USART3, USART_FLAG_PE) != RESET)
		{//同  @arg USART_IT_PE     : Parity Error interrupt
				USART_ClearFlag(USART3, USART_FLAG_PE);
		}

}

void UART5_IRQHandler(void)
{
	/*中断接收*/
	if(USART_GetITStatus(UART5, USART_IT_RXNE) == SET)//reveive data inuterrupt
	{
		UART5_Buffer.buffer[UART5_Buffer.end++]= USART_ReceiveData(UART5);
		UART5_Buffer.end &= 0xFF;
//		UART1_send_byte(USART_ReceiveData(UART5));
		TIM2->CNT = 0;
		TIM2->CR1 |= TIM_CR1_CEN;//TIM_Cmd(TIM2, ENABLE);
		USART_ClearITPendingBit(UART5,USART_IT_RXNE);
	}
}

void TIM2_IRQHandler(void)  // usart1 watch dog
{
		if(TIM_GetITStatus(TIM2 , TIM_IT_Update) == SET)
		{
      TIM2->CNT = 0;
      TIM2->CR1 &= (uint16_t)~TIM_CR1_CEN;//TIM_Cmd(TIM2, DISABLE);
 			TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
 		}
}

/*
@ abstract control per pwm GPIO interface , ON or OFF
*/
void TIM3_IRQHandler(void)    // usart2 watch dog
{
 		if(TIM_GetITStatus(TIM3 , TIM_IT_Update) == SET)
 		{
      TIM3->CNT = 0;
      TIM3->CR1 &= (uint16_t)~TIM_CR1_CEN;//TIM_Cmd(TIM3, DISABLE);
 			TIM_ClearITPendingBit(TIM3 , TIM_IT_Update);
 		}
}

/*
@ abstract control per pwm GPIO interface , ON or OFF
*/
void TIM4_IRQHandler(void)   // usart3 watch dog
{
//  		if(TIM_GetITStatus(TIM4 , TIM_IT_Update) == SET)
//  		{
//                     if(SemUart3 != OS_EVENT_NULL)
//                         OSSemPost(SemUart3);

//                     TIM4->CNT = 0;
//                     TIM4->CR1 &= (uint16_t)~TIM_CR1_CEN;//TIM_Cmd(TIM4, DISABLE);

//  			TIM_ClearITPendingBit(TIM4 , TIM_IT_Update);
//  		}
	
	     TIM4->CNT = 0;
       TIM4->CR1 &= (uint16_t)~TIM_CR1_CEN;//TIM_Cmd(TIM4, DISABLE);
}


void DMA1_Channel4_IRQHandler(void)
{
   if( DMA_GetITStatus(DMA1_IT_TC4) == SET )
	{
		DMA_ClearITPendingBit(DMA1_IT_TC4);
	}
}


//void TIM5_IRQHandler(void)   // IO PWM
//{   
//    if(TIM_GetITStatus(TIM5 , TIM_IT_Update) == SET)
//    {
//        TIM_ClearITPendingBit(TIM5 , TIM_IT_Update);
//    }
//}

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
