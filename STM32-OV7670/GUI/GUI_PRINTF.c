/*--------------------------------------------------
����printf:
��������ĸ�ʽ�������ʽΪ��%[flags][width][.perc][F|N|h|l]type
1.flags �涨�����ʽ��ȡֵ�ͺ������£� 
������ �Ҷ��룬������0�Ϳո� 
����- ����룬�ұ����ո� 
����+ ������ǰ���ӷ��� + �� -
����0 �������ǰ�油��0��ֱ��ռ��ָ���п�Ϊֹ�������Դ���ʹ��-��
�����ո� ���ֵΪ��ʱ���Կո�Ϊ��ʱ���Ը���
2.width ���ڿ�����ʾ��ֵ�Ŀ�ȣ�ȡֵ�ͺ�������
����n(n=1,2,3...) �������Ϊnλ�������Կո���� 
����0n(n=1,2,3...) �������Ϊnλ�����������0���
����* ��ʽ�б��У���һ����������width
3.prec ���ڿ���С��������λ����ȡֵ�ͺ������£� 
������ ��ȱʡ������ʾ 
����0 ��type=d,i,o,u,xʱ��û��Ӱ�� 
����type=e,E,fʱ������ʾС���� 
����n(n=1,2,3...) ��type=e,E,fʱ��ʾ�����С��λ�� 
����type=��������ʾ��ʾ�������
����.* ��ʽ�б��У���һ����������width
4.F|N|h|l ��ʾָ���Ƿ���Զָ��������Ƿ��ǳ����� 
����F Զָ�� 
����n ��ָ�� 
����h �������򵥾��ȸ����� 
����l ��������˫���ȸ�����
5.type �������£� 
����d �з���10�������� 
����i �з���10�������� 
����o �޷���8�������� 
����u �޷���10�������� 
����x �޷��ŵ�16�������֣�����Сдabcdef��ʾ
����X �޷��ŵ�16�������֣����Դ�дABCDEF��ʾ
����F/f ������ 
����E/e �ÿ�ѧ��ʾ��ʽ�ĸ����� 
����g ʹ��%f��%e��ʾ�е��ܵ�λ����ʾ��̵�����ʾ������ G ͬg��ʽ������ʾΪָ�� 
����c �����ַ� 
����s �ַ��� 
����% ��ʾ�ٷֺű��� 
����p ��ʾһ��ָ�룬nearָ���ʾΪ��XXXX far ָ���ʾΪ��XXXX��YYYY 
����n ��������Ӧ��һ��ָ�룬���д����д�ַ��ĸ��� 

----------------------------------------------------*/
//--------------------------------------------------
//		ͷ�ļ�
//--------------------------------------------------
#include "includes.h"

//--------------------------------------------------
//		����
//--------------------------------------------------
/*
typedef char * va_list; 
#define va_start(ap,v)	( ap=(va_list)&v+sizeof(v)) 
//va_arg()ȡ������t�Ŀɱ����ֵ(ȡ��ǰֵ����������һ��)
#define va_arg(ap,t)	(*(t*)((ap+=sizeof(va_list))-sizeof(va_list))) 
#define va_end(ap)		( ap = (va_list)0 )*/

//--------------------------------------------------
//num2str��type �ı�־λ����
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
//		�ж��Ƿ�Ϊ���ֵ��ַ�
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
//		���ַ�������
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
//str:��Ž�� 
//num:���� 
//base:���� 
//size:���ռ��str����ߴ�,�����,��������ո��'0',�ұ����ո�,�ɱ�־λZEROPAD\LEFT����
//precision:��С���ȣ�����λ���� ,���������߲�'0'
//type:��־λ
//--------------------------------------------------
U8 * num2str(U8* str,S32 num,U8 base,S16 size,S16 precision,U16 type)
{
	S16 i;
	U8 c,sign,tmp[35];//ffff ffff=32��1
	const U8 *digits="0123456789abcdefghijklmnopqrstuvwxyz";//Сд����
	if (type & LARGE)digits = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";//��д����
	if (type & LEFT)type &= ~ZEROPAD;//����벻�����0		
	if (base < 2 || base > 36) return 0;//���Ƴ�����Χ
	c = (type & ZEROPAD) ? '0' : ' '; //��'0'��' '���
	sign = 0;
	if (type & SIGN)//�з�����size������λ
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
	if (type & SPECIAL)//16���ƻ�8����,size ��ȥǰ׺
	{		
		if (base == 16)
		size -= 2;
		else if (base == 8)
		size--;
	}
	i = 0;//i���ڼ�¼����λ��
	if (num == 0)
	{
		tmp[i++]='0';
	}
	else 
	{
		while (num != 0) //���޷������ķ�ʽȡ�����ݵĸ���λ����ת���ַ���� tmp[]
		{
		//tmp[i++] = do_div(num,base)];
			tmp[i++] =digits[((U32) num) % (unsigned) base];
			num = ((U32) num) / (unsigned) base; 
		}
	}
	
	if (precision < i)precision = i;	//precision����С������λ��? 
	size -= precision;				//��ȥ������λ��
	
	//---------------���¿�ʼ��str��������-----------------
	if (!(type&(ZEROPAD+LEFT)))//���Ҷ��뷽ʽ��,λ������precision�ģ����Կո����
	{
		while(size-->0) *str++ = ' ';
	}
	if (sign)//���Ҫ��ʾ����
	{
		*str++ = sign;
	}
	if (type & SPECIAL)//8���ƻ�16����
	{		//8��������ǰ�ӡ�0��
		if (base==8)
		{
			*str++ = '0';
		}
		else if (base==16) 
		{	//16��������ǰ�ӡ�0x�� ��0X��
			*str++ = '0';
			*str++ = digits[33];
		}
	}
	
	if (!(type & LEFT))//�Ҷ����������λ���Ҫ���'0' �� ' '
	{
		while (size-- > 0)*str++ = c;//c='0' �� ' '
	}
	while (i < precision--)	*str++ = '0';//����λ�������ȵ���߲�0
	while (i-- > 0)			*str++ = tmp[i];//���������λ
	while (size-- > 0)		*str++ = ' ';//λ������size���ұ߲��ո�?
	return str;
}

