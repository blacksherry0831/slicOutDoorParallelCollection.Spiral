#ifndef CPP_STL
#define CPP_STL          
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
//C++
#include<stdio.h>
#include <iostream>
#include <stdlib.h>
#include <fstream>
//#include <iostream>
#include <math.h>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cfloat>
#include <cmath>
#include <ios>
#include <fstream>
#include <stdexcept>
#include <cstdio> 
#include<stdio.h>
#include<string.h>
#include<numeric>
#if _MSC_VER
#include <mbctype.h>  
#include <io.h>
#include <tchar.h>
#include <assert.h>
#include <memory.h> 
#endif
#if _MSC_VER==1900
#include <functional>
#endif

#if linux||__linux__||__linux||__GNUC__
#include<memory>
#include <tr1/memory>
#include <dirent.h>
#include <unistd.h>
#include <sys/sysinfo.h>
#include <pthread.h>

#ifndef ASSERT
#include <assert.h>
#define  ASSERT(Exp) assert(Exp)
#endif

#include<stdbool.h>

#endif
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#if linux||__linux||__linux__||__GNUC__
#ifndef nullptr
#define nullptr 0
#endif
#endif



#ifndef TimeCountClock_START

#endif 
#ifndef TimeCountClock_END

#endif

#ifndef TimeCountClockTest
#define TimeCountClockTest
//#define TimeCountClockTest(P,M)  {double dur;clock_t start,end;start = clock();	{P} end = clock();dur = (double)(end - start);printf("%s(S):%0.3f\n",M,(dur/CLOCKS_PER_SEC));}

#define TimeCountStart()	double dur=0;clock_t start,end;start = clock();

#define TimeCountStop(MESSAGE_TIME_END)		end = clock();dur = (double)(end - start);printf("%s(S):%0.3f\n",MESSAGE_TIME_END,(dur/CLOCKS_PER_SEC));






#endif

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
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
#ifndef DWORD
typedef unsigned long       DWORD;
#endif

#ifndef BOOL
typedef int                 BOOL;
#endif

#ifndef BYTE
typedef unsigned char       BYTE;
#endif

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
//LPTSTR
//#ifndef LPTSTR
//typedef char* LPTSTR;
//#endif

#ifndef PVOID
typedef void *PVOID;
#endif

#ifndef HANDLE
typedef PVOID HANDLE;
#endif

#ifndef UINT
typedef unsigned int UINT;
#endif // !UINT


#ifndef PUINT
typedef unsigned int        *PUINT;
#endif // !1


//#ifndef CHAR
//typedef char CHAR;
//#endif

//#ifndef LPSTR
//typedef CHAR* LPSTR;
//#endif // 

//#ifndef WCHAR
//typedef WCHAR *LPWSTR;
//#endif

//#ifdef UNICODE
//typedef LPWSTR LPTSTR;
//#else
//typedef LPSTR LPTSTR;
//#endif

#endif


#ifndef boolean
typedef unsigned char boolean;
#endif // !boolean


#if defined(linux) || defined(__linux) || defined(__linux__)

#endif

#if defined(_WIN32) || defined(_WIN64)


//#include <crtdbg.h>


#endif
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
#endif









