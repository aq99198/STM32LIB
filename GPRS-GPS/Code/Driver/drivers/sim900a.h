/*
 * sim900a.h
 *
 */

#ifndef __SIM900A_H__
#define __SIM900A_H__

#include "uartdriver.h"

#define SIM900A_SERIAL_PORT      (UART5_IDX)
#define SIM900A_SERILA_BAUDRATE  (115200)
#define DEFAULT_TIMEOUT     5
#define SIM900A_DEFAULT_POWER_UP_TIMES   (5)
#define SIM900A_DEFAULT_POWER_DOWN_TIMES (5)
#define SIM900A_DEFAULT_RESET_TIMES (5)

/** SIM900A class.
 *  Used for SIM900A communication. attention that SIM900A module communicate with MCU in serial protocol
 */
class SIM900A
{
private:

    SIM900A();
    ~SIM900A();

public:
     /** Create SIM900A Instance 
     */
    static SIM900A * getIntance();
    
    /** check the SIM900A system whether Power up , if the system is not PoweredUp, so ,Power up it,
        if the ,system could not power up ,it will try again,but if it tries more than SIM900A_DEFAULT_POWER_UP_TIMES
        times , so it will return false , the system can not Power up .
     */
    bool SysInit(INT8 RstFlag);
    
    /** read from SIM900A module and save to buffer array
     *  @param  buffer  buffer array to save what read from SIM800 module
     *  @param  count   the maximal bytes number read from SIM800 module
     *  @param  timeOut time to wait for reading from SIM800 module 
     *  @returns
     *      0 on success
     *      -1 on error
     */
    int readBuffer(UINT8* buffer,INT32 count, UINT32 timeOut);

    void readBuffer(UINT8* buffer,INT32 count);
    /** clean Buffer
     *  @param buffer   buffer to clean
     *  @param count    number of bytes to clean
     */
    void cleanBuffer(UINT8* buffer, INT32 count);
    
    /** send AT command to SIM900A module
     *  @param cmd  command array which will be send to GPRS module
     */
    void sendCmd(UINT8* cmd);
		
		void sendCmd(UINT8* cmd, INT32 len);

    /** send flat data to the SIM900A , the data could not be changed.
    */
    INT32 SendFlatData(UINT8 *data, UINT32 *len);

    /** read flat data to the SIM900A , the data could not be changed.
    */
    INT32 ReadFlatData(UINT8 *data, UINT32  & len,UINT32 timeout);

    /**send "AT" to SIM900A module
     */
    void sendATTest(void);
    
    /**send '0x1A' to SIM900A Module
     */
    void sendEndMark(void);
    
    /** check SIM900A module response before time out
     *  @param  *resp   correct response which SIM800 module will return
     *  @param  *timeout    waiting seconds till timeout
     *  @returns
     *      0 on success
     *      1 on error
     */ 
    int waitForResp(const UINT8* resp, UINT32 timeout);

    /** send AT command to GPRS module and wait for correct response
     *  @param  *cmd    AT command which will be send to GPRS module
     *  @param  *resp   correct response which GPRS module will return
     *  @param  *timeout    waiting seconds till timeout
     *  @returns
     *      0 on success
     *      1 on error
     */
    int sendCmdAndWaitForResp(const char* cmd, const char *resp, UINT32 timeout);


    /** used for serial debug, you can specify tx and rx pin and then communicate with GPRS module with common AT commands
     */
    void serialDebug(void);
		
		CUartDriver *getSim();		 

    void NReset(void); 
		
		
private:
    /** used for Power Up the SIM900A,the Power up Pin sequence
     */    
    void PowerUp(void);

 

    /** init the GPIO of the MCU ,  MCU can  control and check  the SIM900A module by the GPIO
     */    
    void InitSIM900AGpio(void);

private:
    static SIM900A * inStance;
    CUartDriver    * serialSim900a;
};

#endif
