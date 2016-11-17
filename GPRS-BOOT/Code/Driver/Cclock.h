


#ifndef _CLOCK_H
#define _CLOCK_H

#include "JacTypeDef.h"

class Cclock
{
public:
/**
  * @brief  初始化系统时钟tick的分辨率 
  * @param  freq ，频率分辨率，如，系统分辨率为1000Hz，设置参数freq=1000
  * @retval None
  * @note   freq最大参数为1000
  */	
	Cclock( UINT32 freq );

/**
  * @brief  
  * @param  None
  * @retval None
  * @note   None
  */	
	~Cclock();

/**
  * @brief  系统延迟时间功能
  * @param  ms 延迟时间，单位毫秒
  * @retval None
  * @note   此延迟实现为同步操作
  */
	void Delay(INT32 ms);

/**
  * @brief  读取系统当前时间tick
  * @param  None
  * @retval 返回系统当前时间tick数
  * @note   系统时间计数是在调用 Cclock::Cclock( UINT32 freq ) 之后开始计数，计数的频率由freq确定
  */
  UINT32  getSysTime();

private:
	
};



#endif 
