/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


This class is mainly for uart driver

HISTORY
--------------------------------------------------------------------------------

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
SENTRY
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
#ifndef __UART_DRIVER_H__
#define __UART_DRIVER_H__

#include "JacTypeDef.h"


enum SERIAL_COM
{
	SERIAL_COM1=0,
	SERIAL_COM2,
	SERIAL_COM3,
	SERIAL_COM4,
	SERIAL_COM5,
	SERIAL_COM6,
	SERIAL_COM7,
	SERIAL_COM8,
	NUM_COM
};

#define FILESTREAM            (SERIAL_COM8)

//!< 
class CSerial
{
public:
/**
  * @brief  
  * @param  None
  * @retval None
  * @note   None
  */	
	CSerial(SERIAL_COM com,UINT32 bd);

/**
  * @brief  
  * @param  None
  * @retval None
  * @note   None
  */	
	~CSerial(void);

/**
  * @brief  
  * @param  None
  * @retval None
  * @note   None
  */	
	void write(UINT8 *data, UINT32 dataLen);

/**
  * @brief  
  * @param  None
  * @retval None
  * @note   None
  */	
  UINT8 getByte();

/**
  * @brief  
  * @param  None
  * @retval None
  * @note   None
  */  	
  	bool available();

/**
  * @brief  刷新接收缓冲区
  * @param  None
  * @retval None
  * @note   None
  */  
  void flush();    
/**
  * @brief  
  * @param  None
  * @retval None
  * @note   None
  */
  	void disableRxInterrupt();

/**
  * @brief  
  * @param  None
  * @retval None
  * @note   None
  */  	
  	void enableRxInterrupt();
	
private:
	SERIAL_COM serailcom;
};

#endif

