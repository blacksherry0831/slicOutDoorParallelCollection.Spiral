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
#include "QT_THREAD_FLOW_CTRL/QtThreadPlcNetClient.hpp"
/*-------------------------------------*/
#include "QT_THREAD_MODULE/IPlcSjts.hpp"
#include "QT_THREAD_MODULE/IStepMotorSjts.hpp"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
class QtThreadPlcSimulatorClient :public QtThreadPlcNetClient
{
	Q_OBJECT
public:
	explicit QtThreadPlcSimulatorClient(QObject *parent = Q_NULLPTR);
	~QtThreadPlcSimulatorClient(void);
protected:
	bool mBlock;
public:
	void SetBlock(bool _block);
	void process_roller_done_end();
public:
	virtual void run();
	virtual void run_thread_work();
	virtual int socket_thread_run_condition();
private:
	void run_normal();
public:
	virtual void ProcMsgEx(QSharedPointer<CMD_CTRL> _msg);
		
};