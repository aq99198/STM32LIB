
文件说明:
User:
	包含用户所需(修改)的文件,比如 
	"main.c": 包含main()函数
	"stm32f10x_it.c": 包含一些常用的中断函数
	"core_cm3.h" 和 "core_cm3c" 中包括Cortex-M3核的全局变量声明和定义，并定义一些静态功能函数
FWlib:
	详见:<STM32F10xxx固件库_3.01.pdf.pdf>
	misc.c		貌似是NVIC中断的一些控制?
	stm32f10x_adc.c		ADC
	stm32f10x_bkp.c		备份寄存器
	stm32f10x_can.c		CAN总线
	stm32f10x_crc.c		CRC(循环冗余码校验)?
	stm32f10x_dac.c		DAC
	stm32f10x_dbgmcu.c	貌似是用于调试的?
	stm32f10x_dma.c		DMA(直接存储器存取)
	stm32f10x_exti.c	外部中断/事件控制器
	stm32f10x_flash.c	FLASH存储器
	stm32f10x_fsmc.c	静态存储器控制器?
	stm32f10x_gpio.c	（GPIO）通用输入/输出
	stm32f10x_i2c.c		I2C总线
	stm32f10x_iwdg.c	独立看门狗
	stm32f10x_pwr.c		功耗控制
	stm32f10x_rcc.c		复位和时钟设置
	stm32f10x_rtc.c		实时时钟
	stm32f10x_sdio.c	SD卡,MMC卡
	stm32f10x_spi.c		SPI串行外设接口
	stm32f10x_tim.c		通用定时器
	stm32f10x_usart.c	USART通用同步异步收发器
	stm32f10x_wwdg.c	窗口看门狗
	system_stm32f10x.c	系统初始化,系统时钟控制?
	
	
	
注意事项:
	堆栈设置过小的话,局部变量过多会引起内存溢出.
	在startup中修改 Stack_Size      EQU     0x00000800	
	若不小心删除了工程的设置文件,编译有可能出问题:
		*** Error: Referred Memory Range 'ROM1' is undefined.
	解决办法:在工程选项里的 Target 里选中 IROM1,IRAM1

	
	