/*
*	Header GPS_UBX.h
*   define Ublox GPS UBX Protocol and it's data structure
*   Ren Bin 2007/06/19
*/

#ifndef __GPS_UBX_H__
#define __GPS_UBX_H__


#define UBX_SYNC_HEAD1 0xB5
#define UBX_SYNC_HEAD2 0x62

#define UBX_PROTO_MASK  0x0001 
#define NMEA_PROTO_MASK 0x0002 
#define RTCM_PROTO_MASK 0x0004 

#if 0
#define NAV_DYN_AIRBORNE_1G 5
#define NAV_DYN_AIRBORNE_2G 6
#define NAV_DYN_AIRBORNE_4G 7
#else
#define NAV_DYN_AIRBORNE_1G 6
#define NAV_DYN_AIRBORNE_2G 7
#define NAV_DYN_AIRBORNE_4G 8
#endif

#define UBX_CLS_CFG 0x06
#define UBX_CLS_CFG_PORT 0x00
#define UBX_CLS_CFG_MSG	 0x01
#define UBX_CLS_CFG_NAV	 0x03
#define UBX_CLS_CFG_NAV5 0x24
#define UBX_CLS_CFG_RATE 0x08
#define UBX_CLS_CFG_RXM	 0x11
#define UBX_CLS_CFG_SBAS 0x16

#define UBX_CLS_NAV 0x01
#define UBX_CLS_NAV_POSLLH 0x02
#define UBX_CLS_NAV_STATUS 0x03
#define UBX_CLS_NAV_SOL    0x06
#define UBX_CLS_NAV_VELNED 0x12
#define UBX_CLS_NAV_TIMEGPS 0x20

#define UBX_CLS_ACK 0x05
#define UBX_CLS_ACK_ACK 0x01
#define UBX_CLS_ACK_NCK 0x00

#pragma  pack(4)

typedef struct 
{
	UINT8	 	SynHead1;
	UINT8		SynHead2;
	UINT8	 	cls;			// message class
	UINT8		id;				// message id
	UINT16  len;
} UBX_MSG_HEAD;

typedef struct  
{
	UBX_MSG_HEAD head;
	UINT8 data[256];
}UBX_PACKET;

typedef struct 
{
	UINT8	 	PortID;
	UINT8		Res0;
	UINT16		Res1;
	UINT32		Mode;
	UINT32		Baudrate;
	UINT16		In_proto_mask;
	UINT16		Out_proto_mask;
	UINT16		Flags;
	UINT16		Res2;
} UBX_MSG_CFG_PRT;

typedef struct 
{
	UINT8	 	Class;
	UINT8		MsgID;
	UINT8		Rate;
} UBX_MSG_CFG_MSG;

typedef struct 
{
	UINT32	ITOW;
	UINT8	GPSfix;
	UINT8	Flags;
	UINT8	DiffS;
	UINT8	res;
	UINT32	TIFF;
	UINT32	MSSS;
} UBX_MSG_NAV_STATUS;

typedef struct 
{
	UINT32	ITOW;
	INT32	LON;
	INT32	LAT;
	INT32	HEIGHT;
	INT32	HMSL;
	UINT32	Hacc;
	UINT32	Vacc;
} UBX_MSG_NAV_POSLLH;

typedef struct 
{
	UINT32	ITOW;
	INT32	VEL_N;
	INT32	VEL_E;
	INT32	VEL_D;
	UINT32	Speed;
	UINT32	GSpeed;
	INT32	Heading;
	UINT32	SAcc;
	UINT32	CAcc;
} UBX_MSG_NAV_VELNED;

typedef struct 
{
	UINT32	ITOW;
	INT32	Frac;
	INT16	week;
	UINT8     GPSfix;
	UINT8     Flags;
	INT32     ECEF_X;
	INT32     ECEF_Y;
	INT32     ECEF_Z;
	UINT32   Pacc;
	INT32     ECEFVX;
	INT32     ECEFVY;
	INT32     ECEFVZ;	
	UINT32   SAcc;
	UINT16   PDOP;
	UINT8     res1;
	UINT8     numSV;
	UINT32   res2;
}UBX_MSG_NAV_SOL;    

