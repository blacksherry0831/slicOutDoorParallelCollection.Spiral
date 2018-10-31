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
class ChannelsData4Show
{
private:
	ChannelsData4Show();
	~ChannelsData4Show();
private:
	static ChannelsData4Show gChannelsData;
	std::vector<QSharedPointer<exCircleData>> mChannelsData;

	QMutex mTimeMutex;
public:
	static ChannelsData4Show* getInstance();
public:
	
public:
	QSharedPointer<exCircleData> getChannelData(int _ch);
	void ConfigRecordImg(QSharedPointer<CMD_CTRL> _cmd);
	void EnqueueImg(QSharedPointer<CMD_CTRL> _cmd);
public:
	void start_record(QSharedPointer<CMD_CTRL> _cmd);
	void stop_record(QSharedPointer<CMD_CTRL> _cmd);
	void save_record(int _is_save);
	

};