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
		CUartDriver(uint16_t uartIndex);
		~CUartDriver(void);
			
		void write(uint8_t *data, int dataLen);
		void read(uint8_t *data, int *dataLen);
		bool available();
		void clearBuffer();
			
		bool waitForRead(uint32_t timeout=0 );
		int ready(void){return isReady;};

		// empty methods
		void setBaudRate(int bd);
		uint32_t getBaudRateByConfig(int config);	

		void disableInterrupt();
		void enableInterrupt();
		
	private:
		uint16_t uartIDX;
		int isReady;
		static UCOS_UART_INDEX  UartInit[NUM_UART_IDX];
};


extern CUartDriver * SerialConsole ;

#define BulidTime "" __DATE__" " __TIME__ ""

#define DEBUG_INFO   
#ifdef  DEBUG_INFO

		#define INFO(x)            do{SerialConsole->write((uint8_t*)"[INFO]  ",9); \
																	 SerialConsole->write((uint8_t*)x,strlen((const char*)x));}while(0)
    #define ERROR(x)            do{SerialConsole->write((uint8_t*)"[ERROR]  ",7); \
																	 SerialConsole->write((uint8_t*)x,strlen((const char*)x));}while(0)
    #define DEBUG(x)            do{SerialConsole->write((uint8_t*)"[DEBUG]  ",7); \
																	 SerialConsole->write((uint8_t*)x,strlen((const char*)x));}while(0)
		#define PRINT(format,args...) printf(format,##args)

#else

    #define ERROR(x)            do{}while(0)
    #define DEBUG(x)            do{}while(0)
		#define PRINT(format,args...) do{}while(0)

#endif 

#endif

