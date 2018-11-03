//#include "stdafx.h"
#include "QtImgProcess.hpp"
#include "QtThread8VideoProcess.hpp"
#include "MY_SDK_LIB/CrackDetection.hpp"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
QtImgProcess::QtImgProcess(int _channel)
{
#if TRUE
	memset(&mImgProc, 0, sizeof(IMG_PROC));
#endif // TRUE
	mImgProc.CurrentChannel = _channel;
	mImgProc.ThresholdClassifyThickly = 0.02;
	
	
	this->M_THREAD_RUN = true;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
QtImgProcess::~QtImgProcess(void)
{
	
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtImgProcess::ImgProcessIpl(IplImage * _img,const IMG_PROC _img_proc)
{
	const CvRect rect = cvGetImageROI(_img);
	cvResetImageROI(_img);

#if TRUE
	if (_img_proc.ShowCutArea) {
		QtThread8VideoProcess::DrawArea(_img, rect);
	}
#endif // TRUE

	if (_img_proc.ShowBinaryImg) {

		float threshold = 0.5;
		float max_value = 255;
		int threshold_type = CV_THRESH_BINARY;
		TimeMeasure tm;
		tm.start("th");
			cvThreshold(_img, _img, threshold, 255, threshold_type);//·§Öµ100	
		tm.stop();
	}

	if (_img_proc.ShowBinaryClassifyThickly) {
		CrackDetection::GetFeatureFastEx(_img, 11, 7, 2);
	}

	cvSetImageROI(_img, rect);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtImgProcess::ImgProcessCMD_CTRL(QSharedPointer<CMD_CTRL> _cmd,const IMG_PROC _img_proc)
{

	if (_cmd->IsImgStart()) {

	}else if (_cmd->IsImgEnd()) {

	}else if (_cmd->IsImgFrame()) {


#if TRUE
			if (_img_proc.CurrentChannel>=0
					&&_img_proc.CurrentChannel<8)
#endif // TRUE
		{

			IplImage* img_t = _cmd->getIplimage();
			ImgProcessIpl(img_t,_img_proc);

		}		

	}else {

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
void QtImgProcess::SetImgProcCutArea(int _param)
{
	this->mImgProc.ShowBinaryClassifyThickly = _param;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtImgProcess::SetImgProcBinaryImg(int _param)
{
	this->mImgProc.ShowBinaryImg = _param;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtImgProcess::SetImgProcBinaryClassifyThickly(int _param)
{
	this->mImgProc.ShowBinaryClassifyThickly = _param;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtImgProcess::SetClassifyThicklyThreshold(float _threshold)
{
	Q_ASSERT(_threshold>=0);
	this->mImgProc.ThresholdClassifyThickly = _threshold;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
float QtImgProcess::GetClassifyThicklyThreshold()
{
	return  this->mImgProc.ThresholdClassifyThickly;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtImgProcess::run()
{
	const int SleepTime = 100;

	this->setPriorityMy();

	ChannelsData*    channels_data_t = ChannelsData::getInstance();

	QSharedPointer<exCircleData> circleData = channels_data_t->getChannelData(mImgProc.CurrentChannel);

	while (M_THREAD_RUN){		
			

			if (channels_data_t->IsReceiving()){
				//TCP now is transfer
				if (JQCPUMonitor::cpuUsagePercentageIn5Second()>0.70) {
			
					QThread::msleep(SleepTime);
			
				}
			}


			if (circleData->QueueSize()) {

							QSharedPointer<CMD_CTRL> cmd_t = circleData->getImg();
#if 1
							ChannelsData4Show::getInstance()->ConfigRecordImg(cmd_t);
#endif // 0								

							ImgProcessCMD_CTRL(cmd_t, mImgProc);
								
							ChannelsData4Show::getInstance()->EnqueueImg(cmd_t);

							this->emit_img_signals(cmd_t);				

							//QThread::yieldCurrentThread();
			}else {
						QThread::msleep(SleepTime);
			}

	}

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtImgProcess::startServer()
{
	this->M_THREAD_RUN = true;
	
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtImgProcess::closeServer()
{
	this->M_THREAD_RUN = false;

}
/*-------------------------------------*/
/**


*
*/
/*-------------------------------------*/
void QtImgProcess::emit_img_signals(QSharedPointer<CMD_CTRL> _cmd)
{
	if (_cmd->IsImgStart()) {
		emit img_stat_ex(_cmd->CmdStat(), 0, 0);
	}
	else if (_cmd->IsImgEnd()) {
		emit img_stat_ex(_cmd->CmdStat(), 0, 0);
	}else if (_cmd->IsImgFrame()) {
		const int CHANNEL = _cmd->Channel();
		QSharedPointer<exCircleData> circleData = ChannelsData::getInstance()->getChannelData(CHANNEL);
		emit img_stat_ex(_cmd->CmdStat(), CHANNEL, circleData->QueueSize());
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
void QtImgProcess::setPriorityMy()
{
	QThread::currentThread()->setPriority(QThread::Priority::IdlePriority);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
