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
void QtImgProcess::ImgProcessIpl_Base(IplImage * _img, const IMG_PROC _img_proc)
{
	const CvRect rect = cvGetImageROI(_img);
	cvResetImageROI(_img);
#if TRUE
	if (_img_proc.ImgProc_Binary) {
		const float threshold = 0.5;
		const float max_value = 255;
		const int threshold_type = CV_THRESH_BINARY;
		cvThreshold(_img, _img, threshold, max_value, threshold_type);
	}

	if (_img_proc.ImgProc_Denoise) {
		CrackDetection::Binary2Smooth_Line(_img);
	}
#endif // TRUE
	cvSetImageROI(_img, rect);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
std::vector<float> QtImgProcess::ImgProcessIpl(IplImage * _img,const IMG_PROC _img_proc)
{
	std::vector<float> f_v_t;
	const CvRect rect = cvGetImageROI(_img);
	cvResetImageROI(_img);
			
#if TRUE

	if (_img_proc.ImgProc_Binary_Thickly_Classify) {
		
		f_v_t = CrackDetection::GetFeatureFast(_img);
		
	}else if (_img_proc.ImgProc_Hough_Block_Classify){

		std::vector<Line2Point> lines_t;
		const CvScalar color_t=CV_RGB(255,255,255);
		const int thickness_t=1;
		f_v_t = CrackDetection::GetCrackImgHoughBlockFeatureBase(_img, _img, lines_t, _img,color_t,thickness_t);

	}else{

	}

#endif // TRUE

	cvSetImageROI(_img, rect);
	return f_v_t;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
std::vector<float> QtImgProcess::ImgProcessCMD_CTRL(QSharedPointer<CMD_CTRL> _cmd,const IMG_PROC _img_proc)
{
	std::vector< float> feature_t;

	if (_cmd->IsImgStart()) {

	}else if (_cmd->IsImgEnd()) {

	}else if (_cmd->IsImgFrame()) {

		Q_ASSERT(_img_proc.CurrentChannel>=0 && _img_proc.CurrentChannel<8);

		IplImage* img_t = _cmd->getIplimage();
		
		ImgProcessIpl_RemoveBorder(img_t, _img_proc);

		ImgProcessIpl_Base(img_t, _img_proc);

		feature_t =ImgProcessIpl(img_t,_img_proc);

		ImgProcessIpl_Draw(img_t, _img_proc);
		
	}else {

	}

	return feature_t;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtImgProcess::ImgProcessIpl_RemoveBorder(IplImage * _img, const IMG_PROC _img_proc)
{
	const int WIDTH = _img->width;
	const int HEIGHT = _img->height;
	
	std::vector<int> row_t;
	std::vector<int> col_t;

	CrackDetection::Init_RemoveBorder(WIDTH,HEIGHT,row_t,col_t);

	CrackDetection::IMG_RemoveBorder(_img, row_t, col_t);

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtImgProcess::ImgProcessIpl_Draw(IplImage * _img, const IMG_PROC _img_proc)
{
	const int Channel = _img_proc.CurrentChannel;
	std::vector<float> f_v_t;
	const CvRect rect = cvGetImageROI(_img);
	cvResetImageROI(_img);
#if TRUE
	if (_img_proc.ShowCutArea) {
		QtThread8VideoProcess::DrawArea(_img, rect);
		DrawFPS(_img,_img_proc.CurrentChannel);
	}
#endif // TRUE
	cvSetImageROI(_img, rect);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int QtImgProcess::ImgProcessCMD_CTRL_AttachResult(
	QSharedPointer<CMD_CTRL>& _cmd,
	const IMG_PROC & _img_proc,
	const std::vector<float>& _result,
	const int _classify)
{
	_cmd->mFeature = _result;
	_cmd->mImgProc = _img_proc;
	_cmd->mClassify = _classify;

	return 0;
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
			
			QSharedPointer<CMD_CTRL> cmd_4_show_t =QSharedPointer<CMD_CTRL>(new CMD_CTRL());

			cmd_4_show_t->Init2BgrBuffer(cmd_t.data());
			
			IMG_PROC_record_work_flow(cmd_t);
			
			std::vector<float> feature_t= ImgProcessCMD_CTRL(cmd_t, mImgProc);
			
			const int classify_t = predictFeature(feature_t,mImgProc);
			
			this->ImgProcessCMD_CTRL_AttachResult(cmd_t, mImgProc, feature_t,classify_t);
			
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
int QtImgProcess::predictFeature(std::vector<float> _f, const IMG_PROC _img_proc)
{
	int Classify_t=0;

	if (_f.size()==0)
	{
		return Classify_t = 0;
	}
	
	if (_img_proc.ImgProc_Binary_Thickly_Classify) {

		float THRESHOLD =mThresholdClassifyThickly;
		float FEATURE = _f.at(0);
		if (FEATURE >= 0 - 1E-6) {
			Classify_t = FEATURE < THRESHOLD ? CrackDetection::NEG_CLASS : CrackDetection::POS_CLASS;
		}

	}else if (_img_proc.ImgProc_Hough_Block_Classify) {
			Q_ASSERT(_f.size()!=0);
			const std::vector<float> th_t = CrackDetection::getThresholdInOutCircleHoughBlock(mIpAddrProperty);
			Classify_t = CrackDetection::predict_in_out_block(_f,mIpAddrProperty,th_t);

	}else {

	}

	return Classify_t;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtImgProcess::SetImgProc_IpAddrProperty(std::string _property)
{
	this->mIpAddrProperty = _property;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtImgProcess::SetImgProcSigma(int _sigma)
{
	this->mImgProc.Sigma = _sigma;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtImgProcess::SetImgProcCutArea(int _param)
{
	this->mImgProc.ShowCutArea = _param;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtImgProcess::SetImgProcBinary(const int _param)
{
	this->mImgProc.ImgProc_Binary = _param;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtImgProcess::SetImgProcDenoise(const int _param)
{
	this->mImgProc.ImgProc_Denoise = _param;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtImgProcess::SetImgProcBinaryClassifyThickly(int _param)
{
	this->mImgProc.ImgProc_Binary_Thickly_Classify = _param;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtImgProcess::SetImgProcHoughBlockClassify(int _param)
{
	this->mImgProc.ImgProc_Hough_Block_Classify = _param;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtImgProcess::DrawFPS(IplImage * _img,const int _ch)
{
	const int WIDTH = _img->width;
	const int HEIGHT = _img->height;
	const ChannelsData*    channels_data_t = ChannelsData::getInstance();
	const float fps = channels_data_t->getChannelData(_ch)->fps();

	CvFont font;
	const int thickness = 2;
	const float scale = 1;
	cvInitFont(&font, CV_FONT_HERSHEY_COMPLEX, scale, scale, 0, thickness);
	
	std::string text_t;

	text_t.append("FPS: ").append(Base::float2str(fps));

	cvPutText(_img,text_t.data(), cvPoint(WIDTH  * 0.7, HEIGHT * 0.1),&font,CV_RGB(255,255,255));
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtImgProcess::SetClassifyThicklyThreshold(float _threshold)
{
	Q_ASSERT(_threshold>=0);
	mThresholdClassifyThickly = _threshold;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
float QtImgProcess::GetClassifyThicklyThreshold()
{
	return  mThresholdClassifyThickly;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtImgProcess::run()
{
	
	this->setPriorityMy();

#if _DEBUG
	const ChannelsData*    channels_data_t = ChannelsData::getInstance();
#endif // _DEBUG
		
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
	const int CHANNEL = _cmd->Channel();
	Q_ASSERT(CHANNEL >= 0 && CHANNEL < 8);
	if (_cmd->IsImgStart()) {
		emit img_stat_ex(_cmd->CmdStat(), CHANNEL, 0);
	}
	else if (_cmd->IsImgEnd()) {
		emit img_stat_ex(_cmd->CmdStat(), CHANNEL, 0);
	}else if (_cmd->IsImgFrame()) {
	
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
