
/*************************************************************
* Copyright 2007-2008 Jovian Advanced Control Technology, Co.
*
*  FileName:
*  Abstract:         DataUp Link
*
*
*  Current Version:
*  Writer:
*  Date:
*---------------------------------------------------------------
* Modification history:
* Date:
* Version:
* Description:
*
****************************************************************/

#ifndef  __RFPORT__H__
#define  __RFPORT__H__

#include "ucos_ii.h"
#include "protocoldef.h"
#include "uartdriver.h"
#include <stdint.h>
#include "ublox_ubx.h"

#define APLINK_MAX_COMMPACKET_MSG      32
#define APLINK_MAX_COMMPACKET_SIZE     256


// class CPCPort;

class CAPlink
{
    // constructor and destructor
    private:
        CAPlink();
        ~CAPlink();
    // obvious obain a pointer of object
    public:
        static CAPlink*  GetInstance();

    // the public method
    public:
        int    SendMsg(COMMPACKET_t &pckt);
			  void   SendMsg(UBX_PACKET &pct);
        int    SendMsgDirect(COMMPACKET_t &pct);
	      int    SendMsgDirect(UBX_PACKET &pct);
        int    RecvMsg(COMMPACKET_t &pckt); //return len as int
        void   SendDaemon();

        void write(UINT8 *data, int dataLen){m_pUartLink->write(data,dataLen);}
        void read(UINT8 *data, int *dataLen){m_pUartLink->read(data,dataLen);}

        void   Run();
        void   Initiliaze();
        static  void   RecvMsgTask(void);
        static  void   SendMsgTask(void);


    // data member
    private:
        CUartDriver       *m_pUartLink;
        OS_EVENT          *m_pQSem;
        bool               m_bRun;
//        CPCPort           *m_pPCPort;
	    
    static CAPlink        *m_pAPlink;

};



#endif /* end of  __ARFOMM__H__ */
/*end of file */
