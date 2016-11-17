/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Header:       UartDriver.h
Author:       Ren Bin
Date started: 2008/11/09

This class is mainly for uart driver

HISTORY
--------------------------------------------------------------------------------

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
SENTRY
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
#ifndef __UART_DRIVER_H__
#define __UART_DRIVER_H__
 
#include <string.h>
#include "JacTypeDef.h"

enum UCOS_UART_INDEX
{
	USART1_IDX=0,
	USART6_IDX,
	USART2_IDX,
	UART4_IDX,
	UART5_IDX,
	USART3_IDX,
	NUM_UART_IDX
};

class CUartDriver
{
public:
	CUartDriver(UINT16 uartIndex);
	~CUartDriver(void);
    
	void write(UINT8 *data, int dataLen);
	void read( UINT8 *data, int *dataLen);
  void flush();
  bool available();
    

	int ready(void){return isReady;};

	// empty methods
	void setBaudRate(int bd);
  UINT32 getBaudRateByConfig(int config);	

	void disableInterrupt();
	void enableInterrupt();
	
private:
	UINT16 uartIDX;
	int isReady;
	static UCOS_UART_INDEX  UartInit[NUM_UART_IDX];
};

#endif
