#pragma once

#include "cpp_stl.h"

#include <QtCore>
#include <QtNetwork>
#include <QSharedPointer>
#include <QThread>

#include "QtThreadSocketClient.hpp"
#include "QtTcpClient.hpp"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
class QtThread8Video :public QtThreadSocketClient
{

public:
	QtThread8Video(qintptr p);
	~QtThread8Video(void);
protected:
	
public:
		
	void Run0();
	void run1();

protected: 
	virtual void run();

};