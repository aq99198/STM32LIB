/*
 * sim900a.cpp
 */

#include "board.h"
#include "uartdriver.h"
#include "sim900a.h"

/*****************************************************/
#define SIM900A_PWRKEY_GPIO_PORT    (GPIOB)
#define SIM900A_PWRKEY_GPIO_PIN     (GPIO_Pin_5)

#define SIM900A_NRESET_GPIO_PORT    (GPIOB)
#define SIM900A_NRESET_GPIO_PIN     (GPIO_Pin_6)

#define SIM900A_STATUS_GPIO_PORT    (GPIOB)
#define SIM900A_STATUS_GPIO_PIN     (GPIO_Pin_7)

#define GPIO_SET_HIGH(GPIOx,GPIO_Pin)             do{GPIOx->BSRR = GPIO_Pin;}while(0);
#define GPIO_SET_LOW(GPIOx,GPIO_Pin)              do{GPIOx->BRR  = GPIO_Pin;}while(0);

#define GPIO_READ_PIN(GPIOx,GPIO_Pin,VALUE)       do{ VALUE = (GPIOx->IDR & GPIO_Pin) ? 1 : 0 ; }while(0);

#define AT_RESP_BUF_LEN  (256)
UINT8 AtResp[AT_RESP_BUF_LEN];

void SIM900A::InitSIM900AGpio(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_StructInit(&GPIO_InitStructure);
    
    /* Configure GPRS POWER_ON as push-pull, pin = PB5*/
    GPIO_InitStructure.GPIO_Pin = SIM900A_PWRKEY_GPIO_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(SIM900A_PWRKEY_GPIO_PORT, &GPIO_InitStructure);

    /* Configure GPRS STATUS as input floating, pin = PB7 */
    GPIO_InitStructure.GPIO_Pin = SIM900A_STATUS_GPIO_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(SIM900A_STATUS_GPIO_PORT, &GPIO_InitStructure);

    /* Configure GPRS RST as push-pull, pin = PB6 */
    GPIO_InitStructure.GPIO_Pin = SIM900A_NRESET_GPIO_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(SIM900A_NRESET_GPIO_PORT, &GPIO_InitStructure);

    GPIO_SET_LOW(SIM900A_PWRKEY_GPIO_PORT,SIM900A_PWRKEY_GPIO_PIN);
    GPIO_SET_HIGH(SIM900A_NRESET_GPIO_PORT,SIM900A_NRESET_GPIO_PIN);   
}

/********************************************************************/

SIM900A * SIM900A::inStance = 0 ;

SIM900A::SIM900A()
{
    //! < init the GPIO of the MCU ,  MCU can  control and check  the SIM900A module by the GPIO
    InitSIM900AGpio();
    serialSim900a = new CUartDriver(SIM900A_SERIAL_PORT);
    serialSim900a->setBaudRate(SIM900A_SERILA_BAUDRATE);
}

SIM900A::~SIM900A()
{

}


SIM900A* SIM900A::getIntance()
{
    if( 0 == inStance )
    {
        inStance = new SIM900A();
    }
    return inStance;
}


void SIM900A::PowerUp(void)
{

    GPIO_SET_HIGH(SIM900A_PWRKEY_GPIO_PORT,SIM900A_PWRKEY_GPIO_PIN);
    OSTimeDly(200);
    GPIO_SET_LOW(SIM900A_PWRKEY_GPIO_PORT,SIM900A_PWRKEY_GPIO_PIN);
    OSTimeDly(2000);
    GPIO_SET_HIGH(SIM900A_PWRKEY_GPIO_PORT,SIM900A_PWRKEY_GPIO_PIN);
    OSTimeDly(3000);
}

void SIM900A::NReset(void)
{
	GPIO_SET_HIGH(SIM900A_NRESET_GPIO_PORT,SIM900A_NRESET_GPIO_PIN);
	OSTimeDly(200);
	GPIO_SET_LOW(SIM900A_NRESET_GPIO_PORT,SIM900A_NRESET_GPIO_PIN);
	OSTimeDly(1);
	GPIO_SET_HIGH(SIM900A_NRESET_GPIO_PORT,SIM900A_NRESET_GPIO_PIN);
	OSTimeDly(2500);  
}

/* TRUE for failure */
bool  SIM900A::SysInit(INT8 RstFlag)
{
    UINT8  pinvalue;
    UINT8  retry = 0;
	
    do
    {
        if( retry > SIM900A_DEFAULT_POWER_UP_TIMES )
        {
            return true;
        }  
				if(RstFlag)
				{
					NReset();				
				}
				else
				{
					PowerUp();  //! < SIM900A_POWER_UP
					OSTimeDly(2200);
				}
        GPIO_READ_PIN(SIM900A_STATUS_GPIO_PORT,SIM900A_STATUS_GPIO_PIN,pinvalue); //! < SIM900A_STATUS
        
        ++retry;
    }while( !pinvalue );	
		
		
		
    serialSim900a->clearBuffer();	
		
    return false;
}

