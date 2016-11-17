/*
 * gprs.cpp
 */

#include "board.h"
#include "uartdriver.h"
#include "gprs.h"

GPRS * GPRS::inStance = 0 ;

GPRS::GPRS()
{
    simInStance = SIM900A::getIntance();
    
}

GPRS::~GPRS()
{
}

GPRS* GPRS::getInstance()
{
    if( 0 == inStance )
    {
        inStance = new GPRS();
    }
    return inStance;
}


INT32 GPRS::init( INT32 baudRate, INT8 RstFlag)
{
    if( simInStance->SysInit(RstFlag) )
    {
				#ifdef SIM900A_DEBUG_ON		
        ERROR("sim900a can not powered up\r\n");
				#endif
        return 1;
    }else
		{
				SystemStatus = SYS_SIM900A_ON;
				#ifdef SIM900A_DEBUG_ON		
				ERROR("power								[OK]\r\n");
				#endif
		}	
		
		if(simInStance->sendCmdAndWaitForResp("AT\r\n","OK\r\n",DEFAULT_TIMEOUT*3))
    {
				#ifdef SIM900A_DEBUG_ON		
        ERROR("AT OK ERROR \r\n");
				#endif
				return 1;
    }	
		
		if(simInStance->sendCmdAndWaitForResp((char*)"ATE0\r\n",(char*)"OK\r\n",DEFAULT_TIMEOUT*3))
    {
				#ifdef SIM900A_DEBUG_ON		
        ERROR("ATE0 ERROR \r\n");
				#endif 
        return 1;
    }
		
		if(simInStance->sendCmdAndWaitForResp((char*)"AT+IPR=0\r\n",(char*)"OK\r\n",DEFAULT_TIMEOUT*3))
    {
				#ifdef SIM900A_DEBUG_ON		
        ERROR("AT IPR 0 ERROR \r\n");
        return 1;
				#endif
    }
		
//		if(simInStance->sendCmdAndWaitForResp((char*)"AT+IFC=2,2\r\n",(char*)"OK\r\n",DEFAULT_TIMEOUT*3))
//    {
//        ERROR("AT IFC 0 ERROR \r\n");
//        return 1;
//    }
	
    if(simInStance->sendCmdAndWaitForResp("AT+CFUN=1\r\n","OK\r\n",DEFAULT_TIMEOUT*3))
    {
				#ifdef SIM900A_DEBUG_ON		
        ERROR("AT CFUN=1 ERROR \r\n");
				#endif
        return 1;
    }		
		
		if(simInStance->sendCmdAndWaitForResp((char*)"AT+CSCLK?\r\n",(char*)"OK\r\n",DEFAULT_TIMEOUT*3))
    {
				#ifdef SIM900A_DEBUG_ON		
        ERROR("AT CSCLK ERROR \r\n");
				#endif
        return 1;
    }
		
    if(checkSIMStatus())
    {
				#ifdef SIM900A_DEBUG_ON		
        ERROR("CHECK SIM STATUS ERROR \r\n");
				#endif
	    return 1;
    }
    return 0;

}

