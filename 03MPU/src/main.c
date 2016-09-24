#include "board.h"
#include "init.h"

core_t core;
drv_pwm_config_t pwm_params;
uint16_t acc_1G = 256; 
// from system_stm32f10x.c
uint8_t sensorsOK;

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
		core.flexport = uartOpen(USART3, NULL, 9600, MODE_RXTX);
	
		pwm_params.airplane=true;
		pwm_params.motorPwmRate=400;
		pwm_params.servoPwmRate=50;
		pwm_params.idlePulse = 1000;
		pwm_params.useServos = true;
		pwm_params.servoCenterPulse = 1500;
		pwmInit(&pwm_params);
		pwmWriteServo(0,1500);
		pwmWriteServo(1,1500);
		pwmWriteServo(2,1500);
	
	// drop out any sensors that don't seem to work, init all the others. halt if gyro is dead.
    sensorsOK  = mpuDetect(&acc, &gyro, &mpu_config);
	
	while(1){
		
		LED1_TOGGLE;
		//LED0_TOGGLE;
		delay(1000);
	}
}
			