//Forward decl. needed for IP address printing stuff... 
//int sprintf(char * buf, const char *fmt, ...);
//--------------------------------------------------
//���ڴ��и�ʽ���ַ���
//*buf: ��ʽ������ַ������λ��
//*fmt: ���뽫����ʽ�����ַ���
//args: ��һ��������ĵ�ַ
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
		//---------------��ȡflags-----------------    
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
		//---------------��ȡ���width(size)----------------- 
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
		//---------------��ȡ����precision----------------- 
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
		//---------------��ȡ���ݵĳ�������----------------- 
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
			case 'c':	//����ַ�
				if (!(flags & LEFT))//�Ҷ���,������ո�
				{
					while (--field_width > 0)  *str++ = ' ';
				}
				*str++ = (unsigned char) va_arg(args, int);
				while (--field_width > 0)    *str++ = ' ';//��������ұ����ո�
				continue;      
			case 's':	//����ַ���
				s = va_arg(args, U8 *);
				len = strnlen(s, precision);		
				if (!(flags & LEFT))//�Ҷ���,������ո�
				while (len < field_width--)	*str++ = ' ';
				for (i = 0; i < len; ++i)	*str++ = *s++;
				while (len < field_width--)	*str++ = ' ';//��������ұ����ո�
				continue;      
			case 'p'://ָ�밴16������ʾ
				if (field_width == -1)
				{
					field_width = 2*sizeof(void *);
					flags |= ZEROPAD;
				}
				str = num2str(str,(unsigned long) va_arg(args, void *),
				  16,field_width, precision, flags);
				continue;      
			case 'n'://?��������Ӧ��һ��ָ��
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
			case '%'://��ʾ%��
				*str++ = '%';
				continue;			
			//integer num2str formats - set up the flags and "break"
			case 'o'://��ʾ�޷���8������
				base = 8;
				break;			
			case 'X'://��ʾ��д16������
				flags |= LARGE;
			case 'x'://��ʾСд16������
				base = 16;
				break;      
			case 'd'://�з���10�������� 
			case 'i'://�з���10�������� 
				flags |= SIGN;
			case 'u'://�޷���10�������� 
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
			if (flags & SIGN)    num = (short) num;//���ò��û��
		}
		else if (flags & SIGN)//ò������������һ����
		{
			num = va_arg(args, int);
		}
		else
		{
			num = va_arg(args, unsigned int);
		}
		str = num2str(str, num, base, field_width, precision, flags);//���strָ������֮��
	}
	*str = '\0';
	return str-buf;//����ת������ַ�������
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
//��ʽ���ַ������
//x,yΪ����
// *fmt:Ϊ������ʽ�����ַ���
//...:�ɱ����
//--------------------------------------------------
int GUI_Printf(U32 x,U32 y,const U8 *fmt, ...)
{
	U8 printf_buf[1024];	
	va_list args;
	int printed;	
	va_start(args, fmt);//args ָ�� fmt ����
	printed = GUI_VsPrintf(printf_buf, fmt, args);
	va_end(args);	
	//puts(printf_buf);
	GUI_PutStr(x,y,printf_buf);
	return printed;
}*/

//#undef va_start
//#undef va_arg
//#undef va_end
