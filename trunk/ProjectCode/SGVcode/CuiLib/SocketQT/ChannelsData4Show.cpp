//#include "stdafx.h"
#include "ChannelsData4Show.hpp"

#include "QT_SDK_LIB/QBase.h"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
ChannelsData4Show ChannelsData4Show::gChannelsData;
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
ChannelsData4Show::ChannelsData4Show()
{
	mChannelsData.resize(CAMERA_CHANNELS);
	const int length = CAMERA_CHANNELS;
	for (size_t chi = 0; chi < length; chi++)
	{
		QSharedPointer<exCircleData> chidata = QSharedPointer<exCircleData>(new exCircleData(chi));
		mChannelsData[chi] = chidata;
	}
}
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
ChannelsData4Show::~ChannelsData4Show()
{
	mChannelsData.clear();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
QSharedPointer<exCircleData> ChannelsData4Show::getChannelData(int _ch)
{
	QSharedPointer<exCircleData> circleData = mChannelsData.at(_ch);
	return circleData;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void ChannelsData4Show::ConfigRecordImg(QSharedPointer<CMD_CTRL> _cmd)
{
	

	if (_cmd->IsImgStart()) {

		this->start_record(_cmd);

	}else if (_cmd->IsImgEnd()) {

		this->stop_record(_cmd);

	}else if (_cmd->IsImgFrame()) {

		this->record(_cmd);

	}else{	
		Q_ASSERT(FALSE);
	}
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void ChannelsData4Show::EnqueueImg(QSharedPointer<CMD_CTRL> _cmd)
{
	if (_cmd->IsImgFrame()) {
		const int CHANNEL = _cmd->Channel();

		mChannelsData.at(CHANNEL)->setImg(_cmd);

	}
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
ChannelsData4Show* ChannelsData4Show::getInstance()
{
	return &gChannelsData;
}
/*-------------------------fr------------*/
/**
*
*/
/*-------------------------------------*/
void ChannelsData4Show::record(QSharedPointer<CMD_CTRL> _cmd)
{
	if (_cmd->IsImgFrame()) {
		const int CHANNEL = _cmd->Channel();

		mChannelsData.at(CHANNEL)->record(_cmd);

	}
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void ChannelsData4Show::start_record(QSharedPointer<CMD_CTRL> _cmd)
{	
	const int CHANNEL = _cmd->Channel();
	Q_ASSERT(CHANNEL >= 0 && CHANNEL <= 7);
	Q_ASSERT(!_cmd->mCurrentCircleTime.empty());
	

	if (!mChannelsData[CHANNEL].isNull()){
			mChannelsData[CHANNEL]->start_record(_cmd);
	}	

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void ChannelsData4Show::stop_record(QSharedPointer<CMD_CTRL> _cmd)
{
	const int CHANNEL = _cmd->Channel();

	if (!mChannelsData[CHANNEL].isNull()){
		
		mChannelsData[CHANNEL]->stop_record();
	}

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void ChannelsData4Show::save_record(int _is_save)
{
	const int length = mChannelsData.size();
	for (size_t chi = 0; chi < length; chi++)
	{
		mChannelsData[chi]->save_record(_is_save);
	}

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int ChannelsData4Show::HaveCrack()
{
	const int SIZE = mChannelsData.size();

	for (size_t chi = 0; chi <SIZE; chi++)
	{
		if (!mChannelsData[chi].isNull())
		{
			if (mChannelsData[chi]->IsCrack()) {
				return TRUE;
			}
		}
	}

	return FALSE;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int  ChannelsData4Show::QueueSize()
{
	int queue_size_t = 0;

	const int SIZE = mChannelsData.size();

	for (size_t chi = 0; chi <SIZE; chi++)
	{
		if (!mChannelsData[chi].isNull())
		{
			queue_size_t += mChannelsData[chi]->QueueSize();
		}

	}

	return queue_size_t;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int ChannelsData4Show::IsImgProcessDone()
{
	if (ChannelsData4Show::getInstance()->QueueSize()>0) {
		return FALSE;
	}

	return TRUE;
}
