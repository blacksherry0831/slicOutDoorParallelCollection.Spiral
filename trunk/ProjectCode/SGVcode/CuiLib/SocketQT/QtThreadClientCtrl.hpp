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
class QtThreadClientCtrl :public QtThreadSocketClient
{

public:
	QtThreadClientCtrl(qintptr p);
	~QtThreadClientCtrl(void);
protected:
			
	
public:

protected: 
	virtual void run();

};