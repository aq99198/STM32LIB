#ifndef __USART_H
#define __USART_H


#ifdef  __cplusplus
extern "C" {
#endif

void USART_gpio(void);
void USART1_Init(void);
void USART2_Init(int baud);
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
void Gprs(void );
void UART3_Send(uint8_t *Buffer, uint32_t Length);
void UART2_Send(uint8_t *Buffer, uint32_t Length);
void DMA1_Channel4_IRQHandler(void);

#ifdef  __cplusplus
}
#endif

#endif /* end of _USART_H_ */
/* end of file */


