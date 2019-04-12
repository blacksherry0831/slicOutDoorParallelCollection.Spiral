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
#include "QT_THREAD_MODULE/QtThreadPlcSocket.hpp"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
class QtThreadPlcSocketClient :public QtThreadPlcSocket,public QtMsgCmdQ
{
	Q_OBJECT
public:
	explicit QtThreadPlcSocketClient(QObject *parent = Q_NULLPTR);
	~QtThreadPlcSocketClient(void);
public:
	virtual void SetMsgWhileRunning(QSharedPointer<CMD_CTRL> _msg);

protected:
	virtual int process_plc_cmd_async(QSharedPointer<CMD_CTRL> _cmd);
protected:
     virtual void emit_roller_abort();
	virtual  void emit_roller_pos_ready();
protected:
	virtual void	do_sjts_roller_ready();
	virtual void    do_sjts_roller_pos_ready();
protected:
	virtual void	do_run_work();
public:
	virtual void ProcMsgEx(QSharedPointer<CMD_CTRL> _msg);
		
};