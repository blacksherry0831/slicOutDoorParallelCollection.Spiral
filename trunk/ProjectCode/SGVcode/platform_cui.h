
// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifndef PLATFORM_CUI_XXX
#define PLATFORM_CUI_XXX



#include "CuiLib/opencv_stl.h"
/************************************************************************/
#ifdef _MSC_VER
#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS


//#include <vld.h> 
#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT
#include <afxcontrolbars.h>     // MFC support for ribbons and control bars
#if _MSC_VER
#include<atlstr.h>
#endif
#endif 
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

//#include <vld.h> 
#include <CuiLib/SGVcode/cfg/cui_confg_400_CUDA_ColorBar20150319.h>
/************************************************************************/



#endif
