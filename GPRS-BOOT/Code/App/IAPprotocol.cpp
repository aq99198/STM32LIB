

#include "IAPprotocol.h"
#include "MsgFrame.h"
#include <string.h>

static IAP_MSG_PKT  buf_iap_msg_pkt;
#define FRAME_SINGLE_TIMEOUT   (2*1000)
/**
  * @brief  
  * @param  None
  * @retval None
  * @note   None
  */        
CIAPprotocol::CIAPprotocol(CMsgFrame * frame):msgFrame(frame)
{

}

/**
  * @brief  
  * @param  None
  * @retval None
  * @note   None
  */        
CIAPprotocol::~CIAPprotocol()
{

}

    //!< 下载协议操作流程
/**
  * @brief  搜索指令帧
  * @param  None
  * @retval bool，返回真，系统搜索到上位机；返回假，系统在规定时间内未搜索到上位机
  * @note   None
  */    
bool CIAPprotocol::SendSearchWaitResp()
{
  msgFrame->clearRxBuf();
  while(1)
  {
      buf_iap_msg_pkt.head.type = 0;
      buf_iap_msg_pkt.head.Dest = 0x01;
      buf_iap_msg_pkt.head.length = 0;

      
      msgFrame->SendMsgPkt(buf_iap_msg_pkt);
      if( msgFrame->ReadMsgPkt(buf_iap_msg_pkt, FRAME_SINGLE_TIMEOUT) )
      {
          if( (buf_iap_msg_pkt.head.type==0)&&
              (buf_iap_msg_pkt.head.Dest==0)&&
              (buf_iap_msg_pkt.head.length==0)
            )
          {
              return true;
          }
      } 
  }

//  return false;
}

/**
  * @brief  数据文件头请求帧
  * @param  filelen,作为返回参数使用，返回数据为请求的到需要下载的数据的长度
  * @retval bool ，返回真，系统请求数据长度成功；返回假，系统在规定时间内未成功请求到数据头
  * @note   None
  */    
bool  CIAPprotocol::SendFileHeadRqstWaitResp(UINT32 & filelen)
{
  msgFrame->clearRxBuf();
  while(1)
  {
      buf_iap_msg_pkt.head.type = 1;
      buf_iap_msg_pkt.head.Dest = 0x01;
      buf_iap_msg_pkt.head.length = 0;

      msgFrame->SendMsgPkt(buf_iap_msg_pkt);
      if( msgFrame->ReadMsgPkt(buf_iap_msg_pkt, FRAME_SINGLE_TIMEOUT) )
      {
          if( (buf_iap_msg_pkt.head.type==1)&&
              (buf_iap_msg_pkt.head.Dest==0)&&
              (buf_iap_msg_pkt.head.length==4)
            )
          {
              filelen = *(UINT32*)&(buf_iap_msg_pkt.Data[0]);
              return true;
          }
      } 
  }
//  return false;
}
  

  /**
  * @brief  下载传输准备开始指令帧
  * @param  rqst,下载传输准备开始指令，值为0为通知上位机无法下载；值为1,为通知上位机准备数据，即将开始下载数据
  * @retval bool ，返回真，系统发送下载传输准备开始指令成功，返回假，系统在规定时间内没有成功请求操作指令。
  * @note   None
  */    
