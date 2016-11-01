#include "board.h"
#include "uartdriver.h"
#include "debugTask.h"
#include "ucloudTask.h"
#include "ubloxTask.h"

void App_Task0();

int main(void)
{
			bsp_init();
			USART1_Init();
			USART2_Init();
			UART5_Init();
	
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

	SemUartW1 = OSSemCreate(1);
	SemUartW2 = OSSemCreate(1);
	SemUartW3 = OSSemCreate(1);
	SemUartW5 = OSSemCreate(1);
	
	SemUart1 = OSSemCreate(0);
	SemUart2 = OSSemCreate(0);
	SemUart3 = OSSemCreate(0);
	
	//MutexUartW5 = OSMutexCreate(APP_TASK_START_PRIO, &g_u8Rerr);
	
	uint8_t a[5]={'a','b','c','d','f'};
	uint8_t *p =a;
	
	uint8_t c[5]={'g','h','i','j','k'};
	uint8_t *p2 =c;
	
	OS_STK_DATA StackBytes;
	
		
	SerialConsole = new CUartDriver(USART1_IDX);
	
	
	DebugTask *debugTask = DebugTask::GetInstance();
  debugTask->Run();
	
	CUbloxGPS * gps = CUbloxGPS::GetInstance();
  //gps->Run();
	
	Ucloud *server = Ucloud::getIntance();
  server->Run();
	
	
	// 

	
	while(1)
	{
		
		
		
		printf("printTest\r\n");
		
			OSTimeDly(30);
			//LED0_TOGGLE;
	}
}



