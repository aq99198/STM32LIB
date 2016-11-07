/*------------------------------------------------
SDA-P1.3
PCL-P1.4
WP -P1.5
WP为高电平时使能写保护
--------------------------------------------------*/

#define PSCL_H 	 	IOSET(GPIOB->ODR,BIT(10))
#define PSCL_L 	 	IOCLR(GPIOB->ODR,BIT(10))
#define PSDA_H	 	IOSET(GPIOB->ODR,BIT(11))
#define PSDA_L 	 	IOCLR(GPIOB->ODR,BIT(11))
#define PINSDA 	 	((GPIOB->IDR)&BIT(11))


//IIC时钟数
#define IIC_CT		20

U8 IICDadd=0x42;//IIC设备地址<<1
//------------------------------------------------
//IIC启始信号
//------------------------------------------------
void IICStart(void)
{
	SDA_MO();
	PSCL_H;
	PSDA_H;
	delayus(IIC_CT);
	PSDA_L;
	delayus(IIC_CT);
	PSCL_L;
	delayus(IIC_CT);
}
//------------------------------------------------
//IIC终止信号
//------------------------------------------------
void IICEnd(void)
{
	SDA_MO();
	PSCL_L;
	PSDA_L;
	delayus(IIC_CT);
	PSCL_H;
	delayus(IIC_CT);
	PSDA_H;
	delayus(IIC_CT);
}
//------------------------------------------------
//IIC回应
//------------------------------------------------
U8 IICACK(U8 ack)	 //reentrant
{ 
	U8 ret=0;
	delayus(IIC_CT);	
	SDA_MIN();	
	if(ack==0)
		PSDA_H;//放开数据线
	else
		PSDA_L;
	PSCL_H;
	delayus(IIC_CT);
	if(PINSDA)ret=1;//读数据线
	PSCL_L;
	delayus(IIC_CT);
	return ret;
}

//------------------------------------------------
//IIC发出一字节数据
//------------------------------------------------
void IICWByte(U8 sdata)  //reentrant//Thigh,Tlow>2US 就行
{
	U8 i;
	SDA_MO();
	for(i=0;i<8;i++)//之前PSCL=0;
	{
		if(sdata&0x80)
			PSDA_H;
		else
			PSDA_L;
		delayus(IIC_CT);//NOP();	//delayus()
		PSCL_H;
		delayus(IIC_CT);//NOP();	
		PSCL_L;
		sdata<<=1;//移位
	}
}
//------------------------------------------------
//IIC读取一字节数据
//------------------------------------------------
U8 IICRByte(void)  //reentrant
{
	U8 ret,i;
	PSDA_H;
	SDA_MIN();
	for(i=0;i<8;i++)//之前PSCL=0;
	{
		ret<<=1;
		delayus(IIC_CT);//NOP();
		PSCL_H;
		delayus(IIC_CT);//NOP();
		if(PINSDA)ret|=1;
		PSCL_L;
	}
	return ret;
}
//------------------------------------------------
//IIC设备写一字节数据
//------------------------------------------------
void IICDEV_WByte(U8 add,U8 d)
{
	IICStart();
	IICWByte(IICDadd);
	IICACK(0);
	IICWByte(add);
	IICACK(0);
	IICWByte(d);
	IICACK(0);
	IICEnd();
}

//------------------------------------------------
//IIC设备读一字节数据
//------------------------------------------------
U8 IICDEV_RByte(U8 add)
{
	U8 d;
	//假写操作
	IICStart();
	IICWByte(IICDadd);
	IICACK(0);
	IICWByte(add);
	IICACK(0);
	//重新启动
	IICEnd();//OV7670需要STOP!
	IICStart();
	IICWByte(IICDadd|1);//读
	IICACK(0);
	d=IICRByte();
	IICACK(0);
	IICEnd();
	return d;
}

//------------------------------------------------
//IIC设备读一串数据
//------------------------------------------------
U8 IICDEV_RStr(U8 add,U8 len,U8 *buf)
{
	U8 d;
	//假写操作
	IICStart();
	IICWByte(IICDadd);
	IICACK(0);
	IICWByte(add);
	IICACK(0);
	//重新启动
	IICStart();
	IICWByte(IICDadd|1);//读
	IICACK(0);
	while(len--)
	{
		*(buf++)=IICRByte();		
		IICACK(len>0);//最后一字节不发ACK
	}
	IICEnd();
	return d;
}


/*
//------------------------------------------------
//IIC存入一字节
//addr:地址
//wdata:数据
//返回:第一个ACK,为0是正常的
//------------------------------------------------
U8 IICEEPW(U16 addr,U8 wdata)//写入24C01
{
	U8 pg;//页地址
	U8 ack;
	pg=addr>>8;
	IICStart();
	IICSendByte(SLAW|(pg<<1));
	ack=IICACK(); 
	IICSendByte(addr);
	IICACK();
	IICSendByte(wdata);
	IICACK();
	IICEnd();
	delayms(6);
	return ack;
}
//------------------------------------------------
//IIC读取一字节
//addr:地址
//rdata:返回读到的数据
//返回:第一个ACK,为0是正常的
//------------------------------------------------
U8 IICEEPR(U16 addr,U8 * rdata)  //reentrant//读一字节
{
	U8 ack,d;
	U8 pg;//页地址
	pg=addr>>8;
	IICStart();
	IICSendByte(SLAW|(pg<<1));
	ack=IICACK();
	IICSendByte(addr);//伪写操作
	IICACK();
	
	IICStart();
	IICSendByte(SLAR|(pg<<1));
	IICACK();
	d=IICReadByte();	
	IICACK();
	IICEnd();

	*rdata=d;
	return ack;
}
//------------------------------------------------
//IIC读取字符串,size为长度限制
//------------------------------------------------
U8 IICReadStr(U16 addr,U16 size,U8* rstr)  //reentrant//读一字节
{
	U16 i;
	U8 rd;
	for(i=0;i<size;i++)
	{
		IICEEPR(addr+i,&rd);
		if(rd==0)break;
		*(rstr+i)=rd;
	}
	*(rstr+i)=0;
	return 1;//
#warning "此处可加入对24C01失效的处理"
}
//------------------------------------------------
//IIC写入字符串,size为长度限制
//------------------------------------------------
U8 IICWriteStr(U16 addr,U16 size,U8* wstr)
{
	U16 i;
	U8 wd;
	for(i=0;i<size;i++)
	{
		wd=*(wstr+i);
		if(wd==0)break;
		IICEEPW(addr+i,wd);		
	}
	if(i<size)//字符串最后补0
	{
		IICEEPW(addr+i,0);		
	}
	return 1;//
#warning "此处可加入对24C01失效的处理"
}*/
//------------------------------------------------
//IIC初始化
//将写保护置低
//------------------------------------------------
void IIC_init(void)
{
	IICEnd();
}
/*void main()
{
 char i,j,data[8];
 init_devices();
 for(i=0;i<8;i++)
 	EEPW(i,i+10);
 for(i=0;i<8;i++)
 	data[i]=EEPR(i);
 while(1)
 {
  for(i=0;i<8;i++)
  	{
	 for(j=0;j<10;j++)
		display(data[i]);
	 delayus(100);
	}
  delayus(1000);
 } 
}*/

