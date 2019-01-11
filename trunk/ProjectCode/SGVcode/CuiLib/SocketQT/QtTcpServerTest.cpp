//#include "stdafx.h"
#include "QtTcpServerTest.hpp"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
QtTcpServerTest::QtTcpServerTest(QObject *parent) :QtTcpServer(parent)
{

}
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
QtTcpServerTest::QtTcpServerTest(QObject *parent, int _serverPort) :QtTcpServer(parent)
{
	this->mPort = _serverPort;
}
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
QtTcpServerTest::QtTcpServerTest(QObject *parent, QSharedPointer<QtThreadSocketClientCmdQ> _clientThread) :QtTcpServer(parent, _clientThread)
{
	
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
QtTcpServerTest::~QtTcpServerTest()
{

	
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtTcpServerTest::incomingConnection(qintptr socketDescriptor)
{

	qDebug() << "New Connect is connect" << socketDescriptor;
	
	auto client_thread=QSharedPointer<QtThreadSocketClientCmdQ>(new QtThreadClientCtrlTest(socketDescriptor));

	client_thread->start();
		
	this->ProcessRunningThread(client_thread);

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/