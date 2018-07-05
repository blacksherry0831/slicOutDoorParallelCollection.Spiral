//#include "stdafx.h"
#include "QtThread8VideoProcess.hpp"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
const unsigned char QtThread8VideoProcess::TaskMap[CAMERA_CHANNELS] = {0,1,2,3,4,5,6,7};
QThreadPool   QtThread8VideoProcess::ThreadPool;
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
QtThread8VideoProcess::QtThread8VideoProcess(int _Channel)
{
	this->init_param();

	this->CHANNEL = _Channel;
	this->WINDOW_NAME ="Channel"+Base::int2str(_Channel);

	this->init_window();
	
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
QtThread8VideoProcess::~QtThread8VideoProcess(void)
{
	qDebug() << "QtThread8Video is Release ! ";
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
void QtThread8VideoProcess::run()
{
	QSharedPointer<exCircleData> circleData = ChannelsData::channelsData()->getChannelData(CHANNEL);
		
	while (M_THREAD_RUN) {
				
		if (circleData->IsValid() == FALSE) {
			QThread::msleep(10);
			continue;			
		}		
		

		QSharedPointer<CMD_CTRL> cmd_ctrl= circleData->getImg();
		

		IplImage *img_t = cmd_ctrl->getIplimage();

		
	
#if _DEBUG
		if (cmd_ctrl->SensorStat() == 0) {
			CvFont font;
			cvInitFont(&font, CV_FONT_HERSHEY_COMPLEX, 4, 4, 1, 2);
			cvPutText(img_t, "Sensor Error", cvPoint(10, img_t->height / 2),&font, CV_RGB(255, 255, 255));
		}
#endif // _DEBUG

		this->resizeWindowOnce( img_t->width,img_t->height);

		cvShowImage(WINDOW_NAME.c_str(), img_t);

		cvWaitKey(1);

	}
		
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThread8VideoProcess::startTask()
{
	ThreadPool.setMaxThreadCount(CAMERA_CHANNELS+1);

	for (size_t chi = 0; chi < CAMERA_CHANNELS; chi++){
			
		QtThread8VideoProcess *task = new  QtThread8VideoProcess(chi);
		ThreadPool.start(task);

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

/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/