/*--------------------------------------------------
关于printf:
参数输出的格式，定义格式为：%[flags][width][.perc][F|N|h|l]type
1.flags 规定输出格式，取值和含义如下： 
　　无 右对齐，左边填充0和空格 
　　- 左对齐，右边填充空格 
　　+ 在数字前增加符号 + 或 -
　　0 将输出的前面补上0，直到占满指定列宽为止（不可以搭配使用-）
　　空格 输出值为正时冠以空格，为负时冠以负号
2.width 用于控制显示数值的宽度，取值和含义如下
　　n(n=1,2,3...) 宽度至少为n位，不够以空格填充 
　　0n(n=1,2,3...) 宽度至少为n位，不够左边以0填充
　　* 格式列表中，下一个参数还是width
3.prec 用于控制小数点后面的位数，取值和含义如下： 
　　无 按缺省精度显示 
　　0 当type=d,i,o,u,x时，没有影响 
　　type=e,E,f时，不显示小数点 
　　n(n=1,2,3...) 当type=e,E,f时表示的最大小数位数 
　　type=其他，表示显示的最大宽度
　　.* 格式列表中，下一个参数还是width
4.F|N|h|l 表示指针是否是远指针或整数是否是长整数 
　　F 远指针 
　　n 近指针 
　　h 短整数或单精度浮点数 
　　l 长整数或双精度浮点数
5.type 含义如下： 
　　d 有符号10进制整数 
　　i 有符号10进制整数 
　　o 无符号8进制整数 
　　u 无符号10进制整数 
　　x 无符号的16进制数字，并以小写abcdef表示
　　X 无符号的16进制数字，并以大写ABCDEF表示
　　F/f 浮点数 
　　E/e 用科学表示格式的浮点数 
　　g 使用%f和%e表示中的总的位数表示最短的来表示浮点数 G 同g格式，但表示为指数 
　　c 单个字符 
　　s 字符串 
　　% 显示百分号本身 
　　p 显示一个指针，near指针表示为：XXXX far 指针表示为：XXXX：YYYY 
　　n 相连参量应是一个指针，其中存放已写字符的个数 

----------------------------------------------------*/
//--------------------------------------------------
//		头文件
//--------------------------------------------------
#include "includes.h"

//--------------------------------------------------
//		定义
//--------------------------------------------------
/*
typedef char * va_list; 
#define va_start(ap,v)	( ap=(va_list)&v+sizeof(v)) 
//va_arg()取得类型t的可变参数值(取当前值，并移向下一个)
#define va_arg(ap,t)	(*(t*)((ap+=sizeof(va_list))-sizeof(va_list))) 
#define va_end(ap)		( ap = (va_list)0 )*/

//--------------------------------------------------
//num2str中type 的标志位定义
//--------------------------------------------------
// pad with zero
#define ZEROPAD    1    
// unsigned/signed long
#define SIGN    2
// show plus 
#define PLUS    4        
//space if plus 
#define SPACE    8        
//left justified
#define LEFT    16        
//0x 
#define SPECIAL    32        
// use 'ABCDEF' instead of 'abcdef' 
#define LARGE    64        

//#define do_div(n,base) ({ \
//int __res; \
//__res = ((unsigned long) n) % (unsigned) base; \
//n = ((unsigned long) n) / (unsigned) base; \
//__res; })
//--------------------------------------------------
//		判断是否为数字的字符
//--------------------------------------------------
  //static inline int isdigit(int ch)
