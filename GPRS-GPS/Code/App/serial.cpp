/*
 * test file £º test communication with baseflight gcs
 *
 */

#include "board.h"
#include "uartdriver.h"

#define  VERSION  231


// Multiwii Serial Protocol 0
#define MSP_VERSION              4
#define CAP_PLATFORM_32BIT          ((uint32_t)1 << 31)
#define CAP_BASEFLIGHT_CONFIG       ((uint32_t)1 << 30)
#define CAP_DYNBALANCE              ((uint32_t)1 << 2)
#define CAP_FW_FLAPS                ((uint32_t)1 << 3)

#define MSP_IDENT                100    //out message         multitype + multiwii version + protocol version + capability variable
#define MSP_STATUS               101    //out message         cycletime & errors_count & sensor present & box activation & current setting number
#define MSP_RAW_IMU              102    //out message         9 DOF
#define MSP_SERVO                103    //out message         8 servos
#define MSP_MOTOR                104    //out message         8 motors
#define MSP_RC                   105    //out message         8 rc chan and more
#define MSP_RAW_GPS              106    //out message         fix, numsat, lat, lon, alt, speed, ground course
#define MSP_COMP_GPS             107    //out message         distance home, direction home
#define MSP_ATTITUDE             108    //out message         2 angles 1 heading
#define MSP_ALTITUDE             109    //out message         altitude, variometer
#define MSP_ANALOG               110    //out message         vbat, powermetersum, rssi if available on RX
#define MSP_RC_TUNING            111    //out message         rc rate, rc expo, rollpitch rate, yaw rate, dyn throttle PID
#define MSP_PID                  112    //out message         P I D coeff (9 are used currently)
#define MSP_BOX                  113    //out message         BOX setup (number is dependant of your setup)
#define MSP_MISC                 114    //out message         powermeter trig
#define MSP_MOTOR_PINS           115    //out message         which pins are in use for motors & servos, for GUI
#define MSP_BOXNAMES             116    //out message         the aux switch names
#define MSP_PIDNAMES             117    //out message         the PID names
#define MSP_WP                   118    //out message         get a WP, WP# is in the payload, returns (WP#, lat, lon, alt, flags) WP#0-home, WP#16-poshold
#define MSP_BOXIDS               119    //out message         get the permanent IDs associated to BOXes
#define MSP_SERVO_CONF           120    //out message         Servo settings
#define MSP_NAV_STATUS           121    //out message         Returns navigation status
#define MSP_NAV_CONFIG           122    //out message         Returns navigation parameters
#define MSP_FW_CONFIG            123    //out message         Returns parameters specific to Flying Wing mode

#define MSP_SET_RAW_RC           200    //in message          8 rc chan
#define MSP_SET_RAW_GPS          201    //in message          fix, numsat, lat, lon, alt, speed
#define MSP_SET_PID              202    //in message          P I D coeff (9 are used currently)
#define MSP_SET_BOX              203    //in message          BOX setup (number is dependant of your setup)
#define MSP_SET_RC_TUNING        204    //in message          rc rate, rc expo, rollpitch rate, yaw rate, dyn throttle PID
#define MSP_ACC_CALIBRATION      205    //in message          no param
#define MSP_MAG_CALIBRATION      206    //in message          no param
#define MSP_SET_MISC             207    //in message          powermeter trig + 8 free for future use
#define MSP_RESET_CONF           208    //in message          no param
#define MSP_SET_WP               209    //in message          sets a given WP (WP#,lat, lon, alt, flags)
#define MSP_SELECT_SETTING       210    //in message          Select Setting Number (0-2)
#define MSP_SET_HEAD             211    //in message          define a new heading hold direction
#define MSP_SET_SERVO_CONF       212    //in message          Servo settings
#define MSP_SET_MOTOR            214    //in message          PropBalance function
#define MSP_SET_NAV_CONFIG       215    //in message          Sets nav config parameters - write to the eeprom
#define MSP_SET_FW_CONFIG        216    //in message          Sets parameters specific to Flying Wing mode

// #define MSP_BIND                 240    //in message          no param

