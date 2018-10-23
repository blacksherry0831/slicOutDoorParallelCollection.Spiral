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
void ChannelsData::start_record()
{
	this->mTimeCurrent = QBase::SYS_getCurrentTime("yyyyMMddhhmmssdd");
	const int length = mChannelsData.size();
	for (size_t chi = 0; chi < length; chi++)
	{
		mChannelsData[chi]->start_record(this->mTimeCurrent);
	}
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void ChannelsData::stop_record()
{
	const int length = mChannelsData.size();
	for (size_t chi = 0; chi < length; chi++)
	{
		mChannelsData[chi]->stop_record();
	}
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void ChannelsData::save_record(int _is_save)
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
ChannelsData* ChannelsData::channelsData()
{
	return &gChannelsData;
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

/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/