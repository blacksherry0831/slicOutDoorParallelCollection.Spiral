//#include "stdafx.h"
#include "QtThreadPlcSocketClient.hpp"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
QtThreadPlcSocketClient::QtThreadPlcSocketClient(QObject *parent ):QtThreadPlcSocket(parent)
{
		
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
	emit status_sjts_roller(CMD_WORK_FLOW::SJTS_MACHINE_STATUS_ROLLER::RollerPosReady, this->GetIntoStepStr());
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
	emit status_sjts(CMD_WORK_FLOW::SJTS_MACHINE_STATUS_ROLLER::MachineAbort, "");
	this->M_THREAD_RUN = FALSE;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/