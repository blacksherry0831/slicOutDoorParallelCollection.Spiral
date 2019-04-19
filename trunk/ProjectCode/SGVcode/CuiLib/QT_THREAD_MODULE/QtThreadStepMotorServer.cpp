#include "QtThreadStepMotorServer.hpp"
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
QtThreadStepMotorServer::QtThreadStepMotorServer(QObject *parent)
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
	if (_circle == 1) {
		emit status_sjts_motor(CMD_WORK_FLOW::SJTS_MACHINE_STATUS_MOTOR::MotorStart00, "");
	}
	else if (_circle == 2)
	{
		emit status_sjts_motor(CMD_WORK_FLOW::SJTS_MACHINE_STATUS_MOTOR::MotorStart01, "");
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
	if (_circle == 1) {
		emit status_sjts_motor(CMD_WORK_FLOW::SJTS_MACHINE_STATUS_MOTOR::MotorStop00, "");
	}
	else if (_circle == 2)
	{
		emit status_sjts_motor(CMD_WORK_FLOW::SJTS_MACHINE_STATUS_MOTOR::MotorStop01, "");
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
		emit status_sjts_motor(CMD_WORK_FLOW::SJTS_MACHINE_STATUS_MOTOR::SerialPortError, "");
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
		this->motor_run_faset();
	}else if (_msg->IsThisCmd00(CMD_WORK_FLOW::SJTS_MACHINE_STATUS_ROLLER::RollerPosReady)){
		this->motor_run_stop();
	}else if (_msg->IsThisCmd00(CMD_WORK_FLOW::SJTS_MACHINE_STATUS_ROLLER::RollerIntoInnerReady)){
		
		const int INTO_STEP = _msg->GetCmd01();
		this->emit_step_motor_start(INTO_STEP);
		this->motor_run_once();
		this->emit_step_motor_stop(INTO_STEP);

	}else if (_msg->IsThisCmd00(CMD_WORK_FLOW::SJTS_MACHINE_STATUS_ROLLER::RollerDoneEnd)){

	}else if (_msg->IsThisCmd00(CMD_WORK_FLOW::SJTS_MACHINE_STATUS_ROLLER::RollerDoneUnQ)){

	}else if (_msg->IsThisCmd00(CMD_WORK_FLOW::SJTS_MACHINE_STATUS_ROLLER::RollerDoneQ)){

	}else if(_msg->IsThisCmd00(CMD_WORK_FLOW::SJTS_MACHINE_STATUS_ROLLER::MachineAbort)){
		this->motor_run_stop();
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
void QtThreadStepMotorServer::run_thread_work()
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
int QtThreadStepMotorServer::motor_run_faset()
{
	const int result_t = this->do_StepMotor_sjts_Run_Fast();
	return result_t;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int QtThreadStepMotorServer::motor_run_stop()
{
	const int result_t = this->do_StepMotor_sjts_Run_Stop();
	return result_t;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int QtThreadStepMotorServer::motor_run_once()
{

	const int result_t = this->do_StepMotor_sjts_Run_Once();

	if (0 == result_t) {
		this->SleepMy(mOneCircleTime);
	}

	return result_t;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/