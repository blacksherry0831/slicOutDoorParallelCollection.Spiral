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
/**
*
*
*/
/*-------------------------------------*/
#include "QtThreadFlowCtrlServerSession.hpp"
/*-------------------------------------*/
#include "SocketQT/QtThreadSocketClient.hpp"
#include "SocketQT/QtTcpClient.hpp"
#include "SocketQT/QtTcpServer.hpp"
#include "SocketQT/conf_ip.h"
#include "socketQT/QtThreadBase.hpp"
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
	void work_flow_done(int);
	void running_client_sessions(int);
	
private:
	QSharedPointer<QtTcpServerFlowCtrl>	 mQtTcpServer;
protected:
	virtual void run();
private:
	void connecTcp2Thread();
	void StopDestoryTcpServer();
public:
	void closeRunningServer();
public:
	void NotifiedClientSession(CMD_CTRL::CMD_TYPE_LOCAL _event);
	void beforeNotifiedClientSession(CMD_CTRL::CMD_TYPE_LOCAL _event);
	QVector<QString> getRunningSessionIpAddr();
private:
	int IsWorkFlowDoneAllThread();

public slots:
	void tcp_server_work_flow_dones(int _status);
	void tcp_server_running_client_sessions(int _running_sessions);


};