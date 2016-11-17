

#ifndef CLI_H
#define CLI_H

#include "JacTypeDef.h"

enum CLI_MODE
{
  CLI_UPDATE_MODE=0,
  CLI_APP_MODE,
  NUM_CLI_MODE
};


class CMsgFrame;
class CCli
{
public:
/**
  * @brief  
  * @param  None
  * @retval None
  * @note   None
  */        
    CCli(CMsgFrame * frame);

/**
  * @brief  
  * @param  None
  * @retval None
  * @note   None
  */        
    ~CCli();

/**
  * @brief  用户字符界面
  * @param  None
  * @retval CLI_MODE，返回系统根据用户选择，决定当前启动的模式
  * @note   None
  */  
  CLI_MODE cliInterface();
private:
  CMsgFrame * msgFrame;
};



#endif 
