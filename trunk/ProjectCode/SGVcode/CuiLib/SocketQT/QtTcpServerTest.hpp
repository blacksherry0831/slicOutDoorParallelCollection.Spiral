#pragma once
/*-------------------------------------*/
#include "cpp_stl.h"
/*-------------------------------------*/
#include <QObject>
#include <QtCore>
#include <QtNetwork>
#include <QSharedPointer>
/*-------------------------------------*/
#include "QtTcpServer.hpp"
#include "QtThreadClientCtrlTest.hpp"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
class QtTcpServerTest :public QtTcpServer
{
	Q_OBJECT
public:
	explicit QtTcpServerTest(QObject *parent);
	explicit QtTcpServerTest(QObject *parent,int _serverPort);
	explicit QtTcpServerTest(QObject *parent,QSharedPointer<QtThreadSocketClient> _clientThread);
	 ~QtTcpServerTest();
	

protected:
	virtual void incomingConnection(qintptr socketDescriptor);
private:	
	
public:
	

};