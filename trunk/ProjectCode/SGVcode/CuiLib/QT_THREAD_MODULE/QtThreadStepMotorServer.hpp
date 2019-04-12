#pragma once
/*-------------------------------------*/
#include "cpp_stl.h"
/*-------------------------------------*/
#include "qt_all.h"
/*-------------------------------------*/
#include "SocketQT/conf_ip.h"
/*-------------------------------------*/
#include "SocketQT/QtThreadBase.hpp"
#include "SocketQT/CMD_CTRL.hpp"
#include "SocketQT/QtThreadSocketClient.hpp"
#include "SocketQT/QtMsgCmdQ.hpp"
/*-------------------------------------*/
#include "QT_THREAD_MODULE/QtThreadClientCtrl.hpp"
#include "QT_THREAD_MODULE/StepMotorSjts.hpp"
#include "QT_THREAD_MODULE/CMD_WORK_FLOW.hpp"
/*-------------------------------------*/
#include "QT_THREAD_FLOW_CTRL/QtThreadFlowCtrlLocal.hpp"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
class QtThreadStepMotorServer:public QtThreadBase, public StepMotorSjts,public QtMsgCmdQ
{
	Q_OBJECT
public:
	QtThreadStepMotorServer();
	~QtThreadStepMotorServer(void);
signals:
	void status_sjts_motor(int, QString);
protected:
	void emit_step_motor_start(int _circle);
	void emit_step_motor_stop(int _circle);
protected:
	virtual void  emit_init_serial_status(int _isOpen);
public:	
	virtual void ProcMsgEx(QSharedPointer<CMD_CTRL> _msg);
public:
	virtual void run();
public:
	virtual void SetMsgWhileRunning(QSharedPointer<CMD_CTRL> _msg);
	
};
