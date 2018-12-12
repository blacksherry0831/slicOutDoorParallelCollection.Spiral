//#include "stdafx.h"
#include "QtTcpServerClientSession.hpp"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/

/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
QtTcpServerClientSession::QtTcpServerClientSession(QObject *parent) :QTcpServer(parent)
{

}
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
QtTcpServerClientSession::QtTcpServerClientSession(QObject *parent, QSharedPointer<QtThreadSocketClient> _clientThread) :QTcpServer(parent)
{
	
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/

QtTcpServerClientSession::~QtTcpServerClientSession()
{
	this->RemoveDoneThread();
	m_clientThreads.clear();
	
}

/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/

/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtTcpServerClientSession::RemoveDoneThread()
{	
	QMutexLocker locker(&m_clients_mutex);
	
	for (QList<QSharedPointer<QtThreadSocketClient>>::iterator item = m_clientThreads.begin(); item != m_clientThreads.end(); item++){

		if ((*item)->isFinished()) {

			m_clientThreads.removeOne(*item);
		}

	}		
	
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtTcpServerClientSession::SaveRunningThread(QSharedPointer<QtThreadSocketClient> _client)
{
	QMutexLocker locker(&m_clients_mutex);
	m_clientThreads.push_back(_client);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtTcpServerClientSession::ProcessRunningThread(QSharedPointer<QtThreadSocketClient> _client)
{
	this->SaveRunningThread(_client);
	this->RemoveDoneThread();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtTcpServerClientSession::execMy()
{
	this->RemoveDoneThread();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void  QtTcpServerClientSession::SendMsg2ClientSession(QSharedPointer<CMD_CTRL> _msg)
{
	QMutexLocker locker(&m_clients_mutex);
	
	for (QList<QSharedPointer<QtThreadSocketClient>>::iterator item = m_clientThreads.begin();item != m_clientThreads.end();item++)
	{

		if ((*item)->isRunning()) {

			(*item)->SetMsg(_msg);
			
		}

	}

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtTcpServerClientSession::BeforeSendMsg2ClientSession(QSharedPointer<CMD_CTRL> _msg)
{
	QMutexLocker locker(&m_clients_mutex);

	for (QList<QSharedPointer<QtThreadSocketClient>>::iterator item = m_clientThreads.begin(); item != m_clientThreads.end(); item++)
	{

		if ((*item)->isRunning()) {

			Q_ASSERT(_msg->IsThisCmd00(CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_START));
			if (_msg->IsThisCmd00(CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_START)) {
				(*item)->beforeSendMsg();
			}
		

		}

	}
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
QVector<QString>  QtTcpServerClientSession::getRunningSessionIpAddr()
{
	QMutexLocker locker(&m_clients_mutex);

	QVector<QString> list_t;
	
	for (QList<QSharedPointer<QtThreadSocketClient>>::iterator item = m_clientThreads.begin(); item != m_clientThreads.end(); item++)
	{

		if ((*item)->isRunning()) {

			  QString ip_str_t=(*item)->GetIpAddr();
			  list_t.push_back(ip_str_t);

		}

	}

	return list_t;
	
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void  QtTcpServerClientSession::StopRunningThread()
{
	
	QMutexLocker locker(&m_clients_mutex);



	for (QList<QSharedPointer<QtThreadSocketClient>>::iterator item = m_clientThreads.begin(); item != m_clientThreads.end(); item++)
	{
		QSharedPointer<QtThreadSocketClient>  thread_socket_session_t = (*item);
		if (thread_socket_session_t->isRunning()) {
			thread_socket_session_t->closeServerAsync();
		}

	}

	
} 
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int QtTcpServerClientSession::IsWorkFlowDoneAllThread()
{
	QMutexLocker locker(&m_clients_mutex);

	int work_flow_done = FALSE;

	for (QList<QSharedPointer<QtThreadSocketClient>>::iterator item = m_clientThreads.begin(); item != m_clientThreads.end(); item++)
	{
		QSharedPointer<QtThreadSocketClient>  thread_socket_session_t = (*item);
		if (thread_socket_session_t->isRunning()) {
			work_flow_done |= thread_socket_session_t->IsWorkFlowDone();
		}

	}

	return work_flow_done;

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void  QtTcpServerClientSession::client_session_status(int _client, int _status)
{

	if (this->IsWorkFlowDoneAllThread())
	{
		//all done
	}

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
