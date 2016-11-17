/*************************************************************
* Copyright Jovian Advanced Control, Co.
*
*  FileName:    ProtocolDef.h
*  Abstract:    Define Communication Packet portocol for JacPilot
*  Reference:   《通信协议(V1.0)》
*  Version:      V1.0
*  Writer:       Ren Bin
*  Date:         2007.10.21
*---------------------------------------------------------------
* Modification history:
*  Date:
*  Version:
*  Description:
****************************************************************/
#ifndef __PROTOCOL_DEF_H__
#define __PROTOCOL_DEF_H__

#include "JacTypeDef.h"


#define MAX_PACKET_LENGTH 256

/************************************************************************/
/* Communication Message structure                                      */
/************************************************************************/

/************************************************************************/
/*define SYNHEAD                                                        */
/************************************************************************/
#define JACX_SYNHEAD0	    0xb5
#define JACX_SYNHEAD1	    0x62
#define JACX_MSG_ACK_REQ_NONE         0x00
#define JACX_MSG_ACK_REQ_RECVED       0x01
#define JACX_MSG_ACK_REQ_RESULT       0x02
#define JACX_MSG_ACK_RET_RECVED       0x05
#define JACX_MSG_ACK_RET_FAILED       0x06
#define JACX_MSG_ACK_RET_SUCCEED      0x07

#define CMD_RET_FAILED        -1
#define CMD_RET_NO_ACK        0
#define CMD_RET_SUCCEED       1
#define CMD_RET_SUCCEED_CONT  2
#define CMD_RET_CONT_FINISHED 3

/************************************************************************/
/*define destination and source ID                                                                   */
/************************************************************************/
enum enm_MSG_SOURCEID
{
    JACX_AUTOPILOT_ID = 0,
    JACX_GCS_ID,
    JACX_OI,
    NUM_MSG_SOURCEID
};


/************************************************************************/
/*define Message ID                                                                                                */
/************************************************************************/
//
enum enm_MSG_ID
{
    /*basic*/
    JACX_OPERATOR_CMD_PULSE = 3,
    JACX_MANUAL_ASSIST_MODE = 5,
    JACX_AP_MODE = 6,

    /*fight plan*/
    JACX_WAYPOINT = 8,
    JACX_WAYPOINT_LIST,
    JACX_WAYPOINT_QUICK,
    JACX_LANDING_PLAN,

    /*safe*/
    JACX_MISSION_LIMITS = 12,
    //add 20120913
    JACX_MOVING_LANDING_PLAN = 14,
    JACX_MOVING_PLATFORM_CFG = 15,

    /*guide and control*/
    JACX_TRACK = 16,
    JACX_AUTOPILOT_LOOP,
    JACX_HEADING_CONTROL,
    JACX_HEIGHT_CONTROL,
    //add 20130628
    JACX_SEMIAUTO_DATA,

    //add 20130709
    JACX_TAKEOFF_PLAN,

    JACX_CONTROLLER_DATA = 25,
    JACX_CONTROLLER_DATA_REQUEST,
    JACX_CONTROLLER_DATA_DEFAULT,
    JACX_ALTCONTROL_SOURCE,

    /*sensor*/
    JACX_SENSOR_ORIENTATION = 31,
    JACX_SENSOR_ERROR,
    JACX_ALTIMETER_SETTING,
    JACX_AIR_DATA_ZERO,

    JACX_MAG_DECLINATION,
    JACX_MAG_CALIBRATION,
    JACX_MAG_CALIBRATION_DATA,
    JACX_VIBRATE_ANALYSIS,

    /*surface*/
    JACX_SURFACE_TABLE_SIMPLE = 41,
    JACX_SURFACE_TABLE,
    JACX_SURFACE_TEST,
    JACX_SURFACE_PRE_TEST,
    JACX_RC_PWM_CAL,

