#ifndef _GUI_BASIC_H_
#define _GUI_BASIC_H_

#define GUI_COLOR U8

#define RGB888_565(r,g,b)	( ((r&0xf8)<<8) | ((g&0xfc)<<3) | ((b&0xf8)>>3) )
//cyan ÇàÉ« darkmagenta=purple×ÏÉ«
#define COLOR_darkred       RGB888_565(0x80, 0x00, 0x00)
#define COLOR_darkgreen     RGB888_565(0x00, 0x80, 0x00)
#define COLOR_darkyellow    RGB888_565(0x80, 0x80, 0x00)
#define COLOR_darkblue      RGB888_565(0x00, 0x00, 0x80)
#define COLOR_darkmagenta   RGB888_565(0x80, 0x00, 0x80)
#define COLOR_darkcyan      RGB888_565(0x00, 0x80, 0x80)
#define COLOR_lightgray     RGB888_565(0xC0, 0xC0, 0xC0)
#define COLOR_darkgray      RGB888_565(0x80, 0x80, 0x80)
#define COLOR_red           RGB888_565(0xFF, 0x00, 0x00)
#define COLOR_green         RGB888_565(0x00, 0xFF, 0x00)
#define COLOR_orange		RGB888_565(0xFF, 0xA5, 0x00)
#define COLOR_yellow        RGB888_565(0xFF, 0xFF, 0x00)
#define COLOR_blue          RGB888_565(0x00, 0x00, 0xFF)
#define COLOR_magenta       RGB888_565(0xFF, 0x00, 0xFF)
#define COLOR_cyan          RGB888_565(0x00, 0xFF, 0xFF)
#define COLOR_white			RGB888_565(0xFF, 0xFF, 0xFF)
#define COLOR_black         RGB888_565(0x00, 0x00, 0x00)
#define COLOR_brown         RGB888_565(0xA5, 0x2A, 0x2A)
#define COLOR_purple		RGB888_565(0x80, 0x00, 0x80)

enum GUICOLOR{
GUI_white,GUI_black,GUI_brown,GUI_red,
GUI_orange,GUI_yellow,GUI_green,GUI_blue,
GUI_purple,GUI_darkgray,GUI_darkred,GUI_darkgreen,
GUI_darkyellow,GUI_darkblue,GUI_darkcyan,GUI_lightgray
};

void GUI_DrawPixel(U16 x,U16 y,GUI_COLOR c);
U8 GUI_ReadPixel(U16 x,U16 y,GUI_COLOR *c);
void GUI_Update(void);
void GUI_UpdateVline(U16 x0, U16 y0, U16 y1);
void GUI_UpdateArea(U16 x0, U16 y0, U16 x1, U16 y1);
void GUI_FillScr(GUI_COLOR c);
void GUI_Rect(U16 x0, U16 y0, U16 x1, U16 y1, GUI_COLOR c);
void GUI_FillRect(U16 x0, U16 y0, U16 x1, U16 y1, GUI_COLOR c);
void GUI_HLine(U16 x0, U16 y0, U16 x1,GUI_COLOR c);
void GUI_VLine(U16 x0, U16 y0, U16 y1,GUI_COLOR c);
void GUI_Line(S16 x0, S16 y0, S16 x1, S16 y1, GUI_COLOR c);
void GUI_SetColor(GUI_COLOR forecolor,GUI_COLOR backcolor);
void GUI_SetFont8x5(void);
void GUI_SetFont16x8(void);
void GUI_PutChar(U16 x,U16 y, U16 asc);
void GUI_PutStr(U16 x,U16 y, U8* s);
void GUI_dispnum(S32 num,U8 digit,U8 dot,U16 x,U16 y);
#endif
