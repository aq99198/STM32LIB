#include "board.h"
#include "uartdriver.h"
#include "debugTask.h"
#include "ucloudTask.h"
#include "ubloxTask.h"


#include "stm32f10x_it.h"

void App_Task0();
void  Facade(void);


void close_gpio(){
			GPIO_InitTypeDef GPIO_InitStructure;
			GPIO_StructInit(&GPIO_InitStructure);
		
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
			GPIO_Init(GPIOA, &GPIO_InitStructure);
			GPIO_Init(GPIOB, &GPIO_InitStructure);
			GPIO_Init(GPIOC, &GPIO_InitStructure);
			GPIO_Init(GPIOD, &GPIO_InitStructure);
			GPIO_Init(GPIOE, &GPIO_InitStructure);
}

int main(void)
{
			
			GPRS_Wakeup();
	
			bsp_init();
	
			SystemStatus = SYS_SIM900A_ON;
	
			//close_gpio();
			
			//Power_ON();
	
			//standbyMode();
	
	
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
	
//	Facade();
//	
//	DebugTask *debugTask = DebugTask::GetInstance();
//  debugTask->Run();
//	
//	CUbloxGPS * gps = CUbloxGPS::GetInstance();
//  gps->Run();
//	
//	/* This task should be put at the end */
//	Ucloud *server = Ucloud::getIntance();
//  server->Run();
	
	
	uint8_t c = 0xaa;
	while(1)
	{
		
		//printf("printTest\r\n");
		//OSTimeDly(1);
		serialCom();
		//SerialConsole->write(&c,1);
		//UART1_send_byte(c);
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



