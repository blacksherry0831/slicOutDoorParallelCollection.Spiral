#pragma once

#include "cpp_stl.h"
/*-------------------------------------*/
#include <QObject>
#include <QtCore>
#include <QtNetwork>
#include <QSharedPointer>
/*-------------------------------------*/
#include "SocketQT/QtTcpServer.hpp"
#include "socketQT/conf_ip_crack_detect.h"
/*-------------------------------------*/
#include "QT_THREAD_MODULE/QtThreadClientCtrl.hpp"
#include "QT_THREAD_MODULE/CMD_WORK_FLOW.hpp"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
class QtTcpServerFlowCtrl :public QtTcpServer
{
	Q_OBJECT
public:
	
	 QtTcpServerFlowCtrl(QObject *parent = nullptr);
	
	 ~QtTcpServerFlowCtrl();

protected:
	void connectClient2Server(const QObject* _sender);
	virtual void incomingConnection(qintptr socketDescriptor);
	int mWorkMode;
	void initWorkMode();
private:
	int mWorkFlowDone;
public:
	void NotifiedClientSession(CMD_WORK_FLOW::WF_FPGA_INNER _type_c,int _cmd_idx=0);
	void beforeNotifiedClientSession(CMD_WORK_FLOW::WF_FPGA_INNER _type_c);
signals:
	void work_flow_done(int,int);
	void client_sessions_status(QString, int, int);
public slots:
	void client_session_work_state_work_flow(int _client, int _status, int _quality);
	void running_client_sessions_change(QString _ip, int _run, int _s);
};