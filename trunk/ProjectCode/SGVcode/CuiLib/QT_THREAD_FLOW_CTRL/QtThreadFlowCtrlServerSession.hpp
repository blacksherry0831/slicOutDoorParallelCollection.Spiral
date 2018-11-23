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
#include "SocketQT/CMD_CTRL_Q.hpp"
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

private:
	CMD_CTRL_Q  mCmds;
public:

protected: 
	virtual void run();

};