#include "board.h"
#include "init.h"

core_t core;
extern rcReadRawDataPtr rcReadRawFunc;
extern uint32_t sbusChannelData[8];


int fputc(int c, FILE *f)
{
    // let DMA catch up a bit when using set or dump, we're too fast.
    //while (!serialTotalBytesWaiting(core.mainport));
    serialWrite(core.mainport, c);
    return c;
}
bool rcReady=false;
int main(void)
{
		SysInit();
		sbusInit(&rcReadRawFunc);
		core.mainport = uartOpen(USART1, NULL, 9600, MODE_RXTX);
	while(1){
		rcReady = sbusFrameComplete();
		printf("reReady = %d \r\n",(uint16_t)(0.625f*sbusChannelData[0]+880));
		rcReady = false;
		LED0_TOGGLE;
		delay(20);
	}
}
			