bool GPRS::join( UINT8 *apn,  UINT8 *userName,  UINT8 *passWord)
{
    UINT8 retry = 0;	

    retry=0;
    do
    {
        retry++;
        if( retry > JOIN_RETRY_TIMES )
        {
            return false;
        }
    }while( simInStance->sendCmdAndWaitForResp((char*)"AT+CGCLASS=\"B\"\r\n",(char*)"OK\r\n",DEFAULT_TIMEOUT*3) );//!< class b设置移动台类别。"B"的意思是模块支持包交换和电路交换模式，但不能同时支持！

    retry=0;
    do
    {
        retry++;
        if( retry > JOIN_RETRY_TIMES )
        {
            return false;
        }
    }while( simInStance->sendCmdAndWaitForResp((char*)"AT+CGDCONT=1,\"IP\",\"CMNET\"\r\n",(char*)"OK\r\n",DEFAULT_TIMEOUT*3) );//!<设置接入点
			
		retry=0;
    do
    {
        retry++;
        if( retry > JOIN_RETRY_TIMES )
        {
            return false;
        }
    }while(simInStance->sendCmdAndWaitForResp((char*)"AT+CSQ\r\n",(char*)"OK\r\n",DEFAULT_TIMEOUT*3) );

    retry=0;
    do
    {
        retry++;
        if( retry > JOIN_RETRY_TIMES )
        {
            return false;
        }
    }while(simInStance->sendCmdAndWaitForResp((char*)"AT+CGATT=1\r\n",(char*)"OK\r\n",DEFAULT_TIMEOUT*3) );//!<附着于GPRS业务
		

    retry=0;
    do
    {
        retry++;
        if( retry > JOIN_RETRY_TIMES )
        {
            return false;
        }
    }while(simInStance->sendCmdAndWaitForResp((char*)"AT+CIPCSGP=1\r\n",(char*)"OK\r\n",DEFAULT_TIMEOUT*3) );//!< cennet CSD ,GPRS

    retry=0;
    do
    {
        retry++;
        if( retry > JOIN_RETRY_TIMES )
        {
            return false;
        }
    }while(simInStance->sendCmdAndWaitForResp((char*)"AT+CIPHEAD=1\r\n",(char*)"OK\r\n",DEFAULT_TIMEOUT*3));//!< msg , head  
#ifdef TCP
    retry=0;
    do
    {
        retry++;
        if( retry > JOIN_RETRY_TIMES )
        {
            return false;
        }
    }while(simInStance->sendCmdAndWaitForResp((char*)"AT+CLPORT=\"TCP\",\"3000\"\r\n",(char*)"OK\r\n",DEFAULT_TIMEOUT*3));//!< locate port
	
	  retry=0;
    do
    {
        retry++;
        if( retry > JOIN_RETRY_TIMES )
        {
            return false;
        }
    }while(simInStance->sendCmdAndWaitForResp((char*)"AT+CIPMODE=1\r\n",(char*)"OK\r\n",DEFAULT_TIMEOUT*3));//!< transform MODE 1为透明模式 

		retry=0;
    do
    {
        retry++;
        if( retry > JOIN_RETRY_TIMES )
        {
            return false;
        }
    }while( simInStance->sendCmdAndWaitForResp((char*)"AT+CIPCCFG=3,2,266,1\r\n",(char*)"OK\r\n",DEFAULT_TIMEOUT*3) );//!<配置透明传输模式  
	
	
#elif defined UDP
	  retry=0;
    do
    {
        retry++;
        if( retry > JOIN_RETRY_TIMES )
        {
            return false;
        }
    }while(simInStance->sendCmdAndWaitForResp((char*)"AT+CLPORT=\"UDP\",\"3000\"\r\n",(char*)"OK\r\n",DEFAULT_TIMEOUT*3));//!< locate port
	
	  #if 0
	  retry=0;
    do
    {
        retry++;
        if( retry > JOIN_RETRY_TIMES )
        {
            return false;
        }
    }while(simInStance->sendCmdAndWaitForResp((char*)"AT+CIPSPRT=2\r\n",(char*)"OK\r\n",DEFAULT_TIMEOUT*3));//!< 当发送数据时不打印"<"以及"SEND OK"
     #else
     retry=0;
     do
     {
         retry++;
         if( retry > JOIN_RETRY_TIMES )
         {
             return false;
         }
     }while(simInStance->sendCmdAndWaitForResp((char*)"AT+CIPMODE=1\r\n",(char*)"OK\r\n",DEFAULT_TIMEOUT*3));//!< transform MODE 1为透明模式
     #endif		 

#endif
    return true;
}

INT32 GPRS::QuitTransparentTrans()
{
    UINT8 retry = 0;
    do
    {
        retry++;
        if( retry > JOIN_RETRY_TIMES )
        {
						#ifdef SIM900A_DEBUG_ON	
            ERROR("QUIT");
						#endif
            return 1;
        }
    }while( simInStance->sendCmdAndWaitForResp((char*)"+++",(char*)"OK",DEFAULT_TIMEOUT*3) );//!< 
    
    return 0;    
}

