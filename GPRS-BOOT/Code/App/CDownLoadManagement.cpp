



#include "CDownLoadManagement.h"
#include "IAPprotocol.h"
#include "CFlashStm32F10x.h"
#include <stdio.h>


typedef void (*iapfun)(void); 
__asm void setMsrAndMsp(unsigned int  addr)
{
        MSR MSP, r0
        BX r14
}

#define DOWNLOAD_SECTOR  (1024)  //!< 分块下载文件，单个文件块大小，单位字节 




/**
  * @brief  
  * @param  None
  * @retval None
  * @note   None
  */    
CDownLoadManagement::CDownLoadManagement( CIAPprotocol * iap ,CFlash * flash ):iapprotocol(iap),appflash(flash)
{

}

/**
  * @brief  
  * @param  None
  * @retval None
  * @note   None
  */     
CDownLoadManagement::~CDownLoadManagement()
{

}




/**
  * @brief  跳转到应用程序
  * @param  None
  * @retval None
  * @note   None
  */
void CDownLoadManagement::jumpToApp()
{
  UINT32 addr = appflash->getAppStart();
  iapfun jump2app;
  jump2app = (iapfun)*(unsigned int*)(addr+4);
  setMsrAndMsp(*(unsigned int*)addr);
  jump2app();
}



/**
  * @brief  
  * @param  None
  * @retval None
  * @note   None
  */   
bool CDownLoadManagement::doDownload()
{
    static UINT8 downAppBuf[DOWNLOAD_SECTOR]; //!< 下载文件缓冲区

    PRINT("\r\n***NOTE!!!***\r\n");
    PRINT("Addr Map : From 0X%08X Bytes, Size %d Bytes\r\n\r\n", appflash->getAppStart(),appflash->getSysSize());

    //!<搜索
    while(!iapprotocol->SendSearchWaitResp());

    //!< 数据文件头--长度
    UINT32 filelen;
    while(!iapprotocol->SendFileHeadRqstWaitResp(filelen) );

    UINT32 appmaxsize = appflash->getSysSize();

    UINT32 downcmd = 0;

    if( appmaxsize < filelen )
    {
      downcmd = 0 ; //!< 大小超限，
    }
    else
    {
      downcmd = 1; //!< 大小合适，可以下载
    }

    if( downcmd )
    {
      //!< 擦除原APP
      appflash->eraseAppFlash(filelen);
    }
    else
    {
      return false;
    }

     //!< 下载传输准备
    while( !iapprotocol->SendDownTransStarWaitResp(downcmd) ) ;



    
    //!< 开始下载传输，并烧写文件

    UINT32 multiple = filelen/DOWNLOAD_SECTOR;
    UINT32 i;
    for( i= 0 ; i != multiple ; i++ )
    {
      while( !iapprotocol->SendDownTrasnRqstWaitResp(i*DOWNLOAD_SECTOR, DOWNLOAD_SECTOR, downAppBuf) ) ;
      while( !appflash->programApptoFlash(i*DOWNLOAD_SECTOR, downAppBuf,DOWNLOAD_SECTOR) ) ;
    }

    UINT32 modelength = filelen%DOWNLOAD_SECTOR;
    if( modelength > 0 )
    {
      while( !iapprotocol->SendDownTrasnRqstWaitResp(i*DOWNLOAD_SECTOR, modelength, downAppBuf) ) ;
      while( !appflash->programApptoFlash(i*DOWNLOAD_SECTOR, downAppBuf,modelength) ) ;
    }


    //!<  停止传输指令，下载并烧写完成
    while( !iapprotocol->SendDownStopWaitResp() );

		
    return true;

}