int SIM900A::readBuffer(UINT8 *buffer,INT32 count, UINT32 timeOut)
{
    INT32 i = 0;
    UINT32 timerStart,timerEnd;
    timerStart = OSTimeGet();
    UINT8 c;
    INT32 rlen;
    while(1)
    {
        if (serialSim900a->available()) 
        {
            rlen=1;
            serialSim900a->read(&c,&rlen);
            if (c == '\r' || c == '\n') 
            {
                c = '$';
            }
            buffer[i++] = c;
            if(i > count-1)
            {
                break;
            }
        }
        else
        {
            OSTimeDly(1);
        }
        
        if(i > count-1)
        {
                break;
        }
        
        timerEnd = OSTimeGet();
        if(timerEnd - timerStart > 1000 * timeOut) 
        {
            break;
        }
    }
    
    OSTimeDly(500);
    while(serialSim900a->available()) 
    {   // display the other thing..
        rlen=1;
        serialSim900a->read(&c,&rlen);
    }
    return 0;
}

void SIM900A::readBuffer(UINT8* buffer,INT32 count)
{
	serialSim900a->read(buffer,&count);
}

INT32 SIM900A::SendFlatData(UINT8 *data, UINT32* len)
{
	serialSim900a->write(data,*len);
	return 0;
}


INT32 SIM900A::ReadFlatData(UINT8 *data, UINT32 & len,UINT32 timeout)
{

    INT32 i = 0;
    UINT32 timerStart,timerEnd;
    timerStart = OSTimeGet();
    UINT8 c;
    INT32 rlen;
    while(1)
    {
        if (serialSim900a->available()) 
        {
            rlen=1;
            serialSim900a->read(&c,&rlen);
            data[i++] = c;
            if(i > len-1)
            {
                break;
            }
        }
        else
        {
            OSTimeDly(1);
        }
        
        if(i > len-1)
        {
                break;
        }
        
        timerEnd = OSTimeGet();
        if(timerEnd - timerStart > 1000 * timeout) 
        {
            len =i;
            return 1;
        }
    }
    
    len =i;
    return 0;
    
}

void SIM900A::cleanBuffer(UINT8 *buffer, INT32 count)
{
    for(int i=0; i < count; i++) {
        buffer[i] = '\0';
    }
}

void SIM900A::sendCmd(UINT8* cmd)
{
    serialSim900a->write(cmd,strlen((const char*)cmd));
}

void SIM900A::sendCmd(UINT8* cmd, INT32 len)
{
	  serialSim900a->write(cmd, len);
}

void SIM900A::sendATTest(void)
{
    sendCmdAndWaitForResp("AT\r\n","OK",DEFAULT_TIMEOUT);
}

int SIM900A::waitForResp(const UINT8 *resp, UINT32 timeout)
{
    INT32 len = strlen((const char*)resp);
    UINT32 timerStart,timerEnd;
    timerStart = OSTimeGet();
    INT32 rlen=0;
    UINT8 c ;
		UINT8 idx = 0;
	  int ret = 0;
	
	  memset(AtResp,0,AT_RESP_BUF_LEN);
    while(1) 
    {
			  if(idx >= AT_RESP_BUF_LEN)
				{
					ret = 1;
					break;
				}
				
        if(serialSim900a->available()) 
        {           
            rlen=1;
            serialSim900a->read(&AtResp[idx], &rlen);
						#ifdef SIM900A_DEBUG_ON					
					  PRINT("%c",AtResp[idx]);
						#endif
					  idx++; 
            if(NULL != strstr((char *)&AtResp[0],(char*)resp)) 
						{
							 ret = 0;
               break;
            }
				    if(NULL != strstr((char *)&AtResp[0],"ERROR")) 
						{
               ret = 1;
							 break;
            }
        }
        else
        {
            OSTimeDly(10);//!< ÊÍ·ÅCPU 
        }
        timerEnd = OSTimeGet();
        if((timerEnd - timerStart) > 1000 * timeout) 
        {
					  ret = 1;
					  break;
        }
    }

    //! < clear the buffer
    while(serialSim900a->available()) 
    {
        rlen=1;
        serialSim900a->read(&c,&rlen);
				#ifdef SIM900A_DEBUG_ON		
			  PRINT("%c",c);
				#endif
    }
		//PRINT("\r\n");

    return ret;
}
                         

void SIM900A::sendEndMark(void)
{
    UINT8 c = 26;
    serialSim900a->write(&c,1);
}


int SIM900A::sendCmdAndWaitForResp(const char* cmd, const char *resp, UINT32 timeout)
{
    sendCmd((UINT8*)cmd);
		#ifdef SIM900A_DEBUG_ON
		PRINT("send commaner: %s",cmd);
		#endif
    return waitForResp((const UINT8*)resp,timeout);
}



void SIM900A::serialDebug(void)
{
//    UINT8 data = 0xAA;
    INT32 len;
	  char* c = "bbbbbbbbbbbbbbbbbbbb";
    while(1) 
    {

		len = strlen(c);
		serialSim900a->write((UINT8*)c,len);
		OSTimeDly(1000);
    }
}

CUartDriver *SIM900A::getSim()
{
	return serialSim900a;
}
