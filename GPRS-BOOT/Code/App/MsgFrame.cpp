

#include "MsgFrame.h"
#include "Cclock.h"

static const UINT16 r_crc16_tab[] = {
    0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7, 
    0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef, 
    0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6, 
    0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de, 
    0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485, 
    0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d, 
    0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4, 
    0xb75b, 0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc, 
    0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823, 
    0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b, 
    0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50, 0x3a33, 0x2a12, 
    0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a, 
    0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41, 
    0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49, 
    0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70, 
    0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78, 
    0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f, 
    0x1080, 0x00a1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067, 
    0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e, 
    0x02b1, 0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256, 
    0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d, 
    0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405, 
    0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c, 
    0x26d3, 0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634, 
    0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab, 
    0x5844, 0x4865, 0x7806, 0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3, 
    0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a, 
    0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92, 
    0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9, 
    0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1, 
    0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8, 
    0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0, 
};



//----------------------------------------------------------------------
/**
  * @brief  
  * @param  None
  * @retval None
  * @note   None
  */        
CMsgFrame::CMsgFrame(CUartDriver * ser, Cclock * clk):serial(ser),clock(clk)
{

}

/**
  * @brief  
  * @param  None
  * @retval None
  * @note   None
  */        
CMsgFrame::~CMsgFrame()
{

}

/**
  * @brief  读取数据帧
  * @param  msg ， 需要接收的数据帧缓冲区
            timeout，接收超时时间，单位毫秒
  * @retval INT32，接收接收状态，值0为接收失败；值为正表示当前接收的数据帧长度
  * @note   None
  */    
INT32 CMsgFrame::ReadMsgPkt(IAP_MSG_PKT & msg,UINT32 timeout)
{
  volatile UINT32 start = clock->getSysTime();
  volatile UINT32 end   = clock->getSysTime();
  UINT32 rstate=0 ;
  UINT8  rdata;
  UINT32 datalen = 0 ;
	int bytelen = 1;
	
  while(true)
  {
    if(serial->available())
    {
      serial->read(&rdata, &bytelen);
      
      switch(rstate) //!< init state is 0
      {
        case 0: //!< msg.head.sync0
        {
          if(SYNC0 == rdata )
          {
            msg.head.sync0 = rdata;
            rstate++;
          }
        }
        break;
        case 1://!< msg.head.sync1
        {
          if(SYNC1 == rdata )
          {
            msg.head.sync1 = rdata;
            rstate++;
          }
        }
        break;
        case 2://!< msg.head.type
        {
          msg.head.type = rdata;
          rstate++;
        }
        break;
        case 3://!< msg.head.Dest
        {
          msg.head.Dest = rdata;
          rstate++;
        }
        break;
        case 4://!< msg.head.length  is 2 bytes , first bytes
        {
          UINT8 * data = (UINT8*)&msg.head.length;
          *data = rdata;
          rstate++;
        }
        break;
        case 5: //!<msg.head.length  is 2 bytes , second bytes
        {
          UINT8 * data = (UINT8*)&msg.head.length;
          ++data;
          *data = rdata;
          rstate++;
        }
        break;
        case 6 : //!< recevie data , length is msg.head.length+2
        {
          msg.Data[datalen++] = rdata;
          if( datalen == (msg.head.length+2) )
          {
            //!< caculate CRC
            UINT16 *crc = (UINT16*)&msg.Data[msg.head.length];
            if(*crc == crc16( (UINT8*)&msg.head.type , msg.head.length+sizeof(msg.head) -2) )
            {
              return msg.head.length+2+sizeof(msg.head);
            }
          }
        }
        break;
        default:
        break;
      }


    }
    else
    {
      end = clock->getSysTime();
    }

    if( end > (start+timeout) )
    {
        break;;
    }
  }

  return 0 ;
}

/**
  * @brief  发送数据帧
  * @param  msg，发送的数据帧的缓冲区
  * @retval None
  * @note   None
  */        
