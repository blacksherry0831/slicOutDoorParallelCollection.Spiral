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
#include "MY_SDK_LIB/TimeMeasure.hpp"
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

private:
	int mWorkFlowStep;
	
private:
	void emit_client_session_work_state(int _port,int _done, int _quality);
public:
	virtual void beforeSendMsg();
	void init_work_flow(QSharedPointer<CMD_CTRL> _cmd);
	void record_work_flow(QSharedPointer<CMD_CTRL> _cmd);
protected: 
	virtual void run();
	void RecordCmd(QSharedPointer<CMD_CTRL> _cmd);
protected:
	virtual void run_socket_work();
	virtual void before_enter_thread();
	virtual void after_exit_thread();
};