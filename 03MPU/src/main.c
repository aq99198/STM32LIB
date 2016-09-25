#include "board.h"
#include "init.h"

core_t core;
drv_pwm_config_t pwm_params;
uint16_t acc_1G = 256; 
// from system_stm32f10x.c
uint8_t sensorsOK;
sensor_t acc;                       // acc access functions
sensor_t gyro;                      // gyro access functions
int16_t accData[3];
int16_t gyroData[3];

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
		// USART3 =	 I2C 
		//core.flexport = uartOpen(USART3, NULL, 9600, MODE_RXTX);
	
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
		i2cInit(I2C_DEVICE);
	// drop out any sensors that don't seem to work, init all the others. halt if gyro is dead.
    mpuDetect(&acc,&gyro);
	
	while(1){
		mpuAccRead(accData);
		mpuGyroRead(gyroData);
		printf("AX=%8d, AY=%8d, AZ=%8d, GX=%8d, GY=%8d, GZ=%8d \r\n",accData[0],accData[1],accData[2],gyroData[0],gyroData[1],gyroData[2]);	
		LED1_TOGGLE;
		//LED0_TOGGLE;
		delay(100);
	}
}
			