typedef struct 
{
	UINT32	ITOW;
	UINT8	NCH;
	UINT8	RES1;
	UINT16	RES2;
	UINT8	chn;
	UINT8	SVID;
	UINT8	Flags;
	char    QI;
	UINT8	CNO;
	char	Elev;
	INT16	Azim;
	INT32 	PRRES;
} UBX_MSG_NAV_SVINFO;

typedef struct 
{
	UINT8	 	Platform;
	UINT8		MinSVs;
	UINT8		MaxSVs;
	UINT8		MinCNO;
	UINT8		AbsCNO;
	UINT8		MinELE;
	UINT8		DGPSTTR;
	UINT8		DGPSTO;
	UINT8		PRCAGE;
	UINT8		CPCAGE;
	UINT16		MinCLT;
	UINT16		AbsCLT;
	UINT8		MaxDR;
	UINT8		NAVOPT;
	UINT16		PDOP;
	UINT16		TDOP;
	UINT16		PACC;
	UINT16		TACC;
	UINT16		FACC;
	UINT8		StaticThres;
	UINT8		reserved;
} UBX_MSG_CFG_NAV;

typedef struct 
{
	UINT8	 	mode;
	UINT8	 	usage;
	UINT8		maxsbas;
	UINT8		reserved;
	UINT32		scanmode;
} UBX_MSG_CFG_SBAS;

typedef struct 
{
	UINT16	 	Meas;
	UINT16	 	Nav;
	UINT16		Time;
} UBX_MSG_CFG_RATE;

typedef struct 
{
	UINT8		gps_mode;
	UINT8		lp_mode;
} UBX_MSG_CFG_RXM;

typedef struct 
{
	UINT16		MssgClassID;
} UBX_MSG_ACK_ACK;

typedef struct  
{
	UINT32 ITOW;
	INT32 Frac;
	INT16 week;
	char LeapS;
	UINT8 Valid;
	UINT32 TAcc;
}UBX_MSG_NAV_TIMEGPS;

typedef struct  
{
	UINT8 Platform;
	UINT8 res0;
	UINT16 res1;
	UINT8 MinSVInitial;
	UINT8 MinSVs;
	UINT8 MaxSVs;
	UINT8 FixMode;
	INT32 FixedAltitude;
	UINT8 MinCN0Initial;
	UINT8 MinCN0After;
	UINT8 MinELE;
	UINT8 DGPSTO;
	UINT8 MaxDR;
	UINT8 NAVOPT;
	UINT16 res2;
	UINT16 PDOP;
	UINT16 TDOP;
	UINT16 PACC;
	UINT16 TACC;
	UINT8 StaticThres;
	UINT8 res3;
	UINT16 res4;
	UINT32 res5;
	UINT32 res6;
}UBX_MSG_CFG_NAV2;

typedef struct  
{
	UINT16 mask;
	UINT8 dynModel;
	UINT8 fixMode;
	INT32 fixedAlt;
	UINT32 fixedAltVar;
	char minElev;
	UINT8 drLimit;
	UINT16 pDop;
	UINT16 tDop;
	UINT16 pAcc;
	UINT16 tAcc;
	UINT8 staticHoldThresh;
	UINT8 res1;
	UINT32 res2;
	UINT32 res3;
	UINT32 res4;
}UBX_MSG_CFG_NAV5;