static int isdigit(int ch)
{
  return (ch >= '0') && (ch <= '9');
}
/*
//unsigned int skip_atou(const char **s);
//unsigned int atou(const char *s);
unsigned int skip_atou(const char **s)
{
	return **s-'0';
}
*/
//--------------------------------------------------
//		求字符串长度
//--------------------------------------------------
static int strnlen(const U8 *s, U32 maxlen)
{
  const U8 *es = s;
  while ( *es && maxlen ) {
    es++; maxlen--;
  }

  return (es-s);
}
//--------------------------------------------------
//str:存放结果 
//num:数据 
//base:进制 
//size:结果占用str的最尺寸,不足的,在左边填充空格或'0',右边填充空格,由标志位ZEROPAD\LEFT决定
//precision:最小精度（数据位数） ,不足的在左边补'0'
//type:标志位
//--------------------------------------------------
U8 * num2str(U8* str,S32 num,U8 base,S16 size,S16 precision,U16 type)
{
	S16 i;
	U8 c,sign,tmp[35];//ffff ffff=32个1
	const U8 *digits="0123456789abcdefghijklmnopqrstuvwxyz";//小写数字
	if (type & LARGE)digits = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";//大写数字
	if (type & LEFT)type &= ~ZEROPAD;//左对齐不需填充0		
	if (base < 2 || base > 36) return 0;//进制超出范围
	c = (type & ZEROPAD) ? '0' : ' '; //以'0'或' '填充
	sign = 0;
	if (type & SIGN)//有符号数size减符号位
	{				
		if (num < 0)
		{
			sign = '-';
			num = -num;
			size--;
		} 
		else if (type & PLUS)
		{
			sign = '+';
			size--;
		}
		else if (type & SPACE)
		{
			sign = ' ';
			size--;
		}
	}
	if (type & SPECIAL)//16进制或8进制,size 减去前缀
	{		
		if (base == 16)
		size -= 2;
		else if (base == 8)
		size--;
	}
	i = 0;//i用于记录数据位数
	if (num == 0)
	{
		tmp[i++]='0';
	}
	else 
	{
		while (num != 0) //以无符号数的方式取得数据的各个位，并转成字符存进 tmp[]
		{
		//tmp[i++] = do_div(num,base)];
			tmp[i++] =digits[((U32) num) % (unsigned) base];
			num = ((U32) num) / (unsigned) base; 
		}
	}
	
	if (precision < i)precision = i;	//precision不能小于数据位数? 
	size -= precision;				//减去了数据位数
	
	//---------------以下开始向str存入数据-----------------
	if (!(type&(ZEROPAD+LEFT)))//在右对齐方式下,位数不足precision的，左方以空格填充
	{
		while(size-->0) *str++ = ' ';
	}
	if (sign)//如果要显示符号
	{
		*str++ = sign;
	}
	if (type & SPECIAL)//8进制或16进制
	{		//8进制数据前加‘0’
		if (base==8)
		{
			*str++ = '0';
		}
		else if (base==16) 
		{	//16进制数据前加“0x” 或“0X”
			*str++ = '0';
			*str++ = digits[33];
		}
	}
	
	if (!(type & LEFT))//右对齐的在数据位左边要填充'0' 或 ' '
	{
		while (size-- > 0)*str++ = c;//c='0' 或 ' '
	}
	while (i < precision--)	*str++ = '0';//数据位不够精度的左边补0
	while (i-- > 0)			*str++ = tmp[i];//输出各数据位
	while (size-- > 0)		*str++ = ' ';//位数不足size的右边补空格?
	return str;
}

