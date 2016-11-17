/*************************************************************
* Copyright 2008-2010 Jovian Advanced Control Technology, Co.
*
*  FileName:        fastMalloc.cpp
*  Abstract:        used internal ram(access faster than sram)
*
*  Current Version: V1.0
*  Writer:          wagn chen
*  Date:            2009.4.30
*---------------------------------------------------------------
* Modification history:
* Date:
* Version:
* Description:
*
****************************************************************/
#include "fastMalloc.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>  
#include "JacTypeDef.h"

#define CCM_HEAP_DEBUG
#undef  CCM_HEAP_DEBUG

void    *CCMcalloc(UINT32 size);

UINT32  SYS_STACK_SIZE = 0x00001000;
UINT32  SYS_HEAP_SIZE   = 0x00000200;

/*
   @breif:   allocates memory from 64KB CCM
   @param    the number of bytes
   @retval   none
   @note     the CCM memory can not be free, the size will be aligned to word( 4 bytes)
*/
void *fastCalloc(int size, int n)
{
    void *ptr  = CCMcalloc(size * n);
    if( ptr )
    {
        return ptr;
    }
    else
    {
#ifdef CCM_HEAP_DEBUG
        printf("\t**WARNING**  fastMalloc change from CCM to HEAP   \r\n");
#endif
        ptr = fastMalloc(size * n);
        if( ptr)
        {
            memset(ptr, 0, size * n);
            return ptr;
        }
        else
        {
#ifdef CCM_HEAP_DEBUG            
            printf("\n **EEROR**  the system CCM and Heap overflow \r\n");
#endif 
            abort();
            return NULL;
        }
    }
    //return (void *)calloc(size, n);

}

void fastFree(void *p)
{
#ifdef CCM_HEAP_DEBUG    
    printf("\n **EEROR**the system can not free \r\n");
#endif 
}

const UINT32  SysHeapSize = SYS_HEAP_SIZE ; // 120 K
      UINT32  SysHeapUse = 0;
/*
   @breif:   allocates memory from the system HEAD
   @param    the number of bytes
   @retval   none
   @note     the CCM memory can not be free, the size will be aligned to word( 4 bytes)
*/
void *fastMalloc(int size)
{
    void *p = NULL;
    p  = malloc(size);
    if( p )
    {
        SysHeapUse += size;
        if( SysHeapUse >= SysHeapSize )
        {
#ifdef CCM_HEAP_DEBUG            
            printf(" **ERROR** system heap overflow \r\n");
#endif 
        }
    }
    else
    {
#ifdef CCM_HEAP_DEBUG
        printf(" **ERROR** system heap overflow \r\n");
#endif 
    }


#ifdef CCM_HEAP_DEBUG

    printf(" \tSysHeap total %d,used %d ,current allocation %d -> from addr 0X%08X \r\n", SysHeapSize, SysHeapUse, size, p);
#endif
    return     p;

}



/*
   @breif:   allocates memory from 64KB CCM
   @param    the number of bytes
   @retval   none
   @note     the CCM memory can not be free, the size will be aligned to word( 4 bytes)
*/

#if 0
const UINT32  CCMADDR_LOW = 0x0;
const UINT32  CCMSRAMMAX  = 0x0;  // 64 K
      UINT32  CCMSRAMUSED = 0x0;

const UINT32  WORDALIGNBYTE = 4;
#endif 
void *CCMcalloc(UINT32 size)
{
    
#ifdef CCM_HEAP_DEBUG
        printf("\t**WARNING**  CCM memory overflow  \r\n");
#endif
        return (void *)0;
}

/**/

#ifdef  CCM_HEAP_DEBUG
#define NEW_OPERATOR_RELOCATION_DEBUG
#endif

#if 1
void *operator new (size_t size)
{
    void *p ;
    p = fastMalloc(size);

#ifdef NEW_OPERATOR_RELOCATION_DEBUG
    printf("over write new \r\n ");
#endif
    return p ;
}

void operator delete (void *p)
{
#ifdef NEW_OPERATOR_RELOCATION_DEBUG
    printf("over write delete \r\n ");
#endif
    fastFree(p);
}

void *operator new [](size_t size)
{
    void *p ;

    p = fastMalloc(size);
#ifdef NEW_OPERATOR_RELOCATION_DEBUG
    printf("over write new [] \r\n ");
#endif

    return p ;
}
void operator delete [] (void *p)
{
#ifdef NEW_OPERATOR_RELOCATION_DEBUG
    printf("over write delete [] \r\n ");
#endif
    fastFree(p);
}
#endif
