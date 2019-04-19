//#include "stdafx.h"
#include "QtThreadFlowCtrlServerSession.hpp"
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
QtThreadFlowCtrlServerSession::QtThreadFlowCtrlServerSession(qintptr _socket):QtThreadServerSession(_socket)
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

		printf_event("WORK FLOW SESSION","CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_START");
			
	}else if (_cmd->IsThisCmd00(CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_START_00)) {

		printf_event("WORK FLOW SESSION", "CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_START_00");

	}else if (_cmd->IsThisCmd00(CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_STOP_00)) {

		printf_event("WORK FLOW SESSION", "CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_STOP_00");

	}else if (_cmd->IsThisCmd00(CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_START_01)) {

		printf_event("WORK FLOW SESSION", "CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_START_01");
	
	}else if (_cmd->IsThisCmd00(CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_STOP_01)) {

		printf_event("WORK FLOW SESSION", "CMD_CTRL::CMD_TYPE_LOCAL:CT_FPGA_STOP_01");

	}else if (_cmd->IsThisCmd00(CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_STOP)) {
		
		if (mWorkFlowStart) {
				Q_ASSERT(mWorkFlowStep==5);
				mWorkFlowEnd = TRUE;
				mWorkFlowResult = _cmd_resp->GetCmdParam();
				emit_client_session_work_state(this->mPort, TRUE, mWorkFlowResult);
		}
		printf_event("WORK FLOW SESSION", "CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_STOP");

	}else {

		printf_event("WORK FLOW SESSION", "ERROR ");

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
