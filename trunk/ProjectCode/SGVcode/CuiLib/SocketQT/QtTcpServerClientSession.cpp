//#include "stdafx.h"
#include "QtTcpServerClientSession.hpp"
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
*/
/*-------------------------------------*/

QtTcpServerClientSession::~QtTcpServerClientSession()
{
	this->RemoveDoneThread();		
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtTcpServerClientSession::RemoveDoneThread()
{	
	QMutexLocker locker(&m_clients_mutex);
	
	for (const auto& session_t : m_clientThreads){
		if (session_t->isFinished()) {
			m_clientThreads.removeOne(session_t);
		}
	}	
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtTcpServerClientSession::SaveRunningThread(QSharedPointer<QtThreadSessionWorkFlow> _client)
{
	QMutexLocker locker(&m_clients_mutex);
	m_clientThreads.push_back(_client);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtTcpServerClientSession::ProcessRunningThread(QSharedPointer<QtThreadSessionWorkFlow> _client)
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
	
	for (auto& session_t: m_clientThreads){
		if (session_t->isRunning()) {
				session_t->SetMsg(_msg);
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

	for (auto& session_t : m_clientThreads) {
		if (session_t->isRunning()) {
			Q_ASSERT(_msg->IsThisCmd00(CMD_WORK_FLOW::WF_FPGA_INNER::CT_FPGA_START));
			if (_msg->IsThisCmd00(CMD_WORK_FLOW::WF_FPGA_INNER::CT_FPGA_START)) {
				session_t->beforeSendMsg();
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
	QVector<QString> list_t;

	QMutexLocker locker(&m_clients_mutex);

	for (auto& session_t : m_clientThreads) {
		if (session_t->isRunning()) {
			QString ip_str_t = session_t->GetIpAddr();
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

	for (auto& session_t : m_clientThreads) {
		if (session_t->isRunning()) {
			session_t->closeServerAsync();
		}
	}
		
} 
#if 1
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int QtTcpServerClientSession::IsWorkFlowDoneAllThread()
{

	int work_flow_done = 0;

	QMutexLocker locker(&m_clients_mutex);

	for (auto& session_t : m_clientThreads) {		
		
		if (session_t->isRunning()) {
			if (session_t->IsWorkFlowDone()) {
				work_flow_done++;
			}
			else
			{
				return FALSE;
			}
		}
	}

	return work_flow_done;

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int QtTcpServerClientSession::WorkFlowDoneQuality()
{
	QMutexLocker locker(&m_clients_mutex);
#if 1
	if (m_clientThreads.size() == 0) {
		return CMD_CTRL::BodyRollerQualified::UnQualified;
	}
#endif	
	for (auto& session_t : m_clientThreads) {

		if (session_t->isRunning()) {
			const int work_done_result_t = session_t->getWorkFlowResult();
			if (work_done_result_t == CMD_CTRL::BodyRollerQualified::UnQualified) {
				return CMD_CTRL::BodyRollerQualified::UnQualified;
			}
		}
	}

	return CMD_CTRL::BodyRollerQualified::Qualified;
}
#endif
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

	Q_ASSERT(0);

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
