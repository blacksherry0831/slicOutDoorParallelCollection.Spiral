#include "QtThreadSession.hpp"
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
QtThreadSession::QtThreadSession(qintptr _socket):QtThreadSocketClient(_socket)
{
	
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
QtThreadSession::~QtThreadSession(void)
{
	qDebug() << __func__;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadSession::run()
{
	this->before_enter_thread();

	this->init_socket_in_thread();

	this->enter_thread();
	 {
			while (socket_thread_run_condition()) {

				this->run_socket_work();

			}

	}
	this->exit_thread();

	this->destory_socket_in_thread();

	this->after_exit_thread();

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadSession::run_socket_work()
{


}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadSession::enter_thread()
{
	this->emit_thread_starting();
	emit client_sessions_status(this->GetClientSessionIpAddr(),1,0);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadSession::exit_thread()
{
	this->emit_thread_stopping();
	emit client_sessions_status(this->GetClientSessionIpAddr(),0,0);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
