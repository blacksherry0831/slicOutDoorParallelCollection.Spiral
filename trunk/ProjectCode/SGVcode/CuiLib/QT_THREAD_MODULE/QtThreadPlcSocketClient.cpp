#include "QtThreadPlcSocketClient.hpp"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
QtThreadPlcSocketClient::QtThreadPlcSocketClient(QObject *parent ):QtThreadPlcSocket(parent)
{
	this->mExternalError = false;
	this->mEvent = "PLC Machine";
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
QtThreadPlcSocketClient::~QtThreadPlcSocketClient(void)
{
	
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadPlcSocketClient::do_run_work()
{

	QSharedPointer<CMD_CTRL> cmd_r_t = QSharedPointer<CMD_CTRL>(new CMD_CTRL());
	
	this->Read_1_plc_cmd_process_hearbeat(cmd_r_t);
	
	this->process_plc_cmd_async(cmd_r_t);
	 
	this->ProcMsg();

	this->process_roller_done_end();

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadPlcSocketClient::ProcMsgEx(QSharedPointer<CMD_CTRL> _msg)
{

	if (_msg->IsThisCmd00(CMD_WORK_FLOW::SJTS_MACHINE_STATUS_MOTOR::MotorStart00)) {

	}else if (_msg->IsThisCmd00(CMD_WORK_FLOW::SJTS_MACHINE_STATUS_MOTOR::MotorStop00)) {

		this->IncIntoStep();
		SendPlcIntoInterEx();

	}else if (_msg->IsThisCmd00(CMD_WORK_FLOW::SJTS_MACHINE_STATUS_MOTOR::MotorStart01)) {

	}else if (_msg->IsThisCmd00(CMD_WORK_FLOW::SJTS_MACHINE_STATUS_MOTOR::MotorStop01)) {

		this->emit_roller_done();

	}else if (_msg->IsThisCmd00(CMD_WORK_FLOW::SJTS_MACHINE_STATUS_MOTOR::SerialPortOpen)){
		
	}else if (_msg->IsThisCmd00(CMD_WORK_FLOW::SJTS_MACHINE_STATUS_MOTOR::SerialPortError)){
		this->mExternalError = true;
	}else {
		Q_ASSERT(FALSE);
	}

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadPlcSocketClient::do_sjts_roller_ready()
{
	sendPlcResp(CMD_CTRL::CMD_TYPE_02_RESP::CT_OK);
	
	this->wait4WorkFlowStart();

	this->InitIntoStep();
	this->emit_roller_ready();
	
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadPlcSocketClient::do_sjts_roller_pos_ready()
{
	sendPlcResp(CMD_CTRL::CMD_TYPE_02_RESP::CT_OK);
	
	this->emit_roller_pos_ready();
	SendPlcIntoInterEx();
	
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int QtThreadPlcSocketClient::process_plc_cmd_async(QSharedPointer<CMD_CTRL> _cmd)
{
		if (_cmd->IsRoolerReady()) {

			this->do_sjts_roller_ready();

		}else if(_cmd->IsRoolerPosReady()){

			this->do_sjts_roller_pos_ready();

		}else if (_cmd->IsIntoInnerReady()) {

			this->emit_roller_into_inner_ready();
			
		}else if (_cmd->IsResp()) {
			
		}else  if (_cmd->isHeartbeatCmd()){
			
		}else if (_cmd->IsAbortStop()){
			
			this->emit_roller_abort();

		}else if (_cmd->IsOperationMode()){
			
		}else if (_cmd->IsRoolerReadyError()){

		}else{
			return FALSE;
		}	

	return TRUE;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadPlcSocketClient::emit_roller_pos_ready()
{
	this->mRollerStatus = CMD_WORK_FLOW::SJTS_MACHINE_STATUS_ROLLER::RollerPosReady;
	this->emit_status_sjts_roller(this->GetIntoStepStr());
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadPlcSocketClient::emit_roller_done()
{
	this->mRollerStatus = CMD_WORK_FLOW::SJTS_MACHINE_STATUS_ROLLER::RollerDoneEnd;
	this->emit_status_sjts_roller();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadPlcSocketClient::emit_roller_into_inner_ready()
{
	this->mRollerStatus = CMD_WORK_FLOW::SJTS_MACHINE_STATUS_ROLLER::RollerIntoInnerReady;
	this->emit_status_sjts_roller(this->GetIntoStepStr());
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadPlcSocketClient::SetMsgWhileRunning(QSharedPointer<CMD_CTRL> _msg)
{
	if (this->isRunning())
		this->SetMsg(_msg);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadPlcSocketClient::emit_roller_abort()
{
	this->mRollerStatus = CMD_WORK_FLOW::SJTS_MACHINE_STATUS_ROLLER::MachineAbort;
	this->emit_status_sjts_roller();
	this->M_THREAD_RUN = FALSE;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadPlcSocketClient::SetBlock(int _block)
{
	Q_ASSERT(0);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadPlcSocketClient::process_roller_done_end()
{
	
	if (IsRollerStatus(CMD_WORK_FLOW::RollerDoneEnd)) {
		const CMD_CTRL::BodyRollerQualified qualified_status_t = this->wait4ImgProcessResult();
		this->emit_roller_done_qualified(qualified_status_t);
		this->sendPlcRollerQualifiedEx(qualified_status_t);
		this->printf_roller_status(mEvent);
	}

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int QtThreadPlcSocketClient::sendPlcRollerQualifiedEx(CMD_CTRL::BodyRollerQualified _qualified)
{
	if (!m_socket.isNull()) {
		return	m_socket->SendPlcRollerQualified(_qualified);
	}
	return FALSE;

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
