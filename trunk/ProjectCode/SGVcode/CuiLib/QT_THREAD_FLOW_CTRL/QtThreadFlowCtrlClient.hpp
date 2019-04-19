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
#include "MY_SDK_LIB/TimeMeasure.hpp"
/*-------------------------------------*/

#include "../SocketQT/QtTcpClient.hpp"
#include "../SocketQT/conf_ip.h"
/*-------------------------------------*/
#include "SocketQT/ChannelsData.hpp"
#include "SocketQT/ChannelsData4Show.hpp"
/*-------------------------------------*/
#include "QT_THREAD_MODULE/QtThreadClientCtrl.hpp"
/*-------------------------------------*/
#include "QtThreadSocketClientRoller.hpp"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
class QtThreadFlowCtrlClient :public  QtThreadSocketClient
{
	Q_OBJECT
public:
	QtThreadFlowCtrlClient();
	~QtThreadFlowCtrlClient(void);

signals:
	void status_sjts_fpga(int, QString);

protected:
	int Is_work_flow_status_fpga(QSharedPointer<CMD_CTRL> _cmd);
	int emit_work_flow_status_sjts(QSharedPointer<CMD_CTRL> _cmd);
public:

	void setRemoteServer();
	void setLocalServer();
public:
	virtual void run_socket_work();
public:
	int wait_4_inner_done();

	int IsImgProcDone();
protected: 


};