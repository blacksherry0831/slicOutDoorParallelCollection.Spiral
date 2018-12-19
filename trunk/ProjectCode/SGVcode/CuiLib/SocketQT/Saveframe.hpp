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
#if defined(QT_VERSION)
#include <QtXml>
#endif // defined(QT_VERSION)
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

	QSharedPointer<QDomDocument> mXml;

	std::string mFeatureFileName;
	std::string mIpAddrPath;
	
	std::string mTimePath;
	std::string mChannelPath;

	std::string mImageName;

	std::string mImageNameFileFullPath;
	
private:
	QSharedPointer<CMD_CTRL> mCmd;
	
	std::string mIpAddrRemote;
	int mChannel;
	int mFrameCount;
	int mIsSaveFrame;
private:
	void init_xml();
	void save_xml(QString _save_full_path, QSharedPointer<QDomDocument> _Xml);
	void add_xml();
public:
	Saveframe* SetImgCmd(QSharedPointer<CMD_CTRL> _img_cmd);
private:
	Saveframe* SetStartTime(std::string _time);
public:
	 void SaveFrame2Disk(IplImage* img_t);
	 
	 std::string  initFileName();
	 
	 void initPath();

	 void init_pos_neg_path(int _pos_neg);

	 void init_normal_path(std::string _prefix="");

	 void init_ip_time_path();

	 void init();

	 int isInitParam();
	 
	 void initParam();
public:
	Saveframe* start_record(QSharedPointer<CMD_CTRL> _img_cmd);
	Saveframe* stop_record();
	Saveframe* save_record(int _is_save);
};