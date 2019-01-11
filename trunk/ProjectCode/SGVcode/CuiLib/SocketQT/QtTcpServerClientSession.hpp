#pragma once

#include "cpp_stl.h"
/*-------------------------------------*/
#include <QObject>
#include <QtCore>
#include <QtNetwork>
#include <QSharedPointer>
/*-------------------------------------*/

#include "QtThreadSocketClientCmdQ.hpp"
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
	QtTcpServerClientSession(QObject *parent ,QSharedPointer<QtThreadSocketClientCmdQ> _clientThread);
	~QtTcpServerClientSession();
private:	
	QList<QSharedPointer<QtThreadSocketClientCmdQ>>	 m_clientThreads;
private:
	QMutex m_clients_mutex;
private:
	void RemoveDoneThread();
	void StopRunningThread();
	void SaveRunningThread(QSharedPointer<QtThreadSocketClientCmdQ> _client);
protected:	
	void ProcessRunningThread(QSharedPointer<QtThreadSocketClientCmdQ> _client);
public:
	void SendMsg2ClientSession(QSharedPointer<CMD_CTRL> _msg);
	void BeforeSendMsg2ClientSession(QSharedPointer<CMD_CTRL> _msg);
	QVector<QString> getRunningSessionIpAddr();
protected:
	int IsWorkFlowDoneAllThread();
	int WorkFlowDoneQuality();
public:
	void execMy();
public:

public slots :
		void client_session_status(int _client, int _status);

};