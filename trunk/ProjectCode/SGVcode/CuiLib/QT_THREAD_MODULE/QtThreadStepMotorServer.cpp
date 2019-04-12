#include "QtThreadStepMotorServer.hpp"
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
QtThreadStepMotorServer::QtThreadStepMotorServer()
{
	this->mThreadName = "Step Motor Server Thread ";
	
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
QtThreadStepMotorServer::~QtThreadStepMotorServer(void)
{
	qDebug() << this->objectName() << "is Release ! ";
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadStepMotorServer::emit_step_motor_start(int _circle)
{
	if (_circle == 0) {
		emit status_sjts_motor(CMD_CTRL::SJTS_MACHINE_STATUS::StepMotorStart00, "");
	}
	else if (_circle == 1)
	{
		emit status_sjts_motor(CMD_CTRL::SJTS_MACHINE_STATUS::StepMotorStart01, "");
	}
	else
	{
		Q_ASSERT(FALSE);
	}
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadStepMotorServer::emit_step_motor_stop(int _circle)
{
	if (_circle == 0) {
		emit status_sjts_motor(CMD_CTRL::SJTS_MACHINE_STATUS::StepMotorStop00, "");
	}
	else if (_circle == 1)
	{
		emit status_sjts_motor(CMD_CTRL::SJTS_MACHINE_STATUS::StepMotorStop01, "");
	}
	else
	{
		Q_ASSERT(FALSE);
	}
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadStepMotorServer::emit_init_serial_status(int _isOpen)
{
	if (_isOpen)
	{
		emit status_sjts_motor(CMD_WORK_FLOW::SJTS_MACHINE_STATUS_MOTOR::SerialPortOpen, "");
	}
	else
	{
		emit status_sjts_motor(CMD_CTRL::SJTS_MACHINE_STATUS::SerialPortError, "");
	}
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadStepMotorServer::ProcMsgEx(QSharedPointer<CMD_CTRL> _msg)
{
	if (_msg->IsThisCmd00(CMD_WORK_FLOW::SJTS_MACHINE_STATUS_ROLLER::RollerReadyStart)) {
		this->do_StepMotor_sjts_Run_Fast();
	}else if (_msg->IsThisCmd00(CMD_WORK_FLOW::SJTS_MACHINE_STATUS_ROLLER::RollerPosReady)){
		this->do_StepMotor_sjts_Run_Stop();
	}else if (_msg->IsThisCmd00(CMD_WORK_FLOW::SJTS_MACHINE_STATUS_ROLLER::RollerIntoInnerReady)){
		
		const int INTO_STEP = _msg->GetCmd01();
		this->emit_step_motor_start(INTO_STEP);
		this->do_StepMotor_sjts_Run_Once();
		this->emit_step_motor_stop(INTO_STEP);

	}else if (_msg->IsThisCmd00(CMD_WORK_FLOW::SJTS_MACHINE_STATUS_ROLLER::RollerDoneEnd)){

	}else if (_msg->IsThisCmd00(CMD_WORK_FLOW::SJTS_MACHINE_STATUS_ROLLER::RollerDoneUnQ)){

	}else if (_msg->IsThisCmd00(CMD_WORK_FLOW::SJTS_MACHINE_STATUS_ROLLER::RollerDoneQ)){

	}else if(_msg->IsThisCmd00(CMD_WORK_FLOW::SJTS_MACHINE_STATUS_ROLLER::MachineAbort)){
		this->do_StepMotor_sjts_Run_Stop();
		this->M_THREAD_RUN = FALSE;
	}else {
		Q_ASSERT(FALSE);
	}
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadStepMotorServer::run()
{
	
	this->do_StepMotor_sjts_Init();

	while (M_THREAD_RUN){
		this->ProcMsg();
		this->SleepMy();
	   }
	
	this->do_StepMotor_sjts_Run_Stop();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadStepMotorServer::SetMsgWhileRunning(QSharedPointer<CMD_CTRL> _msg)
{
	if (this->isRunning())
		this->SetMsg(_msg);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/