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
#include "CMD_CTRL.hpp"
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
	QSharedPointer<CMD_CTRL> mCmd;
	
	std::string mIpaddr;
	int mChannel;
	int mFrameCount;
	int mIsSaveFrame;

public:
	Saveframe* SetImgCmd(QSharedPointer<CMD_CTRL> _img_cmd);
public:
	Saveframe* SetTime(std::string _time);
	Saveframe* SetChannel(int _ch);
	Saveframe* SetFrameCount(int _count);
	Saveframe* SetIpAddr(std::string _ipAddr);
public:
	 void SaveFrame2Disk(IplImage* img_t);
	 
	 std::string  initFileName();
	 
	 void initPath();

	 void init_pos_neg_path(int _pos_neg);

	 void init_normal_path(std::string _prefix="");

	 void init();

	 int isInitParam();
	 
	 void initParam();
public:
	Saveframe* start_record();
	Saveframe* stop_record();
	Saveframe* save_record(int _is_save);
};