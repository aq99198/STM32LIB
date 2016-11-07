//--------------------------------------------------
//		ͷ�ļ�
//--------------------------------------------------
#include "includes.h"
//--------------------------------------------------
//		����
//--------------------------------------------------
//��������
typedef struct _FONT
{
    U16 Width;		//���ֿ��
	U16 Height;		//���ָ߶�
	U16 BPL;		//����һ����ռ�ֽ���
	U16 AscFirst;	//��һ���ֵ� ascii ��
	U16 AscLast;	//���һ���ֵ� ascii ��
	const U8* lib;		//�ֿ�
} FONT;
typedef FONT* PFONT;
typedef const FONT* PCFONT;
//����Ĭ�ϵ�����
const FONT Font8x5={6,8,1,32,127,Asc8x5_tab};
const FONT Font16x8={8,16,1,32,127,Asc16x8_tab};
const FONT* pGUIFont = &Font16x8;
GUI_COLOR	GUI_ForeColor=GUI_black;//ǰ��ɫ
GUI_COLOR	GUI_BackColor=GUI_white;//����ɫ
//#define GUI_COLOR U8
//320*240=76800
//*1/2=38400
//��ÿ��ռ��λ��
GUI_COLOR GUI_Buffer[LCD_HEIGHT][LCD_WIDTH/2];//��ʾ�������ݵĻ���
//16����ɫ�ı�
LCD_COLOR GUI_ColorTab[16]={
COLOR_white,COLOR_black,COLOR_brown,COLOR_red,
COLOR_orange,COLOR_yellow,COLOR_green,COLOR_blue,
COLOR_purple,COLOR_darkgray,COLOR_darkred,COLOR_darkgreen,
COLOR_darkyellow,COLOR_darkblue,COLOR_darkcyan,COLOR_lightgray
};

