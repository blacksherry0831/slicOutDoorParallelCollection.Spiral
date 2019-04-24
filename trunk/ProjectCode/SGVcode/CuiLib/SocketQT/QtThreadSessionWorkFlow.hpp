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
#include "SocketQT/QtThreadSession.hpp"
#include "SocketQT/QtTcpClient.hpp"
#include "SocketQT/conf_ip.h"
#include "SocketQT/CMD_CTRL_Q.hpp"
/*-------------------------------------*/
#include "MY_SDK_LIB/TimeMeasure.hpp"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
class QtThreadSessionWorkFlow :public QtThreadSession
{
	Q_OBJECT
public:
	QtThreadSessionWorkFlow(qintptr _socket);
	~QtThreadSessionWorkFlow(void);
protected:
	int mWorkFlowStart;
	int mWorkFlowEnd;
	int mWorkFlowResult;
	int mWorkFlowStep;
public:
	int IsWorkFlowDone();
	int getWorkFlowResult();
public:
	virtual void beforeSendMsg();
};