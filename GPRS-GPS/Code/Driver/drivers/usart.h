#ifndef __USART_H
#define __USART_H


#ifdef  __cplusplus
extern "C" {
#endif

typedef struct
{
 u8 buffer[256];
 u8 front;
 u8 end;
}Ring_BufferTypeDef;



	
void USART_gpio(void);
void USART1_Init(void);
void USART2_Init(void);
void USART3_Init(void);
void UART5_Init(void);
void USART1_DMA(unsigned char * data , int len);
void USART3_DMA(unsigned char * data , int len);
void USART2_DMA(unsigned char * data , int len);
void UART2_send_byte(uint8_t byte);
void UART1_send_byte(uint8_t byte);
void UART3_send_byte(uint8_t byte);
void UART5_send_byte(uint8_t byte);
void TimeDly(__IO uint32_t nCount);
void UART3_Send(uint8_t *Buffer, uint32_t Length);
void UART2_Send(uint8_t *Buffer, uint32_t Length);
void DMA1_Channel4_IRQHandler(void);
void DMA1_Channel6_IRQHandler(void);

void USART2_IRQHandler(void);
void USART2_SetBaudRate(int bd);

#ifdef  __cplusplus
}
#endif

#ifdef __cplusplus
    extern "C" Ring_BufferTypeDef UART1_Buffer;
    extern "C" Ring_BufferTypeDef UART2_Buffer;
    extern "C" Ring_BufferTypeDef UART3_Buffer;
	extern "C" Ring_BufferTypeDef UART5_Buffer;
    extern "C"{
#else
    extern Ring_BufferTypeDef UART1_Buffer;
    extern Ring_BufferTypeDef UART2_Buffer;
    extern Ring_BufferTypeDef UART3_Buffer;
	extern Ring_BufferTypeDef UART5_Buffer;
#endif

#ifdef __cplusplus
    }
#endif 

#endif /* end of _USART_H_ */
/* end of file */


