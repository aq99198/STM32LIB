/*------------------------------------------------
SDA-P1.3
PCL-P1.4
WP -P1.5
WP为高电平时使能写保护
--------------------------------------------------*/

#define RST_H	IOSET(GPIOB->ODR,BIT(2))
#define RST_L	IOCLR(GPIOB->ODR,BIT(2))

#define PWDN_H	IOSET(GPIOB->ODR,BIT(12))
#define PWDN_L	IOCLR(GPIOB->ODR,BIT(12))
/*
Ov7670的寄存器中跟分辨率有关的寄存器主要是一下几个：
0x17 HSTART输出格式-行频开始高八位（低三位在HREF[2：0]）
0x18 HSTOP输出格式-行频结束高八位（低三位在HREF[5：3]）
0x19 VSTRT输出格式-场频开始高八位（低二位在VREF[1：0]）
0x1A VSTOP输出格式-场频结束高八位（低二位在VREF[3：2]）
0x03 VREF	位[3：2]VREF结束的低两位(高八位见VSTOP[7:0])
			位[1：0]VREF开始的低两位(高八位见VSTOP[7:0])
0x32 HREF读写HREF控制
		位[5：3]：HREF结束的低3位（高8位在HSTOP)
		位[2：0]：HREF结束的低3位（高8位在HSTOP)
网上给的一般的设置方式是这样的：QVGARGB565 320*240

*/
const U8 OV7670_REG[][2]={
{0x11, 0x40},//内部时钟不分频
{0x6b, 0x00},//关PLL,开LDO
//{0x2a, 0x00},//空相素
//{0x2b, 0x00},
//{0x92, 0x00},//空行
//{0x93, 0x00},
//{0x3b, 0x0a},
//Output format
//{0x12, 0x14},//QVGA(320*240)、RGB
{0x12, 0x08},//输出格式QCIF:176×144,
//{0x40, 0x10},//RGB565,effective only when RGB444[1] is low
//{0x8c, 0x00},//RGB444
//Special effects - 特效 
//normal
//{0x3a, 0x04},//YUYV,自动UV
{0x3a, 0x14},//YUYV,手动UV
{0x3d, 0x00},//YUYV
{0x67, 0x40},//手动U
{0x68, 0x60},//手动V
//Mirror/VFlip Enable - 水平镜像/竖直翻转使能
//{0x1e, 0x01},//修改配置值将产生图像显示上下或左右颠倒
//Banding Filter Setting for 24Mhz Input Clock - 条纹滤波器
//30fps for 60Hz light frequency
//{0x13, 0xe7},//banding filer enable
//{0x9d, 0x98},//50Hz banding filer
//{0x9e, 0x7f},//60Hz banding filer
//{0xa5, 0x02},//3 step for 50Hz
//{0xab, 0x03},//4 step for 60Hz
//{0x3b, 0x02},//select 60Hz banding filer
//Simple White Balance - 白平衡
//{0x13, 0xe7},//AWB、AGC、AGC Enable and ...
//{0x6f, 0x9f},//simple AWB
//AWBC - 自动白平衡控制(Automatic white balance control) 
//{0x43, 0x14},//用户手册里这些寄存器的值都是保留(Reserved),不用设置的呀？
//{0x44, 0xf0},
//{0x45, 0x34},        
//{0x46, 0x58},
//{0x47, 0x28},
//{0x48, 0x3a},

//AWB Control
//{0x59, 0x88},//用户手册连寄存器都是保留，初始值都没提供
//{0x5a, 0x88},        
//{0x5b, 0x44},
//{0x5c, 0x67},
//{0x5d, 0x49},
//{0x5e, 0x0e},

//AWB Control
//{0x6c, 0x0a},
//{0x6d, 0x55},       
//{0x6e, 0x11},
//{0x6f, 0x9f},

//AGC/AEC - Automatic Gain Control自动增益补偿/Automatic exposure Control自动曝光控制
//{0x00, 0x00},
//{0x14, 0x20},
//{0x24, 0x75},
//{0x25, 0x63},
//{0x26, 0xA5},

//AEC algorithm selection - AEC公式选择    
{0xaa, 0x94},//基于平均值的AEC算法Average-based AEC algorithm/基于直方图的AEC算法Histogram-based AEC algorithm  

//基于直方图的AGC/AEC的控制
//{0x9f, 0x78},
//{0xa0, 0x68},       
//{0xa6, 0xdf},
//{0xa7, 0xdf},
//{0xa8, 0xf0},
//{0xa9, 0x90},

//Fix Gain Control - 固定增益控制    
//{0x69, 0x5d},//Fix gain for Gr channel/for Gb channel/for R channel/for B channel

//Color saturation 颜色饱和度 + 0
//{0x4f, 0x80},
//{0x50, 0x80},
//{0x51, 0x00},
//{0x52, 0x22},
//{0x53, 0x5e},
//{0x54, 0x80},
//{0x58, 0x9e},

//Brightness - 亮度 + 0
//{0x55, 0x00},
//Contrast - 对比度 + 0
//{0x56, 0x50},

//Gamma Curve - 伽马曲线
//{0x7a, 0x20},
//{0x7b, 0x1c},
//{0x7c, 0x28},        
//{0x7d, 0x3c},
//{0x7e, 0x55},
//{0x7f, 0x68},
//{0x80, 0x76},
//{0x81, 0x80},        
//{0x82, 0x88},
//{0x83, 0x8f},
//{0x84, 0x96},
//{0x85, 0xa3},
//{0x86, 0xaf},        
//{0x87, 0xc4},
//{0x88, 0xd7},
//{0x89, 0xe8},

//Matrix Coefficient - 色彩矩阵系数            
//{0x4f, 0x80},
//{0x50, 0x80},
//{0x51, 0x00},
//{0x52, 0x22},
//{0x53, 0x5e},
//{0x54, 0x80},

//Lens Correction Option - 镜头补偿选项
//{0x62, 0x00},       
//{0x63, 0x00},
//{0x64, 0x04},
//{0x65, 0x20},        
//{0x66, 0x05},
//{0x94, 0x04},//effective only when LCC5[2] is high
//{0x95, 0x08},//effective only when LCC5[2] is high


//注释这些配置的话，就倾斜显示，并显示多块，这到底是控制什么的？跟时序图有关？
//{0x17, 0x16},//行频Horizontal Frame开始高八位(低三位在HREF[2：0])       
//{0x18, 0x04},//行频Horizontal Frame结束高八位(低三位在HREF[5：3])
//{0x19, 0x02},//场频Vertical Frame开始高八位(低二位在VREF[1：0])
//{0x1a, 0x7b},//场频Vertical Frame结束高八位(低二位在VREF[3：2])
//{0x32, 0x80},//HREF
//{0x03, 0x06},//VREF

//Automatic black Level Compensation - 自动黑电平校正
//{0xb0, 0x84},//调试时注释这项配置时，颜色显示不正常了，红色练绿色，绿色变红色，但用户手册对这寄存器是保留RSVD
//{0xb1, 0x0c},
//{0xb2, 0x0e},      
//{0xb3, 0x82},
//{0xb8, 0x0a},
//SCALING_xx寄存器
//{0x70, 0x00},
//{0x71, 0x00},
//{0x72, 0x11},
//{0x73, 0x08},
//{0x3e, 0x00},
//ADC
//{0x37, 0x1d},//ADC控制ADC Control
//{0x38, 0x71},//ADC和模拟共模控制ADC and Analog Common Mode Control
//{0x39, 0x2a},//ADC偏移控制ADC Offset Control

//零杂的寄存器
//{0x92, 0x00},//空行低八位Dummy Line low 8 bits
//{0xa2, 0x02},//像素时钟延时
//{0x0c, 0x0c},
//{0x10, 0x00},
//{0x0d, 0x01},
//{0x0f, 0x4b},
//{0x3c, 0x78},
//{0x74, 0x19},
{0x73, 0x70},//DSP缩放时钟分频控制
{0x13, 0x8F},//使能快速AGC/AEC算法
//{0x14, 0x1a},//最大AGC值

{0x15, 0x22}//PCLK在HSYNC期间不输出,VSYNC负有效
};
void OV7670_config_window(U16 startx,U16 starty,U16 width,U16 height)
{
	U16 endx,endy;
	U8 x_reg,y_reg,temp;
	endx=(startx+width*2)%784;
	endy=(starty+height*2);
	x_reg=IICDEV_RByte(0x32);//HREF
	x_reg&=0xC0;
	y_reg=IICDEV_RByte(0x03);//VREF
	y_reg&=0xF0;
	//设置HREF
	temp=x_reg|((endx&0x7)<<3)|(startx&0x7);
	IICDEV_WByte(0x32,temp);//HREF
	temp=(startx&0x7F8)>>3;
	IICDEV_WByte(0x17,temp);//HSTART
	temp=(endx&0x7F8)>>3;
	IICDEV_WByte(0x18,temp);//HSTOP
	//设置VREF
	temp=y_reg|((endy&0x3)<<2)|(starty&0x3);
	IICDEV_WByte(0x03,temp);//VREF
	temp=(starty&0x3FC)>>2;
	IICDEV_WByte(0x19,temp);//VSTART
	temp=(endy&0x3FC)>>2;
	IICDEV_WByte(0x1A,temp);//VSTOP
}

