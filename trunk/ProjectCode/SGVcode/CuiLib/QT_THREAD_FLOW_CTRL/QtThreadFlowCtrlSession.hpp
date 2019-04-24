#pragma once
/*-------------------------------------*/
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
#include "../SocketQT/QtThreadSessionWorkFlow.hpp"
#include "../SocketQT/QtTcpClient.hpp"
#include "../SocketQT/conf_ip.h"
/*-------------------------------------*/
#include "SocketQT/CMD_CTRL_Q.hpp"
/*-------------------------------------*/
#include "QT_THREAD_MODULE/CMD_WORK_FLOW.hpp"
/*-------------------------------------*/
#include "MY_SDK_LIB/TimeMeasure.hpp"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
class QtThreadFlowCtrlSession :public QtThreadSessionWorkFlow
{
	Q_OBJECT
public:
	QtThreadFlowCtrlSession(qintptr _socket);
	~QtThreadFlowCtrlSession(void);
private:
	void emit_client_session_work_state(int _port,int _done, int _quality);
public:
	void print_result();
public:
	virtual void beforeSendMsg();
	void init_work_flow(QSharedPointer<CMD_CTRL> _cmd);
	void record_work_flow(QSharedPointer<CMD_CTRL> _cmd, QSharedPointer<CMD_CTRL> _cmd_resp);
protected: 
	virtual void run_socket_work();
	virtual void before_enter_thread();
	virtual void after_exit_thread();
protected:

};