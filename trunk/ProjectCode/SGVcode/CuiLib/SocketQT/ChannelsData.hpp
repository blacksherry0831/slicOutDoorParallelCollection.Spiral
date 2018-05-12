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
#include "exCircleData.hpp"
/*-----------------------------------*/
/**
*
*/
/*-----------------------------------*/

/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
class ChannelsData
{
public:
	ChannelsData();
	~ChannelsData();
private:
	std::vector<QSharedPointer<exCircleData>> mChannelsData;
public:
	QSharedPointer<exCircleData> getChannelData(int _ch);
	void init();
	void destory();
private:
	static ChannelsData gChannelsData;
public:
	static ChannelsData* channelsData();
};