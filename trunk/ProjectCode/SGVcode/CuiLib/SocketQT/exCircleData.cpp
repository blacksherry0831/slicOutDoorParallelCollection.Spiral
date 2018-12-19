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
	mIsSaveFrame = FALSE;
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
void exCircleData::start_record(QSharedPointer<CMD_CTRL> _cmd)
{
	this->init();
	
	mSaveFrame.start_record(_cmd);
		
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void exCircleData::stop_record()
{
	mSaveFrame.stop_record();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void exCircleData::save_record(int _is_save)
{
	mSaveFrame.save_record(_is_save);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void exCircleData::record(QSharedPointer<CMD_CTRL> _cmd_ctrl)
{
	this->assert_channel(_cmd_ctrl);

	if (_cmd_ctrl->IsImgFrame()) {
			
			IplImage* img_t = _cmd_ctrl->getIplimage();

				this->mSaveFrame.SetImgCmd(_cmd_ctrl)
					->SaveFrame2Disk(img_t);

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
void exCircleData::setImg(QSharedPointer<CMD_CTRL> _cmd_ctrl)
{

	this->assert_channel(_cmd_ctrl);

	this->setCmd(_cmd_ctrl);	

	this->IncFrameCount();

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
void exCircleData::assert_channel(QSharedPointer<CMD_CTRL> _cmd_ctrl)
{
#if _DEBUG
	if (_cmd_ctrl->IsImgFrame()) {
		const int CHANNEL = _cmd_ctrl->Channel();
		Q_ASSERT(CHANNEL == mChannel);
	}
#endif
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/