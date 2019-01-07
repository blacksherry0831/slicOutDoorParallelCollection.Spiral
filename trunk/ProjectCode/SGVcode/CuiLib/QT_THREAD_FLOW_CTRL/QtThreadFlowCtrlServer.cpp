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
void  QtThreadFlowCtrlServer::connecTcp2Thread()
{
	const QObject* sender_t = mQtTcpServer.data();

	this->connect(sender_t,
		SIGNAL(work_flow_done(int,int)),
		this,
		SIGNAL(work_flow_done(int,int)));

	this->connect(sender_t,
		SIGNAL(running_client_sessions(int)),
		this,
		SIGNAL(running_client_sessions(int)));

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

	this->connecTcp2Thread();

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
void QtThreadFlowCtrlServer::beforeNotifiedClientSession(CMD_CTRL::CMD_TYPE_LOCAL _event)
{
	if (this->isRunning() &&
		M_THREAD_RUN &&
		!this->mQtTcpServer.isNull()
		) {
		this->mQtTcpServer->beforeNotifiedClientSession(_event);
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

	if (!this->mQtTcpServer.isNull()){
			return this->mQtTcpServer->getRunningSessionIpAddr();
		}

	return QVector<QString>();

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