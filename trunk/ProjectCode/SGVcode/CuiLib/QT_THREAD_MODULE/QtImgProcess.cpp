/*-------------------------------------*/
#include "QtImgProcess.hpp"
/*-------------------------------------*/
#include "QT_THREAD_MODULE/QtThread8VideoProcess.hpp"
/*-------------------------------------*/
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
	this->mSleepTime = 0;
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
float QtImgProcess::ImgProcessIpl(IplImage * _img,const IMG_PROC _img_proc)
{
	float feature_t = -1;

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
#if 0
		TimeMeasure tm;
		tm.start("th");
#endif // 0
			cvThreshold(_img, _img, threshold, 255, threshold_type);//·§Öµ100	
#if 0
		tm.stop();
#endif // 0

	
	}

	if (_img_proc.ShowBinaryClassifyThickly) {
		feature_t=CrackDetection::GetFeatureFastEx(_img, 11, 7, 2);
	}

	cvSetImageROI(_img, rect);

	return feature_t;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtImgProcess::ImgProcInner(IplImage * _img)
{

	const int WIDTH = _img->width;
	const int HEIGHT = _img->height;
	if ( WIDTH== 960 && HEIGHT==544) {
	
		for (size_t hi = HEIGHT-4; hi < HEIGHT; hi++)
		{
			cvLine(_img, cvPoint(0, hi), cvPoint(WIDTH, hi), cvScalar(0));
		}
	
	}
	

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int QtImgProcess::ImgProcessCMD_CTRL(QSharedPointer<CMD_CTRL> _cmd,const IMG_PROC _img_proc)
{

	if (_cmd->IsImgStart()) {

	}else if (_cmd->IsImgEnd()) {

	}else if (_cmd->IsImgFrame()) {

		Q_ASSERT(_img_proc.CurrentChannel>=0 && _img_proc.CurrentChannel<8);

		IplImage* img_t = _cmd->getIplimage();
#if 1
		this->ImgProcInner(img_t);
#endif // 1

		const float feature_t=ImgProcessIpl(img_t,_img_proc);
		if (_img_proc.ShowBinaryClassifyThickly)
		{
			_cmd->mFeature = feature_t;
			_cmd->mImgProc = _img_proc;
		}

	}else {

	}

	return TRUE;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtImgProcess::processImgCmd()
{
	ChannelsData*    channels_data_t = ChannelsData::getInstance();
	QSharedPointer<exCircleData> circleData = channels_data_t->getChannelData(mImgProc.CurrentChannel);

	if (circleData->QueueSize()) {
			
			QSharedPointer<CMD_CTRL> cmd_t = circleData->getImg();
			IMG_PROC_record_work_flow(cmd_t);
			const float feature_t = ImgProcessCMD_CTRL(cmd_t, mImgProc);
			
			ChannelsData4Show::getInstance()->ConfigRecordImg(cmd_t);		
			this->EnqueueImg4ShowUI(cmd_t);	

	}else {
		this->SleepMy();
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
	
	this->setPriorityMy();

	const ChannelsData*    channels_data_t = ChannelsData::getInstance();
	
	while (M_THREAD_RUN){	

#if 0
		if (channels_data_t->IsReceiving() && JQCPUMonitor::cpuUsagePercentageIn5Second()>0.99) {
			//TCP now is transfer , cpu is busy
			this->SleepMy();
	}
		else {
			this->processImgCmd();
		}
#else

			this->processImgCmd();

#endif // 0
	


		
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
void QtImgProcess::EnqueueImg4ShowUI(QSharedPointer<CMD_CTRL> _cmd)
{
	ChannelsData4Show::getInstance()->EnqueueImg(_cmd);
	this->emit_img_signals(_cmd);
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

#if USE_THREAD_CRACK_PRIORITY
QThread::currentThread()->setPriority(QThread::Priority::IdlePriority);
#endif // 0

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void  QtImgProcess::SleepMy(int _ms)
{
	const int SLEEP_10MS = 10;
	do {

		QThread::msleep(SLEEP_10MS);
		_ms -= SLEEP_10MS;
		mSleepTime += SLEEP_10MS;

	} while (M_THREAD_RUN  &&_ms > 0);

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void  QtImgProcess::IMG_PROC_Start()
{
	mImgProc.Start=TRUE;
	mImgProc.Frames = 0;
	mImgProc.End = FALSE;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void  QtImgProcess::IMG_PROC_End()
{
	if (mImgProc.Start==TRUE)
	{	
		mImgProc.End = TRUE;

	}

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int  QtImgProcess::IMG_PROC_Done()
{
	if (mImgProc.Start == TRUE  &&
		mImgProc.End == TRUE	&&
		mImgProc.Frames >0) {
		return TRUE;
	}
	return FALSE;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtImgProcess::IMG_PROC_record_work_flow(QSharedPointer<CMD_CTRL> _cmd)
{
	if (_cmd->IsImgStart()) {
		IMG_PROC_Start();
	}
	else if (_cmd->IsImgEnd()) {
		IMG_PROC_End();
	}
	else if (_cmd->IsImgFrame()) {
		IMG_PROC_Frames();
	}
	else {
		Q_ASSERT(0);
	}
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void  QtImgProcess::IMG_PROC_Frames()
{
	mImgProc.Frames++;
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
