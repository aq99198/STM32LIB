#include "board.h"
#include "debugTask.h"
#include "ubloxTask.h"

void App_Task0();

int main(void)
{
			bsp_init();
			USART1_Init();
			USART2_Init();
	
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
	
	DebugTask *debugTask = DebugTask::GetInstance();
  debugTask->Run();
	
	CUbloxGPS * gps = CUbloxGPS::GetInstance();
  gps->Run();
	
	// 
	SerialConsole = new CUartDriver(USART1_IDX);
	
	while(1)
	{
		
			//UART1_send_byte('a');
		
	
				//DEBUG("it work!\r\n");
				//ERROR("sessed!\r\n");
		
//			OSSemPend(SemUartW1,0,&g_u8Rerr);
			  USART2_DMA(p, 5);
//			OSSemPend(SemUartW1,0,&g_u8Rerr);
//			USART1_DMA(p2, 5);
//			OSTaskStkChk(APP_TASK_START_PRIO, &StackBytes);
//			printf("StackBytes.OSFree=%d\t StackBytes.OSUsed=%d\r\n",StackBytes.OSFree,StackBytes.OSUsed);
//			//printf("word");
		
		
			OSTimeDly(30);
			//LED0_TOGGLE;
	}
}



