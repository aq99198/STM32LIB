/*******************************************
				  2.4寸 QVGA显示驱动程序
**********************************************/
//#include "fsmc_sram.h"
#include "includes.h"

#define Bank1_LCD_C    ((uint32_t)0x60000000)	 //disp Reg ADDR
#define Bank1_LCD_D    ((uint32_t)0x60020000)    //disp Data ADDR
//颜色的数据类型

//LCD_COLOR	LCD_ForeColor;//前景色
//LCD_COLOR	LCD_BackColor;//背景色

unsigned long color1=0;
unsigned int color[]={0xf800,0x07e0,0x001f,0xffe0,0x0000,0xffff,0x07ff,0xf81f};

void LCD_Delay(__IO uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}


//写寄存器地址函数
void LCD_WR_CMD(unsigned int index)
{
	*(__IO uint16_t *) (Bank1_LCD_C)= index;
}
//写寄存器数据函数
//输入：dbw 数据位数，1为16位，0为8位。
void LCD_WR_REG(unsigned int index,unsigned int val)
{	
	*(__IO uint16_t *) (Bank1_LCD_C)= index;	
	*(__IO uint16_t *) (Bank1_LCD_D)= val;
}

unsigned int LCD_RD_data(void)
{
	unsigned int a=0;
	//a=(*(__IO uint16_t *) (Bank1_LCD_D)); 	//Dummy
	//a= *(__IO uint16_t *) (Bank1_LCD_D);  	//H
	//a=a<<8;
	a=*(__IO uint16_t *) (Bank1_LCD_D); //L
	return(a);	
}

//写16位数据函数
void    LCD_WR_Data(unsigned int val)
{   
	*(__IO uint16_t *) (Bank1_LCD_D)= val; 	
}

void LCD_WR_Data_8(unsigned int val)
{
	*(__IO uint16_t *) (Bank1_LCD_D)= val;
}
//注意,这是横屏方向,与初始设置中的x\y是倒过来的
//竖屏的左上角为0,0
void LCD_Setxy(U16 x,U16 y)
{
	LCD_WR_REG(0x0020, LCD_HEIGHT-1-y); /* GRAM horizontal Address */
	LCD_WR_REG(0x0021, x); /* GRAM Vertical Address */
	LCD_WR_CMD(0x0022);	//写显示数据
}
//注意,这是横屏方向
void LCD_SetArea(U16 x0,U16 y0,U16 x1,U16 y1)
{
	LCD_WR_REG(0x0050, LCD_HEIGHT-1-y1); /* Horizontal GRAM Start Address */
	LCD_WR_REG(0x0051, LCD_HEIGHT-1-y0); /* Horizontal GRAM End Address */
	LCD_WR_REG(0x0052, x0); /* Vertical GRAM Start Address */
	LCD_WR_REG(0x0053, x1); /* Vertical GRAM End Address */
	LCD_Setxy(x0,y0);
	//LCD_WR_CMD(0x0022);	//写显示数据
}

void LCD_DrawPixel(U16 x,U16 y,LCD_COLOR c)
{
	if((x<LCD_WIDTH)&&(y<LCD_HEIGHT))
	{
		LCD_Setxy(x,y);
		LCD_WR_Data(c);
	}
}

U8 LCD_ReadPixel(U16 x,U16 y,LCD_COLOR *c)
{
	if((x<LCD_WIDTH)&&(y<LCD_HEIGHT))
	{
		LCD_Setxy(x,y);
		*c=LCD_RD_data();
		return 1;
	}
	return 0;
}

void LCD_HLine(U16 x0, U16 y0, U16 x1,LCD_COLOR c)
{
	U16 i;
	if(x0>x1){i=x0;x0=x1;x1=i;}//保证x0<x1
	for(i=x0;i<=x1;i++)
	{
		LCD_DrawPixel(i,y0,c);
	}
}

void LCD_VLine(U16 x0, U16 y0, U16 y1,LCD_COLOR c)
{
	U16 i;
	if(y0>y1){i=y0;y0=y1;y1=i;}
	for(i=y0;i<=y1;i++)
	{
		LCD_DrawPixel(x0,i,c);
	}
}

//以颜色c填充整屏
void LCD_FillScr(LCD_COLOR c)
{
	U16 x,y;
	for(x=0;x<LCD_WIDTH;x++)
	{
		for(y=0;y<LCD_HEIGHT;y++)
		{
			LCD_WR_Data(c);
		}
	}
}

//初始化函数
void LCD_Init(void)
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);		
	FSMC_LCD_Init();
	//void LCD_RST(void){
	GPIO_ResetBits(GPIOE, GPIO_Pin_1);
    LCD_Delay(0xAFFFFf);					   
    GPIO_SetBits(GPIOE, GPIO_Pin_1 );		 	 
	LCD_Delay(0xAFFFFf);//}
	
	LCD_WR_REG(0x00E3, 0x3008); // Set internal timing
	LCD_WR_REG(0x00E7, 0x0012); // Set internal timing
	LCD_WR_REG(0x00EF, 0x1231); // Set internal timing
	LCD_WR_REG(0x0000, 0x0001); // Start Oscillation
	LCD_WR_REG(0x0001, 0x0100); // set SS and SM bit
	LCD_WR_REG(0x0002, 0x0700); // set 1 line inversion
