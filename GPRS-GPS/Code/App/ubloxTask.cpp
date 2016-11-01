
/*************************************************************
* Copyright 2007-2008 Jovian Advanced Control Technology, Co.
* 
*  FileName:        
*  Abstract:       FOR  U_BLOX_5  TESTING   
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
/******************************USER MANNUAL*********************/
/*

NOTE : The SendDaemon Function must be invoked by a task 
*/

/******************************INCLUDES*************************/
#include "board.h"
#include "ubloxTask.h"


/***************THE LOCAL VARIABLES **************************/


// #define APP_TASK1_STK_SIZE       512
// #define APP_TASK1_PRIO           9    // GPS

// static  __align(8)  OS_STK       App_Task1Stack[APP_TASK1_STK_SIZE];

/**************************************************/
CUbloxGPS   *CUbloxGPS::Instance = (CUbloxGPS  *)0 ;

CUbloxGPS::CUbloxGPS(void)
{  
	gpsSerial = new CUartDriver(USART2_IDX);
	_fcLink = new CUartDriver(USART1_IDX);
}

CUbloxGPS::~CUbloxGPS(void)
{

}

CUbloxGPS * CUbloxGPS::GetInstance()
{
    if( (CUbloxGPS *)0 == Instance )
    {
        Instance = new CUbloxGPS();
    }
    return Instance;
}

/*
@brief
@param
@retval
@note
*/
void   CUbloxGPS::Run()
{
    OSTaskCreateExt((void (*)(void *)) GpsTask,
                         (void          * ) this,
                         (OS_STK        * )&App_Task_UBLOX_Stack[APP_TASK_UBLOX_STK_SIZE - 1],
                         (UINT8         ) APP_TASK_UBLOX_PRIO,
                         (UINT16        ) APP_TASK_UBLOX_PRIO,
                         (OS_STK        * )&App_Task_UBLOX_Stack[0],
                         (INT32U          ) APP_TASK_UBLOX_STK_SIZE,
                         (void          * )0,
                         (UINT16        )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));
}

/**
@brief  :  Initiliaze the GPS according the default configuration
@param  :  NONE
@retval :  NONE
@note   :  THIS function invokes the UCOS_II OSTimeDly(), so we must invoke this function after 
           starting the OS systick , starting the dispatch mechanism , the OSTimeDly() would work
*/
void CUbloxGPS::Initiliaze()
{
    OSTimeDly(1000); // waiting for the GPS hardware set up 
    UBX_PACKET  UbxPackt;

   // INITIAL UBLOX , configuration the port and the baud rate	
    UBX_MSG_CFG_PRT *pCfgPrt;
    pCfgPrt = (UBX_MSG_CFG_PRT *)&UbxPackt.data[0] ;
    UbxPackt.head.cls   = UBX_CLS_CFG;
    UbxPackt.head.id    = UBX_CLS_CFG_PORT;
    UbxPackt.head.len   = sizeof(UBX_MSG_CFG_PRT);
    pCfgPrt->PortID = 0x01;	
    //pCfgPrt->Res0 = ;
    //pCfgPrt->Res1 = ;
    pCfgPrt->Mode   = 0x08D0;
    pCfgPrt->Baudrate       = 9600;
    pCfgPrt->In_proto_mask  = 0x01;
    pCfgPrt->Out_proto_mask = 0x01;
    pCfgPrt->Flags = 0x0;
    //pCfgPrt->Res2 = ;
    SendGpgMsg(UbxPackt);
		
    OSTimeDly(500);
		//have a bug for here
		//USART2_SetBaudRate(115200);
		OSTimeDly(500);

    // configuration the 
    UBX_MSG_CFG_NAV5 *pCfgNav5;
    pCfgNav5 = (UBX_MSG_CFG_NAV5 *)&UbxPackt.data[0] ;
    UbxPackt.head.cls   = UBX_CLS_CFG;
    UbxPackt.head.id    = UBX_CLS_CFG_NAV5;
    UbxPackt.head.len   = sizeof(UBX_MSG_CFG_NAV5);
    pCfgNav5->mask      = 0x0101;
    pCfgNav5->dynModel  = 0x07;
    pCfgNav5->fixMode   = 0x03;
    pCfgNav5->fixedAlt  = 0x0C350;//500m
    pCfgNav5->fixedAltVar = 0x0;
    pCfgNav5->minElev     = 0X05;
    pCfgNav5->drLimit     = 0x05;
    pCfgNav5->pDop = 0xFA;
    pCfgNav5->tDop = 0xFA;
    pCfgNav5->pAcc = 0x64;
    pCfgNav5->tAcc = 0x012C;
    pCfgNav5->staticHoldThresh = 0x0;
    pCfgNav5->res1 = 0;
    pCfgNav5->res2 = 0;
    pCfgNav5->res3 = 0;
    pCfgNav5->res4 = 0;
    
    SendGpgMsg(UbxPackt); 
    OSTimeDly(100);
                                            
    UBX_MSG_CFG_RATE *pCfgRate;
    pCfgRate = (UBX_MSG_CFG_RATE *)&UbxPackt.data[0] ;
    UbxPackt.head.cls   = UBX_CLS_CFG;
    UbxPackt.head.id    = UBX_CLS_CFG_RATE;
    UbxPackt.head.len   = sizeof(UBX_MSG_CFG_RATE);
    //pCfgRate->Meas = 0x0FA;//250ms per update
		pCfgRate->Meas = 0x064;//100ms per update
		//pCfgRate->Meas = 0x050;//80ms per update
    pCfgRate->Nav  = 0x01;
    pCfgRate->Time = 0x0;
    SendGpgMsg(UbxPackt); 
    OSTimeDly(100);
    
    UBX_MSG_CFG_MSG *pCfgMsg;
    pCfgMsg = (UBX_MSG_CFG_MSG *)&UbxPackt.data[0] ;
    UbxPackt.head.cls   = UBX_CLS_CFG;
    UbxPackt.head.id    = UBX_CLS_CFG_MSG;
    UbxPackt.head.len   = sizeof(UBX_MSG_CFG_MSG);
    pCfgMsg->Class  = 0x01;
    pCfgMsg->MsgID  = 0x07;
    pCfgMsg->Rate   = 0x01;
    SendGpgMsg(UbxPackt); 
    OSTimeDly(100);
		 
#if 0
    pCfgMsg = (UBX_MSG_CFG_MSG *)&UbxPackt.data[0] ;
    UbxPackt.head.cls   = UBX_CLS_CFG;
    UbxPackt.head.id    = UBX_CLS_CFG_MSG;
    UbxPackt.head.len   = sizeof(UBX_MSG_CFG_MSG);
    pCfgMsg->Class  = 0x01;
    pCfgMsg->MsgID  = 0x20;
    pCfgMsg->Rate   = 0x01;
    SendGpgMsg(UbxPackt); 
#endif 

    OSTimeDly(1000);
}/* end of Initialize */


