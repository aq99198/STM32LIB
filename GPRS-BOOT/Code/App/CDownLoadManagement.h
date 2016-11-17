

#ifndef CDOWNLOAD_MANAGEMENT_H
#define CDOWNLOAD_MANAGEMENT_H

#include "JacTypeDef.h"


class CIAPprotocol ;
class CFlash;

//!< 管理下载流程，检查下载文件大小，管理下载文件分块发送，管理如何分块下载数据和烧写

class CDownLoadManagement
{

public:
/**
  * @brief  
  * @param  None
  * @retval None
  * @note   None
  */    
  CDownLoadManagement( CIAPprotocol * iap ,CFlash * flash);

/**
  * @brief  
  * @param  None
  * @retval None
  * @note   None
  */     
  ~CDownLoadManagement();

/**
  * @brief  进行下载流程管理，调用此方法，将负责从搜索到下载完成整个操作，直至整个操作完成，否则不会返回
  * @param  None
  * @retval bool , 返回真，则下载操作流程操作完毕；否则不返回
  * @note   None
  */   
  bool doDownload();

public:


/**
  * @brief  跳转到应用程序
  * @param  None
  * @retval None
  * @note   None
  */
  void jumpToApp(); 

private:
    
private:
    CIAPprotocol *  iapprotocol; //!< 下载通信协议接口 
    CFlash * appflash; //!< 下载app 编程接口

};



#endif 


