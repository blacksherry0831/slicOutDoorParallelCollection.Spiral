#pragma once

#include "cpp_stl.h"
#include <QObject>
#include <QtCore>
#include <QtNetwork>
#include <QSharedPointer>

#include "../SocketQT/QtThreadClientCtrl.hpp"
#include "../SocketQT/QtTcpServer.hpp"
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
	explicit QtTcpServerFlowCtrl(QObject *parent );
	
	 ~QtTcpServerFlowCtrl();

protected:
	virtual void incomingConnection(qintptr socketDescriptor);
private:	


};