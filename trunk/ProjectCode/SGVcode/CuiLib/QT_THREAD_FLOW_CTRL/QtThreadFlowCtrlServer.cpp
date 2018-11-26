//#include "stdafx.h"
#include "QtThreadFlowCtrlServer.hpp"


/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
QtThreadFlowCtrlServer::QtThreadFlowCtrlServer(QObject * parent):QtThreadBase(parent)
{

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
QtThreadFlowCtrlServer::~QtThreadFlowCtrlServer(void)
{
	qDebug() << "QtThreadFlowCtrlServer is Release ! ";
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadFlowCtrlServer::run()
{
	mQtTcpServer=QSharedPointer<QtTcpServerFlowCtrl>(new QtTcpServerFlowCtrl(Q_NULLPTR));

	mQtTcpServer->StartListen();

	while (M_THREAD_RUN){
		this->exec();
	}

	this->StopTcpServer();

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadFlowCtrlServer::StopTcpServer()
{

	if (mQtTcpServer.isNull()){
		mQtTcpServer->close();
		mQtTcpServer.clear();
	}

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadFlowCtrlServer::NotifiedClientSession(CMD_CTRL::CMD_TYPE_LOCAL _event)
{	
	this->mQtTcpServer->NotifiedClientSession(_event);

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
