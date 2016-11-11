
#ifndef __UCLOUD__H
#define __UCLOUD__H

#include "JacTypedef.h"
#include "ProtocolDef.h"
#include "UcloudMsgTypeDef.h"
#include "Gprs.h"

#define SEND_WAIT_RESP_RETRY_TIMES  (5)

class Ucloud
{
public:
    static Ucloud * getIntance();
private:
    Ucloud();
    ~Ucloud();
public:
	  bool Init(INT8 RstFlag); //RstFlat 1: reset sim900a; 0: power up sim900a
    bool BoundTcp(char* ip,INT32 port);
    bool SetDeviceId(char * device,char * str,INT32 maxlen);
    void SendHeartbeat();
    void SendIdentity();  
    static void SendMsgTask(void* param);
    static void RcvMsgTask(void* param);
    void SendLoop(void);
    void RcvLoop(void);
	  bool SendMsg( JCLOUD_MSG_PACK &pck);
	  void Run();


private:

	  INT32 RecvMsg( JCLOUD_MSG_PACK & pck);
	  void SendMsgToSIM(JCLOUD_MSG_PACK &pck);
	  bool getConnFlag(); 
		

private:
    static Ucloud * inStance;
    GPRS * gprs ;
    AP_RT_TRACK_MSG  rtTrack; 
    UINT32 m_uLatestTime_ms;
    
};

#endif