    /*others*/
    JACX_SET_FUEL_LEVEL = 51,
    JACX_SYSTEM_VERSION,
    JACX_SYSTEM_RESET,
    JACX_TIMER_COUNT_DOWN,
    JACX_LOCK_PARA,
    JACX_HIL_SIM,
    //add 20130529
    JACX_EULER_BIAS,

    /*config*/
    JACX_CONFIG_FLSAH_DATA = 60,
    JACX_SAVE_CONFIG_FLSAH,
    JACX_DEFAULT_CONFIG_FLSAH,
    JACX_RO_CONFIG_FLSAH_DATA,
    JACX_SAVE_RO_CONFIG_FLSAH,
    JACX_DEFAULT_RO_CONFIG_FLSAH,

    /*doublet*/
    JACX_DOUBLET_CMD = 67,
    JACX_DOUBLET_AP_DATA,
    JACX_DOUBLET_SENSOR_DATA,

    /*telemetry*/
    JACX_TELEMETRY_CONFIG = 71,
    JACX_TELEMETRY_HI_RES,
    JACX_TELEMETRY_LO_RES,
    JACX_TELEMETRY_SYSSTATUS,
    JACX_TELEMETRY_PAYLOAD_DATA,
    JACX_TELEMETRY_DBG_CONT,
    JACX_TELEMETRY_RADIO,

    /*gcs*/
    JACX_GCS_RADIO_CONFIG = 81,
    JACX_GCS_GPS_READING,
    JACX_GCS_SYSTEM_STATUS,
    JACX_GCS_PWM_ENABLE,

    /*flight command*/
    JACX_PRELAUNCH  = 86,
    JACX_LAUNCH_NOW,
    JACX_LAND_NOW,
    JACX_ABORT,

    /*record*/
    JACX_FLY_RECORD = 91,
    JACX_RECORD_FREQMODE,

    /*payload*/
    JACX_ENGINE_KILL = 95,
    JACX_PARACHUTE,
    JACX_DROP,
    JACX_AIRBAG,
    JACX_TURNON_LIGHTS,
    JACX_GIMAL_CMD,
    JACX_GIMBAL_STARE,
    JACX_RC_SWITCH,
    JACX_AIRBAG_DATA,
    JACX_PAYLODD_USER,

    /*airphoto*/
    JACX_AIRPHOTO_CFG_DATA = 110,
    JACX_AIRPHOTO_ACT,
    JACX_AIRPHOTO_NUM,
    JACX_AIRPHOTO_DOWNLOAD,
    JACX_EXT_PHOTO,
    JACX_PHOTO_PARA,


    /*radio config*/
    JACX_RADIO_CONFIG = 120,


    /*navigation*/
    JACX_DGPS_OEMV = 150,
    JACX_OP_GUIDE,
    JACX_ORIGIN_POINT,
    JACX_KALMAN_GAINS,
    JACX_KALMAN_GAINS_REQUEST,

    /*helicoptor(reserve)*/
    JACX_READ_HELI_TRIM = 160,
    JACX_THROT_CURVE,
    JACX_RC_CHANNEL_RANG,
    JACX_RC_SWASH_CENTER,
    JACX_SINGLE_CONTROL_DATA,
    JACX_SWASH_AFR,
    JACX_HELI_SURFACE_TEST,
    JACX_AUTHENTICATION_DATA,

    JACX_SUPER_AUTHENTICATION,
    JACX_SUPER_AUTHENTICATION_REQUEST,

    JACX_CAMERA_EXT = 171,

    JACX_AIRCRAFT_NO        = 179,
    JACX_MOVING_LANDING_PARA = 180,
    JACX_HELI_GET_SET_TRIM = 190,
    JACX_HELI_GET_SET_INNER_LOOP_B,
    JACX_HELI_SET_TELE_RCPWM,

		JACX_USER_DEF = 0xf0, /*!< 0xfe */

		/* jinjiabin def message  */
		JACX_JIN_LOW_MSG = 210,
    
    NUM_MSG_ID
};

/* Message's length*/

//FMS command

