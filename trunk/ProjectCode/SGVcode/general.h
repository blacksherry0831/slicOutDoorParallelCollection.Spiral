// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently
#pragma once

#ifndef PLATFORM_GENERAL_XXX
#define PLATFORM_GENERAL_XXX


/************************************************************************/
/************************************************************************/
#ifndef ASSERT
#if linux||__linux||__linux__||__GNUC__
#include <assert.h>
#define  ASSERT(Exp) assert(Exp)
#endif
#endif
/************************************************************************/
#ifndef LPVOID
#if __GNUC__
#define LPVOID  void *
#endif
#if _MSC_VER

#endif
#endif
/************************************************************************/
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif
/************************************************************************/
#ifndef UINT32
typedef unsigned int UINT32;
#endif
//
#ifndef BOOL
typedef int BOOL;
#endif
//
#ifndef INT32
typedef signed int INT32;
#endif
//
#ifndef ULONGLONG
typedef unsigned long long ULONGLONG;
#endif
//
#ifndef LPVOID
#if __GNUC__
#define LPVOID  void *
#endif
#if _MSC_VER

#endif
#endif
/************************************************************************/
/************************************************************************/
#if __GUNC__||linux||__linux||__linux__
#ifndef memcpy_s
#define memcpy_s(D,Ds,S,Ss)  {memcpy(D,S,Ss);}
#endif

#ifndef  strcpy_s
#define strcpy_s(D,SizeBuff,S)  strncpy(D,S,SizeBuff)
#endif

#ifndef _MAX_FNAME
#define _MAX_FNAME  1024
#endif

#ifndef  XXXXXXXXXXXX
#define  XXXXXXXXXXXX  11111
#endif

#endif
#if __GNUC__
#ifndef sprintf_s
#define sprintf_s  snprintf
#endif

#ifndef  TRACE
//#define printf(fmt, args...) myprintf(fmt, ##args)
#define TRACE(FMT,ARGS...) {fprintf(stderr,"%s\t%s\t%d\t%s\n",__FILE__,__FUNCTION__,__LINE__,__TIME__);fprintf(stderr,FMT,##ARGS);}
#endif 

#endif

#if TRUE
//此宏展开后，类似于printf("%d""%d", 1, 2);  
#define TRACE_FUNC_ARGS(fmt,...)  printf("%s(%d)-<%s>: "##fmt, __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__) 

//#define TRACE_FUNC()  printf("\n>>>>>>CALL FUNC : %s(%d)\n<%s>\n", __FUNCTION__,__LINE__,__FILE__) 
#define TRACE_FUNC()  printf("\n>>>>>>CALL FUNC : %s(%d)\n", __FUNCTION__,__LINE__) 

#endif

#if TRUE
#ifndef nullptr
#define nullptr 0
#endif
#endif
/************************************************************************/
#if _MSC_VER
//#define PT_USE_SEM
//#include <pthread.h>
//#include <thread>
//#include <mutex> 
#endif

#if linux||__linux||__linux__||__GNUC__

#include <sys/sysinfo.h>
#include <pthread.h>
#endif

/************************************************************************/
#define DEBUG_CUI FALSE
#define GetHorLineFromFile  FALSE
/************************************************************************/

#endif
