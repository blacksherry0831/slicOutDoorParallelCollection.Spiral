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