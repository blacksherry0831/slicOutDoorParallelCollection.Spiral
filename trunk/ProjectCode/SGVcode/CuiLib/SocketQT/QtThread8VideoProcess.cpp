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
/**
*
*
*/
/*-------------------------------------*/
QtThread8VideoProcess::QtThread8VideoProcess()
{
	
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
QtThread8VideoProcess::QtThread8VideoProcess(int _Channel)
{
	this->CHANNEL = _Channel;
	this->WINDOW_NAME ="Channel"+Base::int2str(_Channel) ;
	cvNamedWindow(WINDOW_NAME.c_str(), CV_WINDOW_NORMAL);

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
void QtThread8VideoProcess::init()
{
	ThreadPool.setMaxThreadCount(CAMERA_CHANNELS);
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
		
		cvShowImage(WINDOW_NAME.c_str(), img_t);
		cvWaitKey(10);

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