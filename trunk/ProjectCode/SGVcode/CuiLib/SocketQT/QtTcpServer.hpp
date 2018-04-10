#pragma once

#include "cpp_stl.h"
#include <QObject>
#include <QtCore>
#include <QtNetwork>
#include <QSharedPointer>
#include "QtThreadClient.hpp"

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
	explicit QtTcpServer(QObject *parent = Q_NULLPTR);
	explicit QtTcpServer(QObject *parent = Q_NULLPTR, QSharedPointer<QtThreadClient> _clientThread=Q_NULLPTR );
	 ~QtTcpServer();
	

protected:
	virtual void incomingConnection(qintptr socketDescriptor);
private:	
	QList<QSharedPointer<QtThreadClient>>	 m_clientThreads;
	QMutex m_clients_mutex;
	void RemoveDoneThread();
	void SaveRunningThread(QSharedPointer<QtThreadClient> _client);
public:
	void execMy();

};