#ifndef __ADC_H__
#define __ADC_H__

#include "stm32f10x.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ADC_TIMES 5
#define CH_NUM    2

//void adc_gpio_init(void);
//void adc_init(void);
//void adc_dma_init();
void adc_config(void);
void adc_start(void);
unsigned short get_ChannelVale(unsigned char ADC_Channel);
void write_onchip_flash(u32 addr,u8 *p,u16 n);	
void read_onchip_flash(u32 addr,u8 *p,u16 n);

#ifdef __cplusplus
}
#endif

#endif
