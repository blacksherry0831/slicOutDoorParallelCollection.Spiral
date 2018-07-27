#pragma once

#include "cpp_stl.h"

#include "qt_all.h"

/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
class QtThreadBase :public QThread
{
	Q_OBJECT
public:
	explicit QtThreadBase(QObject *parent = Q_NULLPTR);
	~QtThreadBase(void);
protected:

	//定义自己需要的方法或变量  
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