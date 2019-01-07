#pragma once

#include "cpp_stl.h"
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
#include "exCircleData.hpp"
#include "Saveframe.hpp"
#include "CMD_CTRL.hpp"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
class ChannelsDataBase
{
protected:
	ChannelsDataBase();
	~ChannelsDataBase();
protected:
	static ChannelsDataBase gChannelsData;
	std::vector<QSharedPointer<exCircleData>> mChannelsData;
	int mIsReceiving;
private:
	void EnqueueCmd(QSharedPointer<CMD_CTRL> _cmd);
	void MyImgAssert(QSharedPointer<CMD_CTRL> _cmd);
public:
	static ChannelsDataBase* getInstance();
	int IsReceiving() const;
public:
	QSharedPointer<exCircleData> getChannelData(int _ch);
	
	void EnqueueImgAll(QSharedPointer<CMD_CTRL> _cmd);

	int  QueueSize();
	int  IsImgProcessDone();
};