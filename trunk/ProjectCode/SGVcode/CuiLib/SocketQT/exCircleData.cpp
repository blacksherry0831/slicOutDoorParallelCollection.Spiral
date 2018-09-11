//#include "stdafx.h"
#include "exCircleData.hpp"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/

/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
exCircleData::exCircleData(int _ch)
{	
	this->mChannel = _ch;
	this->init();
}
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
exCircleData::~exCircleData()
{
	this->clear();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void exCircleData::clear()
{
		mAverage.clear();
		mSigma.clear();
		mStartTime = clock();
		mFrameCount = 0;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void exCircleData::init()
{
	mAverage = Q_NULLPTR;
	mSigma = Q_NULLPTR;
	mStartTime = clock();
	mFrameCount = 0;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void exCircleData::destory()
{
	this->clear();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void exCircleData::start_record(std::string _time_t)
{
	this->init();

	mSaveFrame.start_record()->SetTime(_time_t);
		
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void exCircleData::stop_record()
{
	this->clear();
	mSaveFrame.stop_record();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void exCircleData::SetSaveFrameCfg(QSharedPointer<CMD_CTRL> cmd_ctrl)
{
	const int CHANNEL = cmd_ctrl->Channel();
	std::string IPADDR = cmd_ctrl->mIpAddrRemote;
	assert(CHANNEL == mChannel);

	this->mSaveFrame.SetChannel(mChannel)->SetFrameCount(mFrameCount)->SetIpAddr(IPADDR);

	IplImage* img_t = cmd_ctrl->getIplimage();

	this->mSaveFrame.SaveFrame2Disk(img_t);

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void exCircleData::IncFrameCount()
{
	this->mFrameCount++;

	if (this->mFrameCount == 1) {
		this->mStartTime = clock();
	}
	else {
		this->mCurrentTime = clock();
	}
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
QSharedPointer<CMD_CTRL>  exCircleData::getImg()
{
	return this->getCmd();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void exCircleData::setImg(QSharedPointer<CMD_CTRL> cmd_ctrl)
{

#if _DEBUG
	const int CHANNEL = cmd_ctrl->Channel();
	assert(CHANNEL==mChannel);
#endif //
		
	this->SetSaveFrameCfg(cmd_ctrl);


#if TRUE

	this->setCmd(cmd_ctrl);	

	this->IncFrameCount();

#endif // TRUE

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
float exCircleData::fps()
{
	clock_t elapsed = mCurrentTime - mStartTime;

	float fps=(this->mFrameCount-1) / (elapsed/1000.0);

	//qDebug() << "FPS:" <<fps;

	return fps;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/