#define JACX_WAYPOINT_LEN	                20
#define JACX_WAYPOINT_LIST_LEN	            16
#define JACX_TRACK_LEN	                    4
#define JACX_MISSION_LIMITS_LEN	            32

//Autopilot command
#define JACX_OPERATOR_CMD_ANGLE_LEN	        10
#define JACX_OPERATOR_CMD_PULSE_LEN	        12
#define JACX_MANUAL_ASSIST_MODE_LEN	        2
#define JACX_ENGINE_KILL_LEN	            2
#define JACX_CONTROLLER_DATA_LEN	        4
#define JACX_CONTROLLER_DATA_REQUEST_LEN	6
#define JACX_CONTROLLER_DATA_DEFAULT_LEN	6
#define JACX_AUTOPILOT_LOOP_LEN	            8

#define JACX_INNER_CONTROL_LEN	            0
#define JACX_HEADING_CONTROL_LEN            2
#define JACX_HEIGHT_CONTROL_LEN	            2

//configuration command
#define JACX_SENSOR_ORIENTATION_LEN	        24

#define JACX_MAG_DECLINATION_LEN	        8
#define JACX_MAG_CALIBRATION_LEN	        2
#define JACX_MAG_CALIBRATION_DATA_LEN	    48

#define JACX_SURFACE_TABLE_LEN	            44
#define JACX_SURFACE_TABLE_SIMPLE_LEN	    8

#define JACX_SYSTEM_VERSION_LEN	            36

#define JACX_TELEMETRY_CONFIG_LEN           4
#define JACX_TELEMETRY_SYSSTATUS_LEN	    72


#define JACX_DOUBLET_CMD_LEN                 8
#define JACX_DOUBLET_AP_DATA_LEN             106
#define JACX_DOUBLET_SENSOR_DATA_LEN         86


#define JACX_HEIGHT_PDI_DATA_LEN         12
#define JACX_SWASH_AFR_SENSE_DATA_LEN    16
#define JACX_THROT_CURVE_LEN             10
#define JACX_RC_CHANNEL_RANG_LEN         8

#define JACX_AIRPHOTO_DATA_LEN           20

#define JACX_AIRPHOTO_DOWNLOAD_LEN       36
#define JACX_AIRPHOTO_NUM_LEN            4


#define JACX_RC_PWM_LEN                  16

#define JACX_SINGLE_DATA_LEN             8



/************************************************************************/
/* Packet structure definition                                          */
/************************************************************************/
#pragma pack(1)
typedef struct
{
    UINT8 sync0 ;
    UINT8 sync1 ;
    UINT8 dest ;
    UINT8 source ;
    UINT8 msgID ;
    UINT8 SeqNum;
    UINT8 ACK_NAK;
    UINT8 len;
} JACX_MSG_HEAD;

typedef struct
{
    JACX_MSG_HEAD msgHead;
    UINT8 Data[MAX_PACKET_LENGTH]; //data exclude 2 crc
} JACX_MSG_PACKET;

typedef struct
{
    UINT8	sync0;
    UINT8	sync1;
    UINT8	cls;
    UINT8	id;
    UINT8   len;
    UINT8   res;
    UINT8   mag[6];
    UINT8   CK_A;
    UINT8   CK_B;
} JACX_ExtMag_PACKET;


/////////////////////////////////////////////////////////////////////////
//*AUTOPILOT Message
/////////////////////////////////////////////////////////////////////////
typedef struct
{
    UINT8 ControllerID;
    UINT8 ControllerVer;
    UINT8 Category;
    UINT8 DataID;
    UINT8 NumData;
    UINT8 Res;
} CONTROLLER_DATA_DEF;

typedef struct
{
    UINT8 Loop;
    UINT8 Control;
    UINT16 Res;
    FLOAT32 Value;
} CONTROLLER_LOOP_CMD;

