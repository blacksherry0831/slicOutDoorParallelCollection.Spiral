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
void ChannelsData::EnqueueImgStartEnd(QSharedPointer<CMD_CTRL> _cmd)
{

	if ((_cmd->IsImgStart()) ||
		(_cmd->IsImgEnd())) {

		const int CHANNEL = _cmd->Channel();
		Q_ASSERT(CHANNEL >= 0 && CHANNEL <= 7);
		mChannelsData.at(CHANNEL)->setImg(_cmd);

	}

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
void ChannelsData::EnqueueImg(QSharedPointer<CMD_CTRL> _cmd)
{

	if (_cmd->IsImgStart()) {
		
	}else if (_cmd->IsImgEnd()) {
		
	}else if (_cmd->IsImgFrame()) {

		const int CHANNEL = _cmd->Channel();
		mChannelsData.at(CHANNEL)->setImg(_cmd);



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

