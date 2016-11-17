#include "board.h"

Ring_BufferTypeDef UART1_Buffer;
Ring_BufferTypeDef UART2_Buffer;
Ring_BufferTypeDef UART3_Buffer;
Ring_BufferTypeDef UART5_Buffer;

void USART_gpio(void){
		GPIO_InitTypeDef GPIO_InitStructure;
		GPIO_StructInit(&GPIO_InitStructure);
	
		#ifdef USART1_ON
		/* USART1 SerialConsole Tx(PA9) Rx(PA10)*/
		/* Configure USART1 Tx (PA9) as alternate function push-pull */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		/* Configure USART1 Rx (PA10) as input floating */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		#endif
	
		#ifdef USART2_ON
		/* Configure USART2 Tx (PA2) as alternate function push-pull */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_Init(GPIOA, &GPIO_InitStructure);

		/* Configure USART2 Rx (PA3) as input floating */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		#endif
	
		#ifdef USART3_ON
		/* USART3 ?? Tx(PC12) Rx(PD2)*/
		/* Configure USART3 Tx (PB10) as alternate function push-pull */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_Init(GPIOB, &GPIO_InitStructure);

		/* Configure USART3 Rx (PB11) as input floating */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		#endif
	
		#ifdef UART5_ON
		/* UART5 SIM900A Tx(PC12) Rx(PD2)*/
		/* Configure UART5 Tx (PC12) as alternate function push-pull */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_Init(GPIOC, &GPIO_InitStructure);
		
		/* Configure UART5 Rx (PD2) as input floating */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOD, &GPIO_InitStructure);
		#endif
	
}



// for Debug and FC
void USART1_Init(void)
{
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
  DMA_InitTypeDef  DMA_InitStructure;

  USART_Cmd(USART1, DISABLE);

	//config USART1  for Debug and FC
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE); //使能中断

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_Cmd(USART1, ENABLE);

	DMA_DeInit(DMA1_Channel4);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (unsigned int)&USART1->DR;
	DMA_InitStructure.DMA_MemoryBaseAddr = 0;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStructure.DMA_BufferSize = 0;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_MemoryDataSize_Byte;//DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;//DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;	

	DMA_Init(DMA1_Channel4, &DMA_InitStructure);      

	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;
 	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
 	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
 	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
 	NVIC_Init(&NVIC_InitStructure);
	
	DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, ENABLE);
	
	DMA_Cmd(DMA1_Channel4, ENABLE);
	
	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
	
	
	
}

void USART2_SetBaudRate(int bd){
	USART_InitTypeDef USART_InitStructure;

	//config USART2
	USART_InitStructure.USART_BaudRate = bd;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);
}


// for GPS
void USART2_Init(void)
{
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
  DMA_InitTypeDef  DMA_InitStructure;

  USART_Cmd(USART2, DISABLE);

	//config USART2
	USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No ;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART2, &USART_InitStructure);
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE); //使能中断

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_Cmd(USART2, ENABLE);


	DMA_Cmd(DMA1_Channel7, ENABLE);

	DMA_DeInit(DMA1_Channel7);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (unsigned int)&USART2->DR;
	DMA_InitStructure.DMA_MemoryBaseAddr = 0;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStructure.DMA_BufferSize = 0;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_MemoryDataSize_Byte;//DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;	

	DMA_Init(DMA1_Channel7, &DMA_InitStructure);      

	USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);

	DMA_ITConfig(DMA1_Channel7, DMA_IT_TC, ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel7_IRQn;
 	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
 	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
 	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
 	NVIC_Init(&NVIC_InitStructure);

}




void USART3_Init()
{
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
  DMA_InitTypeDef  DMA_InitStructure;

    USART_Cmd(USART3, DISABLE);

	//config USART3
	USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No ;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART3, &USART_InitStructure);
	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE); //使能中断

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);


 	USART_Cmd(USART3, ENABLE);
   
      RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    
 	  DMA_Cmd(DMA1_Channel2, ENABLE);
    
 	  DMA_DeInit(DMA1_Channel2);
 	  DMA_InitStructure.DMA_PeripheralBaseAddr = (unsigned int)&USART3->DR;//SPI2_DR_Base;
 	  DMA_InitStructure.DMA_MemoryBaseAddr = 0;//(unsigned int )SPI_Buffer_Tx;
 	  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;//DMA的转换模式是SRC模式，就是从外设向内存中搬运DMA_DIR_MemoryToPeripheral;
 	  DMA_InitStructure.DMA_BufferSize = 0;//sizeof(SPI_Buffer_Tx);
 	  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
 	  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
 	  DMA_InitStructure.DMA_PeripheralDataSize = DMA_MemoryDataSize_Byte;//DMA_PeripheralDataSize_HalfWord;
 	  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//DMA_MemoryDataSize_HalfWord;
 	  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;//DMA_Mode_Circular;
 	  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
 	  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;			 //M2M模式禁止
    
 	  DMA_Init(DMA1_Channel2, &DMA_InitStructure);		
    
    
 	  USART_DMACmd(USART3, USART_DMAReq_Tx, ENABLE);
    
 	USART_Cmd(USART3, ENABLE);
 	  DMA_Cmd(DMA1_Channel2,DISABLE);
 	  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel2_IRQn;
 	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
 	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
 	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
 	  NVIC_Init(&NVIC_InitStructure);
   
}