typedef struct  
{
	UINT32 ITOW;
	INT32 Lat; //1e7 degree
	INT32 Lon; //1e7 degree
	INT32 HMSL; //mm
	UINT16 Hacc; //mm
	UINT16 Vacc; //mm
	INT16 VNorth; //cm/s
	INT16 VEast;  //cm/s
	INT16 VDown;  //cm/s
	UINT16 SAcc; //cm/s
	INT32 Heading; //1e5 degree
	UINT32 CAcc; //1e5 degree
	INT16 GPSWeek;
	UINT32 TIFF;
	UINT32 MSSS;
	UINT8 GPSfix;
	UINT8 Flags;
	UINT8 DiffS;
	UINT8 Res;
}GPS_READING;

typedef struct  
{
    UINT8 DeviceID;
    UINT8 Reserved;
    INT32 Lat; //1e7 degree
    INT32 Lon; //1e7 degree
    INT32 HMSL; //mm
    INT16 VNorth; //cm/s
    INT16 VEast;  //cm/s
    INT16 VDown;  //cm/s
    UINT8 GPSfix;
    UINT8 Flags;
    UINT8 pDOP;
    UINT8 numSV;
    UINT16 GPSWeek;
    UINT32 GPSTow;
}GPS_SEND_DATA;

typedef struct  
{
    UINT8 SYNC0;
    UINT8 SYNC1;
    UINT8 DESTINATION;
    UINT8 SOURCE;
    UINT8 MSGID;
    UINT8 SEQ_NUMBER;
    UINT8 ACK_NAK;
    UINT8 LENGTH;
}GPS_SEND_HEAD;

typedef struct  
{
    GPS_SEND_HEAD HEAD;
    GPS_SEND_DATA DATA;
    UINT8 CKA;
    UINT8 CKB;
	
}GPS_SEND_PAC;

typedef struct 
{
	UINT32	ITOW;	
	UINT16  YEAR;
	UINT8   MONTH;
	UINT8   DAY;
	UINT8   HOUR;
	UINT8   MIN;
	UINT8   SEC;
	UINT8   VALID;
	UINT32  TACC;
	INT32   NANO;
	UINT8   FIXTYPE;
	UINT8   FLAGS;
	UINT8   REV1;
	UINT8   NUMSV;
	INT32   LON;
	INT32   LAT;
	INT32   HEIGHT;
	INT32   HMSL;
	UINT32  HACC;
	UINT32  VACC;
	INT32   VELN;
	INT32   VELE;
	INT32   VELD;
	INT32   GSPEED;
	INT32   HEADING;
	UINT32  SACC;
	UINT32  HEADINGACC;
	UINT16  PDOP;
	UINT16  REV2;
	UINT32  REV3;
	
}UBX_MSG_NAV_PVT;    

typedef struct  
{
	UINT8 IP0;
	UINT8 IP1;
    UINT8 IP2;
	UINT8 IP3;
	UINT16 PORT;
}Setting_Pack;


typedef struct  
{
	UINT8 DataFlag;
	UINT8 NumAct;
    UINT8 Limit0;
	UINT8 Limit1;
	UINT32 Time;
	INT32 Lat;
	INT32 Lon;
	INT32 HMSL;
	INT16	VNorth;
	INT16	VEast;
	INT16	VDown;
	UINT8	GPSFix;
	UINT8	Flags;
	UINT8	pDOP;
	UINT8	numSV;
	UINT16	GPSWeek;
	UINT32	GPSTow;
	INT16	ROLL;
	INT16	PITCH;
	UINT16	YAW;
	INT16	ALT;
	INT16	TAS;
	INT16	WSouth;
	INT16	WWest;
	UINT16	LRPM;
	UINT16	RRPM;
	UINT8	DensR;
	INT8	OAT;
	INT16	Xaccel;
	INT16	Yaccel;
	INT16	Zaccel;
	INT16	RollRate;
	INT16	PitchRate;
	INT16	YawRate;
	UINT16  StaticP;
	UINT16	DynP;
	INT16	XMagField;
	INT16	YMagField;
	INT16	ZMagField;
	UINT16	Compass;
	INT16	Actuator0;
	INT16	Actuator1;
	INT16	Actuator2;
}TELEMETRY_HI_RES;

#pragma  pack()

#endif