#define MSP_EEPROM_WRITE         250    //in message          no param

#define MSP_DEBUGMSG             253    //out message         debug string buffer
#define MSP_DEBUG                254    //out message         debug1,debug2,debug3,debug4

// Additional commands that are not compatible with MultiWii
#define MSP_UID                  160    //out message         Unique device ID
#define MSP_ACC_TRIM             240    //out message         get acc angle trim values
#define MSP_SET_ACC_TRIM         239    //in message          set acc angle trim values
#define MSP_GPSSVINFO            164    //out message         get Signal Strength (only U-Blox)
#define MSP_GPSDEBUGINFO         166    //out message         get GPS debugging data (only U-Blox)
#define MSP_SERVOMIX_CONF        241    //out message         Returns servo mixer configuration
#define MSP_SET_SERVOMIX_CONF    242    //in message          Sets servo mixer configuration

// Additional private MSP for baseflight configurator
#define MSP_RCMAP                64     //out message         get channel map (also returns number of channels total)
#define MSP_SET_RCMAP            65     //in message          set rc map, numchannels to set comes from MSP_RCMAP
#define MSP_CONFIG               66     //out message         baseflight-specific settings that aren't covered elsewhere
#define MSP_SET_CONFIG           67     //in message          baseflight-specific settings save
#define MSP_REBOOT               68     //in message          reboot settings
#define MSP_BUILDINFO            69     //out message         build date as well as some space for future expansion

#define INBUF_SIZE 128

typedef struct box_t {
    const uint8_t boxIndex;         // this is from boxnames enum
    const char *boxName;            // GUI-readable box name
    const uint8_t permanentId;      // Permanent ID for reporting to GUI
} box_t;

//static const box_t boxes[] = {

//};

// this is calculated at startup based on enabled features.
// this is the number of filled indexes in above array
// from mixer.c
// cause reboot after MSP processing complete


typedef enum serialState_t {
    IDLE,
    HEADER_START,
    HEADER_M,
    HEADER_ARROW,
    HEADER_SIZE,
    HEADER_CMD,
} serialState_t;

typedef  struct mspPortState_t {
    uint8_t port;
    uint8_t checksum;
    uint8_t indRX;
    uint8_t inBuf[INBUF_SIZE];
    uint8_t cmdMSP;
    uint8_t offset;
    uint8_t dataSize;
    serialState_t c_state;
} mspPortState_t;

static mspPortState_t ports[2];
static mspPortState_t *currentPortState = &ports[0];

// static uint8_t checksum, indRX, inBuf[INBUF_SIZE];
// static uint8_t cmdMSP;

void serialize8(uint8_t a)
{
	
		/* can't use DMA, why? */
		//USART1_DMA(&a,1);
		UART1_send_byte(a);
    currentPortState->checksum ^= a;
}

void serialize32(uint32_t a)
{
    serialize8(a & 0xFF);
    serialize8((a >> 8) & 0xFF);
    serialize8((a >> 16) & 0xFF);
    serialize8((a >> 24) & 0xFF);
}

void serialize16(int16_t a)
{
    serialize8(a & 0xFF);
    serialize8((a >> 8) & 0xFF);
}

uint8_t read8(void)
{
    return currentPortState->inBuf[currentPortState->indRX++] & 0xff;
}

uint16_t read16(void)
{
    uint16_t t = read8();
    t += (uint16_t)read8() << 8;
    return t;
}

uint32_t read32(void)
{
    uint32_t t = read16();
    t += (uint32_t)read16() << 16;
    return t;
}

void headSerialResponse(uint8_t err, uint8_t s)
{
    serialize8('$');
    serialize8('M');
    serialize8(err ? '!' : '>');
    currentPortState->checksum = 0;               // start calculating a new checksum
    serialize8(s);
    serialize8(currentPortState->cmdMSP);
}

void headSerialReply(uint8_t s)
{
    headSerialResponse(0, s);
}

void headSerialError(uint8_t s)
{
    headSerialResponse(1, s);
}

void tailSerialReply(void)
{
    serialize8(currentPortState->checksum);
}

