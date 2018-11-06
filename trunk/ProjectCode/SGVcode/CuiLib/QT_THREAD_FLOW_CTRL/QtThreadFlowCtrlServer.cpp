//#include "stdafx.h"
#include "QtThreadFlowCtrlServer.hpp"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
QtThreadFlowCtrlServer::QtThreadFlowCtrlServer(QObject * parent):QtTcpServer(parent)
{

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
QtThreadFlowCtrlServer::~QtThreadFlowCtrlServer(void)
{
	qDebug() << "QtThreadClient is Release ! ";
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadFlowCtrlServer::incomingConnection(qintptr socketDescriptor)
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
