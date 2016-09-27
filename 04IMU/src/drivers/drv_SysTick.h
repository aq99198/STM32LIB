#pragma once

#include "stdint.h"
#include "stm32f10x_rcc.h"


void cycleCounterInit(void);
uint32_t micros(void);
void delay(uint32_t ms);
void delayMicroseconds(uint32_t us);




