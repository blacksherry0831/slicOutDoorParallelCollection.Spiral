#pragma once

#include "cpp_stl.h"
#include <QObject>
#include <QtCore>
#include <QtNetwork>
#include <QSharedPointer>

#include "SocketQT/QtThreadClientCtrl.hpp"
#include "SocketQT/QtTcpServer.hpp"

#include "socketQT/conf_ip_crack_detect.h"
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
	virtual void incomingConnection(qintptr socketDescriptor);
	int mWorkMode;
	void initWorkMode();
public:
	void NotifiedClientSession(CMD_CTRL::CMD_TYPE_LOCAL _type_c);

};