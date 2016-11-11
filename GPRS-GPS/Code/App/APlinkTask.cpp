
/*************************************************************
* Copyright 2007-2008 Jovian Advanced Control Technology, Co.
*
*  FileName:
*  Abstract:       DataUp Link ,be responsible for the comm port for the RF
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

/***************************************************************/

/******************************USER MANNUAL*********************/
/*
NOTE : The SendDaemon Function must be invoked by a task
*/

/******************************INCLUDES*************************/
#include "board.h"
#include "APlinkTask.h"
#include "ucloudTask.h"


/*************** STATIC MEMBER VAIRBLES ***********************/
CAPlink  * CAPlink::m_pAPlink = (CAPlink  *)0 ;


/***************THE MEMBER FUNCTION **************************/
CAPlink::CAPlink()
{
      m_pUartLink  = new CUartDriver(USART1_IDX);  //USART1_IDX
      m_bRun       = false;
}

CAPlink::~CAPlink(void)
{
}


/*
@brief  :
@param  :
@retval :
@note   :
*/
CAPlink*  CAPlink::GetInstance()
{
    if((CAPlink  *)0== m_pAPlink )
    {
        m_pAPlink = new CAPlink();
    }
    return  m_pAPlink;
}

/*
@brief  :
@param  :
@retval :
@note   :
*/
void  CAPlink::Initiliaze()
{

}



/*
@brief  :
@param  :  the reference of  a COMMPACKET_t package
@retval :  the length of byte received
@NOTE   :
*/
int CAPlink::RecvMsg(COMMPACKET_t &pckt)
{
	UINT8 rcvCKA,rcvCKB;
	UINT8 cka,ckb;
	UINT8 *pchar;
	int   Readlen =0;

	while (1)
	{
		while (1)		// Searching and receive SynHead
		{
      Readlen = 1;
			m_pUartLink->read((UINT8 *)(&(pckt.msgHead.sync0)), &Readlen);
			if(JACX_SYNHEAD0==pckt.msgHead.sync0)
			{
			    Readlen = 1;
			    m_pUartLink->read((UINT8 *)(&(pckt.msgHead.sync1)), &Readlen);
				if(JACX_SYNHEAD1 == pckt.msgHead.sync1)
				{
            break;
        }
			}/* end of if  JACX_SYNHEAD0 */

		}/* end of inside while 1 */

		//Searching and receive package head (JACX_Head)
		pchar=(UINT8*)(&pckt.msgHead);
		pchar += 2; //exclude sycHead1 which already in pckt

    Readlen = HEAD_LEN_EX_SYNC;
		m_pUartLink->read(pchar, &Readlen);		

		Readlen=pckt.msgHead.len +2;
		m_pUartLink->read((UINT8 *)pckt.Data, &Readlen);
		rcvCKA = pckt.Data[pckt.msgHead.len];
		rcvCKB = pckt.Data[pckt.msgHead.len+1];	

		MsgCheckSum(pckt,cka,ckb);
		if (rcvCKA==cka && rcvCKB==ckb)
		{
				return JACX_HEAD_SIZE+pckt.msgHead.len+2;
		}
	}/* end of outside while 1 */
}/* end of RecvMsg */


/*
@brief  :
@param  : NONE
@retval : NONE
@note   :
*/
void   CAPlink::Run()
{
    m_bRun       = true;
    OSTaskCreateExt((void (*)(void *)) RecvMsgTask,
                             (void          * ) 0,
                             (OS_STK        * )&App_Task_APLINKRX_Stack[APP_TASK_APLINKRX_STK_SIZE - 1],
                             (uint8_t         ) APP_TASK_APLINKRX_PRIO,
                             (uint16_t        ) APP_TASK_APLINKRX_PRIO,
                             (OS_STK        * )&App_Task_APLINKRX_Stack[0],
                             (INT32U          ) APP_TASK_APLINKRX_STK_SIZE,
                             (void          * )0,
                             (uint16_t        )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));

}

/*
@brief  :
@param  : NONE
@retval : NONE
@note   :
*/
void   CAPlink::RecvMsgTask()
{
	if( !m_pAPlink->m_bRun )
	{
		OSTaskSuspend(OS_PRIO_SELF);
	}
	
	static COMMPACKET_t commpckt;
  static int dataLen;
	static UINT8* data = (UINT8*)&commpckt.msgHead.sync0;
	static int i;
	static JCLOUD_MSG_PACK serverpckt;
	serverpckt.msgHead.msgid = JCLOUD_MSG_ID_PUSH_DRONE_CONTENT;

	while (1)
	{
		dataLen = m_pAPlink->RecvMsg(commpckt);
		
		if(dataLen == -1)
		{
			OSTimeDly(500);
			continue;
		}
		
		switch(commpckt.msgHead.msgID)
		{	
			case 00:
				/*om msg, used to recv apid/gprsid/ip/port/, write to the last page of flash, 14 bytes*/
				commpckt.Data[14] = 0;//dummy byte
				commpckt.Data[15] = 0;//dummy byte
		
				/* read form data packet*/
				JCLOUD_APID = *((UINT32*)&commpckt.Data[0]);
				JCLOUD_GSMID = *((UINT32*)&commpckt.Data[4]);	
        JCLOUD_IP = *((UINT32*)&commpckt.Data[8]);		
        JCLOUD_PORT = *((UINT16*)&commpckt.Data[12]);	
			
			
				/* save on chip flash*/
				FLASH_Unlock();
				FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
				// use last 2k flash
				FLASH_ErasePage(0x0803f800);
				
				write_onchip_flash(0x0803f800,(u8 *)&JCLOUD_APID,sizeof(UINT32));	
				write_onchip_flash(0x0803f804,(u8 *)&JCLOUD_GSMID,sizeof(UINT32));	
				write_onchip_flash(0x0803f808,(u8 *)&JCLOUD_IP,sizeof(UINT32));	
				write_onchip_flash(0x0803f80c,(u8 *)&JCLOUD_PORT,sizeof(UINT32));
				FLASH_Lock();
		
									
				PRINT("apid:%08d gsmid:%08d ip:%08d port:%08d\r\n",JCLOUD_APID,JCLOUD_GSMID, JCLOUD_IP, JCLOUD_PORT);	
				
			
				/* reconnect to jcloud server */
				//OSSemPend(semMutex,0,&g_u8Rerr);
				m_bConnect = false;
				resetFlag = 1;
				bConnect = m_bConnect;
				//OSSemPost(semMutex);
			
			break;
			case 01:
				/* system reboot and download */
				DEBUG("reboot now\r\n");
				OSTimeDly(5);
				SystemReset();
			break;
			
			case 02:
				
			break;
			
			default:
			 break;
		}		
		OSTimeDly(500);
	}/* end of while */
}



/*end of file */
