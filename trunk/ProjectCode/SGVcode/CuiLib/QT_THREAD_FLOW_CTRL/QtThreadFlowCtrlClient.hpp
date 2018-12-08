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
#include "QT_THREAD_MODULE/QtThreadClientCtrl.hpp"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
class QtThreadFlowCtrlClient :public QtThreadSocketClient
{
	Q_OBJECT
public:
	QtThreadFlowCtrlClient();
	~QtThreadFlowCtrlClient(void);
protected:
	int emit_work_flow_status_sjts(QSharedPointer<CMD_CTRL> _cmd);
public:

	void setRemoteServer();
	void setLocalServer();
public:
	void run_socket_work();
	virtual void run();
	void wait_4_inner_done();
protected: 
/*-------------------------------------*/
signals:
		void status_sjts(int);
/*-------------------------------------*/

};