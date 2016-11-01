#include "board.h"


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