/* For SIM900A */
void UART5_Init(void)
{
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
    //DMA_InitTypeDef  DMA_InitStructure;

  USART_Cmd(UART5, DISABLE);
	
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(UART5, &USART_InitStructure);
	USART_ITConfig(UART5,USART_IT_RXNE,ENABLE); //使能中断

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
 	USART_Cmd(UART5, ENABLE);
}



void USART1_DMA(unsigned char *data , int len)
{
//	NVIC_InitTypeDef NVIC_InitStructure;
//  DMA_InitTypeDef  DMA_InitStructure;
//	
//	DMA_Cmd(DMA1_Channel4, DISABLE);

//	DMA_DeInit(DMA1_Channel4);
//	DMA_InitStructure.DMA_PeripheralBaseAddr = (unsigned int)&USART1->DR;
//	DMA_InitStructure.DMA_MemoryBaseAddr = (UINT32)data;
//	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
//	DMA_InitStructure.DMA_BufferSize = len;
//	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
//	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
//	DMA_InitStructure.DMA_PeripheralDataSize = DMA_MemoryDataSize_Byte;//DMA_PeripheralDataSize_HalfWord;
//	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//DMA_MemoryDataSize_HalfWord;
//	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;//DMA_Mode_Circular;
//	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
//	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;	

//	DMA_Init(DMA1_Channel4, &DMA_InitStructure);      

//	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;
// 	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
// 	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
// 	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
// 	NVIC_Init(&NVIC_InitStructure);
//	
//	DMA_Cmd(DMA1_Channel4, ENABLE);
//	
//	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
//	
//	DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, ENABLE);
	
	
	DMA_Cmd(DMA1_Channel4, DISABLE);
	DMA1_Channel4->CCR &= ~0x00000001;
	DMA1_Channel4->CMAR = (UINT32)data;
	DMA1_Channel4->CNDTR = len;
	DMA1_Channel4->CCR |= 0x00000001;
	DMA_Cmd(DMA1_Channel4, ENABLE);
}


void USART2_DMA(unsigned char * data , int len)
{
	DMA_Cmd(DMA1_Channel7, DISABLE);
	DMA1_Channel7->CCR &= ~0x00000001;
	DMA1_Channel7->CMAR = (UINT32)data;
	DMA1_Channel7->CNDTR = len;
	DMA1_Channel7->CCR |= 0x00000001;
	DMA_Cmd(DMA1_Channel7, ENABLE);
}


// USART1 DMA IRQHander
void DMA1_Channel4_IRQHandler(void)
{
   if( DMA_GetITStatus(DMA1_IT_TC4) == SET )
	 {
		if(SemUartW1 != OS_EVENT_NULL)
		{
			OSSemPost(SemUartW1);
		}
		DMA_ClearITPendingBit(DMA1_IT_TC4);
	}
}

void DMA1_Channel7_IRQHandler(void)
{
   if( DMA_GetITStatus(DMA1_IT_TC7) == SET )
	{
		DMA_ClearITPendingBit(DMA1_IT_TC7);
	    if(SemUartW2 != OS_EVENT_NULL)
		{
			OSSemPost(SemUartW2);
		}
	}
}


void USART1_IRQHandler(void)
{
	
	 if (USART_GetFlagStatus(USART1, USART_FLAG_ORE) != RESET)  
   {  
       //USART_ReceiveData(USART1);  
			 USART_ClearFlag(USART1, USART_FLAG_ORE);  
   }  
	
	 if (USART_GetFlagStatus(USART1, USART_FLAG_PE) != RESET)  
   {  
     USART_ClearFlag(USART1, USART_FLAG_PE);  
   }  
	 
	 if(USART_GetFlagStatus(USART1, USART_FLAG_NE) != RESET)
	 {
			USART_ClearFlag(USART1, USART_FLAG_NE);
	 }

      
   if (USART_GetFlagStatus(USART1, USART_FLAG_FE) != RESET)  
   {  
      USART_ClearFlag(USART1, USART_FLAG_FE);  
   }  
	
	
	 if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)//判断是不是接收中断
	 {
		UART1_Buffer.buffer[UART1_Buffer.end++]= USART_ReceiveData(USART1);
		UART1_Buffer.end &= 0xFF;
		if(SemUart1 != OS_EVENT_NULL)
		{
				OSSemPost(SemUart1);
		}

		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
	 }else{
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
		USART_ClearITPendingBit(USART1,USART_IT_CTS);
		USART_ClearITPendingBit(USART1,USART_IT_LBD);
		USART_ClearITPendingBit(USART1,USART_IT_TC);
	 }
}



