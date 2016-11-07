#ifndef _MAIN_H_
#define _MAIN_H_
//--------------------------------------------------
//		头文件
//--------------------------------------------------
//#include <iom8v.h>
//#include <macros.h>
//--------------------------------------------------
//		类型声明
//--------------------------------------------------
typedef unsigned char uchar;
typedef signed char   schar;
typedef unsigned int  uint;
typedef signed int	  sint;
typedef unsigned long ulong;
typedef signed long	  slong;

typedef unsigned char  INT8U;  
typedef signed   char  INT8S; 
typedef unsigned int   INT16U;
typedef signed   int   INT16S;
typedef unsigned long  INT32U;
typedef signed   long  INT32S;

typedef unsigned char  U8;  
typedef signed   char  S8; 
typedef unsigned int   U16;
typedef signed   int   S16;
typedef unsigned long  U32;
typedef signed   long  S32;

//--------------------------------------------------
//		宏定义
//--------------------------------------------------
#undef  BIT 
#define BIT(x)    (1<<(x)) 
//*
#define IOSET(port,x)  (port)|=(x)
//IOSET(PORTB,BIT(0));//将端口在X为1的位置1
#define IOCLR(port,x)  (port)&=~(x)
//IOCLR(PORTB,BIT(0));//将端口在X为1的位清0
#define IOXOR(port,x)  (port)^=(x)
//IOCLR(PORTB,BIT(0));//将端口在X为1的位取反
#define GETBIT(port,x)  ((port&BIT(x))>>x)
//取位得1或0
//求数组元素个数
#define ARRLEN(arr) (sizeof(arr)/sizeof(arr[0]))
//--------------------------------------------------
//		全局变量
//--------------------------------------------------
//INT16U pro_tick;



//--------------------------------------------------
//		函数声明
//--------------------------------------------------
//void delay(int t);
void delayus(U32 t);
void delayms(U32 t);
void DMA_stop(void);
void DMA_start(void);
#endif
