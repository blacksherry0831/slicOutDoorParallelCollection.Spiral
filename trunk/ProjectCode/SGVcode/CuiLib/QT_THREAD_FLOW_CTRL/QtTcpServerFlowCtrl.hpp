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
	virtual void incomingConnection(qintptr socketDescriptor);
	int mWorkMode;
	void initWorkMode();
public:
	void NotifiedClientSession(CMD_CTRL::CMD_TYPE_LOCAL _type_c);

};