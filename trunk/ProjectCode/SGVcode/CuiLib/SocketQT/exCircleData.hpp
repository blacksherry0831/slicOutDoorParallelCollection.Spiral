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
private:
	void clear();
public:
	void init();
	void destory();
public:
	QSharedPointer<CMD_CTRL> mAverage;
	QSharedPointer<CMD_CTRL> mSigma;
	
public:
	QSharedPointer<CMD_CTRL> getImg();
	void setImg(QSharedPointer<CMD_CTRL> cmd_ctrl);
	float fps();

};