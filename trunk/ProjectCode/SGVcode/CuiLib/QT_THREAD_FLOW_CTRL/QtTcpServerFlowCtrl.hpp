#pragma once

#include "cpp_stl.h"

#if defined(QT_VERSION)
#include <QObject>
#include <QtCore>
#include <QtNetwork>
#include <QSharedPointer>
#endif

#include "SocketQT/QtTcpServer.hpp"
#include "socketQT/conf_ip_crack_detect.h"

#include "QT_THREAD_MODULE/QtThreadClientCtrl.hpp"


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
	void NotifiedClientSession(CMD_CTRL::CMD_TYPE_LOCAL _type_c);
	void beforeNotifiedClientSession(CMD_CTRL::CMD_TYPE_LOCAL _type_c);
signals:
	void work_flow_done(int);
	void running_client_sessions(int);
public slots:
	void client_session_work_state_work_flow(int _client, int _status);
	void running_client_sessions_change();
};