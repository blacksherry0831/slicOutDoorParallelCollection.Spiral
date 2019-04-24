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
	QtImgProcess(int _channel);
	~QtImgProcess(void);
private:
	unsigned long mSleepTime;
	bool M_THREAD_RUN;
	IMG_PROC mImgProc;	
	QSharedPointer<CMD_CTRL> m_cmd_ctrl_image;
	std::string mIpAddrProperty;
	float mThresholdClassifyThickly;
public:
	void IMG_PROC_Start();
	void IMG_PROC_End();
	void IMG_PROC_Frames();
	int  IMG_PROC_Done();
	void IMG_PROC_record_work_flow(QSharedPointer<CMD_CTRL> _cmd);
public:
	void processImgCmd();
	std::vector<float> ImgProcessCMD_CTRL(QSharedPointer<CMD_CTRL> _cmd,const IMG_PROC _img_proc);
	void			   ImgProcessCMD_CTRL_Not(QSharedPointer<CMD_CTRL> _cmd, const IMG_PROC _img_proc);
	void			   ImgProcessIpl_RemoveBorder(IplImage * _img, const IMG_PROC _img_proc);
	void			   ImgProcessIpl_Draw(IplImage * _img, const IMG_PROC _img_proc);
	void			   ImgProcessIpl_Base(IplImage * _img, const IMG_PROC _img_proc);
	std::vector<float> ImgProcessIpl(IplImage * _img, const IMG_PROC _img_proc);
public:


	 int ImgProcessCMD_CTRL_AttachResult(
		 QSharedPointer<CMD_CTRL>& _cmd,
		 const IMG_PROC& _img_proc,
		 const std::vector<float>& _result,
		 const int _classify);
	 
	 int predictFeature(std::vector<float> _f, const IMG_PROC _img_proc);
private:
	void emit_img_signals(QSharedPointer<CMD_CTRL> _cmd);
	void setPriorityMy();
public:

signals:
	void img_stat_ex(int _p_stat, int _channel, int _frames,void* _data=Q_NULLPTR);

public:
	void SetImgProc_IpAddrProperty(std::string _property);

	void SetImgProcSigma(int _sigma);
	void SetImgProcCutArea(int _param);
	
	void SetImgProcBinary(const int _param);
	void SetImgProcDenoise(const int _param);

	void SetImgProcBinaryClassifyThickly(int _param);
	void SetImgProcHoughBlockClassify(int _param);
public:
	void DrawFPS(IplImage * _img,const int _ch);
public:
	void  SetClassifyThicklyThreshold(float _threshold);
	float GetClassifyThicklyThreshold();
public:
	void startServer();
	void closeServer();
public:
	void EnqueueImg4ShowUI(QSharedPointer<CMD_CTRL> _cmd);
public:
	void SleepMy(int _ms = 10);
public:
	void run();

};