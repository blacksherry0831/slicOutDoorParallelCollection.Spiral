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
#include "../SocketQT/QtThreadSocketClient.hpp"
#include "../SocketQT/QtTcpClient.hpp"
#include "../SocketQT/conf_ip.h"
#include "SocketQT/CMD_CTRL_Q.hpp"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
class QtThreadFlowCtrlServerSession :public QtThreadSocketClient
{

public:
	QtThreadFlowCtrlServerSession(qintptr _socket);
	~QtThreadFlowCtrlServerSession(void);
protected:
	
public:

protected: 
	virtual void run();
	void RecordCmd(QSharedPointer<CMD_CTRL> _cmd);
protected:
	virtual void run_socket_work();
	virtual void before_enter_thread();
	virtual void after_exit_thread();
};