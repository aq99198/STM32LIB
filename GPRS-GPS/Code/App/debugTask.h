/*************************************************************
* Copyright 2007-2008 Jovian Advanced Control Technology, Co.
* 
*  FileName:        
*  Abstract:         Debug task 
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

#ifndef __DEBUG_TASK__
#define __DEBUG_TASK__

#include "uartdriver.h"

#define FRFPORT_MAX_COMMPACKET_MSG      32
#define FRFPORT_MAX_COMMPACKET_SIZE     256


class DebugTask;
class DebugTask
{
	public:
		// constructor and destructor
		DebugTask();
		~DebugTask();
	
		 static DebugTask*  GetInstance(); 
	
		 void   Run();
     void   Initiliaze();
									
	private:
			bool  _bRun;
	
			 CUartDriver        *debug_link;
	
			 OS_EVENT           *m_pQSem;
	
			int RecvMsg();
			int SendMsg();
			static void 	ReceMsgTask(void);
      static void   SendMsgTask(void);
			static void   IMUTask(void);
			static DebugTask 		*_pDebugTask;
			void SendLoop(void);
	
			void SendDaemon(void);
			void ReceLoop(void);
	
			void SendPacket();
			void BurstPacket(void);
};


#endif
/* end of file */
