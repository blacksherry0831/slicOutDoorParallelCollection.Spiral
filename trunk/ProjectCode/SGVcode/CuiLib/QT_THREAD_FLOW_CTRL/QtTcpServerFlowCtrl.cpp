//#include "stdafx.h"
#include "QtTcpServerFlowCtrl.hpp"
#include "QtThreadFlowCtrlServerSession.hpp"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
QtTcpServerFlowCtrl::QtTcpServerFlowCtrl(QObject *parent):QtTcpServer(parent)
{
	this->mServerIpAddr= IPC_GUI_ADDR;
	this->mPort= TCP_PORT_IPC_FLOW_CTRL;
	this->initWorkMode();
}
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
QtTcpServerFlowCtrl::~QtTcpServerFlowCtrl()
{
	
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtTcpServerFlowCtrl::connectClient2Server(const QObject * _sender)
{
	this->connect(_sender, SIGNAL(client_session_work_state(int, int)), this, SLOT(client_session_status(int, int)));
	this->connect(_sender, SIGNAL(running_client_sessions_change()), this, SLOT(running_client_sessions_change()));
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtTcpServerFlowCtrl::incomingConnection(qintptr socketDescriptor)
{
	qDebug() << "New Connect is connect" << socketDescriptor;

	QSharedPointer<QtThreadFlowCtrlServerSession> client_thread = QSharedPointer<QtThreadFlowCtrlServerSession>(new QtThreadFlowCtrlServerSession(socketDescriptor));

	this->connectClient2Server(client_thread.data());

	client_thread->start();

	this->ProcessRunningThread(client_thread);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtTcpServerFlowCtrl::NotifiedClientSession(CMD_CTRL::CMD_TYPE_LOCAL _type_c)
{
	
	QSharedPointer<CMD_CTRL> qsp_cc_t = CMD_CTRL::getLocalCmdEx(_type_c);
			
	this->SendMsg2ClientSession(qsp_cc_t);
		
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtTcpServerFlowCtrl::beforeNotifiedClientSession(CMD_CTRL::CMD_TYPE_LOCAL _type_c)
{
	Q_ASSERT(_type_c==CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_START);

	if (_type_c == CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_START) {
		this->mWorkFlowDone = 0;
	}

	QSharedPointer<CMD_CTRL> qsp_cc_t = CMD_CTRL::getLocalCmdEx(_type_c);
	
	this->BeforeSendMsg2ClientSession(qsp_cc_t);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtTcpServerFlowCtrl::initWorkMode()
{

#if 1
	mWorkMode |= CMD_CTRL::WorkMode::WM_ORG_IMG;
#else
	mWorkMode |= CMD_CTRL::WorkMode::WM_DIFF_IMG;
#endif
	
#if 1
	mWorkMode |= CMD_CTRL::WorkMode::WM_SIZE_FULL;
#else
	mWorkMode |= CMD_CTRL::WorkMode::WM_SIZE_CUT;
#endif

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtTcpServerFlowCtrl::client_session_status(int _client, int _status)
{
	const int done_t = this->IsWorkFlowDoneAllThread();
	if (done_t)
	{
		//all done
		emit work_flow_done(done_t);
	}

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtTcpServerFlowCtrl::running_client_sessions_change()
{
	const int sessions_t = this->getRunningSessionIpAddr().size();

	emit running_client_sessions(sessions_t);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/