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
#include "SocketQT/QtThreadSocketClient.hpp"
#include "SocketQT/QtTcpClient.hpp"
#include "SocketQT/conf_ip.h"
#include "SocketQT/CMD_CTRL_Q.hpp"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
class QtThreadFlowCtrlLocal :public QtThreadBase
{
	Q_OBJECT
public:

	explicit QtThreadFlowCtrlLocal(QObject *parent = Q_NULLPTR);
	~QtThreadFlowCtrlLocal(void);
public:
	
	virtual void run();
signals:
	
	void status_sjts(int);

};