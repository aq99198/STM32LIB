
#include "usart.h"
#include "JacTypeDef.h"
#include <stdio.h>
#include "stm32f10x_usart.h"
#include "stm32f10x_dma.h"
#include "misc.h"

void USART1_Init(void)
{
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

  USART_Cmd(USART1, DISABLE);

	//config USART1
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
}
void USART2_Init(void)
{

	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
  //DMA_InitTypeDef  DMA_InitStructure;

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

//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_Cmd(USART2, ENABLE);

}
void USART3_Init(void)
{
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

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
}


/* For SIM900A */
void UART5_Init(void)
{
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

  USART_Cmd(UART5, DISABLE);

	//config USART1
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


