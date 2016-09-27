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


void RCC_Configuration(void)
{
	int HSEStartUpStatus=0;
  RCC_DeInit();
  RCC_HSEConfig(RCC_HSE_ON);   
  HSEStartUpStatus = RCC_WaitForHSEStartUp();
  if(HSEStartUpStatus == SUCCESS)        
  {   
    RCC_HCLKConfig(RCC_SYSCLK_Div1); 
    RCC_PCLK2Config(RCC_HCLK_Div1);   
    RCC_PCLK1Config(RCC_HCLK_Div2);   
    FLASH_SetLatency(FLASH_Latency_2);   
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);    
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_6);    
    RCC_PLLCmd(ENABLE);
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET) ;    
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
    while(RCC_GetSYSCLKSource() != 0x08);     
  }
}

int main(void)
{
	//RCC_Configuration();
	SysInit();
		core.mainport = uartOpen(USART1, NULL, 9600, MODE_RXTX);
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
			
