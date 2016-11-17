/*************************************************************
* Copyright 2008-2010 Jovian Advanced Control Technology, Co.
*
*  FileName:        fastMalloc.h
*  Abstract:        used internal ram(access faster than sram)
*
*  Current Version: V1.0
*  Writer:          wang chen
*  Date:            2009.4.30
*---------------------------------------------------------------
* Modification history:
* Date:
* Version:
* Description:
*
****************************************************************/
#ifndef __FAST_MALLOC_H__
#define __FAST_MALLOC_H__

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

    void *fastCalloc(int size, int n);
    void fastFree(void *p);
    void *fastMalloc(int size);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
void   *operator new (size_t size);
void   operator delete (void *p);
void   *operator new [] (size_t size);
void   operator delete [] (void *p);
#endif

#endif
