#pragma once

#include "cpp_stl.h"
/*-------------------------------------*/
#include <QObject>
#include <QtCore>
#include <QtNetwork>
#include <QSharedPointer>
/*-------------------------------------*/

#include "QtThreadSocketClient.hpp"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
class QtTcpServerClientSession :public QTcpServer
{
	Q_OBJECT
public:
	explicit QtTcpServerClientSession(QObject *parent=nullptr);
	QtTcpServerClientSession(QObject *parent ,QSharedPointer<QtThreadSocketClient> _clientThread);
	~QtTcpServerClientSession();
private:	
	QList<QSharedPointer<QtThreadSocketClient>>	 m_clientThreads;
	QMutex m_clients_mutex;
private:
	void RemoveDoneThread();
	void StopRunningThread();
	void SaveRunningThread(QSharedPointer<QtThreadSocketClient> _client);
protected:	
	void ProcessRunningThread(QSharedPointer<QtThreadSocketClient> _client);
public:
	void SendMsg2ClientSession(QSharedPointer<CMD_CTRL> _msg);
	QVector<QString> getRunningSessionIpAddr();
public:
	void execMy();
public:
	

};