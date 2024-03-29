#pragma once

#include "cpp_stl.h"

#if defined(QT_VERSION)
#include "qt_all.h"
#endif
/*-------------------------------------*/
#include "SocketQT/QtThreadSocketClient.hpp"
/*-------------------------------------*/
#include "SocketQT/QtTcpClient.hpp"
#include "SocketQT/CMD_CTRL_Q.hpp"
/*-------------------------------------*/
#include "SocketQT/conf_ip.h"
/*-------------------------------------*/
#include "QT_THREAD_MODULE/CMD_WORK_FLOW.hpp"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
class QtThreadClientCtrl :public QtThreadSocketClient
{
	Q_OBJECT
public:
	QtThreadClientCtrl();
	QtThreadClientCtrl(qintptr p);
	~QtThreadClientCtrl(void);
protected:
	void initIpPort();
	QSharedPointer <QtThreadSocketClient> mDataPipe;
public:
	void SetDataPipe(QSharedPointer <QtThreadSocketClient> _dataPipe);
	int IsDataPipeOK();
private:
	CMD_CTRL::WorkMode mWorkMode;
	int mHeartBeatFreq;
private:
	static CMD_CTRL_Q cmds;
	static QSharedPointer<CMD_CTRL> GetCmd();
public:
	void SetCmdWhenRunning(QSharedPointer<CMD_CTRL> _cmd);
	static void SetCmd(QSharedPointer<CMD_CTRL> _cmd);
	static void SetLocalCmd(int cmd_00);
	static void ClearCmd();
	
public:
	static int IsCmdLocalFPGA(QSharedPointer<CMD_CTRL> _cmd);
public:

	int SendCmdCtrl();

	int ProcessCmds();

	int ProcessLocalCmds(QSharedPointer<CMD_CTRL> cmd_ctrl_t);
	int ProcessRemoteCmds(QSharedPointer<CMD_CTRL> cmd_ctrl_t);

	void SetWorkMode(CMD_CTRL::WorkMode _wm);

	void SetWorkModeCmd(CMD_CTRL::WorkMode _wm);
	
	void SetImgSigmaCmd(int _sigma);
	void GetImgSigmaCmd();

	int SendCmd2FPGA(CMD_CTRL::CMD_TYPE_02_C _start_stop, uint _circle_seq);
	
	void emit_cmd_resp(QSharedPointer<CMD_CTRL> _cmd_ctrl, QSharedPointer<CMD_CTRL> _cmd_resp);
public slots:

signals:
	   void SigmaChanged(int);


protected:

#if _DEBUG
	virtual void run();
#endif

	virtual void run_socket_work();

};