#ifndef _LCD_DRV_H_
#define _LCD_DRV_H_
/*******************************************
				  2.4´ç QVGAÏÔÊ¾Çý¶¯³ÌÐò
**********************************************/

#define LCD_COLOR U16
#define LCD_WIDTH	320
#define LCD_HEIGHT	240

void LCD_WR_CMD(unsigned int index);
void LCD_WR_REG(unsigned int index,unsigned int val);
unsigned int LCD_RD_data(void);
void LCD_WR_Data(unsigned int val);
void LCD_Setxy(U16 x,U16 y);
void LCD_SetArea(U16 x0,U16 y0,U16 x1,U16 y1);
void LCD_DrawPixel(U16 x,U16 y,LCD_COLOR c);
U8 LCD_ReadPixel(U16 x,U16 y,LCD_COLOR *c);
void LCD_HLine(U16 x0, U16 y0, U16 x1,LCD_COLOR c);
void LCD_VLine(U16 x0, U16 y0, U16 y1,LCD_COLOR c);
void LCD_FillScr(LCD_COLOR c);
void LCD_Init(void);
#endif