//--------------------------------------------------
//		ˢ��һ����ʾ,GUI_Buffer�����������LCD��
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
			c=GUI_ColorTab[GUI_Buffer[y][x]&0x0f];//ȡ����λ
			LCD_WR_Data(c);
		}
		for(y=0;y<LCD_HEIGHT;y++)
		{
			c=GUI_ColorTab[GUI_Buffer[y][x]>>4];//ȡ����λ
			LCD_WR_Data(c);
		}			
	}
}
//--------------------------------------------------
//		ˢ��һ������,GUI_Buffer�����������LCD��
//--------------------------------------------------
void GUI_UpdateVline(U16 x0, U16 y0, U16 y1)
{
	U16 y;
	LCD_COLOR c;	
	LCD_Setxy(x0,y0);
	if((x0&0x01)==0)//ż��
	{
		for(y=y0;y<=y1;y++)
		{
			c=GUI_ColorTab[GUI_Buffer[y][x0>>1]&0x0f];//ȡ����λ
			LCD_WR_Data(c);
		}
	}
	else
	{
		for(y=y0;y<=y1;y++)
		{
			c=GUI_ColorTab[GUI_Buffer[y][x0>>1]>>4];//ȡ����λ
			LCD_WR_Data(c);
		}
	}			
}
//--------------------------------------------------
//		//ˢ��һ������,GUI_Buffer�����������LCD��
//--------------------------------------------------
void GUI_UpdateArea(U16 x0, U16 y0, U16 x1, U16 y1)
{
	U16 x,y;
	LCD_COLOR c;	
	//LCD_Setxy(0,0);
	LCD_SetArea(x0,y0,x1,y1);	
	for(x=x0;x<=x1;x++)
	{
		if((x&0x01)==0)//ż��
		{
			for(y=y0;y<=y1;y++)
			{
				c=GUI_ColorTab[GUI_Buffer[y][x>>1]&0x0f];//ȡ����λ
				LCD_WR_Data(c);
			}
		}
		else
		{
			for(y=y0;y<=y1;y++)
			{
				c=GUI_ColorTab[GUI_Buffer[y][x>>1]>>4];//ȡ����λ
				LCD_WR_Data(c);
			}
		}			
	}
	LCD_SetArea(0,0,LCD_WIDTH-1,LCD_HEIGHT-1);
}
//--------------------------------------------------
//		GUI_Buffer�л���
//--------------------------------------------------
void GUI_DrawPixel(U16 x,U16 y,GUI_COLOR c)
{
	U16 xt;
	if((x<LCD_WIDTH)&&(y<LCD_HEIGHT))
	{
		xt=x>>1;
		if((x&BIT(0))==0)//ż��,����ڵ�λ
		{
			GUI_Buffer[y][xt]&=0xf0;//�������λ
			GUI_Buffer[y][xt]|=(c&0x0f);//���õ���λ		
		}
		else
		{
			GUI_Buffer[y][xt]&=0x0f;//�������λ
			GUI_Buffer[y][xt]|=(c&0x0f)<<4;//���ø���λ			
		}
	}
}
//--------------------------------------------------
//		GUI_Buffer�ж���
//--------------------------------------------------
U8 GUI_ReadPixel(U16 x,U16 y,GUI_COLOR *c)
{
	U16 xt;
	if((x<LCD_WIDTH)&&(y<LCD_HEIGHT))
	{
		xt=x>>1;
		if(x&BIT(0))//����,����ڸ�λ
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
//		GUI_Buffer�����
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
//		GUI_Buffer�л�����
//--------------------------------------------------
void GUI_HLine(U16 x0, U16 y0, U16 x1,GUI_COLOR c)
{
	U16 i;
	if(x0>x1){i=x0;x0=x1;x1=i;}//��֤x0<x1
	for(i=x0;i<=x1;i++)
	{
		GUI_DrawPixel(i,y0,c);
	}
}
//--------------------------------------------------
//		GUI_Buffer������
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
//		GUI_Buffer�л�����
//--------------------------------------------------
void  GUI_Rect(U16 x0, U16 y0, U16 x1, U16 y1, GUI_COLOR c)
{  
	GUI_HLine(x0, y0, x1, c);
	GUI_HLine(x0, y1, x1, c);
	GUI_VLine(x0, y0, y1, c);
	GUI_VLine(x1, y0, y1, c);
}
//--------------------------------------------------
//		GUI_Buffer��������
//--------------------------------------------------
void  GUI_FillRect(U16 x0, U16 y0, U16 x1, U16 y1, GUI_COLOR c)
{
	U16  i;
	/* ���ҳ��������Ͻ������½ǵ������㣬������(x0,y0)��(x1,y1) */
	if(x0>x1) 						// ��x0>x1����x0��x1����
	{
		i = x0;
		x0 = x1;
		x1 = i;
	}
	if(y0>y1)						// ��y0>y1����y0��y1����
	{  
		i = y0;
		y0 = y1;
		y1 = i;
	}	
	/* �ж��Ƿ�ֻ��ֱ�� */
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
		GUI_HLine(x0, y0, x1, c);	// ��ǰ��ˮƽ��
		y0++;							// ��һ��
	}
}
//--------------------------------------------------
//		GUI_Buffer�л���
//--------------------------------------------------
void  GUI_Line(S16 x0, S16 y0, S16 x1, S16 y1, GUI_COLOR c)
{
	S16   dx;						// ֱ��x���ֵ����
	S16   dy;          			// ֱ��y���ֵ����
	S8    dx_sym;					// x����������Ϊ-1ʱ��ֵ����Ϊ1ʱ��ֵ����
	S8    dy_sym;					// y����������Ϊ-1ʱ��ֵ����Ϊ1ʱ��ֵ����
	S16   dx_x2;					// dx*2ֵ���������ڼӿ������ٶ�
	S16   dy_x2;					// dy*2ֵ���������ڼӿ������ٶ�
	S16   di;						// ���߱���
    
	dx = x1-x0;						// ��ȡ����֮��Ĳ�ֵ
	dy = y1-y0;

   /* �ж��������򣬻��Ƿ�Ϊˮƽ�ߡ���ֱ�ߡ��� */
   if(dx>0)							// �ж�x�᷽��
   {  dx_sym = 1;					// dx>0������dx_sym=1
   }
   else
   {  if(dx<0)
      {  dx_sym = -1;				// dx<0������dx_sym=-1
      }
      else
      {  // dx==0������ֱ�ߣ���һ��
         GUI_VLine(x0, y0, y1,c);
      	 return;
      }
   }
   
   if(dy>0)							// �ж�y�᷽��
   {  dy_sym = 1;					// dy>0������dy_sym=1
   }
   else
   {  if(dy<0)
      {  dy_sym = -1;				// dy<0������dy_sym=-1
      }
      else
      {  // dy==0����ˮƽ�ߣ���һ��
         GUI_HLine(x0, y0, x1, c);
      	 return;
      }
   }
    
   /* ��dx��dyȡ����ֵ */
   dx = dx_sym * dx;
   dy = dy_sym * dy;
 
   /* ����2����dx��dyֵ */
   dx_x2 = dx*2;
   dy_x2 = dy*2;
   
   /* ʹ��Bresenham�����л�ֱ�� */
   if(dx>=dy)						// ����dx>=dy����ʹ��x��Ϊ��׼
   {  di = dy_x2 - dx;
      while(x0!=x1)
      {  GUI_DrawPixel(x0, y0, c);
         x0 += dx_sym;
         if(di<0)
         {  di += dy_x2;			// �������һ���ľ���ֵ
         }
         else
         {  di += dy_x2 - dx_x2;
            y0 += dy_sym;
         }
      }
      GUI_DrawPixel(x0, y0, c);		// ��ʾ���һ��
   }
   else								// ����dx<dy����ʹ��y��Ϊ��׼
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
      GUI_DrawPixel(x0, y0, c);		// ��ʾ���һ��
   }   
}
//--------------------------------------------------
//		�����������ʱ��ǰ��ɫ�뱳��ɫ
//--------------------------------------------------
void GUI_SetColor(GUI_COLOR forecolor,GUI_COLOR backcolor)
{
	GUI_ForeColor=forecolor;
	GUI_BackColor=backcolor;
}
//--------------------------------------------------
//		����8*5����
//--------------------------------------------------
void GUI_SetFont8x5(void)
{
	pGUIFont = &Font8x5;
}
//--------------------------------------------------
//		����16*8����
//--------------------------------------------------
void GUI_SetFont16x8(void)
{
	pGUIFont = &Font16x8;
}
//--------------------------------------------------
//		GUI_Buffer���������
//--------------------------------------------------
void GUI_PutChar(U16 x,U16 y, U16 asc)
{
	U8 i,j,w,h,c;	
	const U8* p;
	const U8 btmap[]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};//λ��Ӹߵ���
	if(asc < pGUIFont->AscFirst)return;
	if(asc > pGUIFont->AscLast)return;
	w=pGUIFont->Width;
	h=pGUIFont->Height;	
	p=pGUIFont->lib;
	p+=(asc-pGUIFont->AscFirst)* pGUIFont->BPL * pGUIFont->Height;//�ҵ���ĸ���ڵĵ�ַ

	for(i=0;i<h;i++)//h
	{
		for(j=0;j<w;j++)//w
		{
			c=*(p + i* pGUIFont->BPL + j/8);//��i,j�ҵ����ص������ֽ�
			//c=0xff;
			if(c & btmap[j %8])//��������λ
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
//		GUI_Buffer������ַ���
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
//��ʾ�з�����,digitΪ���ݵ�λ��,dotΪС��λ��
//--------------------------------------------------
void GUI_dispnum(S32 num,U8 digit,U8 dot,U16 x,U16 y)
{
	U8 bt,i,w,n[]={13},buf[15];//λ��,Ҫ��ʾ������ 
	U32 num1;
	//digit=digit%13;//���Ϊ12
	if(digit>12)digit=12;
	w=0;
	if(num<0)
	{
		num1=~num+1;//ԭ��Ϊ�����1
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
		if(n[i]!='0')bt=i+1;//������λ��
	}

	if(bt>digit)
	{
		dot=dot-(bt-digit);//С��λ����
	}
	else
	{
		bt=digit;
	}
	if(dot<1)//С������ʾ������
	{
		digit=digit+1;//λ����1ʹ��������С�����ڵ�ʱ��ռ����ͬ��λ��
		if(digit>6)digit=6;
		bt=bt+dot+1;//������ʼλ
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

