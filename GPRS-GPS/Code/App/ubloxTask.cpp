
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
#include "ucloudTask.h"
#include "ubloxTask.h"
#include "ProtocolDef.h"


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
@brief  :  set gps & usart2 baud rate
@param  :  NONE
@retval :  NONE
@note   :  THIS function invokes the UCOS_II OSTimeDly(), so we must invoke this function after 
           starting the OS systick , starting the dispatch mechanism , the OSTimeDly() would work
*/
void CUbloxGPS::SetBaudRate(uint32_t baud){
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
    pCfgPrt->Baudrate       = baud;
    pCfgPrt->In_proto_mask  = 0x01;
    pCfgPrt->Out_proto_mask = 0x01;
    pCfgPrt->Flags = 0x0;
    //pCfgPrt->Res2 = ;
    SendGpgMsg(UbxPackt);
		
    OSTimeDly(500);
		USART2_SetBaudRate(baud);
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
		UBX_PACKET  UbxPackt;
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
    //pCfgRate->Meas = 0x3E8;//1s per update
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
            }else{
							OSTimeDly(1);  /* Release system resources */
						}

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
@brief  : send GPS to jouav autopilot
@param  : len is 
@retval : NONE
@note   : NONE
*/
void CUbloxGPS::SendGPSAutoPilot(COMMPACKET_t pkt,UINT8 *src,UINT16  len )
{
	
	static UINT8 cka, ckb;
	COMMPACKET_t pct;															// 

	memcpy((UINT8*)&pct.Data,(UINT8*)src,len);	//16 bit
	pct.msgHead.sync0 = JACX_SYNHEAD0;						// 0 bit
	pct.msgHead.sync1 = JACX_SYNHEAD1;						// 0 bit
	pct.msgHead.msgID = 0xD4; // 新的MSGid				// 0 bit
	pct.msgHead.len = len;
	
	// checkSum
	MsgCheckSum(pct, cka, ckb);											// 88 bit
	pct.Data[len++] = cka;												// 0 bit
	pct.Data[len++] = ckb;												// 0 bit

	_fcLink->write((UINT8*)&pct,len+sizeof(pkt.msgHead));
	
}


