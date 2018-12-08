//#include "stdafx.h"
#include "QtThread8VideoProcess.hpp"
/*-------------------------------------*/
#include "SocketQT/ChannelsData4Show.hpp"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
const unsigned char QtThread8VideoProcess::TaskMap[CAMERA_CHANNELS] = {0,1,2,3,4,5,6,7};
QtThread8VideoProcess* QtThread8VideoProcess::TaskObj[CAMERA_CHANNELS] = {Q_NULLPTR};
QThreadPool   QtThread8VideoProcess::ThreadPool;
bool QtThread8VideoProcess::M_THREAD_RUN=true;

/*-------------------------------------*/
int QtThread8VideoProcess::SCREEN_W;
int QtThread8VideoProcess::SCREEM_H;
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
QtThread8VideoProcess::QtThread8VideoProcess()
{
	this->init_param();

	this->init_screen();	
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
QtThread8VideoProcess::QtThread8VideoProcess(int _Channel,bool _showWindow)
{
	this->init_param();

	this->SetShowWindow(_showWindow);
	this->CHANNEL = _Channel;
	this->WINDOW_NAME ="Channel"+Base::int2str(_Channel);
	
	if (mShowWindow){
		this->init_window();
	}

	
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
QtThread8VideoProcess::~QtThread8VideoProcess(void)
{
	qDebug() << "QtThread8Video is Release ! "<<"CHANNEL:"<<this->CHANNEL;
	M_THREAD_RUN = FALSE;

	if (mShowWindow) {
			cvDestroyWindow(WINDOW_NAME.c_str());
	}

	
	TaskObj[CHANNEL] = Q_NULLPTR;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThread8VideoProcess::SetCurrentCutArea(IplImage * img_t)
{
	mCutArea = cvGetImageROI(img_t);
	cvResetImageROI(img_t);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThread8VideoProcess::DrawCurrentCutArea(IplImage * img_t)
{
	DrawArea(img_t, mCutArea);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThread8VideoProcess::DrawFutureCutArea(IplImage * img_t)
{
	DrawAreaGrid(img_t, mCutAreaSet);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThread8VideoProcess::DrawSensorStatArea(IplImage * img_t)
{
#if _DEBUG
	if (cmd_ctrl->SensorStat() == 0) {
		CvFont font;
		cvInitFont(&font, CV_FONT_HERSHEY_COMPLEX, 4, 4, 1, 2);
		cvPutText(img_t, "Sensor Error", cvPoint(10, img_t->height / 2), &font, CV_RGB(255, 255, 255));
	}
#endif // _DEBUG
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThread8VideoProcess::SetShowWindow(bool _flag)
{
	this->mShowWindow = _flag;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThread8VideoProcess::DrawSelectedBoder(IplImage* img_t)
{
	const CvPoint up_left = cvPoint(mCutAreaSet.x, mCutAreaSet.y);
	const CvPoint up_right = cvPoint(mCutAreaSet.x+mCutAreaSet.width, mCutAreaSet.y);
	const CvPoint down_left = cvPoint(mCutAreaSet.x, mCutAreaSet.y+mCutAreaSet.height);
	const CvPoint down_right = cvPoint(mCutAreaSet.x+mCutAreaSet.width, mCutAreaSet.y+mCutAreaSet.height);

	if (this->mRectM==RectMode::DOWN){
		cvDrawLine(img_t, down_left, down_right, CV_RGB(0,0,0 ), 3, CV_AA);
		this->drawTipText(img_t, "DOWN");
	}else if (this->mRectM == RectMode::LEFT) {
		cvDrawLine(img_t, up_left, down_left, CV_RGB(0,0,0),3, CV_AA);
				this->drawTipText(img_t, "LEFT");
	}else if (this->mRectM == RectMode::RIGHT) {
		cvDrawLine(img_t,up_right, down_right, CV_RGB(0,0,0), 3, CV_AA);
		this->drawTipText(img_t, "RIGHT");
	}else if (this->mRectM == RectMode::UP) {
		cvDrawLine(img_t, up_left, up_right, CV_RGB(0,0, 0), 3, CV_AA);
		this->drawTipText(img_t, "UP");
	}else if (this->mRectM == RectMode::RECT) {
		this->drawTipText(img_t, "RECT");
	}else{

	}


}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThread8VideoProcess::SetCutRectBorder(CvPoint point_t,int flags)
{
	if (CutRectFlag[(int)mRectM] == (int)mRectM+1){

						if (this->mRectM == RectMode::LEFT) {
							mCutAreaSet.width=mCutAreaSet.width-(point_t.x- mCutAreaSet.x);
							mCutAreaSet.x = point_t.x;
						}else if (this->mRectM == RectMode::UP) {
							mCutAreaSet.height=mCutAreaSet.height-(point_t.y- mCutAreaSet.y);
							mCutAreaSet.y=point_t.y;
						}else if (this->mRectM == RectMode::RIGHT) {
							mCutAreaSet.width =(point_t.x - mCutAreaSet.x);
						}else if (this->mRectM == RectMode::DOWN) {
							mCutAreaSet.height= (point_t.y - mCutAreaSet.y);
						}else if (this->mRectM == RectMode::IDLE) {

						}else if (this->mRectM == RectMode::RECT){
							
						}else{

						}
	}

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThread8VideoProcess::SetRectBorderAdj(int flag)
{
	int step = 2*flag;

	if (this->mRectM == RectMode::LEFT) {
		mCutAreaSet.x += step;
		mCutAreaSet.width -= step;
	}else if (this->mRectM == RectMode::UP) {
		mCutAreaSet.y += step;
		mCutAreaSet.height -= step;
	}else if (this->mRectM == RectMode::RIGHT) {				
		mCutAreaSet.width += step;
	}else if (this->mRectM == RectMode::DOWN) {
		mCutAreaSet.height += step;
	}else if (this->mRectM == RectMode::IDLE) {

	}else if (this->mRectM == RectMode::RECT) {

	}else {

	}
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThread8VideoProcess::SetCutRect(int x, int y, int flags)
{
	if (CutRectFlag[(int)mRectM] == (int)mRectM + 1) {	
			if (this->mRectM == RectMode::RECT) {
						if ((flags & CV_EVENT_FLAG_CTRLKEY)) {
							this->mCutAreaSet.width = std::abs(x - this->mCutAreaSet.x);
							this->mCutAreaSet.height = std::abs(y - this->mCutAreaSet.y);
						}
			}	
	}


}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThread8VideoProcess::SetCutRectStart()
{
	CutRectFlag[(int)mRectM] = (int)mRectM+1;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThread8VideoProcess::SetCutRectStop()
{
	CutRectFlag[(int)mRectM] = 0;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThread8VideoProcess::SetProcessMode(ProcessMode _pm)
{
	this->mProcessM = _pm;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
QSharedPointer<CMD_CTRL> QtThread8VideoProcess::getCmdRectCfg()
{
	QSharedPointer<CMD_CTRL> cmd=QSharedPointer<CMD_CTRL>(new CMD_CTRL());
		
	if (IsEffectiveRect(&mCutAreaSet)) {
		cmd->getRectCfgCmd(CHANNEL, mCutAreaSet);
	}else{
		cmd.clear();
	}
	
	return cmd;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThread8VideoProcess::drawTipText(IplImage* img_t,std::string txt)
{
	CvFont font;
	const int scale = 2;
	cvInitFont(&font, CV_FONT_HERSHEY_COMPLEX, scale, scale, 1, 2);
	cvPutText(img_t, txt.c_str(), cvPoint(50,50), &font, CV_RGB(255, 255, 255));

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int QtThread8VideoProcess::IsEffectiveRect(CvRect * rect)
{

	if ((rect->x>=0)&&
		(rect->y>=0)&&
		(rect->width>0)&&
		(rect->height>0)) {
	
		return TRUE;
	
	}

	return FALSE;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThread8VideoProcess::DrawArea(IplImage* img_t,CvRect rect)
{
	if (IsEffectiveRect(&rect)) {

		cvRectangleR(img_t, rect, CV_RGB(255, 255, 255));
		
		RectZoom(&rect, -1);
		cvRectangleR(img_t, rect, CV_RGB(0, 0, 0));
		
		RectZoom(&rect, -1);
		cvRectangleR(img_t, rect, CV_RGB(255, 255, 255));

		RectZoom(&rect, -1);
		cvRectangleR(img_t, rect, CV_RGB(0, 0, 0));
	
	}
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThread8VideoProcess::DrawAreaGrid(IplImage * img_t, CvRect rect)
{
	const int W = img_t->width;
	const int H = img_t->height;
	  
	const CvPoint line_v_1_top = cvPoint(rect.x,0);
	const CvPoint line_v_1_bottom = cvPoint(rect.x, H);

	const CvPoint line_v_2_top = cvPoint(rect.x+rect.width, 0);
	const CvPoint line_v_2_bottom = cvPoint(rect.x + rect.width, H);

	const CvPoint line_h_1_left = cvPoint(0, rect.y);
	const CvPoint line_h_1_right = cvPoint(W, rect.y);

	const CvPoint line_h_2_left = cvPoint(0,rect.y+rect.height);
	const CvPoint line_h_2_right = cvPoint(W,rect.y+rect.height);



	if (IsEffectiveRect(&rect)) {

		cvLine(img_t, line_v_1_top, line_v_1_bottom, CV_RGB(255, 255, 255),3, CV_AA);
		cvLine(img_t, line_v_2_top, line_v_2_bottom, CV_RGB(255, 255, 255), 3, CV_AA);
		cvLine(img_t, line_h_1_left, line_h_1_right, CV_RGB(255, 255, 255), 3, CV_AA);
		cvLine(img_t, line_h_2_left, line_h_2_right, CV_RGB(255, 255, 255), 3, CV_AA);

	}
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThread8VideoProcess::RectZoom(CvRect * rect, int scale)
{
	rect->x += scale;
	rect->y += scale;
	rect->width  -= 2 * scale;
	rect->height -= 2 * scale;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThread8VideoProcess::ReSizeRectByKey(int key)
{

	if (key == 'w') {
	
	}else if (key == 's') {
	
	}else if (key == 'a') {

	}else if (key == 'd') {

	}else if (key == '8') {

	}else if (key == '4') {

	}else if (key == '6') {

	}else if (key == '2') {

	}else if(key<=0){
		;
	}else if (key == 2490368){
		;//ио
	}else if (key == 2621440){
		;//об
	}else if (key == 2424832){
		;//вС
	}else if (key == 2555904){
		;//ср
	}else{

	}

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int QtThread8VideoProcess::resizeWindowOnce(int _width,int _height)
{
	mImageWidth =_width ;
	mImageHeight = _height;

	if (mWindowWidth + mWindowHeight < 0) {
	
		if (mImageWidth + mImageHeight > 0) {
		
				float w_scale =1.0*mWidthSetp/mImageWidth;
				float h_scale = 1.0*mHeightStep / mImageHeight;
				const float wh_scale = (w_scale < h_scale) ? w_scale:h_scale;
		
				mWindowWidth = wh_scale*mImageWidth;	
				mWindowHeight = wh_scale*mImageHeight;
				cvResizeWindow(WINDOW_NAME.c_str(), mWindowWidth, mWindowHeight);
		}
		
	
	}	

	return 0;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int QtThread8VideoProcess::WaitKey()
{
	const int result = cvWaitKey(1);
	
	if (result >= 0) {
		ReSizeRectByKey(result);
	}	
	
	return mKeyValue=result;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThread8VideoProcess::ChangeRectMode()
{
	const static RectMode rm[] = {RectMode::RECT,RectMode::LEFT,RectMode::UP,RectMode::RIGHT,RectMode::DOWN};
	const int rm_size = sizeof(rm) / sizeof(RectMode);
	static int current_mode = rm_size - 1;

	mRectM = rm[current_mode++%rm_size];

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThread8VideoProcess::on_mouse(int event, int x, int y, int flags, void * ustc)
{
	QtThread8VideoProcess* const ptr_vp = (QtThread8VideoProcess*)ustc;
	
	if (event == CV_EVENT_MOUSEMOVE){

						ptr_vp->SetCutRectBorder(cvPoint(x,y),flags);
						ptr_vp->SetCutRect(x, y, flags);						

	}else	if (event == CV_EVENT_LBUTTONDOWN){

						if (CV_EVENT_FLAG_ALTKEY & flags){

							ptr_vp->ChangeRectMode();

						}else if (flags & CV_EVENT_FLAG_CTRLKEY){

							if (ptr_vp->mRectM == RectMode::RECT){
									ptr_vp->mCutAreaSet.x = x;
									ptr_vp->mCutAreaSet.y = y;
									ptr_vp->SetCutRectStart();
							}

					
						}else{


						}

					
		
	}else	if (event == CV_EVENT_LBUTTONUP){
		
						if (ptr_vp->mRectM == RectMode::LEFT) {

						}else if (ptr_vp->mRectM == RectMode::UP) {

						}else if (ptr_vp->mRectM == RectMode::RIGHT) {

						}else if (ptr_vp->mRectM == RectMode::DOWN) {

						}else if (ptr_vp->mRectM == RectMode::IDLE) {

						}else if(ptr_vp->mRectM == RectMode::RECT){
			
							if (flags & CV_EVENT_FLAG_CTRLKEY){
								{
									ptr_vp->mCutAreaSet.width = std::abs(x - ptr_vp->mCutAreaSet.x);
									ptr_vp->mCutAreaSet.height = std::abs(y - ptr_vp->mCutAreaSet.y);
									
								}
							}

						}else{

						}

						


	}else  if (event == CV_EVENT_RBUTTONDOWN){
			ptr_vp->SetCutRectStart();
	}else if (event == CV_EVENT_RBUTTONUP){
			ptr_vp->SetCutRectStop();
	}else if (event == CV_EVENT_LBUTTONDBLCLK){
				
	}else if (event == CV_EVENT_RBUTTONDBLCLK){	

						if ((flags & CV_EVENT_FLAG_CTRLKEY) &&
							(ptr_vp->mRectM == RectMode::RECT))	{
								ptr_vp->mCutAreaSet = cvRect(0, 0, 0, 0);
						}
		
	}else {


	}


	

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThread8VideoProcess::init_screen()
{
		QRect rec = QApplication::desktop()->screenGeometry();
		SCREEM_H = rec.height();
		SCREEN_W = rec.width();
		
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThread8VideoProcess::init_window()
{
	this->init_screen();
	
	cvNamedWindow(WINDOW_NAME.c_str(), CV_WINDOW_NORMAL);

	const int ImageARow = 4;
	const int WidthSetp = SCREEN_W  / ImageARow;
	const int HeightStep = SCREEM_H / std::ceil(CAMERA_CHANNELS / ImageARow);

	const int pos_wx = (this->CHANNEL % ImageARow) *WidthSetp;
	const int pos_hy = (this->CHANNEL / ImageARow) *HeightStep;

	cvMoveWindow(WINDOW_NAME.c_str(),pos_wx,pos_hy);

	mWidthSetp= WidthSetp;
	mHeightStep= HeightStep;

	cv::setMouseCallback(WINDOW_NAME,on_mouse,this);




}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThread8VideoProcess::init()
{
	ThreadPool.setMaxThreadCount(CAMERA_CHANNELS);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThread8VideoProcess::init_param()
{
	mWidthSetp = -1;
	mHeightStep = -1;

	mImageWidth = -1;
	mImageHeight = -1;

	mWindowWidth = -1;
	mWindowHeight = -1;

	mCutAreaSet = cvRect(0, 0, 0, 0);
	mCutArea= cvRect(0, 0, 0, 0);
	
	mKeyValue = -1;

	mShowWindow = true;

	mRectM = RectMode::RECT;
	memset(CutRectFlag, 0, sizeof(CutRectFlag));
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThread8VideoProcess::process_img_opencv_self()
{
	QSharedPointer<exCircleData> circleData = ChannelsData4Show::getInstance()->getChannelData(CHANNEL);
	cmd_ctrl = circleData->getImg();

	IplImage *img_t = cmd_ctrl->getIplimage();

#if TRUE	
	this->SetCurrentCutArea(img_t);
#endif // 0

#if TRUE
	this->DrawCurrentCutArea(img_t);
	this->DrawFutureCutArea(img_t);
	this->DrawSelectedBoder(img_t);
#endif // TRUE





	this->resizeWindowOnce(img_t->width, img_t->height);

	cvShowImage(WINDOW_NAME.c_str(), img_t);

	this->WaitKey();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThread8VideoProcess::process_nothing()
{
	QSharedPointer<exCircleData> circleData = ChannelsData::getInstance()->getChannelData(CHANNEL);
	QSharedPointer<CMD_CTRL> cmd_ctrl = circleData->getImg();
	cmd_ctrl.clear();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThread8VideoProcess::process_qtgui()
{
	QSharedPointer<exCircleData> circleData = ChannelsData::getInstance()->getChannelData(CHANNEL);

	
	emit img_incoming(CHANNEL,circleData->IsValid());
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThread8VideoProcess::run()
{
	QSharedPointer<exCircleData> circleData = ChannelsData::getInstance()->getChannelData(CHANNEL);
		
	while (M_THREAD_RUN) {
				
		if (circleData->IsValid()) {
				
				if (mProcessM== OPENCV){
					this->process_img_opencv_self();
				}else if (mProcessM == QTGUI){
					this->process_qtgui();
				}else if (mProcessM == NOTHING) {
					this->process_nothing();
				}else{
					this->process_nothing();
				}

		}else{
				QThread::msleep(10);
		}
		



		

	}
		
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThread8VideoProcess::startTask(ProcessMode _pm)
{
	ThreadPool.clear();
	ThreadPool.setMaxThreadCount(CAMERA_CHANNELS*2);
	ThreadPool.setExpiryTimeout(1000);
	qDebug() << "activeThreadCount: " << ThreadPool.activeThreadCount();

	for (size_t chi = 0; chi < CAMERA_CHANNELS; chi++){
		
		TaskObj[chi]=new  QtThread8VideoProcess(chi);

		TaskObj[chi]->SetProcessMode(_pm);
		
		if (TaskObj[chi]->autoDelete() == false) {
			
			TaskObj[chi]->setAutoDelete(true);
		}

		if (ThreadPool.tryStart(TaskObj[chi]) == false) {
		
			delete TaskObj[chi];
		}

	}

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThread8VideoProcess::stopTask()
{
	M_THREAD_RUN = FALSE;

#if 0
	ThreadPool.waitForDone();
#endif // 0
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThread8VideoProcess::SetRectCmds()
{
	for (size_t chi = 0; chi < CAMERA_CHANNELS; chi++) {

		if (TaskObj[chi]) {
		
			QSharedPointer<CMD_CTRL> cmd_t= TaskObj[chi]->getCmdRectCfg();
			QtThreadClientCtrl::SetCmd(cmd_t);
		}	

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

/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/