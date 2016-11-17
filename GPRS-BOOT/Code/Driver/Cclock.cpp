


#include "Cclock.h"

#include "stm32f10x_rcc.h"
extern "C" volatile INT32  tickDecrease;  
extern "C" volatile UINT32 systick;

/**
  * @brief  初始化系统时钟tick的分辨率 
  * @param  freq ，频率分辨率，如，系统分辨率为1000Hz，设置参数freq=1000
  * @retval None
  * @note   freq最大参数为1000
  */	
Cclock::Cclock( UINT32 freq )
{
	 
  UINT32  cnts;
  RCC_ClocksTypeDef  rcc_clocks;
  RCC_GetClocksFreq(&rcc_clocks); 
  cnts = rcc_clocks.HCLK_Frequency / freq;
  SysTick_Config(cnts);

  systick = 0;
}

/**
  * @brief  
  * @param  None
  * @retval None
  * @note   None
  */	
Cclock::~Cclock()
{

}

/**
  * @brief  系统延迟时间功能
  * @param  ms 延迟时间，单位毫秒
  * @retval None
  * @note   此延迟实现为同步操作
  */ 
void Cclock::Delay(INT32 ms)
{
	tickDecrease = ms ;

	while( tickDecrease > 0  )
	{
		__asm("nop");
		__asm("nop");
		__asm("nop");

	}
}

/**
  * @brief  读取系统当前时间tick
  * @param  None
  * @retval 返回系统当前时间tick数
  * @note   系统时间计数是在调用 Cclock::Cclock( UINT32 freq ) 之后开始计数，计数的频率由freq确定
  */
UINT32 Cclock::getSysTime()
{
	return systick;
}

