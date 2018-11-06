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

#include "../SocketQT/QtThreadSocketClient.hpp"
#include "QtThreadFlowCtrlServerSession.hpp"

#include "../SocketQT/QtTcpClient.hpp"
#include "../SocketQT/QtTcpServer.hpp"
#include "../SocketQT/conf_ip.h"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
class QtThreadFlowCtrlServer :public QtTcpServer
{
	Q_OBJECT
public:
	explicit QtThreadFlowCtrlServer(QObject *parent);
	~QtThreadFlowCtrlServer(void);
protected:
	virtual void incomingConnection(qintptr socketDescriptor);
	


public:



};