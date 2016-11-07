//--------------------------------------------------
//		头文件
//--------------------------------------------------
#include "includes.h"
//--------------------------------------------------
//		定义
//--------------------------------------------------
//定义字体
typedef struct _FONT
{
    U16 Width;		//文字宽度
	U16 Height;		//文字高度
	U16 BPL;		//点阵一行所占字节数
	U16 AscFirst;	//第一个字的 ascii 码
	U16 AscLast;	//最后一个字的 ascii 码
	const U8* lib;		//字库
} FONT;
typedef FONT* PFONT;
typedef const FONT* PCFONT;
//定义默认的字体
const FONT Font8x5={6,8,1,32,127,Asc8x5_tab};
const FONT Font16x8={8,16,1,32,127,Asc16x8_tab};
const FONT* pGUIFont = &Font16x8;
GUI_COLOR	GUI_ForeColor=GUI_black;//前景色
GUI_COLOR	GUI_BackColor=GUI_white;//背景色
//#define GUI_COLOR U8
//320*240=76800
//*1/2=38400
//按每点占四位算
GUI_COLOR GUI_Buffer[LCD_HEIGHT][LCD_WIDTH/2];//显示屏内数据的缓冲
//16种颜色的表
LCD_COLOR GUI_ColorTab[16]={
COLOR_white,COLOR_black,COLOR_brown,COLOR_red,
COLOR_orange,COLOR_yellow,COLOR_green,COLOR_blue,
COLOR_purple,COLOR_darkgray,COLOR_darkred,COLOR_darkgreen,
COLOR_darkyellow,COLOR_darkblue,COLOR_darkcyan,COLOR_lightgray
};

