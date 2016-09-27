#include "board.h"
#include "init.h"

core_t core;
drv_pwm_config_t pwm_params;
// from system_stm32f10x.c
uint8_t sensorsOK;
sensor_t acc;                       // acc access functions
sensor_t gyro;                      // gyro access functions


int fputc(int c, FILE *f)
{
    // let DMA catch up a bit when using set or dump, we're too fast.
    //while (!serialTotalBytesWaiting(core.mainport));
    serialWrite(core.mainport, c);
    return c;
}

int main(void)
{

		SysInit();
		core.mainport = uartOpen(USART1, NULL, 9600, MODE_RXTX);
		i2cInit(I2C_DEVICE);
	// drop out any sensors that don't seem to work, init all the others. halt if gyro is dead.
    mpuDetect(&acc,&gyro);
		imuInit();
	
	while(1){
		computeIMU();
		//printf("AX=%8d, AY=%8d \r\n",accADC[0],accADC[1]);
		printf("ROLL=%8d, PITCH=%8d \r\n",angle[0],angle[1]);	
		LED1_TOGGLE;
		//LED0_TOGGLE;
		delay(100);
	}
}
			
