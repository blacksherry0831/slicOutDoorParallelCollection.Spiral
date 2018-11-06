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
#include "../SocketQT/QtTcpClient.hpp"
#include "../SocketQT/conf_ip.h"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
class QtThreadFlowCtrlServerSession :public QtThreadSocketClient
{

public:
	QtThreadFlowCtrlServerSession(qintptr p);
	~QtThreadFlowCtrlServerSession(void);
protected:
	
public:

protected: 
	virtual void run();

};