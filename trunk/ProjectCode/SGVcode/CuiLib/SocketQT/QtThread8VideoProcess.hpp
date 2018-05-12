#pragma once

#include "cpp_stl.h"

/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
#include <QtCore>
#include <QtNetwork>
#include <QSharedPointer>
#include <QThread>
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

public:
	QtThread8VideoProcess();
	QtThread8VideoProcess(int _Channel);
	~QtThread8VideoProcess(void);
private:
	int CHANNEL;	
	string WINDOW_NAME;
protected:
	void init();
	bool M_THREAD_RUN;
public:
	static const unsigned char TaskMap[CAMERA_CHANNELS];
	static QThreadPool   ThreadPool;
	static void startTask();
protected: 
	virtual void run();

};