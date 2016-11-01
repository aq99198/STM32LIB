/*************************************************************
* Copyright Jovian Advanced Control, Co.
*
*  FileName:    ProtocolDef.cpp
*  Abstract:    Define function used for Communication Packet portocol
*  Reference:   《通信协议(V1.0)》
*  Version:      V1.0
*  Writer:       Ren Bin
*  Date:         2008.03.05
*---------------------------------------------------------------
* Modification history:
*  Date:
*  Version:
*  Description:
****************************************************************/

#include "board.h"

#define HEAD_LEN_EX_SYNC sizeof(JACX_MSG_HEAD)-2

void MsgCheckSum(COMMPACKET_t &pct, UINT8 &CKA, UINT8 &CKB)
{
    UINT8 i;

    CKA = 0;
    CKB = 0;
    UINT8 *p = (UINT8 *)(&(pct.msgHead));
    p += 2; //exclude the sync0,and sync1

    for (i = 0; i < HEAD_LEN_EX_SYNC; ++i)
    {
        CKA += *(p++);
        CKB += CKA;
    }

    p = (UINT8 *)(pct.Data);
    int len = pct.msgHead.len;
    for (i = 0; i < len; ++i)
    {
        CKA += *(p++);
        CKB += CKA;
    }
}



#ifdef ___GCS_OPERATOR__
int Protocol_MSG_Length[NUM_MSG_ID];
CString Protocol_MSG_Name[NUM_MSG_ID];


