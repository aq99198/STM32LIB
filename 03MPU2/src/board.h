#pragma once

// for roundf()
#define __USE_C99_MATH

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>

#include "stm32f10x_conf.h"
#include "core_cm3.h"

#ifndef __CC_ARM
// only need this garbage on gcc
#define USE_LAME_PRINTF
#include "printf.h"
#endif

#include "drv_gpio.h"
#include "drv_SysTick.h"
#include "mymath.h"


#ifndef M_PI
#define M_PI       3.14159265358979323846f
#endif /* M_PI */

#define RADX10 (M_PI / 1800.0f)                  // 0.001745329252f
#define RAD    (M_PI / 180.0f)

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))
#define abs(x) ((x) > 0 ? (x) : -(x))

// Chip Unique ID on F103
#define U_ID_0 (*(uint32_t*)0x1FFFF7E8)
#define U_ID_1 (*(uint32_t*)0x1FFFF7EC)
#define U_ID_2 (*(uint32_t*)0x1FFFF7F0)


typedef void (*serialReceiveCallbackPtr)(uint16_t data);   // used by serial drivers to return frames to app

enum
{
	MPU_6050_0 = 0,
	MPU_6050_1,
	
};

enum
{
 A_X = 0,
 A_Y ,
 A_Z ,
 G_Y ,
 G_X ,
 G_Z ,
 TEM ,
 ITEMS ,
};




typedef struct
{
	float x;
	float y;
	float z;
} _xyz_f_st;

typedef struct
{
	s16 x;
	s16 y;
	s16 z;
} _xyz_s16_st;

typedef struct
{
	float x;
	float y;
	float z;
}
__attribute__((packed)) _xyz_f_st_pk;

typedef struct
{
	s16 x;
	s16 y;
	s16 z;
}__attribute__((packed)) _xyz_s16_st_pk;


typedef struct
{
	s32 kp;			 //????
	s32 ki;			 //????
	s32 kd;		 	 //????

}PID_param_st_pk; 

struct _save_param_st_pk{	
	u8 firstintiflag;
	u16 hardware;
	u16 software;
	u8 sensor_type;
	
	_xyz_f_st_pk acc_offset;//3?float 12??
	_xyz_f_st_pk gyr_offset;
	_xyz_f_st_pk vec_3d_offset;
	
	float gyr_temprea_offset;
	float acc_temprea_offset;
	
	PID_param_st_pk PID_ct4;
	PID_param_st_pk PID_ct3;
	
	PID_param_st_pk PID_rol; //12??,3?float
	PID_param_st_pk PID_pit;
	PID_param_st_pk PID_yaw;
	
	PID_param_st_pk PID_rol_s; //12??,3?float
	PID_param_st_pk PID_pit_s;
	PID_param_st_pk PID_yaw_s;
	
	PID_param_st_pk PID_hs;
	
};


typedef struct
{
	u8 cnt;

	s32 lst_pow_sum;
	
	s32 now_out;
	s32 lst_out;
	s32 now_velocity_xdt;
} _steepest_st;

#define OFFSET_AV_NUM 50


#define LED0
#define LED1


#define LED0_GPIO   GPIOB
#define LED0_PIN    Pin_3 // PB3 (LED)
#define LED1_GPIO   GPIOB
#define LED1_PIN    Pin_4 // PB4 (LED)
#define BEEP_GPIO   GPIOA
#define BEEP_PIN    Pin_12 // PA12 (Buzzer)
#define BARO_GPIO   GPIOC
#define BARO_PIN    Pin_13
#define INV_PIN     Pin_2 // PB2 (BOOT1) abused as inverter select GPIO
#define INV_GPIO    GPIOB


#define SENSORS_SET (SENSOR_ACC | SENSOR_BARO | SENSOR_MAG)
#define I2C_DEVICE (I2CDEV_2)

// #define PROD_DEBUG
// #define SOFT_I2C                 // enable to test software i2c
// #define SOFT_I2C_PB1011          // If SOFT_I2C is enabled above, need to define pinout as well (I2C1 = PB67, I2C2 = PB1011)
// #define SOFT_I2C_PB67


#include "drv_serial.h"
#include "drv_uart.h"
#include "drv_timer.h"
#include "drv_pwm.h"
#include "i2c_soft.h"
#include "ANO_Drv_MPU6050.h"


// Helpful macros
#ifdef LED0
#define LED0_TOGGLE              digitalToggle(LED0_GPIO, LED0_PIN);
#define LED0_OFF                 digitalHi(LED0_GPIO, LED0_PIN);
#define LED0_ON                  digitalLo(LED0_GPIO, LED0_PIN);
#else
#define LED0_TOGGLE
#define LED0_OFF
#define LED0_ON
#endif

#ifdef LED1
#define LED1_TOGGLE              digitalToggle(LED1_GPIO, LED1_PIN);
#define LED1_OFF                 digitalHi(LED1_GPIO, LED1_PIN);
#define LED1_ON                  digitalLo(LED1_GPIO, LED1_PIN);
#else
#define LED1_TOGGLE
#define LED1_OFF
#define LED1_ON
#endif

#ifdef BEEP_GPIO
#define BEEP_TOGGLE              digitalToggle(BEEP_GPIO, BEEP_PIN);
#define BEEP_OFF                 systemBeep(false);
#define BEEP_ON                  systemBeep(true);
#else
#define BEEP_TOGGLE              ;
#define BEEP_OFF                 ;
#define BEEP_ON                  ;
#endif

#ifdef INV_GPIO
#define INV_OFF                  digitalLo(INV_GPIO, INV_PIN);
#define INV_ON                   digitalHi(INV_GPIO, INV_PIN);
#else
#define INV_OFF                 ;
#define INV_ON                  ;
#endif




