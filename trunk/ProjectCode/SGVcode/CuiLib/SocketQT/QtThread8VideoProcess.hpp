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
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
class QtThread8VideoProcess : public QRunnable
{
	//Q_OBJECT
public:
	enum RectMode {
		LEFT=0,
		RIGHT=1,
		UP=2,
		DOWN=3,
		RECT=4,
		IDLE=5,
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
public:
	RectMode mRectM;
	CvRect mCutAreaSet;
	int    mCutAreaMode;
	void DrawSelectedBoder(IplImage* img_t);
	void drawTipText(IplImage* img_t,std::string txt);
private:
	CvRect mCutArea;
	static int IsEffectiveRect(CvRect* rect);
	static void DrawArea(IplImage* img_t,CvRect rect);
	static void RectZoom(CvRect* rect,int scale);
	void ReSizeRectByKey(int key);
	int    mKeyValue;
	int resizeWindowOnce(int _width, int _height);
	int WaitKey();
	void ChangeRectMode();
protected:
	static void on_mouse(int event, int x, int y, int flags, void *ustc);
protected:
	void init_screen();
	void init_window();
	void init();
	void init_param();
public:
	static bool M_THREAD_RUN;
	static const unsigned char TaskMap[CAMERA_CHANNELS];
	static  QtThread8VideoProcess* TaskObj[CAMERA_CHANNELS];
	static QThreadPool   ThreadPool;
	static void startTask();
	static void stopTask();
protected: 
	virtual void run();

};