//#include "stdafx.h"
#include "QtTcpServer.hpp"
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
QtTcpServer::QtTcpServer(QObject *parent) :QtTcpServerClientSession(parent)
{

}
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
QtTcpServer::QtTcpServer(QObject *parent, QSharedPointer<QtThreadSocketClient> _clientThread) :QtTcpServerClientSession(parent,_clientThread)
{
	
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
QtTcpServer::~QtTcpServer()
{

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtTcpServer::incomingConnection(qintptr socketDescriptor)
{
	qDebug() << "New Connect is connect" << socketDescriptor;
	
	QSharedPointer<QtThreadSocketClient> client_thread=QSharedPointer<QtThreadSocketClient>(new QtThreadClientCtrl(socketDescriptor));

	client_thread->start();

	
	this->ProcessRunningThread(client_thread);

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int QtTcpServer::StartListen()
{	
	return this->listen(QHostAddress::Any, mPort);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/

