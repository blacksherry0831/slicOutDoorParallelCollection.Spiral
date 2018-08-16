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
#include "ChannelsData.hpp"
#include "MY_SDK_LIB/Base.h"

#include "QtThreadClientCtrl.hpp"
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
class QtThread8VideoProcess : public QObject,public QRunnable
{
	Q_OBJECT
public:
	enum RectMode {
		IDLE=0,
		LEFT=1,
		RIGHT=2,
		UP=3,
		DOWN=4,
		RECT=5,		
	};
	enum ProcessMode {
		OPENCV,
		QTGUI,
		NOTHING,
	};
public:
	QtThread8VideoProcess();
	QtThread8VideoProcess(int _Channel);
	~QtThread8VideoProcess(void);
private:
	int CHANNEL;	
	std::string WINDOW_NAME;
private:
	static  int SCREEN_W;
	static  int SCREEM_H;
private:
	int mWidthSetp;
	int mHeightStep;

	int mImageWidth;
	int mImageHeight;
	
	int mWindowWidth;
	int mWindowHeight;
private:
	CvRect mCutArea;
	int    mKeyValue;
	QSharedPointer<CMD_CTRL> cmd_ctrl;
public:
	void SetCurrentCutArea(IplImage* img_t);
	void DrawCurrentCutArea(IplImage* img_t);
	void DrawFutureCutArea(IplImage* img_t);
	void DrawSensorStatArea(IplImage* img_t);
public:
	RectMode mRectM;
	ProcessMode mProcessM;
	CvRect mCutAreaSet;
	void DrawSelectedBoder(IplImage* img_t);
	unsigned char CutRectFlag[10];
	void SetCutRectBorder(CvPoint point_t,int flags);
	void SetRectBorderAdj(int flag);
	void SetCutRect(int x,int y, int flags);
	void SetCutRectStart();
	void SetCutRectStop();
	void SetProcessMode(ProcessMode _pm);
	QSharedPointer<CMD_CTRL> getCmdRectCfg();
	void drawTipText(IplImage* img_t,std::string txt);
public:
	static void DrawArea(IplImage* img_t,CvRect rect);
private:
	static int IsEffectiveRect(CvRect* rect);
	
	static void DrawAreaGrid(IplImage* img_t, CvRect rect);
	static void RectZoom(CvRect* rect,int scale);
	void ReSizeRectByKey(int key);
	int resizeWindowOnce(int _width, int _height);
	int WaitKey();
public:
	void ChangeRectMode();
protected:
	static void on_mouse(int event, int x, int y, int flags, void *ustc);
protected:
	void init_screen();
	void init_window();
	void init();
	void init_param();
	void process_img_opencv_self();
	void process_nothing();
	void process_qtgui();
signals:
	void img_incoming(int _channel,int _frames);
public:
	static bool M_THREAD_RUN;
	static const unsigned char TaskMap[CAMERA_CHANNELS];
	static  QtThread8VideoProcess* TaskObj[CAMERA_CHANNELS];
	static QThreadPool   ThreadPool;
	static void startTask(ProcessMode _pm= ProcessMode::OPENCV);
	static void stopTask();
	static void SetRectCmds();
protected: 
	virtual void run();

};