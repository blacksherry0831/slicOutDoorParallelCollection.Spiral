//#include "stdafx.h"
#include "ChannelsData.hpp"

#include "QT_SDK_LIB/QBase.h"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
ChannelsData ChannelsData::gChannelsData;
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
ChannelsData::ChannelsData()
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
ChannelsData::~ChannelsData()
{
	mChannelsData.clear();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
QSharedPointer<exCircleData> ChannelsData::getChannelData(int _ch)
{
	QSharedPointer<exCircleData> circleData = mChannelsData.at(_ch);
	return circleData;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void ChannelsData::EnqueueImgAll(QSharedPointer<CMD_CTRL> _cmd)
{
	this->MyImgAssert(_cmd);

	this->EnqueueCmd(_cmd);

	if (_cmd->IsImgStart()) {

		this->mIsReceiving = TRUE;

	}else if (_cmd->IsImgEnd()) {

		this->mIsReceiving = FALSE;

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

/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void ChannelsData::MyImgAssert(QSharedPointer<CMD_CTRL> _cmd)
{
	
	const int CHANNEL = _cmd->Channel();
	Q_ASSERT(CHANNEL >= 0 && CHANNEL <= 7);

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int ChannelsData::QueueSize()
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
ChannelsData* ChannelsData::getInstance()
{
	return &gChannelsData;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int ChannelsData::IsReceiving() const
{
	return mIsReceiving;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void ChannelsData::EnqueueCmd(QSharedPointer<CMD_CTRL> _cmd)
{
		const int CHANNEL = _cmd->Channel();
		mChannelsData.at(CHANNEL)->setImg(_cmd);
	
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

