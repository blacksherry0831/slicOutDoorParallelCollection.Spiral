#include "QtThreadPlcNetClient.hpp"
/*-------------------------------------*/
#define USE_EXTERNAL_ERROR (0)
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
QtThreadPlcNetClient::QtThreadPlcNetClient(QObject *parent ):QtThreadFlowCtrlBase(parent)
{
	this->init_external_status();
	this->mEvent = "PLC Machine";
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
QtThreadPlcNetClient::~QtThreadPlcNetClient(void)
{
	
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadPlcNetClient::run_socket_work()
{
	while (socket_thread_run_condition()) {

		QSharedPointer<CMD_CTRL> cmd_r_t = QSharedPointer<CMD_CTRL>(new CMD_CTRL());

		this->Read_1_plc_cmd_process_hearbeat(cmd_r_t);

		this->process_plc_cmd_async(cmd_r_t);

		this->ProcMsg();

		this->process_roller_done_end();

	}

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadPlcNetClient::ProcMsgEx(QSharedPointer<CMD_CTRL> _msg)
{

	if (_msg->IsThisCmd00(CMD_WORK_FLOW::SJTS_MACHINE_STATUS_MOTOR::MotorStart00)) {

	}else if (_msg->IsThisCmd00(CMD_WORK_FLOW::SJTS_MACHINE_STATUS_MOTOR::MotorStop00)) {

		this->IncIntoStep();
		SendPlcIntoInterEx();

	}else if (_msg->IsThisCmd00(CMD_WORK_FLOW::SJTS_MACHINE_STATUS_MOTOR::MotorStart01)) {

	}else if (_msg->IsThisCmd00(CMD_WORK_FLOW::SJTS_MACHINE_STATUS_MOTOR::MotorStop01)) {

		this->emit_roller_done();

	}else if (_msg->IsThisCmd00(CMD_WORK_FLOW::SJTS_MACHINE_STATUS_MOTOR::SerialPortOpen)){

		this->set_external_motor(TRUE);

	}else if (_msg->IsThisCmd00(CMD_WORK_FLOW::SJTS_MACHINE_STATUS_MOTOR::SerialPortError)){

		this->set_external_motor(FALSE);

	}else {
		Q_ASSERT(FALSE);
	}

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadPlcNetClient::SetBlock(int _block)
{
	Q_ASSERT(0);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadPlcNetClient::process_roller_done_end()
{
	
	if (IsRollerStatus(CMD_WORK_FLOW::RollerDoneEnd)) {
		
		CMD_CTRL::BodyRollerQualified qualified_status_t=this->wait4ImgProcessResult();	

#if USE_EXTERNAL_ERROR

		if (this->is_external_valid()) {
			
		}else{
			qualified_status_t = CMD_CTRL::CMD_CTRL::BodyRollerQualified::UnQualified;
			printf_event(this->mEvent, "Motor or Client Sessions is error !");
		}

#endif
		this->emit_roller_done_qualified(qualified_status_t);
		this->sendPlcRollerQualifiedEx(qualified_status_t);
		this->printf_roller_status(mEvent);
		printf_event(mEvent, "/*--------------------------------End-----------------------------------*/");
	}

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int QtThreadPlcNetClient::process_plc_cmd_async(QSharedPointer<CMD_CTRL> _cmd)
{
	if (_cmd->IsRoolerReady()) {

		this->do_sjts_roller_ready();

	}
	else if (_cmd->IsRoolerPosReady()) {

		this->do_sjts_roller_pos_ready();

	}
	else if (_cmd->IsIntoInnerReady()) {

		this->emit_roller_into_inner_ready();

	}
	else if (_cmd->IsResp()) {

	}
	else  if (_cmd->isHeartbeatCmd()) {

	}
	else if (_cmd->IsAbortStop()) {

		this->emit_roller_abort();

	}
	else if (_cmd->IsOperationMode()) {

	}
	else if (_cmd->IsRoolerReadyError()) {

	}
	else {
		return FALSE;
	}

	return TRUE;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadPlcNetClient::init_external_status()
{
	this->set_external_status(0, 0, 0);
	this->set_external_status(1, 0, 0);
	this->set_external_status(2, 0, 0);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int QtThreadPlcNetClient::is_external_valid()
{

	if (mMotor[1]==TRUE && mMotor[2]==TRUE){
		if (mSessions[1] == SESSIONS && mSessions[2] == SESSIONS) {
			return TRUE;
		}
	}
	return FALSE;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadPlcNetClient::set_external_status(int _idx, int _m, int _s)
{
	mMotor[_idx]=_m;
	mSessions[_idx]=_s;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadPlcNetClient::set_external_motor(int _m)
{
	const int IDX = this->GetIntoStep();
	mMotor[IDX] = _m;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void  QtThreadPlcNetClient::set_external_session(int _s)
{
	const int IDX = this->GetIntoStep();
	mSessions[IDX] = _s;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void  QtThreadPlcNetClient::do_sjts_roller_ready()
{
	sendPlcResp(CMD_CTRL::CMD_TYPE_02_RESP::CT_OK);
	this->initWorkFlow();
	this->InitIntoStep();
	this->emit_roller_ready();

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/