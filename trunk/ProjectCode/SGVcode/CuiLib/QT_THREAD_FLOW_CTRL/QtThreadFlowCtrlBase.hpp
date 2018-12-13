#pragma once

#include "cpp_stl.h"
/*-------------------------------------*/
#define CLIENT_SESSION_TOTAL (1)
/*-------------------------------------*/
#include "qt_all.h"
/*-------------------------------------*/
#include "MY_SDK_LIB/TimeMeasure.hpp"
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
class QtThreadFlowCtrlBase :public QtThreadSocketClient
{
	Q_OBJECT
public:

	explicit QtThreadFlowCtrlBase(QObject *parent = Q_NULLPTR);
	~QtThreadFlowCtrlBase(void);
private:
	int  mWorkFlowDoneClientThreads;
	int  mClientSessionCount;
protected:
	void emit_roller_done();
public:
	int   getWorkFlowDones();	
	void  setWorkFlowDones(int _work_flow);
	
	int  getClientSessionCount();
	void  setClientSessionCount(int _count);
	
public:

	int wait4WorkFlowStart();
	int wait4ImgProcessResult();

public:


signals:
	
	void status_sjts(int);

};