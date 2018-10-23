#include "StdAfx.h"
#include "TimeMeasure.hpp"
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/

/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
TimeMeasure::TimeMeasure(void)
{
	
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
TimeMeasure::~TimeMeasure(void)
{
	
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
void TimeMeasure::start(std::string _event)
{
	mEVENT = _event;
#if _MSC_VER
	
	QueryPerformanceFrequency(&litmp);
	dfFreq = (double)litmp.QuadPart;// 获得计数器的时钟频率
	QueryPerformanceCounter(&litmp);
	QPart1 = litmp.QuadPart;// 获得初始值
							/////////////////////////////////////////////
#endif

#if TRUE
	start_time = clock();
#endif // TRUE

	


}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
void TimeMeasure::stop()
{
#if _MSC_VER
	///////////////////////////////////////////////
	QueryPerformanceCounter(&litmp);
	QPart2 = litmp.QuadPart;//获得中止值
	dfMinus = (double)(QPart2 - QPart1);
	dfTim = dfMinus / dfFreq;// 获得对应的时间值，单位为秒
	printf("%s(ms):%0.3f\n", mEVENT.c_str(), dfTim*1000);
#endif

#if TRUE
	end_time = clock(); 
	dur = (double)(end_time - start_time);	
	printf("%s(S):%0.3f\n", mEVENT.c_str(), (dur / CLOCKS_PER_SEC));
#endif // 0

	
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/