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
		core.mainport = uartOpen(USART1, NULL, 9600, MODE_RXTX);
		core.flexport = uartOpen(USART3, NULL, 9600, MODE_RXTX);
	while(1){
		printf("jin \r\n");
		LED0_TOGGLE;
		delay(20);
	}
}
			
