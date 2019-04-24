//#include "stdafx.h"
#include "QtTcpServerFlowCtrl.hpp"
#include "QtThreadFlowCtrlSession.hpp"
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
	this->connect(_sender, 
		SIGNAL(client_session_work_state(int, int,int)),
		this,
		SLOT(client_session_work_state_work_flow(int, int,int)));
	
	this->connect(_sender,
		SIGNAL(client_sessions_status(QString, int, int)),
		this,
		SLOT(running_client_sessions_change(QString, int, int)));
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtTcpServerFlowCtrl::incomingConnection(qintptr socketDescriptor)
{
	qDebug() << "New Connect is connect" << socketDescriptor;

	QSharedPointer<QtThreadFlowCtrlSession> client_thread = QSharedPointer<QtThreadFlowCtrlSession>(new QtThreadFlowCtrlSession(socketDescriptor));

	this->connectClient2Server(client_thread.data());

	client_thread->start();

	this->ProcessRunningThread(client_thread);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtTcpServerFlowCtrl::NotifiedClientSession(CMD_WORK_FLOW::WF_FPGA_INNER _type_c,int _cmd_idx)
{
	
	QSharedPointer<CMD_CTRL> qsp_cc_t = CMD_CTRL::getLocalCmdEx(_type_c,0,_cmd_idx);
			
	this->SendMsg2ClientSession(qsp_cc_t);
		
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtTcpServerFlowCtrl::beforeNotifiedClientSession(CMD_WORK_FLOW::WF_FPGA_INNER _type_c)
{
	Q_ASSERT(_type_c==CMD_WORK_FLOW::WF_FPGA_INNER::CT_FPGA_START);

	if (_type_c == CMD_WORK_FLOW::WF_FPGA_INNER::CT_FPGA_START) {
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
void QtTcpServerFlowCtrl::client_session_work_state_work_flow(int _client, int _status,int _quality)
{

	const int done_t = this->IsWorkFlowDoneAllThread();
	const int quality_t = this->WorkFlowDoneQuality();
	if (done_t)
	{
		//all done
		emit work_flow_done(done_t, quality_t);
	}

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtTcpServerFlowCtrl::running_client_sessions_change(QString _ip, int _run, int _s)
{
	emit client_sessions_status(_ip,_run,_s);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/