/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Header:       UartDriver.cpp
Author:       Ren Bin
Date started: 2008/11/09

------------- Copyright (C) 2008-2010  Ren Bin --------------------------------
This class is mainly for uart driver

HISTORY
--------------------------------------------------------------------------------

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
SENTRY
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#include "uartdriver.h"
#include "stm32f10x_usart.h"
#include "stm32f10x.h" 
#include "usart.h"
#include "stm32f10x_it.h"




CUartDriver * SerialConsole =0;

UCOS_UART_INDEX  CUartDriver::UartInit[NUM_UART_IDX]={NUM_UART_IDX,NUM_UART_IDX,NUM_UART_IDX,
                                                      NUM_UART_IDX,NUM_UART_IDX,NUM_UART_IDX};


CUartDriver::CUartDriver(u16 uartIndex)
{
    uartIDX=uartIndex;

    switch(uartIDX)
    {
    case USART1_IDX:
        if( NUM_UART_IDX  == UartInit[USART1_IDX])
        {

            UartInit[USART1_IDX] = USART1_IDX;
        }
		break;
    case USART2_IDX:
        if( NUM_UART_IDX  == UartInit[USART2_IDX])
        {	
            UartInit[USART2_IDX] = USART2_IDX;
        }
		break;
    case USART3_IDX:
        if( NUM_UART_IDX  == UartInit[USART3_IDX])
        {
            UartInit[USART3_IDX] = USART3_IDX;
        }
		break;
	case UART5_IDX:
		if( NUM_UART_IDX == UartInit[UART5_IDX])
		{
			UartInit[UART5_IDX] = UART5_IDX;
		}
		break;
	default:
    	break;
    }

	isReady=1;
}
CUartDriver::~CUartDriver(void)
{
}


void CUartDriver::write(u8 *data, int dataLen)
{
  UINT16 i;
	switch(uartIDX)
    {
		case USART1_IDX:
			for(i=0;i<dataLen;++i)
			{
				UART1_send_byte(data[i]);
			}
			break;

		case USART2_IDX:
			for(i=0;i<dataLen;++i)
			{
				UART2_send_byte(data[i]);
			}
			break;
	
		case USART3_IDX:
			for(i=0;i<dataLen;++i)
			{
				UART3_send_byte(data[i]);
			}
			break;
			
		case UART5_IDX:
			for(i=0;i<dataLen;++i)
			{
				UART5_send_byte(data[i]);
			}	
			break;
		}
}

void CUartDriver::read(u8 *data, int *dataLen)
{
		int n=0;

		switch(uartIDX)
    		{
    		case USART1_IDX:
			while(1)
			{

				while(UART1_Buffer.front !=UART1_Buffer.end)
				{
					data[n++]=UART1_Buffer.buffer[UART1_Buffer.front++];
					//UART1_Buffer.front &= 0xFF;
					if(n>=dataLen[0])
					{
						return ;
					}
				}
			}
			//break;
	
		case USART2_IDX:
			while(1)
			{

 				while(UART2_Buffer.front !=UART2_Buffer.end)
 				{
 					data[n++]=UART2_Buffer.buffer[UART2_Buffer.front++];
 					UART2_Buffer.front &=0xFF;
 					if(n>=dataLen[0])
 					{
 						return ;
 					}
 				}
			}
			//break;
		
		case USART3_IDX:
			while(1)
			{

				while(UART3_Buffer.front !=UART3_Buffer.end)
				{
					data[n++]=UART3_Buffer.buffer[UART3_Buffer.front++];
					UART3_Buffer.front &=0xFF;
					if(n>=dataLen[0])
					{
						return ;
					}
				}
			}
			//break;
			
		case UART5_IDX:
			while(1)
			{
				while(UART5_Buffer.front !=UART5_Buffer.end)
				{
					data[n++]=UART5_Buffer.buffer[UART5_Buffer.front++];
					UART5_Buffer.front &=0xFF;
					if(n>=dataLen[0])
					{
						return ;
					}
				}
			}
			//break;
		}
}

void CUartDriver::flush()
{

		switch(uartIDX)
    {
    	case USART1_IDX:
			{
				UART1_Buffer.front = UART1_Buffer.end = 0;
	      break;
			}
			
		case USART2_IDX:
			{
				UART2_Buffer.front = UART2_Buffer.end = 0;
        break;
			}
		case USART3_IDX:
			{
				UART3_Buffer.front = UART3_Buffer.end = 0;
        break;
			}
			
		case UART5_IDX:
			{
				UART5_Buffer.front = UART5_Buffer.end = 0;
			  break;
			}
    default:
			break;
		}        
}


bool CUartDriver::available()
{

		switch(uartIDX)
    		{
    		case USART1_IDX:
			{
				if(UART1_Buffer.front !=UART1_Buffer.end)
				{
                    return true;
				}
        else
        {
                    return false;
        }
	            //break;
			}
			
		case USART2_IDX:
			{
 				if(UART2_Buffer.front !=UART2_Buffer.end)
 				{
                    return true;
 				}
 				else
 				{
                    return false;
 				}
                //break;
			}
		case USART3_IDX:
			{
				if(UART3_Buffer.front !=UART3_Buffer.end)
				{
					return true;
				}
				else
				{
          return false;
				}
                //break;
			}
			
		case UART5_IDX:
			{
				if(UART5_Buffer.front !=UART5_Buffer.end)
				{
					return true;
				}
				else
				{
                    return false;
				}
                //break;
			}
        default:
            return false;
		} 
        
}

void CUartDriver::setBaudRate(int bd)
{
     switch(uartIDX)
    {
    case USART1_IDX:
			USART1_Init();
			break;
		case USART2_IDX:
			USART2_Init();
			break;
		case USART3_IDX:
			USART3_Init();
			break;
		case UART5_IDX:
			UART5_Init();
			break;
	}
}

u32 CUartDriver::getBaudRateByConfig(int config)
{
    return 0;
}

void CUartDriver::disableInterrupt()
{

	switch (uartIDX)
	{
		case USART1_IDX:
			USART_ITConfig(USART1,USART_IT_RXNE,DISABLE);
		break;
		case USART2_IDX:
			USART_ITConfig(USART2,USART_IT_RXNE,DISABLE);
 		break;
		case USART3_IDX:
			USART_ITConfig(USART3,USART_IT_RXNE,DISABLE);
 		break;
		default:
			break;
	}
}


void CUartDriver::enableInterrupt()
{

	switch (uartIDX)
	{
		case USART1_IDX:
			USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
 		break;
		case USART2_IDX:
			USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
 		break;
		case USART3_IDX:
			USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);
 		break;
		default:
			break;
	}
}


