#pragma once

#include "opencv_stl.h"

class TimeMeasure
{
public:
	TimeMeasure(std::string _event);
	TimeMeasure(void);
	~TimeMeasure(void);
private:
	int IsTimeMeasure;
	std::string mEVENT;
#if _MSC_VER
	LARGE_INTEGER litmp;
	LONGLONG QPart1, QPart2;
	double dfMinus, dfFreq, dfTim;
#endif // _MSC_VER

	double dur;
	clock_t start_time;
	clock_t end_time;
	
public:
	void start(std::string _event);
	void stop();
public:
	

};

