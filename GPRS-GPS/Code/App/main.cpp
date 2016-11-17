#include "board.h"
#include "uartdriver.h"
#include "debugTask.h"
#include "ucloudTask.h"
#include "ubloxTask.h"
#include "APlinkTask.h"

#include "stm32f10x_it.h"

void App_Task0();
void  Facade(void);


int main(void)
{	

			__disable_irq();
	
			GPRS_Wakeup();
	
			bsp_init();
	
			SystemStatus = SYS_SIM900A_ON;
	
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
//	
	//DebugTask *debugTask = DebugTask::GetInstance();
  //debugTask->Run();
//	
	CUbloxGPS * gps = CUbloxGPS::GetInstance();
  gps->Run();

	CAPlink *aplink = CAPlink::GetInstance();
	aplink->Run();
	
	/* This task necessarily put at the task end */
	Ucloud *server = Ucloud::getIntance();
  server->Run();
	
	while(1)
	{
		
		//BEEP_TOGGLE;
		OSTimeDly(1000);
		//serialCom();
	}
}



#define BuildList "**BuildTime " __DATE__" " __TIME__ "\r\n"
void  Facade(void)
{
	PRINT("************** JOUAV GPRS **************** \r\n");
	
	PRINT("**Version bate v1.4\r\n");
	PRINT(BuildList);
	PRINT("************** JOUAV GPRS **************** \r\n");
}



