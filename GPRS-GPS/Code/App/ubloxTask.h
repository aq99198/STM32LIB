
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

class CUbloxGPS
{
    // contruction and destruction
public:
    static  CUbloxGPS   *GetInstance();
    void     Run();
    void     RxGpsDebugMode();

public:
    int     RecvGPSMsg(UBX_PACKET &Upckt);
    void    SendGpgMsg(UBX_PACKET &UbxPackt);
    UBX_PACKET   *GetGPSRead();
    void    UBXMsgCheckSum(UBX_PACKET &pct, UINT8 &CKA, UINT8 &CKB);
private:
    CUbloxGPS(void);
    ~CUbloxGPS(void);

private:
    static void  GpsTask( void *  param );    
    void   Initiliaze();
    void    TaskLoop( );

    // variable member
private:
    static  CUbloxGPS    *Instance;
    CUartDriver    *gpsSerial;
		CUartDriver       *_fcLink;
};/* end of class */

#endif /* end of __UBLOX_GPS_H__ */


