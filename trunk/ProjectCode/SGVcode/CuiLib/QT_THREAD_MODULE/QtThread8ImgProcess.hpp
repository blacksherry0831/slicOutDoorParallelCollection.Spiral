#pragma once
/*-------------------------------------*/
#include "cpp_stl.h"
/*-------------------------------------*/
#include "opencv_stl.h"
/*-------------------------------------*/
#include "qt_all.h"
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
#include "SocketQT/CMD_CTRL.hpp"
/*-------------------------------------*/
#include "QtImgProcess.hpp"
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
class QtThread8ImgProcess : public QObject
{
	Q_OBJECT
public:

public:
	QtThread8ImgProcess();

	~QtThread8ImgProcess(void);
private:
	void init_thread_pool();
private:
	QThreadPool   ThreadPool;
	QVector<QSharedPointer<QtImgProcess>> mTaskObj;
	 bool M_THREAD_RUN;
public:
	 void startTask();
	 void stopTask();

	 int  IMG_PROC_All_Done();
public:
	void ConnectAllImg2View(int _connect, const QObject *receiver);

	void SetAllImgShowCutRect(int _param);
	void SetAllImgBinary(int _param);
	void SetAllImgDenoise(int _param);
	void SetAllImgClassifyThickly(int _param,float _threshold=-1);

	void SetAllImgClassifyHoughBlock(int _param, std::vector<float> _threshold_v);
	
	void SetAllImgSigma(int _sigma);

	void SetAllImg_IpAddrProperty(std::string _in_out);

	float GetClassifyThicklyThreshold();
	

};