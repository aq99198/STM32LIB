
/*************************************************************
* Copyright 2007-2008 Jovian Advanced Control Technology, Co.
* 
*  FileName:        UbloxGPS5.h For Testing
*  Abstract:        Ublox GPS_5 receiver
*                   
*
*  Current Version: 
*  Writer:          
*  Date:            20013.03.15
*---------------------------------------------------------------
* Modification history:
* Date: 
* Version:
* Description:
*
****************************************************************/

#ifndef __UBLOX_GPS_H__
#define __UBLOX_GPS_H__


#include "uartdriver.h"
#include "ProtocolDef.h"
#define GPS_MSG_INTERVAL_MS      	 2000

class CUbloxGPS
{
    // contruction and destruction
public:
    static  CUbloxGPS   *GetInstance();
    void     Run();
    void     RxGpsDebugMode();

public:
		CUartDriver       *_fcLink;
    int     RecvGPSMsg(UBX_PACKET &Upckt);
    void    SendGpgMsg(UBX_PACKET &UbxPackt);
    UBX_PACKET   *GetGPSRead();
    void    UBXMsgCheckSum(UBX_PACKET &pct, UINT8 &CKA, UINT8 &CKB);
		void 		SendGPSAutoPilot(COMMPACKET_t pkt,UINT8 *src,UINT16 len);
private:
    CUbloxGPS(void);
    ~CUbloxGPS(void);

private:
    static void  GpsTask( void *  param );    
    void   Initiliaze();
		void   SetBaudRate(uint32_t baud);
    void   TaskLoop();

    // variable member
private:
    static  CUbloxGPS    *Instance;
    CUartDriver    *gpsSerial;
		
};/* end of class */

#endif /* end of __UBLOX_GPS_H__ */


