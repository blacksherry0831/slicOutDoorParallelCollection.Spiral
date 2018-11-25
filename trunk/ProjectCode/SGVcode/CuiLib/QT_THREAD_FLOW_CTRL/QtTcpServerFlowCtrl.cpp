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
void QtTcpServerFlowCtrl::incomingConnection(qintptr socketDescriptor)
{
	qDebug() << "New Connect is connect" << socketDescriptor;

	QSharedPointer<QtThreadFlowCtrlServerSession> client_thread = QSharedPointer<QtThreadFlowCtrlServerSession>(new QtThreadFlowCtrlServerSession(socketDescriptor));

	client_thread->start();

	this->ProcessRunningThread(client_thread);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtTcpServerFlowCtrl::NotifiedClientSession(CMD_CTRL::CMD_TYPE_02_C _type_c)
{
	
	QSharedPointer<CMD_CTRL> qsp_cc_t = CMD_CTRL::getFpgaStartCmdEx(_type_c, (CMD_CTRL::WorkMode)mWorkMode);
		
	this->SendMsg2ClientSession(qsp_cc_t);
	
	
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