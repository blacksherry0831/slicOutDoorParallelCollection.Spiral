#pragma once

#include "cpp_stl.h"
/*-------------------------------------*/
/**
*
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
*
*/
/*-------------------------------------*/
#include "QtThreadSocketClient.hpp"
#include "QtTcpClient.hpp"
#include "conf_ip.h"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
class QtThreadPLC :public QtThreadSocketClient
{

public:
	QtThreadPLC(qintptr p);
	~QtThreadPLC(void);
protected:
	
public:
		
	void Run0();
	void run1();

	int MoveSlidingThenRunMotor(QSharedPointer<BE_1105_Driver>	 be_1105,int _pos,int _isRun);

protected: 
	virtual void run();

};