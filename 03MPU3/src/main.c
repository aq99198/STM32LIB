#include "board.h"
#include "init.h"

core_t core;
drv_pwm_config_t pwm_params;
// from system_stm32f10x.c
u8 mpu6050_buffer[14];

int16_t ACCX;
int16_t ACCY;
int16_t ACCZ;

int16_t GRYOX;
int16_t GRYOY;
int16_t GRYOZ;

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
//		core.flexport = uartOpen(USART3, NULL, 9600, MODE_RXTX);
//	
//		pwm_params.airplane=true;
//		pwm_params.motorPwmRate=400;
//		pwm_params.servoPwmRate=50;
//		pwm_params.idlePulse = 1000;
//		pwm_params.useServos = true;
//		pwm_params.servoCenterPulse = 1500;
//		pwmInit(&pwm_params);
//		pwmWriteServo(0,1500);
//		pwmWriteServo(1,1500);
//		pwmWriteServo(2,1500);
	
	MPU6050_Init(1);
	while(1){
		
	MPU6050_Read();

	ACCX = ((((int16_t)mpu6050_buffer[0]) << 8) | mpu6050_buffer[1]) ;
	ACCY = ((((int16_t)mpu6050_buffer[2]) << 8) | mpu6050_buffer[3]) ;
	ACCZ = ((((int16_t)mpu6050_buffer[4]) << 8) | mpu6050_buffer[5]) ;
 
	GRYOX = ((((int16_t)mpu6050_buffer[8]) << 8) | mpu6050_buffer[ 9]) ;
	GRYOY = ((((int16_t)mpu6050_buffer[10]) << 8) | mpu6050_buffer[11]) ;
	GRYOZ = ((((int16_t)mpu6050_buffer[12]) << 8) | mpu6050_buffer[13]) ;
	LED1_TOGGLE;
		
	printf("AX=%8d, AY=%8d, AZ=%8d, GX=%8d, GY=%8d, GZ=%8d \r\n",ACCX,ACCY,ACCZ,GRYOX,GRYOY,GRYOZ);	
		//LED0_TOGGLE;
		delay(100);
	}
}
			
