

#ifndef MSG_FRAME_H
#define MSG_FREAM_H

#include "JacTypeDef.h"
#include "UartDriver.h"

#define SYNC0  (0XB5)
#define SYNC1  (0X62)

typedef struct 
{
    UINT8 sync0;
    UINT8 sync1;
    UINT8 type;
    UINT8 Dest;
    UINT16 length;
    /* data */
}IAP_MSG_HEAD;

#define IAP_MSG_LEN_MAX (1024+4)
typedef struct 
{
    IAP_MSG_HEAD head;
    UINT8 Data[IAP_MSG_LEN_MAX];
    /* data */
}IAP_MSG_PKT;

class CSerial;
class Cclock;
class CMsgFrame
{
public:
/**
  * @brief  
  * @param  None
  * @retval None
  * @note   None
  */        
    CMsgFrame(CUartDriver * ser,Cclock * clk);

/**
  * @brief  
  * @param  None
  * @retval None
  * @note   None
  */        
    ~CMsgFrame();

/**
  * @brief  读取数据帧
  * @param  msg ， 需要接收的数据帧缓冲区
            timeout，接收超时时间，单位毫秒
  * @retval INT32，接收接收状态，值0为接收失败；值为正表示当前接收的数据帧长度
  * @note   None
  */    
    INT32 ReadMsgPkt(IAP_MSG_PKT & msg,UINT32 timeout);

/**
  * @brief  发送数据帧
  * @param  msg，发送的数据帧的缓冲区
  * @retval None
  * @note   None
  */        
    void SendMsgPkt(IAP_MSG_PKT & msg);

/**
  * @brief  清空接收缓冲区，防止原接收缓冲中其他数据干扰
  * @param  None
  * @retval None
  * @note   强制清空通信接口的接收缓冲区
  */       
  void clearRxBuf();

/**
  * @brief  读取一行数据
  * @param  str,接收缓冲区
            maxlen,接收缓冲区的长度
            timeout,接收超时时间,单位毫秒
  * @retval INT32,接收接收状态，值0为接收失败；值为正表示当前接收的数据帧长度
  * @note   由于各系统的结尾符号不同，当系统发现'\r','\n',"\r\n"中的任何一个，将之前的数据作为一行数据
            如果一行数据太长，长度超过maxlen的长度，系统直接返回
  */        
    INT32 ReadLine(UINT8 * str, UINT32 maxlen,UINT32 timeout);

/**
  * @brief  发送制定长度的数据
  * @param  str,发送缓冲区
            len,发送数据长度
  * @retval INT32,接收接收状态，值0为接收失败；值为正表示当前接收的数据帧长度
  * @note   None
  */        
    INT32 SendData( UINT8 * str, UINT32 len );

/**
  * @brief  接收制定长度的数据
  * @param  data,接收缓冲区
            len,需要接收数据长度
            timeout,接收超时时间,单位毫秒
  * @retval INT32,接收接收状态，值0为接收失败；值为正表示当前接收的数据帧长度
  * @note   None
  */   
    INT32 ReciveData(UINT8 * data ,UINT32 len, UINT32 timeout);
private:

/**
  * @brief  
  * @param  buf,
            len,数据长度
  * @retval UINT16
  * @note   None
  */   
  UINT16 crc16(UINT8 *buf, UINT32 len);

private:

    CUartDriver * serial;
    Cclock  * clock;
};


#endif 
