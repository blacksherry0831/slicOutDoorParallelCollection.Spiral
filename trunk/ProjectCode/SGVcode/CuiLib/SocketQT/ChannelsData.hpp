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
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
class ChannelsData
{
private:
	ChannelsData();
	~ChannelsData();
private:
	static ChannelsData gChannelsData;
	std::vector<QSharedPointer<exCircleData>> mChannelsData;
	int mIsReceiving;
private:
	void EnqueueCmd(QSharedPointer<CMD_CTRL> _cmd);
	void MyImgAssert(QSharedPointer<CMD_CTRL> _cmd);
public:
	static ChannelsData* getInstance();
	int IsReceiving() const;
public:
	QSharedPointer<exCircleData> getChannelData(int _ch);
	
	void EnqueueImgAll(QSharedPointer<CMD_CTRL> _cmd);

	int  QueueSize();

};