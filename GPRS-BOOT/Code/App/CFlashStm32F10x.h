



#ifndef FLASH_STM32F10X_H
#define FLASH_STM32F10X_H

#include "CFlash.h"



class CFlashStm32F10X:public CFlash
{
public:

/**
  * @brief  
  * @param  None
  * @retval None
  * @note   None
  */    
    CFlashStm32F10X();

/**
  * @brief  
  * @param  None
  * @retval None
  * @note   None
  */    
    ~CFlashStm32F10X();

public:

  /**
  * @brief  获取支持最大APP Flash大小
  * @param  None
  * @retval UINT32 ，返回当前支持最大的APP文件大小，单位字节
  * @note   None
  */    
  virtual UINT32 getSysSize();

/**
  * @brief  获得APP 启动地址
  * @param  None
  * @retval UINT32 ，启动地址
  * @note   None
  */   
  virtual UINT32 getAppStart();

/**
  * @brief  擦除制定大小的APP Flash区域，为写入App做准备
  * @param  None
  * @retval bool 擦除成功，返回true；擦除失败，返回false
  * @note   app的大小必须要不大于支持最大APP的大小
  */    
  virtual bool eraseAppFlash(UINT32 appsize);

/**
  * @brief  将APP写入Flash
  * @param  offset,写入地址偏移，单位字节
            *app  ,需要写入的app缓冲区
            len   ,需要写入的数据长度
  * @retval bool  写入成功返回true；写入失败，返回false
  * @note   None
  */    
  virtual bool programApptoFlash(UINT32 offset,UINT8 *app,UINT32 len );      


/**
  * @brief  
  * @param  None
  * @retval None
  * @note   None
  */   
  virtual void flashEraseProgramDebug();

};




#endif 
