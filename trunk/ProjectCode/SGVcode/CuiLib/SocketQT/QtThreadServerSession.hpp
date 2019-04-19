#pragma once

#include "cpp_stl.h"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
#include "qt_all.h"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
#include "SocketQT/QtThreadSocketClientCmdQ.hpp"
#include "SocketQT/QtTcpClient.hpp"
#include "SocketQT/conf_ip.h"
#include "SocketQT/CMD_CTRL_Q.hpp"
/*-------------------------------------*/
#include "MY_SDK_LIB/TimeMeasure.hpp"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
class QtThreadServerSession :public QtThreadSocketClientCmdQ
{
	Q_OBJECT
public:
	QtThreadServerSession(qintptr _socket);
	~QtThreadServerSession(void);

signals:
	void client_sessions_status(QString,int,int);/**<session (name,run/stop,status)*/
protected: 	
	virtual void enter_thread();
	virtual void exit_thread();
	virtual void run();
	virtual void run_socket_work();	
protected:


};