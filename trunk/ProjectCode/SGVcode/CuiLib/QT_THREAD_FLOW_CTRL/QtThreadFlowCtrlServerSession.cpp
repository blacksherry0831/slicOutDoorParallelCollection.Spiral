//#include "stdafx.h"
#include "QtThreadFlowCtrlServerSession.hpp"
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
QtThreadFlowCtrlServerSession::QtThreadFlowCtrlServerSession(qintptr _socket):QtThreadSocketClient(_socket)
{
	
}
/*-------------------------------------*/
/**

*
*/
/*-------------------------------------*/
QtThreadFlowCtrlServerSession::~QtThreadFlowCtrlServerSession(void)
{
	qDebug() << __func__;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadFlowCtrlServerSession::run()
{

	this->ClearMsg();

	this->init_socket_client_session();
	
	while (M_THREAD_RUN){
		
		QSharedPointer<CMD_CTRL> cmd_t = this->GetMsg();

		if (cmd_t.isNull()){
			this->SleepMy(100);
		}else{
			this->m_socket->Send_1_cmd(cmd_t.data());
		}
		
	}
	
	this->closeSocket();

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/