
�ļ�˵��:
User:
	�����û�����(�޸�)���ļ�,���� 
	"main.c": ����main()����
	"stm32f10x_it.c": ����һЩ���õ��жϺ���
	"core_cm3.h" �� "core_cm3c" �а���Cortex-M3�˵�ȫ�ֱ��������Ͷ��壬������һЩ��̬���ܺ���
FWlib:
	���:<STM32F10xxx�̼���_3.01.pdf.pdf>
	misc.c		ò����NVIC�жϵ�һЩ����?
	stm32f10x_adc.c		ADC
	stm32f10x_bkp.c		���ݼĴ���
	stm32f10x_can.c		CAN����
	stm32f10x_crc.c		CRC(ѭ��������У��)?
	stm32f10x_dac.c		DAC
	stm32f10x_dbgmcu.c	ò�������ڵ��Ե�?
	stm32f10x_dma.c		DMA(ֱ�Ӵ洢����ȡ)
	stm32f10x_exti.c	�ⲿ�ж�/�¼�������
	stm32f10x_flash.c	FLASH�洢��
	stm32f10x_fsmc.c	��̬�洢��������?
	stm32f10x_gpio.c	��GPIO��ͨ������/���
	stm32f10x_i2c.c		I2C����
	stm32f10x_iwdg.c	�������Ź�
	stm32f10x_pwr.c		���Ŀ���
	stm32f10x_rcc.c		��λ��ʱ������
	stm32f10x_rtc.c		ʵʱʱ��
	stm32f10x_sdio.c	SD��,MMC��
	stm32f10x_spi.c		SPI��������ӿ�
	stm32f10x_tim.c		ͨ�ö�ʱ��
	stm32f10x_usart.c	USARTͨ��ͬ���첽�շ���
	stm32f10x_wwdg.c	���ڿ��Ź�
	system_stm32f10x.c	ϵͳ��ʼ��,ϵͳʱ�ӿ���?
	
	
	
ע������:
	��ջ���ù�С�Ļ�,�ֲ���������������ڴ����.
	��startup���޸� Stack_Size      EQU     0x00000800	
	����С��ɾ���˹��̵������ļ�,�����п��ܳ�����:
		*** Error: Referred Memory Range 'ROM1' is undefined.
	����취:�ڹ���ѡ����� Target ��ѡ�� IROM1,IRAM1

	
	