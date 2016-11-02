#include "board.h"
#include "uartdriver.h"
#include "debugTask.h"
#include "ucloudTask.h"
#include "ubloxTask.h"

void App_Task0();
void  Facade(void);


int main(void)
{
			GPRS_Wakeup();
	
			bsp_init();
	
			SystemStatus = SYS_STARTUP;
	
      OSInit();

	    OSTaskCreateExt(
											(void (*)(void *)) App_Task0,
											(void          * ) 0,
											(OS_STK        * )&App_Task_START_Stack[APP_TASK_START_STK_SIZE - 1],
											(INT8U         ) APP_TASK_START_PRIO,
											(INT16U        ) APP_TASK_START_PRIO,
											(OS_STK        * )&App_Task_START_Stack[0],
											(INT32U          ) APP_TASK_START_STK_SIZE,
											(void          * )0,
											(INT16U        )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK)
									 );

      OSStart();
      return 0;
}

void App_Task0()
{
		OS_CPU_SysTickInit();

#if (OS_TASK_STAT_EN > 0)
		OSStatInit();
#endif

	
	SerialConsole = new CUartDriver(USART1_IDX);
	
	Facade();
	
	DebugTask *debugTask = DebugTask::GetInstance();
  debugTask->Run();
	
	CUbloxGPS * gps = CUbloxGPS::GetInstance();
  gps->Run();
	
	/* This task should be put at the end */
	Ucloud *server = Ucloud::getIntance();
  server->Run();
	
	while(1)
	{
		
		//printf("printTest\r\n");
		OSTimeDly(30);
		//LED0_TOGGLE;
	}
}


#define BuildList "**BuildTime " __DATE__" " __TIME__ "\r\n"
void  Facade(void)
{
	DEBUG("************** JOUAV GPRS **************** \r\n");
	DEBUG("**Version bate v1.4\r\n");
	DEBUG(BuildList);
	DEBUG("************** JOUAV GPRS **************** \r\n");
}



