//#include "stdafx.h"
#include "QtTcpServerFlowCtrl.hpp"
#include "QtThreadFlowCtrlServerSession.hpp"
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


