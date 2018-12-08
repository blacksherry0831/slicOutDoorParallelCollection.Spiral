#pragma once
/*-------------------------------------*/
#include "cpp_stl.h"
/*-------------------------------------*/
#include <QObject>
#include <QtCore>
#include <QtNetwork>
#include <QSharedPointer>
/*-------------------------------------*/
#include "QtThreadSocketClient.hpp"

#include "QtTcpServerClientSession.hpp"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
class QtTcpServer :public QtTcpServerClientSession
{
	Q_OBJECT
public:
	explicit QtTcpServer(QObject *parent=nullptr);
	 QtTcpServer(QObject *parent ,QSharedPointer<QtThreadSocketClient> _clientThread);

	 ~QtTcpServer();	

protected:
	virtual void incomingConnection(qintptr socketDescriptor);
protected:
	QString mErrorStr;
	std::string mServerIpAddr;
	int mPort;
public:
	int StartListen();

};