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
/*-------------------------------------*/
#include "QT_THREAD_FLOW_CTRL/QtThreadFlowCtrlBase.hpp"
/*-------------------------------------*/
#include "QT_THREAD_MODULE/IPlcSjts.hpp"
#include "QT_THREAD_MODULE/StepMotorSjts.hpp"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
class QtThreadPlcSocket :public QtThreadFlowCtrlBase,public IPlcSjts
{
	Q_OBJECT
public:
	explicit QtThreadPlcSocket(QObject *parent = Q_NULLPTR);
	~QtThreadPlcSocket(void);
private:
	int mCurrentStep;
protected:
	QString GetIntoStepStr();
	int     GetIntoStep();
	void    IncIntoStep();
	void    InitIntoStep();
protected:
	int SendPlcIntoInterEx();
protected:
	void print_socket_connected();
protected:
	virtual void process_fatal_error_msg(QString _msg);
protected:
	void emit_step_motor_start(int _circle);
	void emit_step_motor_stop(int _circle);
	void emit_init_serial_status(int _isOpen);
public:
	virtual int  Read_1_plc_cmd_process_hearbeat(QSharedPointer<CMD_CTRL> _cmd);
	virtual void process_plc_cmd(QSharedPointer<CMD_CTRL> _cmd);
	virtual int  process_plc_cmd_async(QSharedPointer<CMD_CTRL> _cmd);
protected:
	virtual void do_run_work();
	virtual void run_socket_work();
		
};