//#include "stdafx.h"
#include "ChannelsData.hpp"
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
ChannelsData* ChannelsData::channelsData()
{
	return &gChannelsData;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void ChannelsData::init()
{	
	const int length = mChannelsData.size();
	for (size_t chi = 0; chi < length; chi++)
	{
		mChannelsData[chi]->init();
	}
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void ChannelsData::destory()
{
	const int length = mChannelsData.size();
	for (size_t chi = 0; chi < length; chi++)
	{
		mChannelsData[chi]->destory();
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