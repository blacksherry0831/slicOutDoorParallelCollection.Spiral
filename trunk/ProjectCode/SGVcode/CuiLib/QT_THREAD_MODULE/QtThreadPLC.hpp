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
#include "QT_THREAD_MODULE/QtThreadPlcSocket.hpp"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
class QtThreadPLC :public QtThreadPlcSocket,public StepMotorSjts
{
	Q_OBJECT
public:

public:
	explicit QtThreadPLC(QObject *parent = Q_NULLPTR);
	~QtThreadPLC(void);
private: 
	int stepMotorRun();
	int doPlcStepMotorRun();
protected:
	virtual void  init_serial_port();
public:
	
	int MoveSlidingThenRunMotor(const int _pos,int _isRun);

	int sendPlcRollerQualifiedEx(int _qualified);
	
	void process_plc_cmd(QSharedPointer<CMD_CTRL> _cmd);	

	int wait4PlcResp(QSharedPointer<CMD_CTRL> _cmd);
	int wait4PlcRoolerReady(QSharedPointer<CMD_CTRL> _cmd);
	int wait4PlcRoolerPosReady(QSharedPointer<CMD_CTRL> _cmd);

protected:
		virtual void	do_sjts_roller_ready();
		virtual void    do_sjts_roller_pos_ready();
		virtual void	do_sjts_roller_run(const int _step);
protected:
		virtual void do_run_work();
		virtual void run_socket_work();
		
};