/************************************************************************/
/* Telemetry Message                                                    */
/************************************************************************/
#define TELE_MSG_DATA_FLAG_BIT_GPS      0x1
#define TELE_MSG_DATA_FLAG_BIT_SENSOR   0x2
#define TELE_MSG_DATA_FLAG_BIT_RAW      0x4
#define TELE_MSG_DATA_FLAG_BIT_MAG      0x8
#define TELE_MSG_DATA_FLAG_BIT_AGL      0x10
#define TELE_MSG_DATA_FLAG_BIT_FUEL     0x20
#define TELE_MSG_DATA_FLAG_BIT_ACT      0x40
#define TELE_MSG_DATA_FLAG_BIT_OTHER    0x80

/************************************************************************/
/* go around reason                                                     */
/************************************************************************/

#define GOAROUND_YTRACK_BIT       0x1
#define GOAROUND_ALT_BIT          0x2
#define GOAROUND_SPEED_BIT        0x4
#define GOAROUND_LOW_ALT_BIT      0x8
#define GOAROUND_HIGH_ALT_BIT     0x10

/************************************************************************/
/* authority reason                                                     */
/************************************************************************/
#define FULL_PERMIT_BIT            0x00
#define DATE_AUTH_PERMIT_BIT       0x01
#define WP_AUTH_PERMIT_BIT         0x02
#define LOCATION_AUTH_PERMIT_BIT   0x04

#define DATE_AUTH_PERMIT_EXPIRED_BIT  0x08   //许可到期
#define LOCATION_AUTH_BREAK_BIT       0x10   //地理越界


typedef struct
{
    UINT8 dataFlags;
    UINT8 NumAct;
    UINT16 Limits;
    UINT32 sysTime;
} MSG_TELEMETRY_HEAD;

typedef struct
{
    INT32 Lat; //1e7 rad
    INT32 Lon; //1e7 rad
    INT32 Height; //cm
    INT16 Vnorth; //cm/s
    INT16 Veast;  //cm/s
    INT16 Vdown;  //cm/s
    UINT8 gpsFix;
    UINT8 flags;
    UINT8 pDOP; //5X position DOP
    UINT8 numSV;
    UINT16 GPSWeek;
    UINT32 GPSTow;
} MSG_TELEMETRY_GPS_SUB;

typedef struct
{
    INT16	ROLL ;//1e4 rad
    INT16	PITCH ;//1e4 rad
    UINT16	YAW ; //1e4 rad
    INT16	AltvGPSAlt ; //altitude above gps altitude in cm (wangchen modified) change to decimeter
    INT16   TAS; //cm/s
    INT16	WSouth ; //cm/s
    INT16	WWest ;// cm/s
    UINT16	LeftRPM ;
    UINT16	RightRPM ;
    UINT8   DensityRatio; //1/200, 1.225 is 200
    char    OAT; //deg
} MSG_TELEMETRY_SENSOR_SUB;

typedef struct
{
    INT16	Xaccel ; //500 m/s/s
    INT16	Yaccel ; //500 m/s/s
    INT16	Zaccel ; //500 m/s/s
    INT16	RollRate ; //5e3 rad/s
    INT16	PitchRate; //5e3 rad/s
    INT16	YawRate ;// 5e3 rad/s
    UINT16	StaticP; //0.5 pa
    UINT16  DynPressure; //10 Pa
} MSG_TELEMETRY_RAW_SUB;

typedef struct
{
    INT16   XMagFiled;
    INT16   YMagFiled;
    INT16   ZMagFiled;
    UINT16  Compass;
} MSG_TELEMETRY_MAG_SUB;

typedef struct
{
    INT16 FuelFlow;
    INT16 Fuel;
} MSG_TELEMETRY_FUEL_SUB;

/************************************************************************/
/* Low res telemetry packet structure                                   */
/************************************************************************/

typedef struct
{
    INT8	ROLL ;//deg
    INT8	PITCH ;//deg
    UINT8	YAW ; //deg
    INT8	AltvGPSAlt ; //0.5m altitude above gps altitude in cm
    INT8    TAS; //m/s
    INT8	WSouth ; //m/s
    INT8	WWest ;// m/s
    UINT8	LeftRPM ;
    UINT8	RightRPM ;
    UINT8   DensityRatio; //1/200, 1.225 is 200
    INT8    OAT; //deg
} MSG_TELEMETRY_LO_SENSOR_SUB;

