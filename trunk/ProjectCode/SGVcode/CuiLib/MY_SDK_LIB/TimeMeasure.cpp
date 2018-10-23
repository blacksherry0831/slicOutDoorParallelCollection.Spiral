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
	dfFreq = (double)litmp.QuadPart;// ��ü�������ʱ��Ƶ��
	QueryPerformanceCounter(&litmp);
	QPart1 = litmp.QuadPart;// ��ó�ʼֵ
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
	QPart2 = litmp.QuadPart;//�����ֵֹ
	dfMinus = (double)(QPart2 - QPart1);
	dfTim = dfMinus / dfFreq;// ��ö�Ӧ��ʱ��ֵ����λΪ��
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