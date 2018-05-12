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
class exCircleData
{
public:
	enum  CIRCLE_STATUS
	{
		INVALID=0,
		VALID=1
	};
public:
	exCircleData(int _ch);
	~exCircleData();
private:
	QMutex mutex;
	CIRCLE_STATUS STATUS;
public:
	int Channel;
	QSharedPointer<CMD_CTRL> mAverage;
	QSharedPointer<CMD_CTRL> mSigma;
	QQueue<QSharedPointer<CMD_CTRL>> mImageFrame;
public:
	std::vector<exCircleData> ChannelData;
private:
	void clear();
public:
	void init();
	void destory();
	int IsValid();
public:
	QSharedPointer<CMD_CTRL> getImg();
	void setImg(QSharedPointer<CMD_CTRL> cmd_ctrl);

};