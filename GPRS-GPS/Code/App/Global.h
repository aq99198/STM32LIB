#ifndef __GLOBAL_H__
#define __GLOABL_H__


/********            INCLUDE FILES        ***********/
#include "ucos_ii.h"


extern  OS_EVENT 						*SemUartW1;
extern  OS_EVENT 						*SemUartW3;
extern  OS_EVENT 						*SemUartW2;
extern  OS_EVENT            *SemUartW5;

extern 	OS_EVENT         		*SemUart1;
extern 	OS_EVENT         		*SemUart2;
extern 	OS_EVENT         		*SemUart3;
extern 	OS_EVENT         		*SemUart5;

//                                                 // ResourceMute for malloc , malloc is not thread safe function
extern  OS_EVENT            *g_pResourceMutex;
extern  OS_EVENT            *MutexUartW5;
extern  OS_EVENT            *SemGprsLinkConnected;

// extern  OS_EVENT            *g_pTimer;          // the task wait for the event ,and caculate the altitude ,driver the motor
extern  INT8U                g_u8Rerr;          // the error code for create EVENT
extern  INT8U								 g_waitTime;

extern  INT32U JCLOUD_APID;
extern  INT32U JCLOUD_GSMID;
extern  INT32U JCLOUD_IP;
extern  INT32U JCLOUD_PORT;
extern  INT32U JCLOUD_BUGPS;

// connection
extern bool m_bConnect;
extern bool bConnect;

extern OS_EVENT *semMutex;
extern UINT8 resetFlag;

extern volatile INT8U  SystemStatus;



#define SYS_STARTUP     	   (0)
#define SYS_SIM900A_ON    	 (1)
#define SYS_CONNECT_SERVER   (2)
#define SYS_GPS_FIX   			 (3)
#define SYS_SAVE  		       (4)


/************************  debug control  ***************************/
//#define DEBUG_INFO  /* all debug output switch */
//#define DEBUG_ON    /* watch the stack */
//#define SIM900A_DEBUG_ON
//#define UCLOUD_DEBUG_ON
//#define GPS_DEBUG_ON


/************************ the global Function ***************************/


#define APP_TASK_APLINKRX_STK_SIZE   256
#define APP_TASK_START_STK_SIZE      256
#define APP_TASK_UBLOX_STK_SIZE      512			//max can use stack size 2048bit
#define APP_TASK_SERVER_TX_STK_SIZE  256
#define APP_TASK_SERVER_RX_STK_SIZE  256
#define APP_TASK_RECORD_COM_STK_SIZE 256


#define APP_MUTEX_APLINKTX_PRIO      21
#define APP_MUTEX_UART5_PRIO         22
#define APP_TASK_START_PRIO          10


#define APP_TASK_APLINKRX_PRIO       2
#define APP_TASK_UBLOX_PRIO          3
#define APP_TASK_SERVER_TX_PRIO      5
#define APP_TASK_SERVER_RX_PRIO      6
#define APP_TASK_RECORD_COM_PRIO     8

__align(8) extern OS_STK		App_Task_APLINKRX_Stack[APP_TASK_APLINKRX_STK_SIZE];
__align(8) extern OS_STK		App_Task_START_Stack[APP_TASK_START_STK_SIZE];
__align(8) extern OS_STK		App_Task_UBLOX_Stack[APP_TASK_UBLOX_STK_SIZE];
__align(8) extern OS_STK		App_Task_SERVER_TX_Stack[APP_TASK_SERVER_TX_STK_SIZE];
__align(8) extern OS_STK		App_Task_SERVER_RX_Stack[APP_TASK_SERVER_RX_STK_SIZE];
__align(8) extern OS_STK		App_Task_RECORD_COM_Stack[APP_TASK_RECORD_COM_STK_SIZE];


/************************ the debug Task ***************************/
#define APP_DebugTASK1_STK_SIZE  512
#define APP_DebugTASK2_STK_SIZE  512
#define APP_DebugTASK3_STK_SIZE  512

#define APP_DebugTASK1_PRIO 11
#define APP_DebugTASK2_PRIO 12
#define APP_DebugTASK3_PRIO 13

__align(8) extern OS_STK		App_DebugTask1Stack[APP_DebugTASK1_STK_SIZE];
__align(8) extern OS_STK		App_DebugTask2Stack[APP_DebugTASK2_STK_SIZE];
__align(8) extern OS_STK		App_DebugTask3Stack[APP_DebugTASK3_STK_SIZE];

#endif /* end of __GLOBAL_H__*/
/* end of file */