UINT32 GPRS::str_to_ip( UINT8* str)
{
    UINT32 ip = 0;
    char* p = (char*)str;
    for(int i = 0; i < 4; i++) 
    {
        ip |= atoi((const char*)p);
        p = strchr((char*)p, '.');
        if (p == NULL) 
        {
            break;
        }
        ip <<= 8;
        p++;
    }
    return ip;
}

//HACERR lo de la IP gasta muuuucho espacio (ver .h y todo esto)
INT8 * GPRS::getIPAddress()
{
	  #if 0
 //Get local IP address
    simInStance->sendCmd((UINT8*)"AT+CIFSR\r\n");
    simInStance->cleanBuffer(ipAddr,32);
    simInStance->readBuffer(ipAddr,32,2);
#if 1
		#ifdef SIM900A_DEBUG_ON	
    DEBUG((UINT8*)"ipAddr: ");
		#endif
    DEBUG(ipAddr);
#endif

    if(NULL != strstr((char*)ipAddr,"AT+CIFSR")) 
    {
        _ip = str_to_ip(ipAddr+12);
        if(_ip != 0)
        {
            return true;
        }
    }
    #endif 
    snprintf((char*)ip_string, sizeof(ip_string), "%d.%d.%d.%d", (_ip>>24)&0xff,(_ip>>16)&0xff,(_ip>>8)&0xff,_ip&0xff);
    return ip_string;
}

INT32 GPRS::checkSIMStatus(void)
{
    UINT8 gprsBuffer[32];
    int count = 0;
    simInStance->cleanBuffer(gprsBuffer,32);
    while(count < 3) 
    {
        simInStance->sendCmd((UINT8*)"AT+CPIN?\r\n");
        simInStance->readBuffer(gprsBuffer,32,DEFAULT_TIMEOUT);
        if((NULL != strstr((char*)gprsBuffer,"+CPIN: READY"))) 
        {
            break;
        }
        count++;
        OSTimeDly(300);
    }
    if(count == 3) 
    {
        return -1;
    }
    return 0;
}

INT32 GPRS::networkCheck(void)
{
    OSTimeDly(1000);
    if(0 != simInStance->sendCmdAndWaitForResp("AT+CGREG?\r\n","+CGREG: 0,1",DEFAULT_TIMEOUT*3))
		{
        #ifdef SIM900A_DEBUG_ON		
				ERROR("ERROR:CGREG");
				#endif
        return -1;
    }
    OSTimeDly(1000);
    if(0 != simInStance->sendCmdAndWaitForResp("AT+CGATT?\r\n","+CGATT: 1",DEFAULT_TIMEOUT)) 
    {
				#ifdef SIM900A_DEBUG_ON		
        ERROR("ERROR:CGATT");
				#endif
        return -1;
    }
    return 0;
}

#ifdef TCP
INT32 GPRS::connectTcp()
{

    char cipstart[50];
    char _ip_str[25];

    UINT8 retry = 0;
    do
    {
        retry++;
        if( retry > JOIN_RETRY_TIMES )
        {
						#ifdef SIM900A_DEBUG_ON	
            ERROR("ERROR:CIPSTART");
						#endif
						OSTimeDly(1);
            return 1;
        }
						/* update ip and port */
						read_onchip_flash(0x0803f808,(u8 *)&JCLOUD_IP,sizeof(UINT32));	
						read_onchip_flash(0x0803f80c,(u8 *)&JCLOUD_PORT,sizeof(UINT32));
				
						sprintf(_ip_str,"%d.%d.%d.%d",JCLOUD_IP&0xff,JCLOUD_IP>>8&0xff,JCLOUD_IP>>16&0xff,JCLOUD_IP>>24&0xff);		
				
						sprintf(cipstart, "AT+CIPSTART=\"TCP\",\"%s\",\"%d\"\r\n", _ip_str, JCLOUD_PORT);
				
    }while(  simInStance->sendCmdAndWaitForResp(cipstart, "OK\r\n\r\nCONNECT", DEFAULT_TIMEOUT*3) ) ;
    
    return 0;
}