typedef struct
{
    INT8	Xaccel ; //500 m/s/s
    INT8	Yaccel ; //500 m/s/s
    INT8	Zaccel ; //500 m/s/s
    INT8	RollRate ; //5e3 rad/s
    INT8	PitchRate; //5e3 rad/s
    INT8	YawRate ;// 5e3 rad/s
    UINT16	StaticP; //0.5 pa
    UINT16  DynPressure; //10 Pa
} MSG_TELEMETRY_LO_RAW_SUB;

typedef struct
{
    INT8   XMagFiled; //0.01 gauss
    INT8   YMagFiled;//0.01 gauss
    INT8   ZMagFiled;//0.01 gauss
    UINT8  Compass;//deg
} MSG_TELEMETRY_LO_MAG_SUB;

/************************************************************************/
/* System_status message                                                */
/************************************************************************/
typedef struct
{
    UINT8 MPowerALo;
    UINT8 MPowerAHi: 4;
    UINT8 MPowerVLo: 4;
    UINT8 MPowerVHi;
    UINT8 SPowerALo;
    UINT8 SPowerAHi: 4;
    UINT8 SPowerVLo: 4;
    UINT8 SPowerVHi;
    UINT8 InternalV;
    INT8 BoardT;
    INT8 RSSI;
    UINT8 VSWR;

    //	UINT8 errSensor;
    UINT8 dsExIMU: 1;
    UINT8 dsExGPS: 1;
    UINT8 dsExAir: 1;
    UINT8 dsExMag: 1;
    UINT8 dsRes: 4;
    /*
    	UINT8 errGrosX : 1;	//!< Set if grosX is bad
    	UINT8 errGrosY : 1;	//!< Set if grosY is bad
    	UINT8 errGrosZ : 1;	//!< Set if grosZ is bad
    	UINT8 errAcclX : 1;	//!< Set if acclX is bad
    	UINT8 errAcclY : 1;	//!< Set if acclY is bad
    	UINT8 errAcclZ : 1;	//!< Set if acclZ is bad
    	UINT8 errStaticP:1;    //!< Set if StaticP is bad
    	UINT8 errDynamicP:1;
    */
    UINT8 Res1;
    UINT16 NavHealth;
    INT16 HorizStdDev;
    UINT16 VertStdDev;
    INT8 RollBias;
    INT8 PitchBias;
    INT8 YawBias;
    INT8 XaccBias;
    INT8 YaccBias;
    INT8 ZaccBias;
#if 0
    INT8 XmagBias;
    INT8 YmagBias;
    INT8 ZmagBias;
#else
    INT16 PhotoNum;
    INT8 altControlSource;
#endif
    INT8 Res2;
    UINT8 GlobalStatus;
    UINT8 Failure;
    UINT16 Action;
    UINT16 Tracker;
    UINT16 TrackerStatus;
    UINT8 OrbitRadius;
    UINT8 NumLoops;
    UINT16 LoopStatus;
    FLOAT32 lpTarget[8];
    UINT16  P_Sol_Status;
    UINT16  Pos_Type;
    UINT16  P_X_Sigma;
    UINT16  P_Y_Sigma;
    UINT16  P_Z_Sigma;
    UINT16  V_Z_Sigma;
    //	UINT   Ytracker;
    UINT8   Ext_Sol_Stat;
    UINT8   Sig_Mask;
    UINT16  Flag;
} MSG_SYSTEM_STATUS;