//CMD:0x0003设置显示起始位置(从该处向右再向下) 
//向右扫描的配置
//0x1030:左上起,0x1018:左下起,0x1000:右下起,0x1028:右上起
//向左扫描的配置
//0x1038:左上起,0x1010:左下起,0x1008:右下起,0x1020:右上起
	LCD_WR_REG(0x0003, 0x1020); // set GRAM write direction and BGR=0,262K colors,1 transfers/pixel.
	LCD_WR_REG(0x0004, 0x0000); // Resize register
	LCD_WR_REG(0x0008, 0x0202); // set the back porch and front porch
	LCD_WR_REG(0x0009, 0x0000); // set non-display area refresh cycle ISC[3:0]
	LCD_WR_REG(0x000A, 0x0000); // FMARK function
	LCD_WR_REG(0x000C, 0x0000); // RGB interface setting
	LCD_WR_REG(0x000D, 0x0000); // Frame marker Position
	LCD_WR_REG(0x000F, 0x0000); // RGB interface polarity
//Power On sequence 
	LCD_WR_REG(0x0010, 0x0000); // SAP, BT[3:0], AP, DSTB, SLP, STB
	LCD_WR_REG(0x0011, 0x0007); // DC1[2:0], DC0[2:0], VC[2:0]
	LCD_WR_REG(0x0012, 0x0000); // VREG1OUT voltage
	LCD_WR_REG(0x0013, 0x0000); // VDV[4:0] for VCOM amplitude
	LCD_Delay(200); // Dis-charge capacitor power voltage
	LCD_WR_REG(0x0010, 0x1690); // SAP, BT[3:0], AP, DSTB, SLP, STB
	LCD_WR_REG(0x0011, 0x0227); // R11h=0x0221 at VCI=3.3V, DC1[2:0], DC0[2:0], VC[2:0]
	LCD_Delay(50); // Delay 50ms
	LCD_WR_REG(0x0012, 0x001C); // External reference voltage= Vci;
	LCD_Delay(50); // Delay 50ms
	LCD_WR_REG(0x0013, 0x1800); // R13=1200 when R12=009D;VDV[4:0] for VCOM amplitude
	LCD_WR_REG(0x0029, 0x001C); // R29=000C when R12=009D;VCM[5:0] for VCOMH
	LCD_WR_REG(0x002B, 0x000D); // Frame Rate = 91Hz
	LCD_Delay(50); // Delay 50ms
	LCD_WR_REG(0x0020, 0x0000); // GRAM horizontal Address
	LCD_WR_REG(0x0021, 0x0000); // GRAM Vertical Address
// ----------- Adjust the Gamma Curve ----------//
	LCD_WR_REG(0x0030, 0x0007);
	LCD_WR_REG(0x0031, 0x0302);
	LCD_WR_REG(0x0032, 0x0105);
	LCD_WR_REG(0x0035, 0x0206);
	LCD_WR_REG(0x0036, 0x0808);
	LCD_WR_REG(0x0037, 0x0206);
	LCD_WR_REG(0x0038, 0x0504);
	LCD_WR_REG(0x0039, 0x0007);
	LCD_WR_REG(0x003C, 0x0105);
	LCD_WR_REG(0x003D, 0x0808);
//------------------ Set GRAM area ---------------//
	LCD_WR_REG(0x0050, 0x0000); // Horizontal GRAM Start Address
	LCD_WR_REG(0x0051, 0x00EF); // Horizontal GRAM End Address
	LCD_WR_REG(0x0052, 0x0000); // Vertical GRAM Start Address
	LCD_WR_REG(0x0053, 0x013f); // Vertical GRAM Start Address
	LCD_WR_REG(0x0060, 0xA700); // Gate Scan Line
	LCD_WR_REG(0x0061, 0x0001); // NDL,VLE, REV
	LCD_WR_REG(0x006A, 0x0000); // set scrolling line
//-------------- Partial Display Control ---------//
	LCD_WR_REG(0x0080, 0x0000);
	LCD_WR_REG(0x0081, 0x0000);
	LCD_WR_REG(0x0082, 0x0000);
	LCD_WR_REG(0x0083, 0x0000);
	LCD_WR_REG(0x0084, 0x0000);
	LCD_WR_REG(0x0085, 0x0000);
//-------------- Panel Control -------------------//
	LCD_WR_REG(0x0090, 0x0010);
	LCD_WR_REG(0x0092, 0x0000);
	LCD_WR_REG(0x0093, 0x0003);
	LCD_WR_REG(0x0095, 0x0110);
	LCD_WR_REG(0x0097, 0x0000);
	LCD_WR_REG(0x0098, 0x0000);
	LCD_WR_REG(0x0007, 0x0133); // 262K color and display ON
		
	//LCD_WR_REG(0x0003,0x1030);//CMD:0x0003:0x1030:左上起,0x1018:左下起,0x1010):右下起,0x1028:右上起
	LCD_WR_CMD(0x0022);	//写显示数据
	LCD_FillScr(0xffff);
}





/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
