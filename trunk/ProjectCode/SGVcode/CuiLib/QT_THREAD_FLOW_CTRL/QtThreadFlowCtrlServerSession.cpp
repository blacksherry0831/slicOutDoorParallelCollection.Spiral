//#include "stdafx.h"
#include "QtThreadFlowCtrlServerSession.hpp"
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
QtThreadFlowCtrlServerSession::QtThreadFlowCtrlServerSession(qintptr _socket):QtThreadSocketClientCmdQ(_socket)
{
	mWorkFlowStart=FALSE;
	mWorkFlowEnd=FALSE;
	mWorkFlowStep = 0;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
QtThreadFlowCtrlServerSession::~QtThreadFlowCtrlServerSession(void)
{
	qDebug() << __func__;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadFlowCtrlServerSession::run()
{

	this->before_enter_thread();
	this->enter_thread();

	this->init_socket_in_thread();

	do {

		this->connect2ServerIfNoConnected();

		while (socket_thread_run_condition()) {

			this->run_socket_work();

		}

		this->close_destory_socket_4_server();

	} while (0);

	this->destory_socket_in_thread();

	this->exit_thread();
	this->after_exit_thread();

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadFlowCtrlServerSession::RecordCmd(QSharedPointer<CMD_CTRL> _cmd)
{

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadFlowCtrlServerSession::run_socket_work()
{
	TimeMeasure tm;

	QSharedPointer<CMD_CTRL> cmd_t = this->GetMsg();
	QSharedPointer<CMD_CTRL> cmd_resp_t = QSharedPointer<CMD_CTRL>(new CMD_CTRL());

	if (cmd_t.isNull()) {
		this->SleepMy(100);
	}else {
	
		
#if 1
		if (cmd_t->IsThisCmd00(CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_STOP)) {
			tm.start("CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_STOP");
		}
#endif	
		int status_t=this->send_and_read_cmd_resp(cmd_t,cmd_resp_t);
#if 1
		if (cmd_t->IsThisCmd00(CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_STOP)) {
			tm.stop();
		}
#endif
		this->record_work_flow(cmd_t,cmd_resp_t);

	}

	this->SendHeartBeatCmdReadResp5s();

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadFlowCtrlServerSession::before_enter_thread()
{
	this->ClearMsg();
	mWorkFlowStart = FALSE;
	mWorkFlowEnd = FALSE;

	emit running_client_sessions_change();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadFlowCtrlServerSession::after_exit_thread()
{
	mWorkFlowStart = FALSE;
	mWorkFlowEnd = FALSE;
	emit running_client_sessions_change();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadFlowCtrlServerSession::beforeSendMsg()
{
	mWorkFlowStart = FALSE;
	mWorkFlowEnd = FALSE;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadFlowCtrlServerSession::init_work_flow(QSharedPointer<CMD_CTRL> _cmd)
{
	
	if (_cmd->IsThisCmd00(CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_START)) {
			mWorkFlowStart = FALSE;
			mWorkFlowEnd = FALSE;
	}

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadFlowCtrlServerSession::record_work_flow(QSharedPointer<CMD_CTRL> _cmd, QSharedPointer<CMD_CTRL> _cmd_resp)
{
#if _DEBUG
	mWorkFlowStep++;
#endif

	if (_cmd->IsThisCmd00(CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_START)) {

				Q_ASSERT(mWorkFlowStart == FALSE && mWorkFlowEnd == FALSE);
				mWorkFlowStart = TRUE;
				mWorkFlowStep = 0;
				mWorkFlowResult = CMD_CTRL::CMD_TYPE_02_RESP::CT_ERROR;

		QBase::printf_event("WORK FLOW SESSION","CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_START");
			
	}else if (_cmd->IsThisCmd00(CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_START_00)) {

		QBase::printf_event("WORK FLOW SESSION", "CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_START_00");

	}else if (_cmd->IsThisCmd00(CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_STOP_00)) {

		QBase::printf_event("WORK FLOW SESSION", "CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_STOP_00");

	}else if (_cmd->IsThisCmd00(CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_START_01)) {

		QBase::printf_event("WORK FLOW SESSION", "CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_START_01");
	
	}else if (_cmd->IsThisCmd00(CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_STOP_01)) {

		QBase::printf_event("WORK FLOW SESSION", "CMD_CTRL::CMD_TYPE_LOCAL:CT_FPGA_STOP_01");

	}else if (_cmd->IsThisCmd00(CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_STOP)) {
		
		if (mWorkFlowStart) {
				Q_ASSERT(mWorkFlowStep==5);
				mWorkFlowEnd = TRUE;
				mWorkFlowResult = _cmd_resp->GetCmdParam();
				emit_client_session_work_state(this->mPort, TRUE, mWorkFlowResult);
		}
		QBase::printf_event("WORK FLOW SESSION", "CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_STOP");

	}else {

		QBase::printf_event("WORK FLOW SESSION", "ERROR ");

		Q_ASSERT(FALSE);

	}

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadFlowCtrlServerSession::emit_client_session_work_state(int _port, int _done, int _quality)
{
	Q_ASSERT(_quality == CMD_CTRL::BodyRollerQualified::Qualified || _quality == CMD_CTRL::BodyRollerQualified::UnQualified);
	emit client_session_work_state(_port,_done, _quality);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
