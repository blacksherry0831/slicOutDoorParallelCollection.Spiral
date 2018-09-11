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
	std::string mTimeCurrent;
public:
	static ChannelsData* channelsData();
public:
	QSharedPointer<exCircleData> getChannelData(int _ch);

	void start_record();
	void stop_record();


};