//Forward decl. needed for IP address printing stuff... 
//int sprintf(char * buf, const char *fmt, ...);
//--------------------------------------------------
//在内存中格式化字符串
//*buf: 格式化后的字符串存放位置
//*fmt: 输入将被格式化的字符串
//args: 第一个变参数的地址
//--------------------------------------------------
int GUI_VsPrintf(U8 *buf, const U8 *fmt, va_list args)
{
	U32 len;
	U32 num;
	U32 i, base;
	U8* str;
	const U8 *s;
	
	U16 flags;        //flags to num2str() 
	
	S16 field_width;    // width of output field 
	S16 precision;        // min. # of digits for integers; max
	           //num2str of chars for from string 
	S8 qualifier;        //'h', 'l', or 'L' for integer fields 
	
	for (str=buf ; *fmt ; ++fmt)
	{
		if (*fmt != '%')
		{
			*str++ = *fmt;
			continue;
		}
		//---------------获取flags-----------------    
		// process flags 
		flags = 0;
repeat:
		++fmt;        //this also skips first '%' 
		switch (*fmt)
		{
			case '-': flags |= LEFT; goto repeat;
			case '+': flags |= PLUS; goto repeat;
			case ' ': flags |= SPACE; goto repeat;
			case '#': flags |= SPECIAL; goto repeat;
			case '0': flags |= ZEROPAD; goto repeat;
		}
		//---------------获取宽度width(size)----------------- 
		// get field width 
		field_width = -1;
		if (isdigit(*fmt))
		{
			field_width = (*fmt)-'0';//skip_atou(&fmt);
			++fmt;//lcwadd			
		}
		else if (*fmt == '*')
		{
			++fmt;
			// it's the next argument 
			field_width = va_arg(args, int);
			if (field_width < 0) 
			{
				field_width = -field_width;
				flags |= LEFT;
			}
		}
		//---------------获取精度precision----------------- 
		//get the precision
		precision = -1;
		if (*fmt == '.')
		{
			++fmt;    
			if (isdigit(*fmt))
			{
				precision = (*fmt)-'0';//skip_atou(&fmt);
				++fmt;//lcwadd	
			}
			else if (*fmt == '*') 
			{
				++fmt;
				//it's the next argument
				precision = va_arg(args, int);
			}
			if (precision < 0)
			precision = 0;
		}
		//---------------获取数据的长短类型----------------- 
		//get the conversion qualifier
		qualifier = -1;
		if (*fmt == 'h' || *fmt == 'l' || *fmt == 'L')
		{
			qualifier = *fmt;
			++fmt;
		}		
		//default base
		base = 10;		
		switch (*fmt)
		{
			case 'c':	//输出字符
				if (!(flags & LEFT))//右对齐,左边填充空格
				{
					while (--field_width > 0)  *str++ = ' ';
				}
				*str++ = (unsigned char) va_arg(args, int);
				while (--field_width > 0)    *str++ = ' ';//左对齐在右边填充空格
				continue;      
			case 's':	//输出字符串
				s = va_arg(args, U8 *);
				len = strnlen(s, precision);		
				if (!(flags & LEFT))//右对齐,左边填充空格
				while (len < field_width--)	*str++ = ' ';
				for (i = 0; i < len; ++i)	*str++ = *s++;
				while (len < field_width--)	*str++ = ' ';//左对齐在右边填充空格
				continue;      
			case 'p'://指针按16进制显示
				if (field_width == -1)
				{
					field_width = 2*sizeof(void *);
					flags |= ZEROPAD;
				}
				str = num2str(str,(unsigned long) va_arg(args, void *),
				  16,field_width, precision, flags);
				continue;      
			case 'n'://?相连参量应是一个指针
				if (qualifier == 'l')
				{
					long * ip = va_arg(args, long *);
					*ip = (str - buf);
				} 
				else
				{
					int * ip = va_arg(args, int *);
					*ip = (str - buf);
				}
				continue;			
			case '%'://显示%号
				*str++ = '%';
				continue;			
			//integer num2str formats - set up the flags and "break"
			case 'o'://显示无符号8进制数
				base = 8;
				break;			
			case 'X'://显示大写16进制数
				flags |= LARGE;
			case 'x'://显示小写16进制数
				base = 16;
				break;      
			case 'd'://有符号10进制整数 
			case 'i'://有符号10进制整数 
				flags |= SIGN;
			case 'u'://无符号10进制整数 
				break;      
			default:
				*str++ = '%';
				if (*fmt)
					*str++ = *fmt;
				else
					--fmt;
				continue;
		}
		if (qualifier == 'l')
		{
			num = va_arg(args, unsigned long);
		}
		else if (qualifier == 'h')
		{
			num = (unsigned short) va_arg(args, int);
			if (flags & SIGN)    num = (short) num;//这句貌似没用
		}
		else if (flags & SIGN)//貌似下面两种是一样的
		{
			num = va_arg(args, int);
		}
		else
		{
			num = va_arg(args, unsigned int);
		}
		str = num2str(str, num, base, field_width, precision, flags);//最后，str指向数据之后
	}
	*str = '\0';
	return str-buf;//返回转换后的字符串长度
}
/*
int GUI_VsPrintf(char * buf, const char *fmt, ...)
{
  va_list args;
  int i;
  
  va_start(args, fmt);
  i=vsprintf(buf,fmt,args);
  va_end(args);
  return i;
}*/
/*
//--------------------------------------------------
//格式化字符串输出
//x,y为座标
// *fmt:为将被格式化的字符串
//...:可变参数
//--------------------------------------------------
int GUI_Printf(U32 x,U32 y,const U8 *fmt, ...)
{
	U8 printf_buf[1024];	
	va_list args;
	int printed;	
	va_start(args, fmt);//args 指向 fmt 后面
	printed = GUI_VsPrintf(printf_buf, fmt, args);
	va_end(args);	
	//puts(printf_buf);
	GUI_PutStr(x,y,printf_buf);
	return printed;
}*/

//#undef va_start
//#undef va_arg
//#undef va_end
