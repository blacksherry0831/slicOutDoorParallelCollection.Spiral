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
#include "QtThreadFlowCtrlSession.hpp"
/*-------------------------------------*/
#include "SocketQT/QtThreadSocketClient.hpp"
#include "SocketQT/QtTcpClient.hpp"
#include "SocketQT/QtTcpServer.hpp"
#include "SocketQT/conf_ip.h"
/*-------------------------------------*/
#include "QT_SDK_LIB/QtThreadBase.hpp"
/*-------------------------------------*/
#include "QT_THREAD_FLOW_CTRL/QtTcpServerFlowCtrl.hpp"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
class QtThreadFlowCtrlServer:public QtThreadBase
{
	Q_OBJECT
public:
	explicit QtThreadFlowCtrlServer(QObject *parent);
	~QtThreadFlowCtrlServer(void);

signals:
	void work_flow_done(int,int);
	void client_sessions_status(QString, int, int);
	/**<session (name,run/stop,status)*/
private:
	QSharedPointer<QtTcpServerFlowCtrl>	 mQtTcpServer;
protected:
	virtual void run();
private:
	void connecTcp2Thread();
	void StopDestoryTcpServer();
public:	
	void closeServerAsync();
public:
	void NotifiedClientSession(CMD_WORK_FLOW::WF_FPGA_INNER _event, int _cmd_idx=0);
	void beforeNotifiedClientSession(CMD_WORK_FLOW::WF_FPGA_INNER _event);
	QVector<QString> getThreadRunningSessionIpAddr();
private:

};