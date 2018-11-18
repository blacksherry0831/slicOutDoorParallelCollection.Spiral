#pragma once

#include "cpp_stl.h"
#include <QObject>
#include <QtCore>
#include <QtNetwork>
#include <QSharedPointer>
#include "QtThreadClientCtrl.hpp"

/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
class QtTcpServer :public QTcpServer
{
	Q_OBJECT
public:
	explicit QtTcpServer(QObject *parent );
	 QtTcpServer(QObject *parent ,QSharedPointer<QtThreadSocketClient> _clientThread);

	 ~QtTcpServer();	

protected:
	virtual void incomingConnection(qintptr socketDescriptor);
private:	
	QList<QSharedPointer<QtThreadSocketClient>>	 m_clientThreads;
	QMutex m_clients_mutex;
private:
	void RemoveDoneThread();
	void SaveRunningThread(QSharedPointer<QtThreadSocketClient> _client);
protected:	
	void ProcessRunningThread(QSharedPointer<QtThreadSocketClient> _client);
public:
	void execMy();
protected:
	std::string mServerIpAddr;
	int mPort;
public:
	int StartListen();

};