/*
void OV7670_init(void)
{
	PWDN_H;
	delayms(1);
	PWDN_L;
	delayms(1);
	RST_L;
	delayms(1);
	RST_H;
	//yuv_12fps
	IICDEV_WByte(0x12, 0x80);//SCCB复位
	delayms(1);
	//IICDEV_WByte(0x12, 0x00);//输出格式YUV
	IICDEV_WByte(0x12, 0x08);//输出格式QCIF:176×144,	
	IICDEV_WByte(0x11, 0x40);//内部时钟不分频
	IICDEV_WByte(0x92, 0x66);//空行低8位
	IICDEV_WByte(0x93, 0x00);//空行高8位
	IICDEV_WByte(0x9d, 0x4c);//50HZ条纹滤波器值
	IICDEV_WByte(0x9e, 0x3f);//60HZ条纹波波器值
	IICDEV_WByte(0x3b, 0x0a);//关闭夜晚模式
	IICDEV_WByte(0x13, 0xf0);//快速AGC\AEC
	IICDEV_WByte(0x10, 0x00);//曝光值
	IICDEV_WByte(0x00, 0x00);//AGC
//	IICDEV_WByte(0x00, 0xff); // modified by me
//	IICDEV_WByte(0x07, 0x3f); // added by me
	IICDEV_WByte(0x01, 0x80);//蓝色通道增益
	IICDEV_WByte(0x02, 0x80);//红色通道增益
	IICDEV_WByte(0x13, 0xf5);//关闭AGC,AWB,AEC
	//

	IICDEV_WByte(0x04, 0x00);//关CCIR
	//窗口控制
	IICDEV_WByte(0x18, 0x01);//行频结束高八位
	IICDEV_WByte(0x17, 0x13);//行频开始高八位
	IICDEV_WByte(0x32, 0x92);//HREF控制
	IICDEV_WByte(0x19, 0x02);//场频开始高八位
	IICDEV_WByte(0x1a, 0x7a);//场频结束高八位
	IICDEV_WByte(0x03, 0x00);//帧竖直方向控制
	//
	IICDEV_WByte(0x0e, 0x84);//通用控制5,保留
	IICDEV_WByte(0x0f, 0x62);//当格式变化时复位所有时序
	//IICDEV_WByte(0x15, 0x02);//VSYNC负有效
	IICDEV_WByte(0x15, 0x22);//PCLK在HSYNC期间不输出,VSYNC负有效
	IICDEV_WByte(0x16, 0x02);//保留
	IICDEV_WByte(0x1b, 0x01);//像素延迟选择
	IICDEV_WByte(0x1e, 0x01);//水平镜像/竖直翻转使能
	IICDEV_WByte(0x29, 0x3c);//保留
	IICDEV_WByte(0x33, 0x00);//感光阵列电流控制,保留
	IICDEV_WByte(0x34, 0x07);//感光阵列参考电压控制,保留
	IICDEV_WByte(0x35, 0x84);//保留
	IICDEV_WByte(0x36, 0x00);//保留
	IICDEV_WByte(0x38, 0x13);//ADC和模拟共模控制,保留
	IICDEV_WByte(0x39, 0x43);//ADC偏移控制,保留
	IICDEV_WByte(0x3a, 0x00);//行缓冲测试选项
	IICDEV_WByte(0x3c, 0x6c);//在VSYNC为低时没有HREF
	IICDEV_WByte(0x3d, 0x90);//Gamma使能
	//IICDEV_WByte(0x3e, 0x1C);//缩小pclc	
	
	IICDEV_WByte(0x3f, 0x29);//边缘增强调整
	IICDEV_WByte(0x40, 0xc1);//数据形式-全范围输出使能
	IICDEV_WByte(0x41, 0x20);//针对YUV边缘增强阈值自动调整
	IICDEV_WByte(0x6b, 0x00);//关PLL,开LDO//0x0a);
	//IICDEV_WByte(0x6b, 0xC0);//PLL*8,开LDO
	IICDEV_WByte(0xa1, 0xc8);//保留
	//
	IICDEV_WByte(0x69, 0x80);//固定增益控制
	IICDEV_WByte(0x43, 0xf0);//保留
	IICDEV_WByte(0x44, 0x10);//保留
	IICDEV_WByte(0x45, 0x78);//保留
	IICDEV_WByte(0x46, 0xa8);//保留
	IICDEV_WByte(0x47, 0x60);//保留
	IICDEV_WByte(0x48, 0x80);//保留
	IICDEV_WByte(0x59, 0xba);//AWB控制<
	IICDEV_WByte(0x5a, 0x9a);//
	IICDEV_WByte(0x5b, 0x22);//
	IICDEV_WByte(0x5c, 0xb9);//
	IICDEV_WByte(0x5d, 0x9b);//
	IICDEV_WByte(0x5e, 0x10);//
	IICDEV_WByte(0x5f, 0xe0);//
	IICDEV_WByte(0x60, 0x85);//
	IICDEV_WByte(0x61, 0x60);//AWB控制>
	IICDEV_WByte(0x9f, 0x9d);//基于直方图的AEC/AGC的控制1
	IICDEV_WByte(0xa0, 0xa0);//基于直方图的AEC/AGC的控制2
	//
	IICDEV_WByte(0x4f, 0x66);//色彩矩阵系数1
	IICDEV_WByte(0x50, 0x6b);//色彩矩阵系数2
	IICDEV_WByte(0x51, 0x05);//色彩矩阵系数3
	IICDEV_WByte(0x52, 0x19);//色彩矩阵系数4
	IICDEV_WByte(0x53, 0x40);//色彩矩阵系数5
	IICDEV_WByte(0x54, 0x59);//色彩矩阵系数6
	IICDEV_WByte(0x55, 0x40);//亮度控制
	IICDEV_WByte(0x56, 0x40);//对比度控制
	IICDEV_WByte(0x57, 0x40);//对比度中心
	IICDEV_WByte(0x58, 0x0d);//色彩矩阵系数5~0的符号
	//
	IICDEV_WByte(0x8b, 0xcc);//保留
	IICDEV_WByte(0x8c, 0xcc);//RGB444
	IICDEV_WByte(0x8d, 0xcf);//保留
	//
	IICDEV_WByte(0x6c, 0x40);//AWB控制3
	IICDEV_WByte(0x6d, 0x30);//AWB控制2
	IICDEV_WByte(0x6e, 0x4b);//AWB控制1
	IICDEV_WByte(0x6f, 0x60);//AWB控制0
	IICDEV_WByte(0x70, 0x70);//测试图案[0],水平缩放系数
	IICDEV_WByte(0x71, 0x70);//测试图案[1],水平缩放系数
	IICDEV_WByte(0x72, 0x70);//DCW 控制
	IICDEV_WByte(0x73, 0x70);//DSP缩放时钟分频控制
	IICDEV_WByte(0x74, 0x60);//手动数字增益\数字增益手动控制
	IICDEV_WByte(0x75, 0x60);//边缘增强下限
	IICDEV_WByte(0x76, 0x50);//白点校正使能,边缘增强上限
	IICDEV_WByte(0x77, 0x48);//噪声去除偏移
	IICDEV_WByte(0x78, 0x3a);//保留
	IICDEV_WByte(0x79, 0x2e);//保留
	IICDEV_WByte(0x7a, 0x28);//伽马曲线最高段斜率
	IICDEV_WByte(0x7b, 0x22);//伽马曲线1节输入结束点0x04输出值
	IICDEV_WByte(0x7c, 0x04);//伽马曲线2节输入结束点0x08输出值
	IICDEV_WByte(0x7d, 0x07);//伽马曲线3节输入结束点0x10输出值
	IICDEV_WByte(0x7e, 0x10);//伽马曲线4节输入结束点0x20输出值
	IICDEV_WByte(0x7f, 0x28);//伽马曲线5节输入结束点0x28输出值
	IICDEV_WByte(0x80, 0x36);//伽马曲线6节输入结束点0x30输出值
	IICDEV_WByte(0x81, 0x44);//伽马曲线7节输入结束点0x38输出值
	IICDEV_WByte(0x82, 0x52);//伽马曲线8节输入结束点0x40输出值
	IICDEV_WByte(0x83, 0x60);//伽马曲线9节输入结束点0x48输出值
	IICDEV_WByte(0x84, 0x6c);//伽马曲线10节输入结束点0x50输出值
	IICDEV_WByte(0x85, 0x78);//伽马曲线11节输入结束点0x60输出值
	IICDEV_WByte(0x86, 0x8c);//伽马曲线12节输入结束点0x70输出值
	IICDEV_WByte(0x87, 0x9e);//伽马曲线13节输入结束点0x90输出值
	IICDEV_WByte(0x88, 0xbb);//伽马曲线14节输入结束点0xB0输出值
	IICDEV_WByte(0x89, 0xd2);//伽马曲线15节输入结束点0xD0输出值
	IICDEV_WByte(0x8a, 0xe6);//保留
	//
	IICDEV_WByte(0x14, 0x1a);//最大AGC值
	IICDEV_WByte(0x24, 0x78);//AGC/AEC-稳定运行区域（上限）
	IICDEV_WByte(0x25, 0x6c);//AGC/AEC-稳定运行区域（下限）
	IICDEV_WByte(0x62, 0x00);//镜头补偿选项1-对于光学中心补偿中心的X轴坐标
	IICDEV_WByte(0x63, 0x00);//镜头补偿选项2-对于光学中心补偿中心的Y轴坐标
	IICDEV_WByte(0x64, 0x18);//镜头补偿选项3-G通道的补偿系数
	IICDEV_WByte(0x65, 0x30);//镜头补偿选项4-避免补偿的半径
	IICDEV_WByte(0x66, 0x05);//镜头补偿选项5
	IICDEV_WByte(0x94, 0x18);//镜头校正选项6（在LCC5[2]=1有效）
	IICDEV_WByte(0x95, 0x1c);//镜头校正选项7（在LCC5[2]=1有效）
	delayms(1);//udelay(8000);
	IICDEV_WByte(0x13, 0xf7);//使能快速AGC/AEC算法
	
	//------------------相素控制
	OV7670_config_window(200,200,100,100);//(184，10,320,240）	
	//IICDEV_WByte(0x3e, BIT(3));
	//IICDEV_WByte(0x72, 0x11);//垂直亚抽样,水平亚抽样
	//IICDEV_WByte(0x73, 0x00);//DSP缩放分频
}*/
//------------------------------------------------
//初始化
//------------------------------------------------
void OV7670_init(void)
{
	U8 i;
	PWDN_H;
	delayms(1);
	PWDN_L;
	delayms(1);
	RST_L;
	delayms(1);
	RST_H;	
	IICDEV_WByte(0x12, 0x80);//SCCB复位
	delayms(1);
	for(i=0;i<ARRLEN(OV7670_REG);i++)
	{
		IICDEV_WByte(OV7670_REG[i][0],OV7670_REG[i][1]);
	}
	//OV7670_config_window(184,10,100,100);//(10,10,100,100);//(184,10,320,240)
	//OV7670_config_window(402,20,200,100*5/3);//200*100的YUV图像,只取Y则为100*100
	OV7670_config_window(402,20,CAMWIDTH*2,CAMHEIGHT*5/3);//200*100的YUV图像,只取Y则为100*100
}
