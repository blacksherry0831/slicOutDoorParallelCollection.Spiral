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
	this->clear();
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
	this->setCmd(cmd_ctrl);
	this->mFrameCount++;

	if (this->mFrameCount==1){
		this->mStartTime = clock();
	}else {
		this->mCurrentTime = clock();	
	}


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