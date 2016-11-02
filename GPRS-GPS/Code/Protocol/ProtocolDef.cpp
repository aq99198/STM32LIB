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
#include "ProtocolDef.h"

#define HEAD_LEN_EX_SYNC sizeof(JACX_MSG_HEAD)-2

void MsgCheckSum(JACX_MSG_PACKET &pct, UINT8 &CKA, UINT8 &CKB)
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

