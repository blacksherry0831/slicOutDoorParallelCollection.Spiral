#pragma once

#include "opencv_stl.h"
/*-----------------------------------*/
/**
*
*/
/*-----------------------------------*/
#include "qt_all.h"
/*-----------------------------------*/
/**
*
*/
/*-----------------------------------*/

/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/

/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
class Saveframe
{
	
public:
	Saveframe();
	~Saveframe();
private:
	
	std::string mIpAddrPath;
	std::string mTimePath;
	std::string mChannelPath;

	std::string mImageName;

	std::string mImageNameFileFullPath;
	
private:
	std::string mIpaddr;
	std::string mTimeCurrent;
	int mChannel;
	int mFrameCount;
public:
	Saveframe* SetTime(std::string _time);
	Saveframe* SetChannel(int _ch);
	Saveframe* SetFrameCount(int _count);
	Saveframe* SetIpAddr(std::string _ipAddr);

	 void SaveFrame2Disk(IplImage* img_t);
	 
	 std::string  initFileName();
	 
	 void initPath();

	 void init();

	 int isInitParam();
	 
	 void initParam();
public:
	Saveframe* start_record();
	Saveframe* stop_record();
};