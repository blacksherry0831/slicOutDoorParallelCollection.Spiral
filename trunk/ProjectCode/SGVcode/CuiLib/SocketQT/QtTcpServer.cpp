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
	
	QSharedPointer<QtThreadSessionWorkFlow> client_thread;
	Q_ASSERT(FALSE);

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
	bool stat_t= this->listen(QHostAddress::Any, mPort);


	if (false==stat_t)
	{
		mErrorStr =this->errorString().toLocal8Bit().data();
	}

	return stat_t;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/

