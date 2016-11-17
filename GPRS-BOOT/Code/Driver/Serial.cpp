/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


------------- Copyright (C) 2008-2010  Ren Bin --------------------------------
This class is mainly for uart driver

HISTORY
--------------------------------------------------------------------------------

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
SENTRY
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#include "stm32f10x_it.h"
#include "usart.h"
#include "Serial.h"


CSerial::CSerial(SERIAL_COM com,UINT32 bd):serailcom(com)
{  
}
CSerial::~CSerial(void)
{
}


void CSerial::write(UINT8 *data, UINT32 dataLen)
{
	//!< 关于数据发送，使用忙等查询进行单个字节的发送，使用同步操作，则无法使用CPU进行其他操作
	//!< 如果使用DMA进行数据发送，则可以提供提高操作效率
	//!< 发送操作的代码优化则是使用DMA发送替代单字节发送操作
   switch(serailcom)
    {
    case SERIAL_COM1:
	    {
            for(INT32 i=0 ;i != dataLen ; ++i )
            {
                COM1_SendByte(data[i]);
            }
	    }
	break;
	case SERIAL_COM2:
        {
            for(INT32 i=0 ;i != dataLen ; ++i )
            {
                COM2_SendByte(data[i]);
            }			
	    }
	break;
	case SERIAL_COM3:
		{
            for(INT32 i=0 ;i != dataLen ; ++i )
            {
                COM3_SendByte(data[i]);
            }			
		}
	break;
	case SERIAL_COM4:
		{               
            for(INT32 i=0 ;i != dataLen ; ++i )
            {
                COM4_SendByte(data[i]);
            }			
		}
	break;
	case SERIAL_COM5:
		{
            for(INT32 i=0 ;i != dataLen ; ++i )
            {
                COM5_SendByte(data[i]);
            }			
		}
	break;
	case SERIAL_COM6:
		{
            for(INT32 i=0 ;i != dataLen ; ++i )
            {
                COM6_SendByte(data[i]);
            }			
		}
	break;
	case SERIAL_COM8:
		{
            for(INT32 i=0 ;i != dataLen ; ++i )
            {
                COM8_SendByte(data[i]);
            }			
		}
	break;    
	}
}



UINT8 CSerial::getByte()
{

	UINT8 data ;
	switch(serailcom)
	{
		case SERIAL_COM1:
		{
			data=UART1_Buffer.buffer[UART1_Buffer.front++];
		}
		break;
		case SERIAL_COM2:
		{
			data=UART2_Buffer.buffer[UART2_Buffer.front++];
		}
		break;
		case SERIAL_COM3:
		{
			data=UART3_Buffer.buffer[UART3_Buffer.front++];
		}
		break;
		case SERIAL_COM4:
		{
			data=UART4_Buffer.buffer[UART4_Buffer.front++];	
		}
		break;
		case SERIAL_COM5:
		{
			data=UART5_Buffer.buffer[UART5_Buffer.front++];
		}
		break;
		case SERIAL_COM6:
		{
			data=UART6_Buffer.buffer[UART6_Buffer.front++];
		}
		break;
		case SERIAL_COM8:
		{
			data=UART8_Buffer.buffer[UART8_Buffer.front++];
		}
		break;        
	}
	return data;
}

/**
  * @brief  
  * @param  None
  * @retval None
  * @note   None
  */    
bool CSerial::available()
{
    bool ret = false;
    switch(serailcom)
    {
        case SERIAL_COM1:
        {
            ret = ( UART1_Buffer.front!=UART1_Buffer.end) ;  
        }
        break;
        case SERIAL_COM2:
        {
            ret = ( UART2_Buffer.front!=UART2_Buffer.end) ;
        }
        break;
        case SERIAL_COM3:
        {
            ret = ( UART3_Buffer.front!=UART3_Buffer.end) ;
        }
        break;
        case SERIAL_COM4:
        {
            ret = ( UART4_Buffer.front!=UART4_Buffer.end) ;
        }
        break;
        case SERIAL_COM5:
        {
            ret = ( UART5_Buffer.front!=UART5_Buffer.end) ;
        }
        break;
        case SERIAL_COM6:
        {
            ret = ( UART6_Buffer.front!=UART6_Buffer.end) ;
        }
        break;
        case SERIAL_COM8:
        {
            ret = ( UART8_Buffer.front!=UART8_Buffer.end) ;
        }
        break;        
    }

    return ret ;

}


/**
  * @brief  刷新接收缓冲区
  * @param  None
  * @retval None
  * @note   None
  */  
void CSerial::flush()
{
    switch(serailcom)
    {   
        case SERIAL_COM1:
        {
            ( UART1_Buffer.front = UART1_Buffer.end = 0 ) ;  
        }
        break;
        case SERIAL_COM2:
        {
             UART2_Buffer.front = UART2_Buffer.end = 0 ;
        }
        break;
        case SERIAL_COM3:
        {
            ( UART3_Buffer.front = UART3_Buffer.end = 0) ;
        }
        break;
        case SERIAL_COM4:
        {
            ( UART4_Buffer.front = UART4_Buffer.end = 0) ;
        }
        break;
        case SERIAL_COM5:
        {
            ( UART5_Buffer.front = UART5_Buffer.end = 0) ;
        }
        break;
        case SERIAL_COM6:
        {
            ( UART6_Buffer.front = UART6_Buffer.end = 0) ;
        }
        break;
        case SERIAL_COM8:
        {
            ( UART8_Buffer.front = UART8_Buffer.end = 0) ;
        }
        break;        
    }    

}


void CSerial::disableRxInterrupt()
{

	switch (serailcom)
	{
	    case SERIAL_COM1:
                USART_ITConfig(USART1,USART_IT_RXNE,DISABLE);
 		break;
        case SERIAL_COM2:
                USART_ITConfig(USART2,USART_IT_RXNE,DISABLE);
 		break;
        case SERIAL_COM3:
                USART_ITConfig(USART3,USART_IT_RXNE,DISABLE);
 		break;
        case SERIAL_COM4:
                USART_ITConfig(UART4,USART_IT_RXNE,DISABLE);
 		break;
        case SERIAL_COM5:
                USART_ITConfig(UART5,USART_IT_RXNE,DISABLE);
 		break;
        case SERIAL_COM6:
                USART_ITConfig(USART6,USART_IT_RXNE,DISABLE);
 		break;
        case SERIAL_COM8:
                USART_ITConfig(UART8,USART_IT_RXNE,DISABLE);
 		break;        
        default:
        break;
	}

}
    

void CSerial::enableRxInterrupt()
{

	switch (serailcom)
	{
        case SERIAL_COM1:
        	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
        break;
        case SERIAL_COM2:
        	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
        break;
        case SERIAL_COM3:
        	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);
        break;
        case SERIAL_COM4:
        	USART_ITConfig(UART4,USART_IT_RXNE,ENABLE);
        break;
        case SERIAL_COM5:
        	USART_ITConfig(UART5,USART_IT_RXNE,ENABLE);
        break;
        case SERIAL_COM6:
        	USART_ITConfig(USART6,USART_IT_RXNE,ENABLE);
        break;
        case SERIAL_COM8:
        	USART_ITConfig(UART8,USART_IT_RXNE,ENABLE);
        break;
        default:            
        break;
	}
}


