#ifndef _GUI_PRINTF_H_
#define _GUI_PRINTF_H_

typedef char * va_list; 
#define va_start(ap,v)	( ap=(va_list)&v+sizeof(v)) 
//va_arg()取得类型t的可变参数值(取当前值，并移向下一个)
#define va_arg(ap,t)	(*(t*)((ap+=sizeof(va_list))-sizeof(va_list))) 
#define va_end(ap)		( ap = (va_list)0 )


U8 * num2str(U8* str,S32 num,U8 base,S16 size,S16 precision,U16 type);
int GUI_VsPrintf(U8 *buf, const U8 *fmt, va_list args);
int GUI_Printf(U32 x,U32 y,const U8 *fmt, ...);
#endif
