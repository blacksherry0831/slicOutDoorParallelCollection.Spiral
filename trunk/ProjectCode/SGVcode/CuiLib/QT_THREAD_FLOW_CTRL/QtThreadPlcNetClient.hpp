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
#include "SocketQT/QtThreadSocketClient.hpp"
#include "SocketQT/QtTcpClient.hpp"
#include "SocketQT/conf_ip.h"
#include "SocketQT/CMD_CTRL_Q.hpp"
#include "SocketQT/QtMsgCmdQ.hpp"
/*-------------------------------------*/
#include "QT_THREAD_FLOW_CTRL/QtThreadFlowCtrlBase.hpp"
/*-------------------------------------*/
#include "QT_THREAD_MODULE/IPlcSjts.hpp"
#include "QT_THREAD_MODULE/IStepMotorSjts.hpp"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
class QtThreadPlcNetClient :public QtThreadFlowCtrlBase
{
	Q_OBJECT
public:
	explicit QtThreadPlcNetClient(QObject *parent = Q_NULLPTR);
	~QtThreadPlcNetClient(void);
private:
	int		mMotor[3];
	int		mSessions[3];
	const int SESSIONS = 1;
protected:
	void init_external_status();
	int is_external_valid();
public:
	void set_external_status(int _idx,int _m,int _s);
	void set_external_motor(int _m);
	void set_external_session(int _s);
public:
	virtual void	do_sjts_roller_ready();
public:
	void SetBlock(int _block);
	void process_roller_done_end();
	int  process_plc_cmd_async(QSharedPointer<CMD_CTRL> _cmd);
protected:
	virtual void	run_socket_work();
public:
	virtual void	ProcMsgEx(QSharedPointer<CMD_CTRL> _msg);
		
};