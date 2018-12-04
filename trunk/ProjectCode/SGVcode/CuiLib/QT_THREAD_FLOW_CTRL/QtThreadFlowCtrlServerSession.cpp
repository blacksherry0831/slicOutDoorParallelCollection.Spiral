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

	this->before_enter_thread();
	this->enter_thread();

	this->init_socket_in_thread();

	do {

		this->connect2ServerIfNoConnected();

		while (M_THREAD_RUN && mSocketConnected) {

			this->run_socket_work();

		}

		this->close_destory_socket_4_server();

	} while (0);

	this->destory_socket_in_thread();

	this->exit_thread();
	this->after_exit_thread();

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadFlowCtrlServerSession::run_socket_work()
{
	QSharedPointer<CMD_CTRL> cmd_t = this->GetMsg();

	if (cmd_t.isNull()) {
		this->SleepMy(100);
	}
	else {
		Send_1_cmd(cmd_t);
	}

	if (0 == mSleepTime % (HEART_BEAT_FREQUENCY*1000)) {
		mSocketConnected = SendHearbeatCmd();
	}

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadFlowCtrlServerSession::before_enter_thread()
{
	this->ClearMsg();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadFlowCtrlServerSession::after_exit_thread()
{

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/