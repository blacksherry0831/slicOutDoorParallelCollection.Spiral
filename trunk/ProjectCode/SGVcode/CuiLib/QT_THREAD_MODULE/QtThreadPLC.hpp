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

/*-------------------------------------*/
#include "../SocketQT/QtThreadSocketClient.hpp"
#include "../SocketQT/QtTcpClient.hpp"
#include "../SocketQT/conf_ip.h"
#include "SocketQT/CMD_CTRL_Q.hpp"
/*-------------------------------------*/
#include "QT_THREAD_FLOW_CTRL/QtThreadFlowCtrlBase.hpp"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
class QtThreadPLC :public QtThreadFlowCtrlBase
{
	Q_OBJECT
public:

public:
	explicit QtThreadPLC(QObject *parent = Q_NULLPTR);
	~QtThreadPLC(void);
public:
	
private:
	CMD_CTRL_Q  mCmds;
private: 
	
	void init_serial_port(QSharedPointer<BE_1105_Driver>	 be_1105);
	int stepMotorRun(QSharedPointer<BE_1105_Driver>	 _be_1105);
protected:
	void emit_step_motor_start(int _circle);
	void emit_step_motor_stop(int _circle);
	
	void emit_roller_done_qualified(CMD_CTRL::BodyRollerQualified _qualified);

public:
		
	int MoveSlidingThenRunMotor(QSharedPointer<BE_1105_Driver>	 be_1105,const int _pos,int _isRun);

	int read_plc_cmd(QSharedPointer<CMD_CTRL> _cmd);
	void process_plc_cmd(QSharedPointer<CMD_CTRL> _cmd, QSharedPointer<BE_1105_Driver> _be_1105);

	int process_plc_cmd_easy(QSharedPointer<CMD_CTRL> _cmd);

	void printf_event(std::string _event,std::string _msg);

	int wait4PlcResp(QSharedPointer<CMD_CTRL> _cmd);
	int wait4PlcRoolerReady(QSharedPointer<CMD_CTRL> _cmd);

	void do_run_work(QSharedPointer<BE_1105_Driver>	 _be_1105);

protected:

	virtual void run_socket_work();



};