
#ifndef __UCLOUD_MSG_TYPE_DEF_H__
#define __UCLOUD_MSG_TYPE_DEF_H__

#include "JacTypeDef.h"

#define JCLOUD_MSG_SYNC0   (0xAA)
#define JCLOUD_MSG_SYNC1   (0x44)
#define JCLOUD_MSG_SYNC2   (0x12)

enum JCLOUD_MSG_ID
{    
    JCLOUD_MSG_ID_PUSH_DRONE_IDENTITY     =0x0B,
    JCLOUD_MSG_ID_PULL_IDENTITY_RESP      =0x0C,
  	JCLOUD_MSG_ID_PUSH_DRONE_HEART        =0x0D,
	  JCLOUD_MSG_ID_PULL_DRONE_HEART        =0x0E,
	  JCLOUD_MSG_ID_PUSH_DRONE_CONTENT      =0x0F,
		JCLOUD_MSG_ID_SETTING_REQ							=0x10,
		JCLOUD_MSG_ID_DATA_LINK								=0x12,
		JCLOUD_MSG_ID_BAT                     =0x0A,       
};

#pragma pack(1)
typedef struct
{
    UINT8   sync0;
    UINT8   sync1;
	  UINT8   sync2;
    UINT8   version;
    UINT32  jcloud_apid;
    UINT32  jcloud_gsmid;
    UINT8   msgid;
    UINT8   reserved;
    UINT16  msglen;
}JCLOUD_MSG_HAED;


#define JCLOUD_MSG_PACK_MAX_LEN   (256)	

typedef struct
{
    JCLOUD_MSG_HAED msgHead;
    UINT8 Data[JCLOUD_MSG_PACK_MAX_LEN];
}JCLOUD_MSG_PACK;

#define JCLOUD_IDENTITY_MSG_RESP_OK           (0x15)
#define JCLOUD_IDENTITY_MSG_RESP_UNREGISTER   (0x29)
#define JCLOUD_IDENTITY_MSG_RESP_REJECTED     (0x2A)
#define JCLOUD_IDENTITY_MSG_RESP_LOGGEDIN     (0x2B)

typedef struct
{
    UINT32 resp;
}JCLOUD_IDENTITY_MSG_RESP;

#define  SIZE_OF_JCLOUD_MSG_HEAD  (sizeof(JCLOUD_MSG_HAED))
#endif 


