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
public:
	void EnqueueCmd(QSharedPointer<CMD_CTRL> _cmd);
private:
	void MyImgAssert(QSharedPointer<CMD_CTRL> _cmd);
public:
	static ChannelsDataBase* getInstance();
	int IsReceiving() const;
public:
	void initInternalState_Rcv(QSharedPointer<CMD_CTRL> _cmd);
	void initInternalState_Record(QSharedPointer<CMD_CTRL> _cmd);
public:
	QSharedPointer<exCircleData> getChannelData(int _ch);
	
	void EnqueueImgAll(QSharedPointer<CMD_CTRL> _cmd);

	int  QueueSize();
	int  IsImgProcessDone();
public:
	void start_record(QSharedPointer<CMD_CTRL> _cmd);
	void ConfigInternalState(QSharedPointer<CMD_CTRL> _cmd);

};