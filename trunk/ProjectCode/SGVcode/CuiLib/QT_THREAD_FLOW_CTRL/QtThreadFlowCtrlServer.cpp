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

	this->StopDestoryTcpServer();

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadFlowCtrlServer::StopDestoryTcpServer()
{

	if (mQtTcpServer.isNull()){
		
	}else{
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
	if (	this->isRunning() &&
			M_THREAD_RUN &&
			!this->mQtTcpServer.isNull()
		){				
				this->mQtTcpServer->NotifiedClientSession(_event);
		 }


}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadFlowCtrlServer::closeRunningServer()
{
	if (this->isRunning()) {
		this->closeServer();
	}
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
QVector<QString> QtThreadFlowCtrlServer::getRunningSessionIpAddr()
{
	return this->mQtTcpServer->getRunningSessionIpAddr();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/