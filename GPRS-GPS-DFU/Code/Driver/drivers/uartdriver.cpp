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

#include "board.h"
#include "uartdriver.h"


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
            SemUart1 = OSSemCreate(0);
    		SemUartW1 = OSSemCreate(1);
			g_pResourceMutex = OSMutexCreate(APP_MUTEX_APLINKTX_PRIO, &g_u8Rerr);
        }
		break;
    case USART2_IDX:
        if( NUM_UART_IDX  == UartInit[USART2_IDX])
        {
		

            UartInit[USART2_IDX] = USART2_IDX;
            SemUart2  = OSSemCreate(0);
    		SemUartW2 = OSSemCreate(1);
        }
		break;
    case USART3_IDX:
        if( NUM_UART_IDX  == UartInit[USART3_IDX])
        {
            UartInit[USART3_IDX] = USART3_IDX;
            SemUart3 = OSSemCreate(0);
    		SemUartW3 = OSSemCreate(1);
        }
		break;
	case UART5_IDX:
		if( NUM_UART_IDX == UartInit[UART5_IDX])
		{
			UartInit[UART5_IDX] = UART5_IDX;
            SemUart5 = OSSemCreate(0);
    		SemUartW5 = OSSemCreate(1);
			MutexUartW5 = OSMutexCreate(APP_MUTEX_UART5_PRIO, &g_u8Rerr);
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
  uint8_t i;
	switch(uartIDX)
    {
		case USART1_IDX:
			#if USART_DMA
				OSSemPend(SemUartW1, 0, &g_u8Rerr);
				USART1_DMA(data,dataLen);
			#else
				for(i=0;i<dataLen;++i)
				{
					while(!((USART1->SR)&(1<<7)));
					USART1->DR=data[i];	
				}
			#endif
			break;

		case USART2_IDX:
			#if USART_DMA
				OSSemPend(SemUartW2, 0, &g_u8Rerr);
				USART2_DMA(data,dataLen);
			#else
				for(i=0;i<dataLen;++i)
				{
					while(!((USART2->SR)&(1<<7)));
					USART2->DR=data[i];	
				}
			#endif
			break;
	
		case USART3_IDX:
		#if USART_DMA
				OSSemPend(SemUartW3, 0, &g_u8Rerr);
				//USART3_DMA(data,dataLen);
			#else
				for(i=0;i<dataLen;++i)
				{
					while(!((USART3->SR)&(1<<7)));
					USART3->DR=data[i];	
				}
			#endif
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
		INT8U error;

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
				OSSemPend(SemUart1, 0, &error);
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
 				OSSemPend(SemUart2, 0, &error);
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
				OSSemPend(SemUart3, 0, &error);
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
				OSSemPend(SemUart5, 0, &error);
			}
			//break;
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

void CUartDriver::clearBuffer()
{
	  switch(uartIDX)
		{
		case USART1_IDX:
			UART1_Buffer.front = UART1_Buffer.end;
		break;	
		case USART2_IDX:
			UART2_Buffer.front = UART2_Buffer.end;
		break;	
		case USART3_IDX:
			UART3_Buffer.front = UART3_Buffer.end;
		break;	
		case UART5_IDX:
			UART5_Buffer.front = UART5_Buffer.end;
		break;
		default:
			break;
		}        
}

bool CUartDriver::waitForRead(uint32_t timeout )
{
//        INT32 n=0;
		INT8U error;

        switch(uartIDX)
        {
        case USART1_IDX:
        {

            OSSemPend(SemUart1, timeout, &error);
            if( OS_ERR_NONE != error )
            {
                return false;
            }
            return true;
            //break;
        }

        case USART2_IDX:
        {

            OSSemPend(SemUart2, timeout, &error);
            if( OS_ERR_NONE != error )
            {
                return false;
            }
            return true;
            //break;
        }
        case USART3_IDX:
        {

            OSSemPend(SemUart3, timeout, &error);
            if( OS_ERR_NONE != error )
            {
                return false;
            }
            return true;
            //break;
        }
        }    
				
				return 0;
}

void CUartDriver::setBaudRate(int bd)
{
     switch(uartIDX)
    {
        case USART1_IDX:
			USART1_Init();
			break;
		case USART2_IDX:
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
               // USART_ITConfig(USART1,USART_IT_RXNE,DISABLE);
 		break;
        	case USART2_IDX:
               // USART_ITConfig(USART2,USART_IT_RXNE,DISABLE);
 		break;
        	case USART3_IDX:
               // USART_ITConfig(USART3,USART_IT_RXNE,DISABLE);
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
               // USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
 		break;
        	case USART2_IDX:
               // USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
 		break;
        	case USART3_IDX:
               // USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);
 		break;
        	
              default:
               break;
	}
}