INT32 GPRS::sendTcpData(UINT8 *data,UINT32 len)
{
    #if 0
    char cmd[32];	 	
    snprintf(cmd,sizeof(cmd),"AT+CIPSEND=%d\r\n",len);	
    if(0 != simInStance->sendCmdAndWaitForResp(cmd,">",2*DEFAULT_TIMEOUT))
    {
				#ifdef SIM900A_DEBUG_ON	
        ERROR("ERROR:CIPSEND");
				#endif
        return -1;
    }
    if(0 != simInStance->sendCmdAndWaitForResp((char*)data,"SEND OK",2*DEFAULT_TIMEOUT))
    {
				#ifdef SIM900A_DEBUG_ON	
        ERROR("ERROR:SendTCPData");
				#endif
        return -1;
    }
    return 0;
	 
    #else

	  #if 0
	  static int cnt=0;
	  int i;
		#ifdef SIM900A_DEBUG_ON	
	  printf("-----%d\r\n",cnt);	
		#endif
	  cnt++;
	  for(i=0; i<len; i++)
	  {
				#ifdef SIM900A_DEBUG_ON	
				printf(" 0x%x", data[i]);
				#endif
		  if((i&0x1F) == 0x1F)
		  {
				#ifdef SIM900A_DEBUG_ON	
			  printf("\r\n");
				#endif
		  }
	  }	
			#ifdef SIM900A_DEBUG_ON	
			printf("\r\n");	
			#endif
	  #endif
	
    simInStance->SendFlatData( data, &len);
    return 0;
    #endif	

}


INT32 GPRS::closeTcp(void)
{
    #if 0
    simInStance->sendCmd((UINT8*)"AT+CIPCLOSE\r\n");
    return 0;
    #else
    return 1;
    #endif 
}

INT32 GPRS::shutTcp(void)
{
    UINT8 retry = 0;
    do
    {
        retry++;
        if( retry > JOIN_RETRY_TIMES )
        {
						#ifdef SIM900A_DEBUG_ON	
            ERROR("ERROR:AT+CIPSHUT");
						#endif
            return 1;
        }
    }while( simInStance->sendCmdAndWaitForResp((char*)"AT+CIPSHUT\r\n",(char*)"SHUT OK",DEFAULT_TIMEOUT*3) );//!< 
    
    return 0;
}


INT32 GPRS::TcpPrepare(INT8 RstFlag) 
{
		#ifdef SIM900A_DEBUG_ON	
		PRINT("Try-PowerUp-the-SIM900A-GPRS-Business\r\n");  
    #endif

		/* config SIM900A(GPRS) */
		if( init(115200, RstFlag) )
    {
				#ifdef SIM900A_DEBUG_ON		
        DEBUG("SIM900A  ERROR\r\n");
				#endif
        return 1;
    }
    else
    {
        if( !join((UINT8*)"CMNET") )
        {
						#ifdef SIM900A_DEBUG_ON		
            DEBUG("JOIN ERROR\r\n");
						#endif
            return 1;
        }
        else
        {
						#ifdef SIM900A_DEBUG_ON	
            DEBUG("commader							[OK]\r\n");	
						#endif
					  if( connectTcp()) //
					  //if( connectTcp("121.69.39.114",9120) ) //  test server
            //if( connectTcp("www.u-cloud.cn",9119) ) // "124.202.183.106",9120  "101.200.213.143",4000 "120.24.6.72",2222
            {
								#ifdef SIM900A_DEBUG_ON	
                DEBUG("ERROR CONNECT THE SERVER						[fault]\r\n");
								#endif
                return 1;
            }
            else
            {
								SystemStatus = SYS_CONNECT_SERVER;
								#ifdef SIM900A_DEBUG_ON	
                DEBUG("SUCC TO CONNECT THE SERVER					[OK]\r\n");
								#endif
								//OSSemPend(semMutex,0,&g_u8Rerr);
							
								resetFlag = 0;
						
							
            }
        
        }
    }


    return 0;
}

