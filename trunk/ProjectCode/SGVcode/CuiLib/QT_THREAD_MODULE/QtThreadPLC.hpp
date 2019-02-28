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
private:
	int mBe1105RunDir;
private:
	CMD_CTRL_Q  mCmds;
private: 
	void init_serial_port(QSharedPointer<BE_1105_Driver>	 be_1105);
	int stepMotorRun(QSharedPointer<BE_1105_Driver>	 _be_1105);
	int doPlcStepMotorRun(QSharedPointer<BE_1105_Driver>	 _be_1105);


	int stepMotorRunFast(QSharedPointer<BE_1105_Driver>	 _be_1105);
	int stepMotorRunStop(QSharedPointer<BE_1105_Driver>	 _be_1105);

protected:
	void emit_step_motor_start(int _circle);
	void emit_step_motor_stop(int _circle);	
	void process_fatal_error(QSharedPointer<CMD_CTRL> _cmd);
protected:
	void print_undefined_cmd(QSharedPointer<CMD_CTRL> _cmd);
	void print_cmd(QSharedPointer<CMD_CTRL> _cmd);
	void print_socket_connected();
public:
		
	int MoveSlidingThenRunMotor(QSharedPointer<BE_1105_Driver>	 be_1105,const int _pos,int _isRun);

	int sendPlcRollerQualifiedEx(int _qualified);
	int Read_1_plc_cmd_process_hearbeat(QSharedPointer<CMD_CTRL> _cmd);

	void process_plc_cmd(QSharedPointer<CMD_CTRL> _cmd, QSharedPointer<BE_1105_Driver> _be_1105);

	int process_plc_cmd_easy(QSharedPointer<CMD_CTRL> _cmd);



	int wait4PlcResp(QSharedPointer<CMD_CTRL> _cmd);
	int wait4PlcRoolerReady(QSharedPointer<CMD_CTRL> _cmd);

	int wait4PlcRoolerPosReady(QSharedPointer<CMD_CTRL> _cmd, QSharedPointer<BE_1105_Driver>	 _be_1105);

	void do_run_work(QSharedPointer<BE_1105_Driver>	 _be_1105);

protected:

	virtual void run_socket_work();



};