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
#include "QtThreadSocketClientSig.hpp"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
class QtThreadFlowCtrlClient :public  QtThreadSocketClientSig
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
#if _DEBUG
	virtual void run();
#endif
	int wait_4_inner_done();

	int IsImgProcDone();
protected: 


};