void CMsgFrame::SendMsgPkt(IAP_MSG_PKT & msg)
{
  msg.head.sync0 = SYNC0;
  msg.head.sync1 = SYNC1;
  UINT16 * crc = (UINT16*)&msg.Data[msg.head.length];
  *crc = crc16( (UINT8*)&msg.head.type , msg.head.length+sizeof(msg.head) -2);
  serial->write((UINT8*)&(msg.head), sizeof(msg.head)+msg.head.length+2);//!< check CRC 2 bytes     
}

/**
  * @brief  清空接收缓冲区，防止原接收缓冲中其他数据干扰
  * @param  None
  * @retval None
  * @note   强制清空通信接口的接收缓冲区
  */       
void CMsgFrame::clearRxBuf()
{
  serial->flush();
}

/**
  * @brief  读取一行数据
  * @param  str,接收缓冲区
            maxlen,接收缓冲区的长度
            timeout,接收超时时间,单位毫秒
  * @retval INT32,接收接收状态，值0为接收失败；值为正表示当前接收的数据帧长度
  * @note   由于各系统的结尾符号不同，当系统发现'\r','\n',"\r\n"中的任何一个，将之前的数据作为一行数据
            如果一行数据太长，长度超过maxlen的长度，系统直接返回
  */        
INT32 CMsgFrame::ReadLine(UINT8 * str, UINT32 maxlen,UINT32 timeout)
{
  volatile UINT32 start = clock->getSysTime();
  volatile UINT32 end   = clock->getSysTime();
  UINT32 rlen=0 ;
  UINT8  c;
	UINT8 bytelen = 1;
  while(rlen<maxlen)
  {
    if(serial->available())
    {
      serial->read(&c,(int*)&bytelen);
      if(c != '\r') //!< 回车符过滤
      {      
        str[rlen] = c;
        if( str[rlen]=='\n' )
        {
          str[rlen]='\0'; //!< 发现为换行符号，则直接填充结束符
          return ++rlen; 
        }
        rlen++;
      }

    }
    else
    {
      end = clock->getSysTime();
    }

    if( end > (start+timeout) )
    {
        return 0;
    }
  }
  return rlen ;
}

/**
  * @brief  发送制定长度的数据
  * @param  str,发送缓冲区
            len,发送数据长度
  * @retval INT32,接收接收状态，值0为接收失败；值为正表示当前接收的数据帧长度
  * @note   None
  */        
INT32 CMsgFrame::SendData(UINT8 * str, UINT32 len )
{
  serial->write(str, len);
  return 0;
}

/**
  * @brief  接收制定长度的数据
  * @param  data,接收缓冲区
            len,需要接收数据长度
            timeout,接收超时时间,单位毫秒
  * @retval INT32,接收接收状态，值0为接收失败；值为正表示当前接收的数据帧长度
  * @note   None
  */   
INT32 CMsgFrame::ReciveData(UINT8 * data ,UINT32 len, UINT32 timeout)
{
  volatile UINT32 start = clock->getSysTime();
  volatile UINT32 end   = clock->getSysTime();
  UINT32 rlen=0 ;
	UINT8 c;
	UINT8 bytelen = 1;
  while(rlen<len)
  {
    if(serial->available())
    {
			serial->read(&c,(int*)&bytelen);
      data[rlen++] = c;
    }
    else
    {
      end = clock->getSysTime();
    }

    if( end > (start+timeout) )
    {
        return 0;
    }
  }
  return rlen ;
}


/**
  * @brief  
  * @param  buf,
            len,数据长度
  * @retval UINT16
  * @note   None
  */   
UINT16 CMsgFrame::crc16(UINT8 *buf, UINT32 len)
{
    INT32 i;
    UINT16 cksum;

    cksum = 0;
    for (i = 0;  i < len;  i++)
    {
        cksum = r_crc16_tab[((cksum>>8) ^ *buf++) & 0xFF] ^ (cksum << 8);
    }
    return cksum;
}

