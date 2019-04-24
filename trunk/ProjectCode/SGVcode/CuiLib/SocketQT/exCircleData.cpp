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
		mStartTime = QTime::currentTime();
		mFrameCount = 0;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void exCircleData::initFrameState_FPS()
{
	mStartTime = QTime::currentTime();
	mFrameCount = 0;
	mCurrentTime = QTime::currentTime();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void exCircleData::init()
{
	//this->mStartFrameCount = 1;
	mAverage = Q_NULLPTR;
	mSigma = Q_NULLPTR;
	this->initFrameState_FPS();
	mIsSaveFrame = FALSE;
	mResult.clear();
	this->mResultPos=0;
	this->mResultNeg=0;
	this->mResultUnDo=0;
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

				this->mSaveFrame.init_CMD_CTRL(_cmd_ctrl)
					->SaveFrame2Disk(img_t);

	}

	this->setResult(_cmd_ctrl);

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void exCircleData::IncFrameCount()
{
	this->mFrameCount++;

	if (this->mFrameCount == START_FRAME_COUNT) {
		this->mStartTime = QTime::currentTime();
	}
	else {
		this->mCurrentTime = QTime::currentTime();
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
	
	float msecs= mStartTime.msecsTo(mCurrentTime);
	float secs = msecs / 1000;
	float frames = this->mFrameCount - START_FRAME_COUNT;
	float fps=frames / secs;

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
void exCircleData::setResult(QSharedPointer<CMD_CTRL> _cmd_ctrl)
{

	const int   CLSAAIFY = _cmd_ctrl->mClassify;

	if (_cmd_ctrl->IsImgFrame()) {
			this->mResult.push_back(CLSAAIFY);
#if 1
			if (CLSAAIFY == 1) {
				mResultPos++;
			}
			else if (CLSAAIFY == 1) {
				mResultNeg++;
			}
			else if (CLSAAIFY == 0) {
				mResultUnDo++;
			}
			else {
				Q_ASSERT(0);
			}
#endif

	}

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int exCircleData::Pos()
{
	return this->mResultPos;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int exCircleData::IsCrack()
{
	int pos_count = 0;
	int neg_count = 0;
	int undo_count = 0;
	const double  BLANK = 0.05;
	const int TOTAL = mResult.size();
	const int START = TOTAL*BLANK;
	const int END = TOTAL*(1-BLANK);

			for (int i = START;i< END; i++){
						const int reslut_classify_t = mResult.at(i);
						if (reslut_classify_t == 1) {
							pos_count++;
						}else if (reslut_classify_t == -1){
							neg_count++;
						}else if (reslut_classify_t == 0){
							undo_count++; 
						}else{
							Q_ASSERT(0);
						}		
				}

			

			if (undo_count==0){
				printf("P:%d , N: %d \n",pos_count,neg_count);
				return pos_count;
			}else {			
				printf("UNDO: %d \n", undo_count);
				return TRUE;//спик╨ш
			}
			
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/