/*
@brief
@param
@retval
@note
*/
void  CUbloxGPS::TaskLoop()
{		
		static UINT8 len;
	
    static UBX_PACKET pckt_main;
		static JCLOUD_MSG_PACK serverpckt;
    UBX_MSG_HEAD *REV_MSG_HEAD;
		UINT8 *src = (UINT8*)&pckt_main.data;	
		Ucloud  *server = Ucloud::getIntance();
		static COMMPACKET_t pkt;
		static INT32  tmp_lati, tmp_long, tmp;
		static UINT8 cka, ckb;
		static UINT32 timerEnd, timerStart;
		timerStart = OSTimeGet();
	
	 memset(pkt.Data,0,MAX_PACKET_LENGTH);
    pkt.msgHead.sync0 = JACX_SYNHEAD0;
    pkt.msgHead.sync1 = JACX_SYNHEAD1;
	  pkt.msgHead.dest = 0;	
    pkt.msgHead.source = 0;
  	pkt.msgHead.msgID = 210; // 新定义的消息
    pkt.msgHead.SeqNum = 0;
	  pkt.msgHead.ACK_NAK = 0;
	  pkt.msgHead.len = 22;//fix wing
	
		gpsSerial->clearBuffer();
    while(1)
    {	 
        RecvGPSMsg(pckt_main); // probleam 
        REV_MSG_HEAD = (UBX_MSG_HEAD*)&pckt_main;
        if( 0x01 == REV_MSG_HEAD->cls )	
        {
            switch(REV_MSG_HEAD->id)  //ptv packet
            {
								 case 0x07: 
									{
										
										UBX_MSG_NAV_PVT ubx_pvt;
										memcpy(&ubx_pvt,src,pckt_main.head.len);
										// logic
										
										//memset(&pkt.Data, 0,pckt_main.head.len);	 // init packet	
                    tmp_lati = (INT32)floor((*((INT32*)&src[28]))/5.72957795131 + 0.5);	//latitude					
									  memcpy(&pkt.Data[0],  &tmp_lati, 4);
									  tmp_long = (INT32)floor((*((INT32*)&src[24]))/5.72957795131 + 0.5);	//longitude					
									  memcpy(&pkt.Data[4],  &tmp_long, 4);										
										tmp = *((INT32*)&src[36]);	//altitude					
									  memcpy(&pkt.Data[8],  &tmp, 4);

								
										// 发送速度
										*((INT16*)&pkt.Data[12]) = (INT16)floor((*((INT32*)&src[48]))/10.0+0.5);
										*((INT16*)&pkt.Data[14]) = (INT16)floor((*((INT32*)&src[52]))/10.0+0.5);	
										*((INT16*)&pkt.Data[16]) = (INT16)floor((*((INT32*)&src[56]))/10.0+0.5);	
											// 发送 航向
										*((UINT16*)&pkt.Data[18]) = (UINT16)floor((*((INT32*)&src[64]))/572.957795131+0.5);
										*((UINT16*)&pkt.Data[20]) = (UINT16)floor((*((INT32*)&src[64]))/572.957795131+0.5);
										//pkt.msgHead.len =20;
										MsgCheckSum(pkt, cka, ckb);
										pkt.Data[22] = cka;//fix wing msg length:118
										pkt.Data[23] = ckb;	
										
										serverpckt.msgHead.msgid = JCLOUD_MSG_ID_PUSH_DRONE_CONTENT;
										serverpckt.msgHead.msglen = sizeof(pkt.msgHead) + 24; 
										memcpy((UINT8*)serverpckt.Data,&pkt.msgHead.sync0,serverpckt.msgHead.msglen);
										
										
										
											/************* send back_up GPS *******************/ 
											len = REV_MSG_HEAD->len;
											COMMPACKET_t pct;															// 
											memcpy((UINT8*)&pct.Data,(UINT8*)src,len);	//16 bit
											pct.msgHead.sync0 = JACX_SYNHEAD0;						// 0 bit
											pct.msgHead.sync1 = JACX_SYNHEAD1;						// 0 bit
											pct.msgHead.dest = 0x00;
											pct.msgHead.source = 0x00;
											pct.msgHead.msgID = 0xD4; // 新的MSGid				// 0 bit
											pct.msgHead.ACK_NAK = 0x00;
											pct.msgHead.SeqNum = 0x00;
											pct.msgHead.len = len;
											
											// checkSum
											MsgCheckSum(pct, cka, ckb);											// 88 bit
											pct.Data[len++] = cka;												// 0 bit
											pct.Data[len++] = ckb;												// 0 bit
											
											
											read_onchip_flash(0x0803f810,(u8 *)&JCLOUD_BUGPS,sizeof(UINT32));
											if(JCLOUD_BUGPS){
												/* not need OSMutex ,usartDriver aready have*/
												_fcLink->write((UINT8*)&pct,len+sizeof(pkt.msgHead));
												
											}
										
											/************* send back_up GPS *******************/ 
											
										
										if((src[21] != 0) && (src[20] == 0x03) && ((*(UINT16*)&src[76]) <= 410))
										{
											timerEnd = OSTimeGet();
											if((timerEnd - timerStart) > GPS_MSG_INTERVAL_MS){
												OSMutexPend(SemUartW5, 0, &g_u8Rerr);	
												server->SendMsg(serverpckt);
												OSMutexPost(SemUartW5);	
												SystemStatus = SYS_GPS_FIX;
												#ifdef GPS_DEBUG_ON
												DEBUG("SEND GPS\r\n");
												#endif
												timerStart = OSTimeGet();
												}
							
											
										}	else{
											SystemStatus = SYS_CONNECT_SERVER;
											#ifdef GPS_DEBUG_ON
											PRINT("fix flag:%d fix_type:%d DOP:%d\r\n", src[21],src[20],*((UINT16*)&src[76]));
											if((src[21] != 0) && (src[20] == 0x03)&& ((*(UINT16*)&src[76]) > 410) ){
												DEBUG("GPS_FIXED\r\n");
											}
											#endif
										}							
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
		gps->SetBaudRate(115200);
		gps->Initiliaze();
    gps->TaskLoop();
}

/* end of file */