//--------------------------------------------------
//		刷新一屏显示,GUI_Buffer的数据输出到LCD上
//--------------------------------------------------
void GUI_Update(void)
{
	U16 x,y;
	LCD_COLOR c;
	
	LCD_Setxy(0,0);
	for(x=0;x<(LCD_WIDTH>>1);x++)
	{
		for(y=0;y<LCD_HEIGHT;y++)
		{
			c=GUI_ColorTab[GUI_Buffer[y][x]&0x0f];//取低四位
			LCD_WR_Data(c);
		}
		for(y=0;y<LCD_HEIGHT;y++)
		{
			c=GUI_ColorTab[GUI_Buffer[y][x]>>4];//取高四位
			LCD_WR_Data(c);
		}			
	}
}
//--------------------------------------------------
//		刷新一个竖线,GUI_Buffer的数据输出到LCD上
//--------------------------------------------------
void GUI_UpdateVline(U16 x0, U16 y0, U16 y1)
{
	U16 y;
	LCD_COLOR c;	
	LCD_Setxy(x0,y0);
	if((x0&0x01)==0)//偶数
	{
		for(y=y0;y<=y1;y++)
		{
			c=GUI_ColorTab[GUI_Buffer[y][x0>>1]&0x0f];//取低四位
			LCD_WR_Data(c);
		}
	}
	else
	{
		for(y=y0;y<=y1;y++)
		{
			c=GUI_ColorTab[GUI_Buffer[y][x0>>1]>>4];//取高四位
			LCD_WR_Data(c);
		}
	}			
}
//--------------------------------------------------
//		//刷新一个区域,GUI_Buffer的数据输出到LCD上
//--------------------------------------------------
void GUI_UpdateArea(U16 x0, U16 y0, U16 x1, U16 y1)
{
	U16 x,y;
	LCD_COLOR c;	
	//LCD_Setxy(0,0);
	LCD_SetArea(x0,y0,x1,y1);	
	for(x=x0;x<=x1;x++)
	{
		if((x&0x01)==0)//偶数
		{
			for(y=y0;y<=y1;y++)
			{
				c=GUI_ColorTab[GUI_Buffer[y][x>>1]&0x0f];//取低四位
				LCD_WR_Data(c);
			}
		}
		else
		{
			for(y=y0;y<=y1;y++)
			{
				c=GUI_ColorTab[GUI_Buffer[y][x>>1]>>4];//取高四位
				LCD_WR_Data(c);
			}
		}			
	}
	LCD_SetArea(0,0,LCD_WIDTH-1,LCD_HEIGHT-1);
}
//--------------------------------------------------
//		GUI_Buffer中画点
//--------------------------------------------------
void GUI_DrawPixel(U16 x,U16 y,GUI_COLOR c)
{
	U16 xt;
	if((x<LCD_WIDTH)&&(y<LCD_HEIGHT))
	{
		xt=x>>1;
		if((x&BIT(0))==0)//偶数,存放在低位
		{
			GUI_Buffer[y][xt]&=0xf0;//清除低四位
			GUI_Buffer[y][xt]|=(c&0x0f);//设置低四位		
		}
		else
		{
			GUI_Buffer[y][xt]&=0x0f;//清除高四位
			GUI_Buffer[y][xt]|=(c&0x0f)<<4;//设置高四位			
		}
	}
}
//--------------------------------------------------
//		GUI_Buffer中读点
//--------------------------------------------------
U8 GUI_ReadPixel(U16 x,U16 y,GUI_COLOR *c)
{
	U16 xt;
	if((x<LCD_WIDTH)&&(y<LCD_HEIGHT))
	{
		xt=x>>1;
		if(x&BIT(0))//奇数,存放在高位
		{
			*c=GUI_Buffer[y][xt]>>4;		
		}
		else
		{
			*c=GUI_Buffer[y][xt]&0x0f;		
		}
		return 1;
	}
	return 0;
}
//--------------------------------------------------
//		GUI_Buffer中填充
//--------------------------------------------------
void GUI_FillScr(GUI_COLOR c)
{
	U16 x,y;
	for(x=0;x<LCD_WIDTH;x++)
	{
		for(y=0;y<LCD_HEIGHT;y++)
		{
			GUI_DrawPixel(x,y,c);
		}
	}
}
//--------------------------------------------------
//		GUI_Buffer中画横线
//--------------------------------------------------
void GUI_HLine(U16 x0, U16 y0, U16 x1,GUI_COLOR c)
{
	U16 i;
	if(x0>x1){i=x0;x0=x1;x1=i;}//保证x0<x1
	for(i=x0;i<=x1;i++)
	{
		GUI_DrawPixel(i,y0,c);
	}
}
//--------------------------------------------------
//		GUI_Buffer中竖线
//--------------------------------------------------
void GUI_VLine(U16 x0, U16 y0, U16 y1,GUI_COLOR c)
{
	U16 i;
	if(y0>y1){i=y0;y0=y1;y1=i;}
	for(i=y0;i<=y1;i++)
	{
		GUI_DrawPixel(x0,i,c);
	}
}
//--------------------------------------------------
//		GUI_Buffer中画矩形
//--------------------------------------------------
void  GUI_Rect(U16 x0, U16 y0, U16 x1, U16 y1, GUI_COLOR c)
{  
	GUI_HLine(x0, y0, x1, c);
	GUI_HLine(x0, y1, x1, c);
	GUI_VLine(x0, y0, y1, c);
	GUI_VLine(x1, y0, y1, c);
}
//--------------------------------------------------
//		GUI_Buffer中填充矩形
//--------------------------------------------------
void  GUI_FillRect(U16 x0, U16 y0, U16 x1, U16 y1, GUI_COLOR c)
{
	U16  i;
	/* 先找出矩形左上角与右下角的两个点，保存在(x0,y0)，(x1,y1) */
	if(x0>x1) 						// 若x0>x1，则x0与x1交换
	{
		i = x0;
		x0 = x1;
		x1 = i;
	}
	if(y0>y1)						// 若y0>y1，则y0与y1交换
	{  
		i = y0;
		y0 = y1;
		y1 = i;
	}	
	/* 判断是否只是直线 */
	if(y0==y1) 
	{
		GUI_HLine(x0, y0, x1, c);
		return;
	}
	if(x0==x1) 
	{
		LCD_VLine(x0, y0, y1, c);
		return;
	}
	
	while(y0<=y1)						
	{
		GUI_HLine(x0, y0, x1, c);	// 当前画水平线
		y0++;							// 下一行
	}
}
//--------------------------------------------------
//		GUI_Buffer中画线
//--------------------------------------------------
void  GUI_Line(S16 x0, S16 y0, S16 x1, S16 y1, GUI_COLOR c)
{
	S16   dx;						// 直线x轴差值变量
	S16   dy;          			// 直线y轴差值变量
	S8    dx_sym;					// x轴增长方向，为-1时减值方向，为1时增值方向
	S8    dy_sym;					// y轴增长方向，为-1时减值方向，为1时增值方向
	S16   dx_x2;					// dx*2值变量，用于加快运算速度
	S16   dy_x2;					// dy*2值变量，用于加快运算速度
	S16   di;						// 决策变量
    
	dx = x1-x0;						// 求取两点之间的差值
	dy = y1-y0;

   /* 判断增长方向，或是否为水平线、垂直线、点 */
   if(dx>0)							// 判断x轴方向
   {  dx_sym = 1;					// dx>0，设置dx_sym=1
   }
   else
   {  if(dx<0)
      {  dx_sym = -1;				// dx<0，设置dx_sym=-1
      }
      else
      {  // dx==0，画垂直线，或一点
         GUI_VLine(x0, y0, y1,c);
      	 return;
      }
   }
   
   if(dy>0)							// 判断y轴方向
   {  dy_sym = 1;					// dy>0，设置dy_sym=1
   }
   else
   {  if(dy<0)
      {  dy_sym = -1;				// dy<0，设置dy_sym=-1
      }
      else
      {  // dy==0，画水平线，或一点
         GUI_HLine(x0, y0, x1, c);
      	 return;
      }
   }
    
   /* 将dx、dy取绝对值 */
   dx = dx_sym * dx;
   dy = dy_sym * dy;
 
   /* 计算2倍的dx及dy值 */
   dx_x2 = dx*2;
   dy_x2 = dy*2;
   
   /* 使用Bresenham法进行画直线 */
   if(dx>=dy)						// 对于dx>=dy，则使用x轴为基准
   {  di = dy_x2 - dx;
      while(x0!=x1)
      {  GUI_DrawPixel(x0, y0, c);
         x0 += dx_sym;
         if(di<0)
         {  di += dy_x2;			// 计算出下一步的决策值
         }
         else
         {  di += dy_x2 - dx_x2;
            y0 += dy_sym;
         }
      }
      GUI_DrawPixel(x0, y0, c);		// 显示最后一点
   }
   else								// 对于dx<dy，则使用y轴为基准
   {  di = dx_x2 - dy;
      while(y0!=y1)
      {  GUI_DrawPixel(x0, y0, c);
         y0 += dy_sym;
         if(di<0)
         {  di += dx_x2;
         }
         else
         {  di += dx_x2 - dy_x2;
            x0 += dx_sym;
         }
      }
      GUI_DrawPixel(x0, y0, c);		// 显示最后一点
   }   
}
//--------------------------------------------------
//		设置输出文字时的前景色与背景色
//--------------------------------------------------
void GUI_SetColor(GUI_COLOR forecolor,GUI_COLOR backcolor)
{
	GUI_ForeColor=forecolor;
	GUI_BackColor=backcolor;
}
//--------------------------------------------------
//		设置8*5字体
//--------------------------------------------------
void GUI_SetFont8x5(void)
{
	pGUIFont = &Font8x5;
}
//--------------------------------------------------
//		设置16*8字体
//--------------------------------------------------
void GUI_SetFont16x8(void)
{
	pGUIFont = &Font16x8;
}
//--------------------------------------------------
//		GUI_Buffer中输出文字
//--------------------------------------------------
void GUI_PutChar(U16 x,U16 y, U16 asc)
{
	U8 i,j,w,h,c;	
	const U8* p;
	const U8 btmap[]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};//位序从高到低
	if(asc < pGUIFont->AscFirst)return;
	if(asc > pGUIFont->AscLast)return;
	w=pGUIFont->Width;
	h=pGUIFont->Height;	
	p=pGUIFont->lib;
	p+=(asc-pGUIFont->AscFirst)* pGUIFont->BPL * pGUIFont->Height;//找到字母所在的地址

	for(i=0;i<h;i++)//h
	{
		for(j=0;j<w;j++)//w
		{
			c=*(p + i* pGUIFont->BPL + j/8);//由i,j找到像素点所在字节
			//c=0xff;
			if(c & btmap[j %8])//像素所在位
			{
				GUI_DrawPixel(x+j,y+i,GUI_ForeColor);
			}
			else
			{
				GUI_DrawPixel(x+j,y+i,GUI_BackColor);
			}
		}
	}
}
//--------------------------------------------------
//		GUI_Buffer中输出字符串
//--------------------------------------------------
void GUI_PutStr(U16 x,U16 y, U8* s)
{
	while(*s)
	{
		GUI_PutChar(x,y,*s);//GAL_TextInRect(hdc, dst, *c, x, y);
		s++;
		x+=pGUIFont->Width;
	}
}
//--------------------------------------------------
//显示有符号数,digit为数据的位数,dot为小数位数
//--------------------------------------------------
void GUI_dispnum(S32 num,U8 digit,U8 dot,U16 x,U16 y)
{
	U8 bt,i,w,n[]={13},buf[15];//位数,要显示的数字 
	U32 num1;
	//digit=digit%13;//最大为12
	if(digit>12)digit=12;
	w=0;
	if(num<0)
	{
		num1=~num+1;//原码为补码加1
		buf[w++]='-';
	}
	else
	{
		num1=num;
		buf[w++]='+';
	}
	bt=0;
	for(i=0;i<6;i++)
	{
		n[i]=num1%10+'0';
		num1=num1/10;
		if(n[i]!='0')bt=i+1;//得数据位数
	}

	if(bt>digit)
	{
		dot=dot-(bt-digit);//小数位右移
	}
	else
	{
		bt=digit;
	}
	if(dot<1)//小数点显示不出来
	{
		digit=digit+1;//位数加1使得与有无小数存在的时候占用相同的位数
		if(digit>6)digit=6;
		bt=bt+dot+1;//计算启始位
		if(bt>6)bt=6;
	}
	for(i=0;i<digit;i++)
	{
		if(i==(digit-dot))
			buf[w++]='.';
		buf[w++]=n[bt-i-1];
	}
	buf[w++]=0;
	GUI_PutStr(x,y,buf);
	//GUI_PutStr(x,y,"123456789");
}