void InitializeMsgDefinition()
{
    //#ifdef ___GCS_OPERATOR__
    Protocol_MSG_Length[JACX_OPERATOR_CMD_PULSE] = JACX_OPERATOR_CMD_PULSE_LEN;
    Protocol_MSG_Length[JACX_MANUAL_ASSIST_MODE] = JACX_MANUAL_ASSIST_MODE_LEN;

    Protocol_MSG_Length[JACX_WAYPOINT] = JACX_WAYPOINT_LEN;
    Protocol_MSG_Length[JACX_WAYPOINT_LIST] = JACX_WAYPOINT_LIST_LEN;
    //	Protocol_MSG_Length[JACX_LANDING_PLAN]=0;
    Protocol_MSG_Length[JACX_MISSION_LIMITS] = JACX_MISSION_LIMITS_LEN;

    Protocol_MSG_Length[JACX_ENGINE_KILL] = JACX_ENGINE_KILL_LEN;
    //	Protocol_MSG_Length[JACX_PRELAUNCH]=0;
    //	Protocol_MSG_Length[JACX_LAUNCH_NOW]=0;
    //	Protocol_MSG_Length[JACX_LAND_NOW]=0;
    //	Protocol_MSG_Length[JACX_ABORT]=0;

    Protocol_MSG_Length[JACX_TRACK] = JACX_TRACK_LEN ;
    Protocol_MSG_Length[JACX_AUTOPILOT_LOOP] = JACX_AUTOPILOT_LOOP_LEN;
    Protocol_MSG_Length[JACX_HEADING_CONTROL] = JACX_HEADING_CONTROL_LEN;
    Protocol_MSG_Length[JACX_HEIGHT_CONTROL]  = JACX_HEIGHT_CONTROL_LEN;

    Protocol_MSG_Length[JACX_CONTROLLER_DATA] = JACX_CONTROLLER_DATA_LEN;
    Protocol_MSG_Length[JACX_CONTROLLER_DATA_REQUEST] = JACX_CONTROLLER_DATA_REQUEST_LEN;
    Protocol_MSG_Length[JACX_CONTROLLER_DATA_DEFAULT] = JACX_CONTROLLER_DATA_DEFAULT_LEN;

    Protocol_MSG_Length[JACX_SENSOR_ORIENTATION] = JACX_SENSOR_ORIENTATION_LEN;
    Protocol_MSG_Length[JACX_ALTIMETER_SETTING] = 0;
    Protocol_MSG_Length[JACX_SENSOR_ERROR] = 0;
    Protocol_MSG_Length[JACX_AIR_DATA_ZERO] = 0;

    Protocol_MSG_Length[JACX_MAG_DECLINATION] = JACX_MAG_DECLINATION_LEN;
    Protocol_MSG_Length[JACX_MAG_CALIBRATION] = JACX_MAG_CALIBRATION_LEN;
    Protocol_MSG_Length[JACX_MAG_CALIBRATION_DATA] = JACX_MAG_CALIBRATION_DATA_LEN;

    Protocol_MSG_Length[JACX_SURFACE_TABLE] = JACX_SURFACE_TABLE_LEN;
    Protocol_MSG_Length[JACX_SURFACE_TEST] = 0;
    Protocol_MSG_Length[JACX_SET_FUEL_LEVEL] = 0;
    Protocol_MSG_Length[JACX_SYSTEM_VERSION] = JACX_SYSTEM_VERSION_LEN;
    Protocol_MSG_Length[JACX_SYSTEM_RESET] = 0;
    Protocol_MSG_Length[JACX_SAVE_CONFIG_FLSAH] = 0;

    Protocol_MSG_Length[JACX_TELEMETRY_CONFIG] = JACX_TELEMETRY_CONFIG_LEN;
    Protocol_MSG_Length[JACX_TELEMETRY_HI_RES] = 0;
    Protocol_MSG_Length[JACX_TELEMETRY_LO_RES] = 0;
    Protocol_MSG_Length[JACX_TELEMETRY_SYSSTATUS] = JACX_TELEMETRY_SYSSTATUS_LEN;
    Protocol_MSG_Length[JACX_GCS_RADIO_CONFIG] = 0;

    Protocol_MSG_Length[JACX_GCS_GPS_READING] = 0;


    Protocol_MSG_Name[JACX_OPERATOR_CMD_PULSE] = "";
    Protocol_MSG_Name[JACX_MANUAL_ASSIST_MODE] = "";

    Protocol_MSG_Name[JACX_WAYPOINT] = "";
    Protocol_MSG_Name[JACX_WAYPOINT_LIST] = "";
    //	Protocol_MSG_Name[JACX_LANDING_PLAN]="";
    Protocol_MSG_Name[JACX_MISSION_LIMITS] = "";

    Protocol_MSG_Name[JACX_ENGINE_KILL] = "";
    //	Protocol_MSG_Name[JACX_PRELAUNCH]="";
    //	Protocol_MSG_Name[JACX_LAUNCH_NOW]="";
    //	Protocol_MSG_Name[JACX_LAND_NOW]="";
    //	Protocol_MSG_Name[JACX_ABORT]="";

    Protocol_MSG_Name[JACX_TRACK] = "";
    Protocol_MSG_Name[JACX_AUTOPILOT_LOOP] = "";
    Protocol_MSG_Name[JACX_HEADING_CONTROL] = "航向控制";
    Protocol_MSG_Name[JACX_HEIGHT_CONTROL] = "高度控制";

    Protocol_MSG_Name[JACX_CONTROLLER_DATA] = "";
    Protocol_MSG_Name[JACX_CONTROLLER_DATA_REQUEST] = "";
    Protocol_MSG_Name[JACX_CONTROLLER_DATA_DEFAULT] = "";

    Protocol_MSG_Name[JACX_SENSOR_ORIENTATION] = "";
    Protocol_MSG_Name[JACX_ALTIMETER_SETTING] = "";
    Protocol_MSG_Name[JACX_SENSOR_ERROR] = "";
    Protocol_MSG_Name[JACX_AIR_DATA_ZERO] = "";

    Protocol_MSG_Name[JACX_MAG_DECLINATION] = "";
    Protocol_MSG_Name[JACX_MAG_CALIBRATION] = "";
    Protocol_MSG_Name[JACX_MAG_CALIBRATION_DATA] = "";

    Protocol_MSG_Name[JACX_SURFACE_TABLE] = "";
    Protocol_MSG_Name[JACX_SURFACE_TEST] = "";
    Protocol_MSG_Name[JACX_SET_FUEL_LEVEL] = "";
    Protocol_MSG_Name[JACX_SYSTEM_VERSION] = "";
    Protocol_MSG_Name[JACX_SYSTEM_RESET] = "";
    Protocol_MSG_Name[JACX_SAVE_CONFIG_FLSAH] = "";

    Protocol_MSG_Name[JACX_TELEMETRY_CONFIG] = "";
    Protocol_MSG_Name[JACX_TELEMETRY_HI_RES] = "";
    Protocol_MSG_Name[JACX_TELEMETRY_LO_RES] = "";
    Protocol_MSG_Name[JACX_TELEMETRY_SYSSTATUS] = "";
    Protocol_MSG_Name[JACX_GCS_RADIO_CONFIG] = "";

    Protocol_MSG_Name[JACX_GCS_GPS_READING] = "";
    //#endif

}
#endif
