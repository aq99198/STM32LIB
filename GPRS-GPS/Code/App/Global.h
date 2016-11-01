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



/************************ the global Function ***************************/


//#define APP_TASK_APLINKTX_STK_SIZE   512
#define APP_TASK_APLINKRX_STK_SIZE   512
#define APP_TASK_START_STK_SIZE      40
#define APP_TASK_UBLOX_STK_SIZE      1024
#define APP_TASK_SERVER_TX_STK_SIZE  512
#define APP_TASK_SERVER_RX_STK_SIZE  512
//#define APP_TASK_RECORD_READ_STK_SIZE 512
#define APP_TASK_RECORD_COM_STK_SIZE  512


#define APP_MUTEX_APLINKTX_PRIO      1
#define APP_MUTEX_UART5_PRIO         2
#define APP_TASK_START_PRIO          10
//#define APP_TASK_APLINKTX_PRIO       3
#define APP_TASK_APLINKRX_PRIO       4
#define APP_TASK_UBLOX_PRIO          9
#define APP_TASK_SERVER_TX_PRIO      5
#define APP_TASK_SERVER_RX_PRIO      6
//#define APP_TASK_RECORD_READ_PRIO    7
#define APP_TASK_RECORD_COM_PRIO     8

//__align(8) extern OS_STK		App_Task_APLINKTX_Stack[APP_TASK_APLINKTX_STK_SIZE];
__align(8) extern OS_STK		App_Task_APLINKRX_Stack[APP_TASK_APLINKRX_STK_SIZE];
__align(8) extern OS_STK		App_Task_START_Stack[APP_TASK_START_STK_SIZE];
__align(8) extern OS_STK		App_Task_UBLOX_Stack[APP_TASK_UBLOX_STK_SIZE];
__align(8) extern OS_STK		App_Task_SERVER_TX_Stack[APP_TASK_SERVER_TX_STK_SIZE];
__align(8) extern OS_STK		App_Task_SERVER_RX_Stack[APP_TASK_SERVER_RX_STK_SIZE];
//__align(8) extern OS_STK		App_Task_RECORD_READ_Stack[APP_TASK_RECORD_READ_STK_SIZE];
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
