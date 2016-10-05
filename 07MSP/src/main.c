#include "board.h"
#include "init.h"


int fputc(int c, FILE *f)
{
  
}

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
			