void USART2_IRQHandler(void)
{
	
	 if (USART_GetFlagStatus(USART2, USART_FLAG_ORE) != RESET)  
   {  
       USART_ReceiveData(USART2);  
			 USART_ClearFlag(USART2, USART_FLAG_ORE);  
   }  
	
	 if (USART_GetFlagStatus(USART2, USART_FLAG_PE) != RESET)  
   {  
     USART_ClearFlag(USART2, USART_FLAG_PE);  
   }  
	 
	 if(USART_GetFlagStatus(USART2, USART_FLAG_NE) != RESET)
	 {
			USART_ClearFlag(USART2, USART_FLAG_NE);
	 }

      
   if (USART_GetFlagStatus(USART2, USART_FLAG_FE) != RESET)  
   {  
      USART_ClearFlag(USART2, USART_FLAG_FE);  
   }  
	
	
	 if(USART_GetITStatus(USART2, USART_IT_RXNE) == SET)//判断是不是接收中断
	 {
		UART2_Buffer.buffer[UART2_Buffer.end++]= USART_ReceiveData(USART2);
		UART2_Buffer.end &= 0xFF;
		if(SemUart2 != OS_EVENT_NULL)
		{
				OSSemPost(SemUart2);
		}

		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
	 }else{
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
		USART_ClearITPendingBit(USART2,USART_IT_CTS);
		USART_ClearITPendingBit(USART2,USART_IT_LBD);
		USART_ClearITPendingBit(USART2,USART_IT_TC);
	 }
}


void UART5_IRQHandler(void)
{
	
	 if (USART_GetFlagStatus(UART5, USART_FLAG_ORE) != RESET)  
   {  
       USART_ReceiveData(UART5);  
			 USART_ClearFlag(UART5, USART_FLAG_ORE);  
   }  
	
	 if (USART_GetFlagStatus(UART5, USART_FLAG_PE) != RESET)  
   {  
     USART_ClearFlag(UART5, USART_FLAG_PE);  
   }  
	 
	 if(USART_GetFlagStatus(UART5, USART_FLAG_NE) != RESET)
	 {
			USART_ClearFlag(UART5, USART_FLAG_NE);
	 }

      
   if (USART_GetFlagStatus(UART5, USART_FLAG_FE) != RESET)  
   {  
      USART_ClearFlag(UART5, USART_FLAG_FE);  
   }  
	
	
	 if(USART_GetITStatus(UART5, USART_IT_RXNE) == SET)//判断是不是接收中断
	 {
		UART5_Buffer.buffer[UART5_Buffer.end++]= USART_ReceiveData(UART5);
		UART5_Buffer.end &= 0xFF;
		if(SemUart5 != OS_EVENT_NULL)
		{
				OSSemPost(SemUart5);
		}

		USART_ClearITPendingBit(UART5,USART_IT_RXNE);
	 }else{
		USART_ClearITPendingBit(UART5,USART_IT_RXNE);
		USART_ClearITPendingBit(UART5,USART_IT_CTS);
		USART_ClearITPendingBit(UART5,USART_IT_LBD);
		USART_ClearITPendingBit(UART5,USART_IT_TC);
	 }
}





void UART2_send_byte(uint8_t byte) //发送1字节数据
{
     while(!((USART2->SR)&(1<<7)));
     USART2->DR=byte;	
}	

void UART1_send_byte(uint8_t byte) //发送1字节数据
{
     while(!((USART1->SR)&(1<<7)));
     USART1->DR=byte;	
}		


void UART3_send_byte(uint8_t byte)
{
     while(!((USART3->SR)&(1<<7)));
     USART3->DR=byte;	
}

void UART5_send_byte(uint8_t byte)
{
     while(!((UART5->SR)&(1<<7)));
     UART5->DR=byte;	
}


void TimeDly(__IO uint32_t nCount)
{
    for(; nCount != 0; nCount--);
}


void UART3_Send(uint8_t *Buffer, uint32_t Length)
{
    while(*Buffer != 0)
    {
        while(!((USART3->SR)&(1<<7)));//等待发送完
        USART3->DR= *Buffer;
        Buffer++;
        Length--;
    }
}

void UART2_Send(uint8_t *Buffer, uint32_t Length)
{
	while(Length != 0)
	{
		while(!((USART2->SR)&(1<<7)));//等待发送完
		USART2->DR= *Buffer;
		Buffer++;
		Length--;
	}
}




