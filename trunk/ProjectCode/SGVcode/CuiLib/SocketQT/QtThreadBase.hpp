#pragma once

#include "cpp_stl.h"

#include <QtCore>
#include <QtNetwork>
#include <QSharedPointer>
#include <QThread>

/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
class QtThreadBase :public QThread
{

public:
	QtThreadBase();
	~QtThreadBase(void);
protected:

	//�����Լ���Ҫ�ķ��������  
	bool M_THREAD_RUN;
	int  MAX_MSECS;
	int IDENTIFY;
public:
	void Sleep(int _ms);
	void startServer();
	void closeServer();
	
protected: 
	virtual void run();

};