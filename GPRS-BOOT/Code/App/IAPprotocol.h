


//!< 设置软件IAP下载流程中，每个数据包的握手过程
#ifndef IAP_PROTOCOL_H
#define IAP_PROTOCOL_H

#include "JacTypeDef.h"



class CMsgFrame;
class CIAPprotocol
{
public:
/**
  * @brief  
  * @param  None
  * @retval None
  * @note   None
  */        
    CIAPprotocol(CMsgFrame * frame);

/**
  * @brief  
  * @param  None
  * @retval None
  * @note   None
  */        
    ~CIAPprotocol();

    //!< 下载协议操作流程所需要的通信协议

    
/**
  * @brief  搜索指令帧
  * @param  None
  * @retval bool，返回真，系统搜索到上位机；返回假，系统在规定时间内未搜索到上位机
  * @note   None
  */    
    bool SendSearchWaitResp();

/**
  * @brief  数据文件头请求帧
  * @param  filelen,作为返回参数使用，返回数据为请求的到需要下载的数据的长度
  * @retval bool ，返回真，系统请求数据长度成功；返回假，系统在规定时间内未成功请求到数据头
  * @note   None
  */    
  bool  SendFileHeadRqstWaitResp(UINT32 & filelen);
  

  /**
  * @brief  下载传输准备开始指令帧
  * @param  rqst,下载传输准备开始指令，值为0为通知上位机无法下载；值为1,为通知上位机准备数据，即将开始下载数据
  * @retval bool ，返回真，系统发送下载传输准备开始指令成功，返回假，系统在规定时间内没有成功请求操作指令。
  * @note   None
  */    
  bool SendDownTransStarWaitResp(UINT32 rqst);

  /**
  * @brief  数据下载传输帧
  * @param  addr,下载数据起始地址，单位字节
            len , 从当前其实地址开始，下载的长度
            data, 返回下载得到的数据缓冲区，缓冲区长度应不小于 len 
  * @retval bool, 返回真，系统请求下载数据成功；返回假，系统在规定时间内，下载数据失败
  * @note   None
  */    
  bool SendDownTrasnRqstWaitResp(UINT32 addr,UINT32 len,UINT8 * data);


  /**
  * @brief  停止下载指令帧
  * @param  None
  * @retval bool，返回真，系统发送传输完毕，停止传输通知指令成功；返回假，系统在规定时间内没有成功发送停止通知指令。
  * @note   None
  */    
  bool SendDownStopWaitResp();

  /**
  * @brief  None
  * @param  None
  * @retval None
  * @note   None
  */    
  bool DownLoadCommDebug();
private:
    CMsgFrame   *msgFrame;

};


#endif 
