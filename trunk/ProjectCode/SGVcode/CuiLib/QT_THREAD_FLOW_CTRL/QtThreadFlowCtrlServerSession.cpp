//#include "stdafx.h"
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
*/
/*-------------------------------------*/
QtThreadFlowCtrlServerSession::QtThreadFlowCtrlServerSession(qintptr p)
{
	m_socket = QSharedPointer<QtTcpClient>(new QtTcpClient());

	mPort = 2001;

	m_socket->moveToThread(this);
}
/*-------------------------------------*/
/**

*
*/
/*-------------------------------------*/
QtThreadFlowCtrlServerSession::~QtThreadFlowCtrlServerSession(void)
{
	qDebug() << "QtThreadClient is Release ! ";
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadFlowCtrlServerSession::run()
{
	

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