bool CIAPprotocol::SendDownTransStarWaitResp(UINT32 rqst)
{
  msgFrame->clearRxBuf();
  while(1)
  {
      buf_iap_msg_pkt.head.type = 2;
      buf_iap_msg_pkt.head.Dest = 0x01;
      buf_iap_msg_pkt.head.length = 4;

      UINT32 *cmd = (UINT32*)buf_iap_msg_pkt.Data;
      *cmd = rqst;

      msgFrame->SendMsgPkt(buf_iap_msg_pkt);
      if( msgFrame->ReadMsgPkt(buf_iap_msg_pkt, FRAME_SINGLE_TIMEOUT) )
      {
          if( (buf_iap_msg_pkt.head.type==2)&&
              (buf_iap_msg_pkt.head.Dest==0)&&
              (buf_iap_msg_pkt.head.length==4)
            )
          {
              if( 1 == *(UINT32*)&(buf_iap_msg_pkt.Data ) )
              {
                  return true;
              }
          }
      } 
  }    
//  return false;
}

  /**
  * @brief  数据下载传输帧
  * @param  addr,下载数据起始地址，单位字节
            len , 从当前其实地址开始，下载的长度
            data, 返回下载得到的数据缓冲区，缓冲区长度应不小于 len 
  * @retval bool, 返回真，系统请求下载数据成功；返回假，系统在规定时间内，下载数据失败
  * @note   None
  */    
bool CIAPprotocol::SendDownTrasnRqstWaitResp(UINT32 addr,UINT32 len,UINT8 * data)
{
    msgFrame->clearRxBuf();
    while(1)
    {
        buf_iap_msg_pkt.head.type = 3;
        buf_iap_msg_pkt.head.Dest = 0x01;
        buf_iap_msg_pkt.head.length = 8;

        UINT32 *addrcmd = (UINT32*)buf_iap_msg_pkt.Data;
        *addrcmd = addr;

        UINT32 *lencmd  = (UINT32*)&buf_iap_msg_pkt.Data[4];
        *lencmd  = len;

        msgFrame->SendMsgPkt(buf_iap_msg_pkt);
        if( msgFrame->ReadMsgPkt(buf_iap_msg_pkt, FRAME_SINGLE_TIMEOUT) )
        {
            if( (buf_iap_msg_pkt.head.type==3)&&
                (buf_iap_msg_pkt.head.Dest==0)&&
                (buf_iap_msg_pkt.head.length==len)
              )
            {
                memcpy(data,buf_iap_msg_pkt.Data,len);
                return true;    
            }
        } 
    }        
//    return false;
}


  /**
  * @brief  停止下载指令帧
  * @param  None
  * @retval bool，返回真，系统发送传输完毕，停止传输通知指令成功；返回假，系统在规定时间内没有成功发送停止通知指令。
  * @note   None
  */    
bool CIAPprotocol::SendDownStopWaitResp()
{
    msgFrame->clearRxBuf();
    while(1)
    {
        buf_iap_msg_pkt.head.type = 4;
        buf_iap_msg_pkt.head.Dest = 0x01;
        buf_iap_msg_pkt.head.length = 0;

        msgFrame->SendMsgPkt(buf_iap_msg_pkt);
        if( msgFrame->ReadMsgPkt(buf_iap_msg_pkt, FRAME_SINGLE_TIMEOUT) )
        {
            if( (buf_iap_msg_pkt.head.type==4)&&
                (buf_iap_msg_pkt.head.Dest==0)&&
                (buf_iap_msg_pkt.head.length==4)
              )
            {
                if( 1 == *(UINT32*)&(buf_iap_msg_pkt.Data ) )
                {
                    return true;
                }
            }
        } 
    }        
//    return false;
}

  /**
  * @brief  None
  * @param  None
  * @retval None
  * @note   None
  */    
bool CIAPprotocol::DownLoadCommDebug()
{
#if 0
  static UINT8 downbuf[1024];
  //!<搜索
  while(!SendSearchWaitResp());

  //!< 数据文件头--长度
  UINT32 filelen;
  while(!SendFileHeadRqstWaitResp(filelen) );

  //!< 下载传输准备
  while( !SendDownTransStarWaitResp(1) ) ;
  
  //!< 开始下载传输，并烧写文件
  while( !SendDownTrasnRqstWaitResp(0, 1024, downbuf) ) ;

  //!<  停止传输指令，下载并烧写完成
  while( !SendDownStopWaitResp() );

  return true ;
#else
  return false;
 #endif 
}


