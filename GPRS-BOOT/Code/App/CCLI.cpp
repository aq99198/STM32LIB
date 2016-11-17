

#include "CCli.h"
#include <stdio.h>
#include <string.h>
#include "MsgFrame.h"

#define BuildList "JOUAV STBoot BuildTime " __DATE__" " __TIME__ "\r\n"
#define BOOTLOG1  "l********************************1\r\n"
#define BOOTLOG2  "s********************************5\r\n"
#define BOOTLOG3  "c********************************0\r\n"
//-----------------------------------------------------------------------
/**
  * @brief  
  * @param  None
  * @retval None
  * @note   None
  */        
CCli::CCli(CMsgFrame * frame) :msgFrame(frame)
{

}

/**
  * @brief  
  * @param  None
  * @retval None
  * @note   None
  */        
CCli::~CCli()
{

}


/**
  * @brief  用户字符界面
  * @param  None
  * @retval CLI_MODE，返回系统根据用户选择，决定当前启动的模式
  * @note   None
  */  
CLI_MODE CCli::cliInterface()
{
//  static UINT8 txbuf[256];
  static UINT8 rxbuf[256];
  PRINT(BOOTLOG1);
  PRINT(BOOTLOG2);
  PRINT(BuildList);
  PRINT(BOOTLOG3);
  
  //msgFrame->SendData(BuildList, strlen(BuildList));
  PRINT("Jouav Bootloader \r\n");  
#if 1
  UINT32 rlen = 255;
  UINT32 retry =5;
  do
  {
    rlen = 255;
    rlen =  msgFrame->ReadLine(rxbuf, rlen, 1*1000) ;
    --retry;

    if( rlen &&( !strcmp((char*)rxbuf,"CMD") ) )
    {       
      PRINT("Into command mode \r\n");
      break;      
    }
    else
    {
      PRINT("chose mode %d seconds\r\n",retry);
    }
  }while(retry);

  if( !retry )
  {
    return CLI_APP_MODE;
  }

  PRINT("JouavSTBoot: \r\n");
  while(1)
  {
    rlen = 255;
		rlen =  msgFrame->ReadLine(rxbuf, rlen, 1*1000) ;
    if( rlen )
    {
      if( !strcmp((char*)rxbuf,"down") )
      {
        PRINT("\r\nget command down \r\n");
        return CLI_UPDATE_MODE;
      }
      if( !strcmp((char*)rxbuf,"run") )
      {
        PRINT("get commmand run \r\n");
        return CLI_APP_MODE;
      }
      else
      {
        PRINT("\r\nJouavSTBoot:");
      }
      msgFrame->SendData(rxbuf, rlen);
    }
  }
#endif 

//  return CLI_APP_MODE;
}
