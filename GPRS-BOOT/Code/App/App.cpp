

#include "App.h"
#include "CCLI.h"
#include "MsgFrame.h"
#include "CClock.h"
#include "IAPprotocol.h"
#include "CDownLoadManagement.h"
#include "CFlashStm32F10x.h"
#include "UartDriver.h"
#include <stdio.h>

/**
  * @brief  
  * @param  None
  * @retval None
  * @note   None
  */	
int CApp::mainloop(void) 
{
  CUartDriver * serial = new CUartDriver(USART1_IDX);
  Cclock  * clk    = new Cclock(1000);
  CMsgFrame *frame = new CMsgFrame(serial, clk);
  CCli * cli = new CCli(frame);
  CIAPprotocol *iap = new CIAPprotocol(frame);
  CFlash * appflash = new CFlashStm32F10X();
  
  CDownLoadManagement * downmanage = new CDownLoadManagement(iap, appflash);
	
//	int i=0;
//	char *src =(char*)0x08020000;
//	printf("\r\n");
//	for(i=0;i<0x10000;i++)
//	{
//		printf("%.2x ",src[i]);
//		if((i&0x1F) == 0x1F)
//			printf("\r\n");
//	}
	
  switch( cli->cliInterface())
  {
    case CLI_UPDATE_MODE:
    {
			//appflash->flashEraseProgramDebug();
      PRINT("down mode now , wating ...\n");
      //iap->DownLoadCommDebug();
      downmanage->doDownload();
      while(1)
      {
        PRINT("Down laod OK\r\n");
        clk->Delay(1000);
				downmanage->jumpToApp();
      }
    }
//    break;
    case CLI_APP_MODE:
    {
      PRINT("\r\n run application now ...\r\n\r\n\r\n");
      clk->Delay(1000);
      downmanage->jumpToApp();
    }
    break;
    default:
    break;
  }
  while(1)
  {
      __asm("nop ");
      __asm("nop ");
  }
//	return 1;
}

	
/**
  * @brief  
  * @param  None
  * @retval None
  * @note   None
  */	
CApp::CApp()
{

}

/**
  * @brief  
  * @param  None
  * @retval None
  * @note   None
  */	
CApp::~CApp()
{

}

