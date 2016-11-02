/*
@brief this file`s aim is that definition the global variables
*/

#include "board.h"
#include "Global.h"
/************************ the global varisbles ***************************/
           // the cradle heading configuration information buffer


INT8U                g_u8Rerr;          // the error code for create EVENT



OS_EVENT         		*SemUart1 = OS_EVENT_NULL;
OS_EVENT         		*SemUart2 = OS_EVENT_NULL;
OS_EVENT         		*SemUart3 = OS_EVENT_NULL;
OS_EVENT         		*SemUart5 = OS_EVENT_NULL;


OS_EVENT 						*SemUartW1 = OS_EVENT_NULL;
OS_EVENT 						*SemUartW3 = OS_EVENT_NULL;
OS_EVENT 						*SemUartW2 = OS_EVENT_NULL;
OS_EVENT            *SemUartW5 = OS_EVENT_NULL;


OS_EVENT            *g_pResourceMutex;
OS_EVENT            *MutexUartW5;
OS_EVENT            *SemGprsLinkConnected;



INT32U JCLOUD_APID = 0;//0xABCD0001;
INT32U JCLOUD_GSMID = 0;//0x00000002;
INT32U JCLOUD_IP = 0;
INT16U JCLOUD_PORT = 0;
volatile INT8U  SystemStatus;


//__align(8) OS_STK		App_Task_APLINKTX_Stack[APP_TASK_APLINKTX_STK_SIZE];
__align(8) OS_STK		App_Task_APLINKRX_Stack[APP_TASK_APLINKRX_STK_SIZE];
__align(8) OS_STK		App_Task_START_Stack[APP_TASK_START_STK_SIZE];
__align(8) OS_STK		App_Task_UBLOX_Stack[APP_TASK_UBLOX_STK_SIZE];
__align(8) OS_STK		App_Task_SERVER_TX_Stack[APP_TASK_SERVER_TX_STK_SIZE];
__align(8) OS_STK		App_Task_SERVER_RX_Stack[APP_TASK_SERVER_RX_STK_SIZE];
//__align(8) OS_STK		App_Task_RECORD_READ_Stack[APP_TASK_RECORD_READ_STK_SIZE];
__align(8) OS_STK		App_Task_RECORD_COM_Stack[APP_TASK_RECORD_COM_STK_SIZE];


/************************ the debug Task ***************************/
__align(8) OS_STK		App_DebugTask1Stack[APP_DebugTASK1_STK_SIZE];
__align(8) OS_STK		App_DebugTask2Stack[APP_DebugTASK2_STK_SIZE];
__align(8) OS_STK		App_DebugTask3Stack[APP_DebugTASK3_STK_SIZE];




/* end of file */




