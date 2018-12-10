//#include "stdafx.h"
#include "QtThread8ImgProcess.hpp"
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
QtThread8ImgProcess::QtThread8ImgProcess()
{
	this->init_thread_pool();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
QtThread8ImgProcess::~QtThread8ImgProcess(void)
{	
	this->stopTask();
	ThreadPool.clear();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThread8ImgProcess::init_thread_pool()
{
	ThreadPool.clear();
	ThreadPool.setMaxThreadCount(CAMERA_CHANNELS);
	ThreadPool.setExpiryTimeout(1000);
	qDebug() << "ImgProcessThreadCount[active]: " << ThreadPool.activeThreadCount();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThread8ImgProcess::startTask()
{
	mTaskObj.clear();

	for (size_t chi = 0; chi < CAMERA_CHANNELS; chi++) {

		QSharedPointer<QtImgProcess> qip_t = QSharedPointer<QtImgProcess>(new QtImgProcess(chi));

		mTaskObj.push_back(qip_t);
		
		
		if (mTaskObj[chi]->autoDelete() == false) {

			mTaskObj[chi]->setAutoDelete(true);
		}

		if (ThreadPool.tryStart(mTaskObj[chi].data()) == false) {

			mTaskObj[chi]=Q_NULLPTR;
		}

	}
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThread8ImgProcess::stopTask()
{
	M_THREAD_RUN = FALSE;

	for (size_t i = 0; i <mTaskObj.size(); i++)
	{
		mTaskObj[i]->closeServer();
	}

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThread8ImgProcess::ConnectAllImg2View(int _connect, const QObject * receiver)
{
	for (size_t i = 0; i <mTaskObj.size(); i++)
	{
#if TRUE

		const Qt::ConnectionType QT_UC = Qt::UniqueConnection;

		const QObject * sender = mTaskObj[i].data();

		if (_connect) {
			connect(sender,
				SIGNAL(img_stat_ex(int, int, int, void*)),
				receiver,
				SLOT(img_stat_show_ex(int, int, int,void*)),
				QT_UC);
		}
		else {
			disconnect(sender,
				SIGNAL(img_stat_ex(int, int, int,void*)),
				receiver,
				SLOT(img_stat_show_ex(int, int, int, void*)));
		}

#endif

	}
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThread8ImgProcess::SetAllImgShowCutRect(int _param)
{
	for (size_t i = 0; i <mTaskObj.size(); i++)
	{
		mTaskObj[i]->SetImgProcCutArea(_param);	
	}
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThread8ImgProcess::SetAllImgBinary(int _param)
{
	for (size_t i = 0; i <mTaskObj.size(); i++)
	{
		mTaskObj[i]->SetImgProcBinaryImg(_param);
	}
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThread8ImgProcess::SetAllImgClassifyThickly(int _param, float _threshold)
{
		for (size_t i = 0; i <mTaskObj.size(); i++)
		{
					mTaskObj[i]->SetImgProcBinaryClassifyThickly(_param);

					if ((_param==TRUE)&&
						(_threshold>=0)&&
						(_threshold<=1)){
						mTaskObj[i]->SetClassifyThicklyThreshold(_threshold);
					}

		}
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int   QtThread8ImgProcess::IMG_PROC_All_Done()
{
	for (size_t i = 0; i <mTaskObj.size(); i++)
	{
		if (mTaskObj[i]->IMG_PROC_Done() == FALSE) {
			return FALSE;
		}
	}

	return TRUE;

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
float QtThread8ImgProcess::GetClassifyThicklyThreshold()
{

	for (size_t i = 0; i <mTaskObj.size(); i++){
		return	mTaskObj[i]->GetClassifyThicklyThreshold();
	}
	return -1.0;

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/