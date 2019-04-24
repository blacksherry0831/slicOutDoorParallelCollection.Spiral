#pragma once

#include "cpp_stl.h"
/*-------------------------------------*/
#include <QObject>
#include <QtCore>
#include <QtNetwork>
#include <QSharedPointer>
/*-------------------------------------*/
#include "QT_THREAD_MODULE/CMD_WORK_FLOW.hpp"
/*-------------------------------------*/
#include "QtThreadSessionWorkFlow.hpp"
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

	~QtTcpServerClientSession();
private:	
	QList<QSharedPointer<QtThreadSessionWorkFlow>>	 m_clientThreads;
private:
	QMutex m_clients_mutex;
private:
	void RemoveDoneThread();
	void StopRunningThread();
	void SaveRunningThread(QSharedPointer<QtThreadSessionWorkFlow> _client);
protected:	
	void ProcessRunningThread(QSharedPointer<QtThreadSessionWorkFlow> _client);
public:
	void SendMsg2ClientSession(QSharedPointer<CMD_CTRL> _msg);
	void BeforeSendMsg2ClientSession(QSharedPointer<CMD_CTRL> _msg);
	QVector<QString> getRunningSessionIpAddr();
protected:
#if 1
	int IsWorkFlowDoneAllThread();
	int WorkFlowDoneQuality();
#endif
public:
	void execMy();
public:

public slots :
		void client_session_status(int _client, int _status);

};