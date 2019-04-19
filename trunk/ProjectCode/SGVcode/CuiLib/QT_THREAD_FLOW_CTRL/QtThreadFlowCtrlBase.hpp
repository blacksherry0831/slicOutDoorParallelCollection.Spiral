#pragma once

#include "cpp_stl.h"
/*-------------------------------------*/
#define CLIENT_SESSION_TOTAL (1)
/*-------------------------------------*/
#include "qt_all.h"
/*-------------------------------------*/
#include "MY_SDK_LIB/TimeMeasure.hpp"
/*-------------------------------------*/
#include "SocketQT/QtTcpClient.hpp"
#include "SocketQT/conf_ip.h"
/*-------------------------------------*/
#include "SocketQT/CMD_CTRL_Q.hpp"
/*-------------------------------------*/
#include "QtThreadSocketClientRoller.hpp"
/*-------------------------------------*/

/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
class QtThreadFlowCtrlBase :public QtThreadSocketClientRoller
{
	Q_OBJECT
public:
	explicit QtThreadFlowCtrlBase(QObject *parent = Q_NULLPTR);
	~QtThreadFlowCtrlBase(void);
private:
	int  mWorkFlowDoneClientThreads;
	int  mWorkFlowDoneClientThreadsResult;
	int  mClientSessionCount;
protected:
	virtual void emit_roller_ready();
	virtual void emit_roller_pos_ready();
	virtual void emit_roller_into_inner_ready();
	virtual void emit_roller_done();
	virtual void emit_roller_abort();
protected:
	void emit_machine_error(QString _msg);
	void process_machine_error(QString _msg);
	QString  CircleSeq();
protected:
	int sendPlcResp(CMD_CTRL::CMD_TYPE_02_RESP _type);
	int SendPlcIntoInter(int _step);
public:
	int   getWorkFlowDones();	
	int   getWorkFLowResult();
	CMD_CTRL::BodyRollerQualified   getWorkFLowQualified();
	void  setWorkFlowDones(int _work_flow, int _result);
	
	int   getClientSessionCount();
	void  setClientSessionCount(int _count);
signals:
	void status_sjts_client_session(int, QString);

public:

	int wait4WorkFlowStart();
	CMD_CTRL::BodyRollerQualified wait4ImgProcessResult();
	CMD_WORK_FLOW::SJTS_MACHINE_STATUS_ROLLER BodyRollerQualified_2_SJTS_MACHINE_STATUS(CMD_CTRL::BodyRollerQualified _qualified);
	void emit_roller_done_qualified(CMD_CTRL::BodyRollerQualified _qualified);
public:
	

};