#else

INT32 GPRS::connectUdp(const char *ip, INT32 port)
{

    char cipstart[50];
    sprintf(cipstart, "AT+CIPSTART=\"UDP\",\"%s\",\"%d\"\r\n", ip, port);

    UINT8 retry = 0;
    do
    {
        retry++;
        if( retry > JOIN_RETRY_TIMES )
        {
						#ifdef SIM900A_DEBUG_ON	
            ERROR("ERROR:CIPSTART");
						#endif
            return 1;
        }
    }while(  simInStance->sendCmdAndWaitForResp(cipstart, "OK\r\n\r\nCONNECT", DEFAULT_TIMEOUT*3) ) ;
    
    return 0;
}

INT32 GPRS::sendUdpData(UINT8 *data,UINT32 len)
{
#define MIN_CMD_DATA_INTERVAL_MS 200 //verificated by experiment, 100ms is not enough
#define MIN_MSG_INTERVAL_MS 800
	static char cmd[32];
  sprintf(cmd,"AT+CIPSEND=%d\r\n",len);	
	
	#if 0
	static int cnt=0;
	int i;
		#ifdef SIM900A_DEBUG_ON	
		DEBUG("-----%d\r\n",cnt);
		#endif
	cnt++;
	for(i=0; i<len; i++)
	{
		#ifdef SIM900A_DEBUG_ON	
		DEBUG(" 0x%x", data[i]);
		#endif
		if((i&0x1F) == 0x1F)
		{
			#ifdef SIM900A_DEBUG_ON	
			DEBUG("\r\n");
			#endif
		}
	}	
		#ifdef SIM900A_DEBUG_ON	
		DEBUG("\r\n");
		#endif
	#endif	

	#if 0
	simInStance->sendCmd((UINT8*)cmd);
	OSTimeDly(MIN_CMD_DATA_INTERVAL_MS);//这个延时是确保命令与数据不粘在一起。
    simInStance->SendFlatData( data, &len);
	OSTimeDly(MIN_MSG_INTERVAL_MS);//这个延时保证数据已经发出，可能需调整	
	#else
	simInStance->SendFlatData( data, &len);
	#endif
  return 0;
}



INT32 GPRS::UdpPrepare(INT8 RstFlag) 
{
	  char IP[16];
		#ifdef SIM900A_DEBUG_ON	
		DEBUG("Try PowerUp the SIM900A GPRS Business\r\n");
		#endif
	  memset(IP, 0, sizeof(IP));
    
    if( init(115200, RstFlag) )
    {
				#ifdef SIM900A_DEBUG_ON	
        DEBUG("SIM900A  ERROR\r\n");
				#endif
        return 1;
    }
    else
    {
        if( !join((UINT8*)"CMNET") )
        {
						#ifdef SIM900A_DEBUG_ON	
            DEBUG("JOIN ERROR\r\n");
						#endif
            return 1;
        }
        else
        {
						#ifdef SIM900A_DEBUG_ON	
            DEBUG("JOIN OK\r\n");
						#endif
					  snprintf(IP,16,"%d.%d.%d.%d",JCLOUD_IP>>24,(JCLOUD_IP>>16)&0xFF,(JCLOUD_IP>>8)&0xFF, JCLOUD_IP&0xFF);
					
            //if( connectUdp("120.24.6.72",8000) ) // "124.202.183.106",9120  "101.200.213.143",4000 "120.24.6.72",2222
					  if( connectUdp(IP,JCLOUD_PORT) )
            {
								#ifdef SIM900A_DEBUG_ON	
                DEBUG("ERROR CONNECT THE SERVER\r\n");
								#endif
                return 1;
            }
            else
            {
								#ifdef SIM900A_DEBUG_ON	
                DEBUG("CONNECT THE SERVER\r\n");
								#endif
            }
        
        }
    }

    return 0;

}
#endif 
void GPRS::RcvData(UINT8* data,INT32 len)
{
	simInStance->readBuffer(data,len);
}

INT32 GPRS::GetDataLen()
{
	return simInStance->getSim()->available();
}
