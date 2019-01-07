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
	QVector<int> mResult;
private:
	void clear();
	void assert_channel(QSharedPointer<CMD_CTRL> _cmd_ctrl);
public:
	void init();
	void destory();
	void start_record(QSharedPointer<CMD_CTRL> _cmd);
	void stop_record();
	void record(QSharedPointer<CMD_CTRL> _cmd_ctrl);
	void save_record(int _is_save);
private:
	void setResult(QSharedPointer<CMD_CTRL> _cmd_ctrl);
public:
	QSharedPointer<CMD_CTRL> mAverage;
	QSharedPointer<CMD_CTRL> mSigma;
private:
	
	void IncFrameCount();
public:
	QSharedPointer<CMD_CTRL> getImg();
	void setImg(QSharedPointer<CMD_CTRL> _cmd_ctrl);
	float fps();
	int IsCrack();
};