/************************************************************************/
/* NAVIGATOR KALMAN FILTER DEBUG MSG                                    */
/************************************************************************/
typedef struct
{
    UINT32 msTime; //in ms
    INT32  stPosX; //cm
    INT32  stPosY; //cm
    INT32  stPosZ; //cm
    INT16	stVNorth ; //cm/s
    INT16	stVEast ; //cm/s
    INT16	stVDown ; //cm/s
    INT16	stQ0 ; //1e4
    INT16	stQ1 ; //1e4
    INT16	stQ2 ; //1e4
    INT16	stQ3 ; //1e4

    INT16	stAxBias ; //1e4
    INT16	stAyBias ; //1e4
    INT16	stAzBias ; //1e4
    INT16	stPBias ;  //1e4
    INT16	stQBias ; //1e4
    INT16	stRBias ; //1e4

    INT16	stAltBias ; //dm
    INT16	stWn ; //cm/s
    INT16	stWe ; //cm/s

    INT16	ssXaccel ; //5e2 m/s/s
    INT16	ssYaccel ; //5e2 m/s/s
    INT16	ssZaccel ; //5e2 m/s/s
    INT16	ssRollRate ;//5e3 rad/s
    INT16	ssPitchRate ;//5e3 rad/s
    INT16	ssYawRate ;// 5e3 rad/s

    INT32   smGPSX; //cm
    INT32   smGPSY; //cm
    UINT16  smGPSZ; //dm
    INT16	smVNorth ; //cm/s
    INT16	smVEast ; //cm/s
    INT16	smVDown ; //cm/s

    UINT16  smAlt;  //dm
    INT16   smTAS; //cm/s
} MSG_DEBUG_NAVKALMAN;

typedef struct
{
    UINT32 msTime; //in ms
    double	stQ ;
    double	stCm0 ;
    double	stCma;
    double	stCmq ;
    double	stCmde ;
    double	stP ;
    double	stClp ;
    double	stCln;
    double	stClda ;
    double	stCldr ;
    double	stR ;
    double	stCnp ;
    double	stCnr;
    double	stCnda ;
    double	stCndr ;
} MSG_DEBUG_ESTIMATOR;


/************************************************************************/
/* CONTROLLER DEBUG MSG                                    */
/************************************************************************/
typedef struct
{
    UINT32 msTime;
    UINT16 stLoop;

    UINT16 cmdTrack;
    INT16 cmdHeading; //0~2PI 1e4 rad/s
    INT16 cmdBank; //1e4 rad/s
    UINT16 cmdAlt; //dm
    INT16 cmdVrate; //cm/s
    INT16 cmdTAS; //cm/s
    INT16 cmdFlap; //1e4 rad

    INT16 actCmdAileron; //1e4 rad
    INT16 actCmdElv;  //1e4 rad
    INT16 actCmdThrot; //1e4
    INT16 actCmdRudder; //1e4 rad
    INT16 actCmdFlap; //1e4 rad

    INT16 fsTAS; //cm/s
    INT16 fsTASdot; //cm/s/s
    UINT16 fsAlt; //dm
    INT16 fsVrate; //cm/s
    INT16 fsVertAcc; //cm/s/s
    INT16 fsRoll; //1e4 rad
    INT16 fsPitch; //1e4 rad
    UINT16 fsYaw; //1e4 rad
    INT16 fsRollRate; //5e3 rad
    INT16 fsPitchRate; //5e3 rad
    INT16 fsYawRate; //5e3 rad
} MSG_DEBUG_CONTROLL;


typedef struct
{
    INT16 cmdTarget;
    INT16 target;
} CONTROL_TARGET;


typedef struct
{
    UINT16         type;
    CONTROL_TARGET debug[20];
} MSG_DEBUG_CONTROLL_2;


typedef struct
{
    FLOAT32   MagX;
    FLOAT32   MagY;
    FLOAT32   MagZ;
    FLOAT32   XAccel;
    FLOAT32   YAccel;
    FLOAT32   ZAccel;
} MSG_MAG_SENSORS_CALIB_DATA;

