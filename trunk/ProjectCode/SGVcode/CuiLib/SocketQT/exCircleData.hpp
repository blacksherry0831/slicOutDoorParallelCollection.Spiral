#pragma once

#include "cpp_stl.h"
/*-----------------------------------*/
/**
*
*/
/*-----------------------------------*/
#include <QtCore>
#include <QtNetwork>
#include <QSharedPointer>
#include <QThread>
/*-----------------------------------*/
/**
*
*/
/*-----------------------------------*/
#include "CMD_CTRL_Q.hpp"
#include "Saveframe.hpp"
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
class exCircleData : public CMD_CTRL_Q
{
	
public:
	exCircleData(int _ch);
	~exCircleData();
private:
	clock_t mStartTime;
	clock_t mCurrentTime;
	int mFrameCount;
	int mChannel;
	int mIsSaveFrame;
	Saveframe mSaveFrame;
private:
	void clear();
public:
	void init();
	void destory();
	void start_record(std::string _time_t);
	void stop_record();
	void save_record(int _is_save);
public:
	QSharedPointer<CMD_CTRL> mAverage;
	QSharedPointer<CMD_CTRL> mSigma;
private:
	void SetSaveFrameCfg(QSharedPointer<CMD_CTRL> _cmd_ctrl);
	void IncFrameCount();
public:
	QSharedPointer<CMD_CTRL> getImg();
	void setImg(QSharedPointer<CMD_CTRL> cmd_ctrl);
	float fps();

};