/*
@brief  :
@param  :  a refrence of UBX_PACKET
@retval :  the length of bytes received
@NOTE   :  NONE
*/
int CUbloxGPS::RecvGPSMsg(UBX_PACKET &pckt)
{

    UINT8* pchar;
    int   Readlen =0;

    while (1)
    {
        while (1)		// Searching and receive SynHead
        {
            Readlen = 1;
            gpsSerial->read((UINT8 *)(&(pckt.head.SynHead1)), &Readlen);

            if( UBX_SYNC_HEAD1 == pckt.head.SynHead1 )
            {
                Readlen = 1;
                gpsSerial->read((UINT8 *)(&(pckt.head.SynHead2)), &Readlen);
                if(UBX_SYNC_HEAD2 == pckt.head.SynHead2)
                {
                    break;
                }
            } /* end of if */

        }/* end of  inside while */

				// 数据没有校验
				
        //Searching and receive package head (JACX_Head)
        pchar = (UINT8*)(&pckt.head);
        pchar += 2; //exclude sycHead1 which already in pckt

        Readlen  = sizeof(UBX_MSG_HEAD) -2 ;
        gpsSerial->read(pchar, &Readlen);

        Readlen = pckt.head.len +2;

        gpsSerial->read((UINT8 *)pckt.data, &Readlen);

        return sizeof(UBX_MSG_HEAD)+2+pckt.head.len;
    }/* end of outside while */

}/* end of RecvMsg */


void CUbloxGPS::SendGpgMsg( UBX_PACKET  &UbxPackt )
{
    UINT8 cka,ckb;

    UbxPackt.head.SynHead1 = UBX_SYNC_HEAD1;
    UbxPackt.head.SynHead2 = UBX_SYNC_HEAD2;
    
    UBXMsgCheckSum(UbxPackt , cka,ckb );
    UINT16 count=UbxPackt.head.len;
    UbxPackt.data[count++]   = cka;
    UbxPackt.data[ count++ ] = ckb;
    
    gpsSerial->write((UINT8 * )&UbxPackt,sizeof(UBX_MSG_HEAD)+count);
}

/*
@brief  : check the checksum about a package
@param  : the reference of a UBX_PACKET ,  the high byte and low byte of check sum 
@retval : NONE
@note   : NONE
*/
void CUbloxGPS::UBXMsgCheckSum(UBX_PACKET &pct, UINT8 &CKA, UINT8 &CKB)
{
    CKA = CKB = 0;
    int len   = sizeof(UBX_MSG_HEAD)-2+pct.head.len;
    UINT8 * p = (UINT8 *)&pct.head.cls; 
    for(int i = 0 ; i < len ; ++i )
    {
        CKA = CKA + p[i];
        CKB += CKA;
    }
}

/*
@brief
@param
@retval
@note
*/
void  CUbloxGPS::TaskLoop()
{
		#define HEARTBEAT_MSG_INTERVAL_MS      10000
    static UBX_PACKET pckt_main;
    UBX_MSG_HEAD *REV_MSG_HEAD;
		UINT8 *src = (UINT8*)&pckt_main.data;	
    while(1)
    {	 
        RecvGPSMsg(pckt_main);
        REV_MSG_HEAD = (UBX_MSG_HEAD*)&pckt_main;
        if( 0x01 == REV_MSG_HEAD->cls )	
        {
            switch(REV_MSG_HEAD->id)  //这里接收到的就是PVT 数据包
            {
								 case 0x07: 
									{
										DEBUG("GPS\r\n");
											//PRINT("Send GSM TIME:%d\r\n",timerEnd_Count- timerStart_Count);
									}
                break;
                case 0x20:
                {
										//UBX_MSG_NAV_TIMEGPS  *REV_MSG_DATA_time;
                    //REV_MSG_DATA_time = (UBX_MSG_NAV_TIMEGPS*)(&pckt_main.data);
									  DEBUG("unknown ublox msg2\r\n");
                }
                break;
                default:
									DEBUG("unknown ublox msg\r\n");
                break;
            }
        }
				else
				{
					DEBUG("unknown ublox msg3\r\n");
				}
				OSTimeDly(1);
    }       
}

// COMMPACKET_t DGPSPckt;
void   CUbloxGPS::GpsTask( void * param )
{
		CUbloxGPS *  gps = ( CUbloxGPS*)param;
		gps->Initiliaze();
    gps->TaskLoop();
}

/* end of file */