typedef struct
{
    INT16   RollRate;    //1e4 rad
    INT16   PitchRate;    //1e4 rad
    INT16   YawRate;     //1e4 rad
    INT16   XAccel;      //2e2
    INT16   YAccel;      //2e2
    INT16   ZAccel;      //2e2
    INT16   DynamicP;
    UINT16  StaticP;     //2
} DOUBLET_SENSOR_DATA;

typedef struct
{
    UINT16              SampleIndex;
    UINT16              SampleRate;
    UINT16              TotalSamples;
    DOUBLET_SENSOR_DATA SensorData[5];
} MSG_DOUBLET_SENSOR_DATA;

typedef struct
{
    UINT16 LeftRPM;
    UINT16 RightRPM;
    INT16  ROLL;     //1e4 rad
    INT16  PITCH;    //1e4 rad
    UINT16  YAW;      //1e4 rad
    INT16  Aileron;  //1e4 rad
    INT16  Elevator; //1e4 rad
    UINT16 Throtle;  //1e4
    INT16  Rudder;   //1e4 rad
    INT16  Flap;     //1e4 rad
} DOUBLET_AP_DATA;


typedef struct
{
    UINT16          SampleIndex;
    UINT16          SampleRate;
    UINT16          TotalSamples;
    DOUBLET_AP_DATA ApData[5];
} MSG_DOUBLET_AP_DATA;


typedef struct
{
    UINT8 Flags;
    UINT8 Duration;
    UINT16 Pulse;
    INT16 Center;
    INT16 Delta;
} MSG_DOUBLET_CMD;

/************************************************************************/
/* GCS command msg                                                      */
/* add by Ren Bin  2008/12/16                                           */
/************************************************************************/
typedef struct
{
    UINT8 power;
    UINT8 RSSI;
} MSG_RADIO_CONFIG;


typedef struct
{
    UINT16 Aileron;// chanel 0副翼
    UINT16 Elevator;//升降舵 chanel 1
    UINT16 Throttle; //遥控器油门/风门滑条 chanel 2
    UINT16 Rudder; // chanel 3
    UINT8  Manual;//=1 manual; =0 auto; = 3 面板优先	chanel 4
    UINT8  IPB; //
    UINT16 Flap;// chanel 5
    UINT8  IPBbak;
    UINT8  InstructionL;
    UINT8  InstructionC;
    UINT8  InstructionR;
} OPERATOR_CMD;

#define APMODE_NO_POSITION (0) //! < 无位置信息
#define APMODE_TAKEOFF     (1) //! < 起飞
#define APMODE_TRACK       (2) //! < 位置信息
#define APMODE_LAND        (3) //! < 降落

typedef struct
{
   UINT32 ApMode;
   INT32  lon;//!< 1e7 deg
   INT32  lat; //!< 1e7  deg
   INT32  hight;//!< 1e3  m
   INT16  yaw;//!< 1e1   deg,0~360
   INT16  pitch;//!< 1e1  deg
   INT16  roll;//!< 1e1   deg
   INT32  Vground;//!< 1e2  m/s
   UINT32 loc_time;//!< 1000*(H*3600+M*60+S)+MS
}AP_RT_TRACK_MSG;

typedef struct
{
   UINT32 ApMode;
   INT32  lon;//!< 1e7 deg
   INT32  lat; //!< 1e7  deg
   INT32  hight;//!< 1e3  m
   INT32  Vground;//!< 1e2  m/s
   UINT32 loc_time;//!< 1000*(H*3600+M*60+S)+MS
   INT16  yaw;//!< 1e1   deg,0~360
   INT16  pitch;//!< 1e1  deg
   INT16  roll;//!< 1e1   deg   
}MSG_AP_RT_TRACK_MSG;


typedef JACX_MSG_PACKET COMMPACKET_t;
#define JACX_HEAD_SIZE sizeof(JACX_MSG_HEAD)
#define HEAD_LEN_EX_SYNC sizeof(JACX_MSG_HEAD)-2
void MsgCheckSum(JACX_MSG_PACKET &pct, UINT8 &CKA, UINT8 &CKB);

#pragma pack()





#endif

