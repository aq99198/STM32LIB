


#ifndef CFLASH_H
#define CFLASH_H
#include "JacTypeDef.h"


//!< 定义整个芯片的 APP软件所可烧写的区域
//!< 提供flash的擦除操作、写操作、读操作
//!< 1、查询APP flash的起始地址及大小
//!< 2、按块擦除接口
//!< 3、写入接口


//!< 操作流程为，1、检查APP BIN的大小与FLASH的大小，是否合格；如果合格，则根据BIN大小选择最小包含分区进行擦除操作，否则通知无法下载。
//!< 2、开始写入数据

//!< 此为标准任意平台提供的接口，实现该接口，则可以实现对ST单片机可运行APP的FLASH抽象接口

class CFlash
{
public:

/**
  * @brief  
  * @param  None
  * @retval None
  * @note   None
  */    
  CFlash();

/**
  * @brief  
  * @param  None
  * @retval None
  * @note   None
  */    
  ~CFlash();

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

private:

};


#endif 
