

#include "stm32f10x_usart.h"
#include <stdio.h>
#include <rt_misc.h>

#pragma import(__use_no_semihosting_swi)

int SendChar(int ch); // 声明外部函数，在main文件中定义

int GetKey(void);

struct __FILE
{

    int handle; // Add whatever you need here

};

FILE __stdout;

FILE __stdin;

int fputc(int ch, FILE *f)
{

    return (SendChar(ch));

}

int fgetc(FILE *f)
{

    return (SendChar(GetKey()));

}

void _ttywrch(int ch)
{

    SendChar (ch);

}

int ferror(FILE *f)   // Your implementation of ferror
{

    return EOF;

}

void _sys_exit(int return_code)
{

label:
    goto label; // endless loop

}



/* Usart functions ---------------------------------  ------------------------*/


#if 1
#define   PRINTF_USART_PORT   USART1

int SendChar (int ch)
{

    while (!(PRINTF_USART_PORT->SR & USART_FLAG_TXE)); // USART1

    PRINTF_USART_PORT->DR = (ch & 0x1FF);

    return (ch);

}

int GetKey (void)
{

    while (!(PRINTF_USART_PORT->SR & USART_FLAG_RXNE));

    return ((int)(PRINTF_USART_PORT->DR & 0x1FF));

}

#endif

