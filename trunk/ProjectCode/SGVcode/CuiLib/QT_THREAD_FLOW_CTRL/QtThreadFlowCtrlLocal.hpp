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
#include "QT_THREAD_FLOW_CTRL/QtThreadFlowCtrlBase.hpp"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
class QtThreadFlowCtrlLocal :public QtThreadFlowCtrlBase
{
	Q_OBJECT
public:

	explicit QtThreadFlowCtrlLocal(QObject *parent = Q_NULLPTR);
	~QtThreadFlowCtrlLocal(void);
public:
	void SetBlock(bool _block);
	
private: 
	bool mBlock;
public:
	virtual void run();

	virtual int socket_thread_run_condition();

	

};