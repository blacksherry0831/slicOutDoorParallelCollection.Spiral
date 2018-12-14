#pragma once

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
#include "SocketQT/type_socket_qt.h"
#include "SocketQT/CMD_CTRL.hpp"
#include "SocketQT/ChannelsData4Show.hpp"
/*-------------------------------------*/
#include "MY_SDK_LIB/TimeMeasure.hpp"
/*-------------------------------------*/
#if 0
#include "JQLibrary/src/JQCPUMonitor/JQCPUMonitor"
//JQCPUMonitor::cpuUsagePercentage()
#endif // 0
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
class QtImgProcess : public QObject,public QRunnable
{
	Q_OBJECT
public:

public:
	QtImgProcess(int _channel);

	~QtImgProcess(void);

	float ImgProcessIpl(IplImage * _img, const IMG_PROC _img_proc);

private:
	unsigned long mSleepTime;
	bool M_THREAD_RUN;
	IMG_PROC mImgProc;	
	QSharedPointer<CMD_CTRL> m_cmd_ctrl_image;
public:
	void IMG_PROC_Start();
	void IMG_PROC_End();
	void IMG_PROC_Frames();
	int  IMG_PROC_Done();
	void IMG_PROC_record_work_flow(QSharedPointer<CMD_CTRL> _cmd);
public:
	 void ImgProcInner(IplImage * _img);
	 int ImgProcessCMD_CTRL(QSharedPointer<CMD_CTRL> _cmd,const IMG_PROC _img_proc);
	 
	 void processImgCmd();

private:
	void emit_img_signals(QSharedPointer<CMD_CTRL> _cmd);
	void setPriorityMy();
public:

signals:
	void img_stat_ex(int _p_stat, int _channel, int _frames,void* _data=Q_NULLPTR);

public:

	void SetImgProcCutArea(int _param);
	void SetImgProcBinaryImg(int _param);
	void SetImgProcBinaryClassifyThickly(int _param);
	void SetClassifyThicklyThreshold(float _threshold);
	float GetClassifyThicklyThreshold();
	
	void startServer();
	void closeServer();

	void EnqueueImg4ShowUI(QSharedPointer<CMD_CTRL> _cmd);
public:
	void SleepMy(int _ms = 10);
public:
	void run();

};