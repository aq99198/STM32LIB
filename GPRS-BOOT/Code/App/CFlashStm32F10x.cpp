

#include "CFlashStm32F10x.h"
#include "stm32f10x_flash.h"



/* stm32f405 flash address map , Base address of the Flash sectors */
#define ADDR_FLASH_SECTOR_0     ((UINT32)0x08000000) /* Base @ of Sector 0, 16 Kbytes */
#define ADDR_FLASH_SECTOR_1     ((UINT32)0x08004000) /* Base @ of Sector 1, 16 Kbytes */
#define ADDR_FLASH_SECTOR_2     ((UINT32)0x08008000) /* Base @ of Sector 2, 32 Kbytes */
#define ADDR_FLASH_SECTOR_3     ((UINT32)0x0800C000) /* Base @ of Sector 3, 48 Kbytes */
#define ADDR_FLASH_SECTOR_4     ((UINT32)0x08010000) /* Base @ of Sector 4, 64 Kbytes */
#define ADDR_FLASH_SECTOR_5     ((UINT32)0x08020000) /* Base @ of Sector 5, 128 Kbytes */



//!< 定义使用stm32f405 单片机，使用的bootloader地址空间为ADDR_FLASH_SECTOR_0~ADDR_FLASH_SECTOR_4，共128K
//!< 定义使用stm32f405 单片机，使用的APP应用程序地址空间为ADDR_FLASH_SECTOR_5，共128K


#define STM32F10X_BOOTLOADER_START   ADDR_FLASH_SECTOR_0
#define STM32F10X_BOOTLOADER_SIZE    (128*1024) //!<128kbytes

#define STM32F10X_APP_START          ADDR_FLASH_SECTOR_5
#define STM32F10X_APP_SIZE           (128*1024) //!<128kbytes
#define STM32F10X_PAGE_SIZE          (2*1024) 
#define STM32F10X_FLASH_APP_PAGES    (STM32F10X_APP_SIZE/STM32F10X_PAGE_SIZE)
typedef struct 
{
  UINT32 size;
  UINT32 addr;
}STM32F10XFLASH_SEGMENT;

STM32F10XFLASH_SEGMENT FlashSectorMap[1] =
{
  {128*1024,ADDR_FLASH_SECTOR_5},
};
/**
  * @brief  
  * @param  None
  * @retval None
  * @note   None
  */    
CFlashStm32F10X::CFlashStm32F10X()
{

}

/**
  * @brief  
  * @param  None
  * @retval None
  * @note   None
  */    
CFlashStm32F10X::~CFlashStm32F10X()
{

}



/**
* @brief  获取支持最大APP Flash大小
* @param  None
* @retval UINT32 ，返回当前支持最大的APP文件大小，单位字节
* @note   None
*/   
UINT32 CFlashStm32F10X::getSysSize()
{
    return STM32F10X_APP_SIZE;
}

/**
  * @brief  获得APP 启动地址
  * @param  None
  * @retval UINT32 ，启动地址
  * @note   None
  */   
UINT32 CFlashStm32F10X::getAppStart()
{
  return STM32F10X_APP_START;
}

/**
  * @brief  擦除制定大小的APP Flash区域，为写入App做准备
  * @param  None
  * @retval bool 擦除成功，返回true；擦除失败，返回false
  * @note   app的大小必须要不大于支持最大APP的大小
  */      
bool CFlashStm32F10X::eraseAppFlash(UINT32 appsize)
{
	UINT32 i;
	UINT32 addr;
  if( appsize > STM32F10X_APP_SIZE )
  {
    return false;
  }
  if( appsize < 1 )
  {
    return true;
  }
	
	FLASH_Unlock();
	addr = FlashSectorMap[0].addr;
  for(i=0; i < STM32F10X_FLASH_APP_PAGES ; i++)
  {
		do
		{
		}
		while(FLASH_COMPLETE != FLASH_ErasePage(addr));	
		addr += STM32F10X_PAGE_SIZE;
  } 
	FLASH_Lock();
  return true;
}

/**
  * @brief  将APP写入Flash
  * @param  offset,写入地址偏移，单位字节
            *app  ,需要写入的app缓冲区
            len   ,需要写入的数据长度
  * @retval bool  写入成功返回true；写入失败，返回false
  * @note   None
  */     
bool CFlashStm32F10X::programApptoFlash(UINT32 offset,UINT8 *app,UINT32 len )
{

    UINT16 *pshort;
    UINT32 *pword;
    UINT32 step;
    

    offset+=STM32F10X_APP_START;

    UINT32 Address = offset;
    UINT32 Wordlen = len / 4;
    UINT32 Bytelen = len % 4;


    FLASH_Unlock();

    step = 0 ;
    pword = (UINT32 *)app;
    while( step < Wordlen )
    {
      while (FLASH_COMPLETE != FLASH_ProgramWord(Address, *pword)) ;
      
      Address = Address + 4;
      ++pword;
      ++step;
    }

    FLASH_Lock();

    pshort = (UINT16 *)pword;
		switch (Bytelen)
		{
			case 1:
				FLASH_Unlock();
			  *((UINT16*)pshort) = (*((UINT16*)pshort))&0xFF;
			  FLASH_ProgramHalfWord(Address, *pshort);			
			  FLASH_Lock();
				break;
			case 2:
				FLASH_Unlock();
			  FLASH_ProgramHalfWord(Address, *pshort);
        FLASH_Lock();
				break;
			case 3:
				FLASH_Unlock();
			  *((UINT32*)pword) = (*((UINT32*)pword))&0xFFFFFF;
			  FLASH_ProgramWord(Address, *pword);
			  FLASH_Lock();
				break;
			default:
				break;
		}
    return true;
}

/**
  * @brief  
  * @param  None
  * @retval None
  * @note   None
  */   
void CFlashStm32F10X::flashEraseProgramDebug()
{

  #if 1  
  static UINT8  wbuf[1024]; 
  while( !eraseAppFlash(STM32F10X_APP_SIZE) );

  UINT32 i=0 ; 
  for( i = 0 ; i!= 1024 ; ++i )
  {
    wbuf[i]=0;
  }
  for(i=0; i != STM32F10X_APP_SIZE ; i+=1024)
  {
    programApptoFlash(i,wbuf,1024);
  }

  #endif 
}
