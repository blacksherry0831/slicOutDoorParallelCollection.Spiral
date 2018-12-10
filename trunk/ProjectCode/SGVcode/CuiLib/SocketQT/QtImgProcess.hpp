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
#include "type_socket_qt.h"
#include "CMD_CTRL.hpp"
#include "ChannelsData4Show.hpp"
#include "MY_SDK_LIB/TimeMeasure.hpp"

#include "JQLibrary/src/JQCPUMonitor/JQCPUMonitor"
//JQCPUMonitor::cpuUsagePercentage()
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
public:
	static void ImgProcessIpl(IplImage * _img,const IMG_PROC _img_proc);
	static int ImgProcessCMD_CTRL(QSharedPointer<CMD_CTRL> _cmd,const IMG_PROC _img_proc);

	void processImgCmd();
	void processImgCmdDone(QSharedPointer<CMD_CTRL> _cmd);
	void processImgCmdFlowCtrl(QSharedPointer<CMD_CTRL> _cmd);
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
public:
	void SleepMy(int _ms = 10);
public:
	void run();

};