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
public:
	static ChannelsData* getInstance();
public:
	QSharedPointer<exCircleData> getChannelData(int _ch);
	void EnqueueImg(QSharedPointer<CMD_CTRL> _cmd);
	void EnqueueImgStartEnd(QSharedPointer<CMD_CTRL> _cmd);
	int  QueueSize();

};