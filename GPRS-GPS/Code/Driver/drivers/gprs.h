/*
 * gprs.h

 */

#ifndef __GPRS_H__
#define __GPRS_H__

#define TCP
//#define UDP

#include "sim900a.h"

#define JOIN_RETRY_TIMES  (5)

#ifdef TCP
#define GprsPrepare TcpPrepare
#define sendData      sendTcpData

#elif defined UDP
#define GprsPrepare UdpPrepare
#define sendData      sendUdpData
#endif


/** GPRS class.
 *  used to realize GPRS communication
 */
#define MESSAGE_LENGTH  20

class GPRS
{
private:
    GPRS();
    ~GPRS();

public:
    /** Create GPRS instance
     */     
    static GPRS * getInstance();
   

    /** initialize GPRS module including SIM card check & signal strength
     *  @returns
     *      0 on success
     *      -1 on error
     */
    INT32 init( INT32 baudRate, INT8 RstFlag);

    /** Join APN
     *  @return
     *        true Jion APN successfully
     *        false failed to join VPN
     */
    bool join( UINT8  *apn = 0,  UINT8 *userName = 0,  UINT8 *passWord = 0);

    /** 
     *  @returns
     *      0 on success
     *      -1 on error
     */
    INT32 QuitTransparentTrans();

    
    /** parse IP string
     *  @return
     *      ip in hex
     */
    UINT32 str_to_ip(UINT8* str);

    /** get Ip address
     *  @return
     *       IP string
     */
    INT8* getIPAddress();

    /** check SIM card' Status
     *  @returns
     *      0 on success
     *      -1 on error
     */
    INT32 checkSIMStatus(void);

    /** check network is OK or not
     *  @returns
     *      0 on success
     *      -1 on error
     */
    INT32 networkCheck(void);

    #ifdef TCP
    /** build TCP connect
     *  @param  ip  ip address which will connect to
     *  @param  port    TCP server' port number
     *  @returns
     *      0 on success
     *      -1 on error
     */
    INT32 connectTcp(const char* ip, INT32 port); 

     /** send data to TCP server
     *  @param  data    data that will be send to TCP server
     *  @returns
     *      0 on success
     *      -1 on error
     */
    INT32 sendTcpData(UINT8* data,UINT32 len);
		
		 /** close TCP connection
     *  @returns
     *      0 on success
     *      -1 on error
     */
    INT32 closeTcp(void);

    /** close TCP service
     *  @returns
     *      0 on success
     *      -1 on error
     */
    INT32 shutTcp(void);  
		
		INT32 TcpPrepare(INT8 RstFlag,UINT32 JCLOUD_IP,UINT32 JCLOUD_PORT);
	
		#else
	
	  INT32 connectUdp(const char* ip, INT32 port);	
	  INT32 sendUdpData(UINT8 *data,UINT32 len);
		INT32 UdpPrepare(INT8 RstFlag);
		#endif
    
    /** Reice data to TCP server
     *  @param  data    data that will be send to TCP server
     *  @returns
     *      0 on success
     *      -1 on error
     */

	  void RcvData(UINT8* data,INT32 len);
		
		INT32 GetDataLen();

private:

    static GPRS    * inStance;
    SIM900A * simInStance;   
    UINT32 _ip;
    INT8 ip_string[20];    
};
#endif
