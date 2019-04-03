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
#include "ChannelsDataBase.hpp"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
class ChannelsData: public ChannelsDataBase
{
private:
	ChannelsData();
	~ChannelsData();
private:
	static ChannelsData gChannelsData;
private:
	void MyImgAssert(QSharedPointer<CMD_CTRL> _cmd);
public:
	static ChannelsData* getInstance();
	int IsReceiving() const;
public:
	QSharedPointer<exCircleData> getChannelData(int _ch) const;
	
	void EnqueueImgAll(QSharedPointer<CMD_CTRL> _cmd);

	int  QueueSize();
	int  IsImgProcessDone();
};