void s_struct(uint8_t *cb, uint8_t siz)
{
    headSerialReply(siz);
    while (siz--)
        serialize8(*cb++);
}

void serializeNames(const char *s)
{
    const char *c;
    for (c = s; *c; c++)
        serialize8(*c);
}

void serializeBoxNamesReply(void)
{

}

void serialInit(uint32_t baudrate)
{

    //ports[0].port  = uartOpen(USART1, NULL, baudrate, MODE_RXTX); 

}

static void evaluateCommand(void)
{
    uint32_t i,junk;
		static int pitch=0,roll=0;
#ifdef GPS
    uint8_t wp_no;
    int32_t lat = 0, lon = 0, alt = 0;
#endif
    const char *build = __DATE__;

    switch (currentPortState->cmdMSP) {
				case MSP_ACC_TRIM:
							headSerialReply(4);
							serialize16(0);
							serialize16(0);
							break;
			
			case MSP_CONFIG:
            headSerialReply(1 + 4 + 1 + 2 + 2 + 2 + 2 + 2 + 2 + 2 + 1 + 1 + 2 + 1);
            serialize8(0);
            serialize32(0);
            serialize8(0);
            serialize16(0);
            serialize16(0);
            serialize16(0);
            serialize16(0);
            serialize16(0);
            serialize16(0);
            serialize8(0);
            serialize8(0);
            serialize8(0);
            serialize8(0);
            serialize16(0);
            serialize8(0);
            /// ???
            break;
			
			 case MSP_MISC:
            headSerialReply(2 * 6 + 4 + 2 + 4);
            serialize16(0);
            serialize16(0);
            serialize16(0);
            serialize16(0);
            serialize16(0);
            serialize8(0);
            serialize8(0);
            serialize8(0);
            serialize8(0);
            serialize8(0);
            serialize8(0);
            serialize16(0); // TODO check this shit
            serialize8(0);
            serialize8(0);
            serialize8(0);
            serialize8(0);
            break;
			 
			 case MSP_ATTITUDE:
            headSerialReply(6);
            
            serialize16(pitch+=5);
						serialize16(roll+=5);
            serialize16(0);
						if(pitch>=2048) pitch =0;
						if(roll>=2048) roll =0;
            break;
			 
			 
			 
			 case MSP_ANALOG:  //110
            headSerialReply(7);
            serialize8(0);
            serialize16(0); // milliamphours drawn from battery
            serialize16(0);
            
            serialize16(0); // send amperage in 0.001 A steps
          
            break;
			
			 
			  case MSP_RAW_GPS:
            headSerialReply(16);
            serialize8(0);
            serialize8(0);
            serialize32(0);
            serialize32(0);
            serialize16(0);
            serialize16(0);
            serialize16(0);
            break;
        case MSP_COMP_GPS:
            headSerialReply(5);
            serialize16(0);
            serialize16(0);
            serialize8(0);
            break;
				
				 case MSP_STATUS:
            headSerialReply(11);
            serialize16(0);
            serialize16(4);
            serialize16(1 | 1 << 1 | 1 << 2 | 0 << 3 | 1 << 4);
            // OK, so you waste all the fucking time to have BOXNAMES and BOXINDEXES etc, and then you go ahead and serialize enabled shit simply by stuffing all
            // the bits in order, instead of setting the enabled bits based on BOXINDEX. WHERE IS THE FUCKING LOGIC IN THIS, FUCKWADS.
            // Serialize the boxes in the order we delivered them, until multiwii retards fix their shit
            junk = 0;
           
            serialize32(junk);
            serialize8(0);
            break;
				
				
			
			
        case MSP_IDENT:
            headSerialReply(7);
            serialize8(VERSION);                    // multiwii version
            serialize8(2);    // type of multicopter
            serialize8(MSP_VERSION);                // MultiWii Serial Protocol Version
            serialize32(CAP_PLATFORM_32BIT | CAP_BASEFLIGHT_CONFIG | CAP_DYNBALANCE | CAP_FW_FLAPS); // "capability"
            break;
      
        case MSP_UID:
            headSerialReply(12);
            serialize32(U_ID_0);
            serialize32(U_ID_1);
            serialize32(U_ID_2);
            break;

        case MSP_BUILDINFO:
            headSerialReply(11 + 4 + 4);
            for (i = 0; i < 11; i++)
                serialize8(build[i]); // MMM DD YYYY as ascii, MMM = Jan/Feb... etc
            serialize32(0); // future exp
            serialize32(0); // future exp
            break;

        default:                   // we do not know how to handle the (valid) message, indicate error MSP $M!
            headSerialError(0);
            break;
    }
    tailSerialReply();
}


