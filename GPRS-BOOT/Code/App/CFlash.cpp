

#include "CFlash.h"



/**
  * @brief  
  * @param  None
  * @retval None
  * @note   None
  */    
CFlash::CFlash()
{

}

/**
  * @brief  
  * @param  None
  * @retval None
  * @note   None
  */    
CFlash::~CFlash()
{
    
}


/**
* @brief  获取支持最大APP Flash大小
* @param  None
* @retval UINT32 ，返回当前支持最大的APP文件大小，单位字节
* @note   None
*/    
UINT32 CFlash::getSysSize()
{return 0;}


/**
  * @brief  获得APP 启动地址
  * @param  None
  * @retval UINT32 ，启动地址
  * @note   None
  */   
UINT32 CFlash::getAppStart()
{
  return 0;
}

/**
* @brief  擦除制定大小的APP Flash区域，为写入App做准备
* @param  None
* @retval bool 擦除成功，返回true；擦除失败，返回false
* @note   app的大小必须要不大于支持最大APP的大小
*/   
bool CFlash::eraseAppFlash(UINT32 appsize)
{return false;}

/**
* @brief  将APP写入Flash
* @param  offset,写入地址偏移，单位字节
          *app  ,需要写入的app缓冲区
          len   ,需要写入的数据长度
* @retval bool  写入成功返回true；写入失败，返回false
* @note   None
*/    
bool CFlash::programApptoFlash(UINT32 offset,UINT8 *app,UINT32 len )
{return false;}

/**
* @brief  
* @param  None
* @retval None
* @note   None
*/   
void CFlash::flashEraseProgramDebug()
{}
