//#include "stdafx.h"
#include "ChannelsDataBase.hpp"

#include "QT_SDK_LIB/QBase.h"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
ChannelsDataBase ChannelsDataBase::gChannelsData;
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
ChannelsDataBase::ChannelsDataBase()
{
	mChannelsData.resize(CAMERA_CHANNELS);
	const int length = CAMERA_CHANNELS;
	for (size_t chi = 0; chi < length; chi++)
	{
		QSharedPointer<exCircleData> chidata = QSharedPointer<exCircleData>(new exCircleData(chi));
		mChannelsData[chi] = chidata;
	}

	this->mIsReceiving = FALSE;
}
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
ChannelsDataBase::~ChannelsDataBase()
{
	mChannelsData.clear();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
QSharedPointer<exCircleData> ChannelsDataBase::getChannelData(int _ch)
{
	QSharedPointer<exCircleData> circleData = mChannelsData.at(_ch);
	return circleData;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void ChannelsDataBase::EnqueueImgAll(QSharedPointer<CMD_CTRL> _cmd)
{
	this->MyImgAssert(_cmd);

	this->EnqueueCmd(_cmd);

}
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
void ChannelsDataBase::MyImgAssert(QSharedPointer<CMD_CTRL> _cmd)
{
	
	const int CHANNEL = _cmd->Channel();
	Q_ASSERT(CHANNEL >= 0 && CHANNEL <= 7);

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int ChannelsDataBase::QueueSize()
{
	int queue_size_t = 0;

	const int SIZE = mChannelsData.size();

	for (size_t chi = 0; chi <SIZE; chi++)
	{
		if (!mChannelsData[chi].isNull())
		{
			queue_size_t+= mChannelsData[chi]->QueueSize();
		}
		
	}

	return queue_size_t;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
ChannelsDataBase* ChannelsDataBase::getInstance()
{
	return &gChannelsData;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int ChannelsDataBase::IsReceiving() const
{
	return mIsReceiving;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void ChannelsDataBase::initInternalState_Rcv(QSharedPointer<CMD_CTRL> _cmd)
{
	if (_cmd->IsImgStart()) {

		this->mIsReceiving = TRUE;

	}
	else if (_cmd->IsImgEnd()) {

		this->mIsReceiving = FALSE;

	}
	else if (_cmd->IsImgFrame()) {

	}
	else {

		Q_ASSERT(FALSE);
	}
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void ChannelsDataBase::initInternalState_Record(QSharedPointer<CMD_CTRL> _cmd)
{
	if (_cmd->IsImgStart()) {
		
		this->start_record(_cmd);

	}else if (_cmd->IsImgEnd()) {
		
	}else if (_cmd->IsImgFrame()) {

	}else {
		Q_ASSERT(FALSE);
	}
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void ChannelsDataBase::EnqueueCmd(QSharedPointer<CMD_CTRL> _cmd)
{
		const int CHANNEL = _cmd->Channel();
		mChannelsData.at(CHANNEL)->setImg(_cmd);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int ChannelsDataBase::IsImgProcessDone()
{
	if (ChannelsDataBase::getInstance()->QueueSize()>0) {
		return FALSE;
	}

	return TRUE;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void ChannelsDataBase::start_record(QSharedPointer<CMD_CTRL> _cmd)
{
	const int CHANNEL = _cmd->Channel();
	Q_ASSERT(CHANNEL >= 0 && CHANNEL <= 7);
	Q_ASSERT(!_cmd->mCurrentCircleTime.empty());


	if (!mChannelsData[CHANNEL].isNull()) {
		mChannelsData[CHANNEL]->start_record(_cmd);
	}

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void ChannelsDataBase::ConfigInternalState(QSharedPointer<CMD_CTRL> _cmd)
{

	this->initInternalState_Rcv(_cmd);
	this->initInternalState_Record(_cmd);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/

