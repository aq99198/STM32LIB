#include "board.h"
#include "init.h"

core_t core;

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
		drv_adc_config_t adc_params;
		adc_params.rssiAdcChannel=1;
		adc_params.powerAdcChannel = 5;
		adcInit(&adc_params);
		core.mainport = uartOpen(USART1, NULL, 115200, MODE_RXTX);
	while(1){
		// 1,5,9
		printf("ADC1=%8d,ADC5=%8d,ADC9=%8d \r\n",adcGetChannel(1),adcGetChannel(5),adcGetChannel(9));
		LED0_TOGGLE;
		delay(40);
	}
}
			