void rccWriteBkpDr(uint32_t value)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
    PWR->CR |= PWR_CR_DBP;

    *((uint16_t *)BKP_BASE + 0x04) = value & 0xffff;
    *((uint16_t *)BKP_BASE + 0x08) = (value & 0xffff0000) >> 16;
}

#define AIRCR_VECTKEY_MASK    ((uint32_t)0x05FA0000)

#define BKP_SOFTRESET (0x50F7B007)

void systemReset(bool toBootloader)
{
    if (toBootloader) {
        // 1FFFF000 -> 20000200 -> SP
        // 1FFFF004 -> 1FFFF021 -> PC
        //*((uint32_t *)0x20004FF0) = 0xDEADBEEF; // 20KB STM32F103   0x20004FF0==19kb
			  *((uint32_t *)0x2000BFF0) = 0xDEADBEEF; // 48KB STM32F103VC 0x2000BFF0==47kb
    }

    // write magic value that we're doing a soft reset
    rccWriteBkpDr(BKP_SOFTRESET);

    // Generate system reset
    SCB->AIRCR = AIRCR_VECTKEY_MASK | (uint32_t)0x04;
}



// evaluate all other incoming serial data
static void evaluateOtherData(uint8_t sr)
{
    if (sr == '#')
        //cliProcess();
			;
    else if (sr == 'R')
        systemReset(true);      // reboot to bootloader
}

void serialCom(void)
{
    uint8_t c;
		int len=1;


        currentPortState = &ports[0];

         // in cli mode, all serial stuff goes to here. enter cli mode by sending #
       //
  

        while (SerialConsole->available()) {
					
            //c = serialRead(currentPortState->port);
						SerialConsole->read(&c,&len);
						
            if (currentPortState->c_state == IDLE) {
                currentPortState->c_state = (c == '$') ? HEADER_START : IDLE;
                if (currentPortState->c_state == IDLE )
                    evaluateOtherData(c); // if not armed evaluate all other incoming serial data
            } else if (currentPortState->c_state == HEADER_START) {
                currentPortState->c_state = (c == 'M') ? HEADER_M : IDLE;
            } else if (currentPortState->c_state == HEADER_M) {
                currentPortState->c_state = (c == '<') ? HEADER_ARROW : IDLE;
            } else if (currentPortState->c_state == HEADER_ARROW) {
                if (c > INBUF_SIZE) {       // now we are expecting the payload size
                    currentPortState->c_state = IDLE;
                    continue;
                }
                currentPortState->dataSize = c;
                currentPortState->offset = 0;
                currentPortState->checksum = 0;
                currentPortState->indRX = 0;
                currentPortState->checksum ^= c;
                currentPortState->c_state = HEADER_SIZE;      // the command is to follow
            } else if (currentPortState->c_state == HEADER_SIZE) {
                currentPortState->cmdMSP = c;
                currentPortState->checksum ^= c;
                currentPortState->c_state = HEADER_CMD;
            } else if (currentPortState->c_state == HEADER_CMD && currentPortState->offset < currentPortState->dataSize) {
                currentPortState->checksum ^= c;
                currentPortState->inBuf[currentPortState->offset++] = c;
            } else if (currentPortState->c_state == HEADER_CMD && currentPortState->offset >= currentPortState->dataSize) {
                if (currentPortState->checksum == c) {        // compare calculated and transferred checksum
                    evaluateCommand();      // we got a valid packet, evaluate it
                }
                currentPortState->c_state = IDLE;
            }
        }
    
}
