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
}
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