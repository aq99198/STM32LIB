#include "board.h"
#include "init.h"
#include "stm32f10x_gpio.h"


int main(void)
{
		SysInit();
	
	serialInit(115200);
	while(1){
		serialCom();
		LED0_TOGGLE;
		delay(1);
	}
}
			
