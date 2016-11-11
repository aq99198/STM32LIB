#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f10x.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_flash.h"
#include "adc.h"
#include "string.h"


static uint16_t ad_value[ADC_TIMES][CH_NUM]={0};

void adc_gpio_init(void)
{
	 GPIO_InitTypeDef gpio;
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	 gpio.GPIO_Mode = GPIO_Mode_AIN;        
	 /*GPIOC PIN 4: battery    GPIOC PIN 5: main power*/	 
	 gpio.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
	 gpio.GPIO_Speed = GPIO_Speed_50MHz;
	 GPIO_Init(GPIOC,&gpio);
}

void adc_init(void)
 {
     ADC_InitTypeDef adc;
     RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
     RCC_ADCCLKConfig(RCC_PCLK2_Div6);
     
     ADC_DeInit(ADC1);
     adc.ADC_Mode                = ADC_Mode_Independent;    
     adc.ADC_ContinuousConvMode  = ENABLE;                  
     adc.ADC_DataAlign           = ADC_DataAlign_Right;      
     adc.ADC_ExternalTrigConv    = ADC_ExternalTrigConv_None;
     adc.ADC_NbrOfChannel        = CH_NUM;
     adc.ADC_ScanConvMode        = ENABLE;
     ADC_Init(ADC1,&adc);

     ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 1, ADC_SampleTime_239Cycles5);          //14CH
     ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 2, ADC_SampleTime_239Cycles5);          //15CH

     ADC_DMACmd(ADC1,ENABLE);

     ADC_Cmd(ADC1, ENABLE);
     /* Enable ADC1 reset calibaration register */
     ADC_ResetCalibration(ADC1); 
     /* Check the end of ADC1 reset calibration register */
     while(ADC_GetResetCalibrationStatus(ADC1));
     /* Start ADC1 calibaration */
     ADC_StartCalibration(ADC1);
     /* Check the end of ADC1 calibration */
     while(ADC_GetCalibrationStatus(ADC1));
}

 void adc_dma_init()
 {
     DMA_InitTypeDef dma;
     RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
     
     DMA_DeInit(DMA1_Channel1);
     dma.DMA_PeripheralBaseAddr  = (u32)&(ADC1->DR);
     dma.DMA_MemoryBaseAddr      = (u32)&ad_value[0][0];
     dma.DMA_DIR                 = DMA_DIR_PeripheralSRC;
     dma.DMA_M2M                 = DMA_M2M_Disable;
     dma.DMA_PeripheralDataSize  = DMA_PeripheralDataSize_HalfWord;
     dma.DMA_MemoryDataSize      = DMA_MemoryDataSize_HalfWord;    
     dma.DMA_BufferSize          = CH_NUM*ADC_TIMES;                      
     dma.DMA_MemoryInc           = DMA_MemoryInc_Enable;        
     dma.DMA_PeripheralInc       = DMA_PeripheralInc_Disable;
     dma.DMA_Mode                = DMA_Mode_Circular;           
     dma.DMA_Priority            = DMA_Priority_High;           
     DMA_Init(DMA1_Channel1,&dma); 
 }

void adc_config(void)
{
     adc_gpio_init();
     adc_dma_init();
     adc_init();
}

void adc_start(void)
 {
     /* Start ADC1 Software Conversion */ 
	   memset(&ad_value[0][0], 0, ADC_TIMES * CH_NUM * sizeof(uint16_t));
     ADC_SoftwareStartConvCmd(ADC1, ENABLE);         //soft triger
     DMA_Cmd(DMA1_Channel1,ENABLE);
 }

uint16_t get_ChannelVale(uint8_t ADC_Channel)
 {
     uint16_t ret = 0;
	   int i;
	   for(i=0; i<ADC_TIMES; i++)
	   {
			 ret += ad_value[i][ADC_Channel];
	   }
		 return (ret+(ADC_TIMES>>1))/ADC_TIMES;
 }

#ifdef __cplusplus
}
#endif

