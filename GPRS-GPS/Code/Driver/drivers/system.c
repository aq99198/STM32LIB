#include "board.h"

#define SET_LED_MODE(mode)  do{ LedMode = mode ;}while(0)

void GPRS_Wakeup(void){
	JTAG_Set(01);
	Power_ON();
	OSTimeDly(30);
}

void JTAG_Set(u8 mode)
{
	u32 temp;
	temp=mode;
	temp<<=25;
	RCC->APB2ENR|=1<<0;//开启辅助时钟
	AFIO->MAPR&=0XF8FFFFFF;//清除MAPR的[26:24]
	AFIO->MAPR|=temp;//设置jtag模式
}

void Power_ON(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_3);
	
	GPIO_SetBits(GPIOB,GPIO_Pin_3);
}



void write_onchip_flash(u32 addr,u8 *p,u16 n)
{
	u32 r1;
	n=n/4;
	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
	FLASH_ErasePage(addr);
	while(n--)
	{
		r1=*(p++);
		r1|=*(p++)<<8;
		r1|=*(p++)<<16;
		r1|=*(p++)<<24;
		FLASH_ProgramWord(addr, r1);
		addr+=4;
	}
	FLASH_Lock();
}


void read_onchip_flash(u32 addr,u8 *p,u16 n)
{
	while(n--)
	{
		*(p++)=*((u8*)addr++);
	}
}


void EndianSwap32(void *p)
{
    static char *pc1, *pc2;
    static unsigned int b;
    pc1 = (char *)p;
    pc2 = (char *)&b;
    pc2[3] = pc1[0];
    pc2[2] = pc1[1];
    pc2[1] = pc1[2];
    pc2[0] = pc1[3];
    *((unsigned int *)p) = b;
}

void EndianSwap16(void *p)
{
    static char *pc1, *pc2;
    static unsigned short int b;
    pc1 = (char *)p;
    pc2 = (char *)&b;
    pc2[1] = pc1[0];
    pc2[0] = pc1[1];
    *((unsigned short int *)p) = b;
}




