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

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadFlowCtrlServer::NotifiedClientSession(CMD_CTRL::CMD_TYPE_02_C _event)
{	
	this->mQtTcpServer->NotifiedClientSession(_event);

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
