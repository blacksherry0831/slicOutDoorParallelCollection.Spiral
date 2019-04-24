#include "QtThreadPlcSimulatorClient.hpp"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
QtThreadPlcSimulatorClient::QtThreadPlcSimulatorClient(QObject *parent ):QtThreadPlcNetClient(parent)
{
	this->mBlock = 0;
	this->mEvent = "PLC Simulator";
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
QtThreadPlcSimulatorClient::~QtThreadPlcSimulatorClient(void)
{
	
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int QtThreadPlcSimulatorClient::socket_thread_run_condition()
{
	return M_THREAD_RUN;// simulator  no socket
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadPlcSimulatorClient::run()
{
	this->before_enter_thread();
	this->enter_thread();

	while (M_THREAD_RUN)
	{
		this->run_thread_work();
	}

	this->exit_thread();
	this->after_exit_thread();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadPlcSimulatorClient::ProcMsgEx(QSharedPointer<CMD_CTRL> _msg)
{

	if (_msg->IsThisCmd00(CMD_WORK_FLOW::SJTS_MACHINE_STATUS_MOTOR::MotorStart00)) {

	}else if (_msg->IsThisCmd00(CMD_WORK_FLOW::SJTS_MACHINE_STATUS_MOTOR::MotorStop00)) {
		
		this->IncIntoStep();
		this->emit_roller_into_inner_ready();

	}else if (_msg->IsThisCmd00(CMD_WORK_FLOW::SJTS_MACHINE_STATUS_MOTOR::MotorStart01)) {

	}else if (_msg->IsThisCmd00(CMD_WORK_FLOW::SJTS_MACHINE_STATUS_MOTOR::MotorStop01)) {

		this->emit_roller_done();

	}else if (_msg->IsThisCmd00(CMD_WORK_FLOW::SJTS_MACHINE_STATUS_MOTOR::SerialPortOpen)){
		
	}else if (_msg->IsThisCmd00(CMD_WORK_FLOW::SJTS_MACHINE_STATUS_MOTOR::SerialPortError)){
		
	}else {
		Q_ASSERT(FALSE);
	}

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadPlcSimulatorClient::run_normal()
{
	const int TIME_INTERVAL = 2000;
	const int TIME_VIDEO = 15 * 1000;
		
		/*-------------------------------------*/
		this->SleepMy(TIME_INTERVAL);
		/*-------------------------------------*/
		this->SleepMy(TIME_INTERVAL);
		this->InitIntoStep();
		this->emit_roller_ready();
		/*-------------------------------------*/
		this->SleepMy(TIME_INTERVAL);
		this->emit_roller_pos_ready();
		/*-------------------------------------*/
		this->SleepMy(TIME_INTERVAL);
		this->emit_roller_into_inner_ready();
		/*-------------------------------------*/

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadPlcSimulatorClient::run_thread_work()
{
	
	this->run_normal();

	while (M_THREAD_RUN)
	{
		this->ProcMsg();

		this->process_roller_done_end();

		if (IsRollerStatus(CMD_WORK_FLOW::RollerDoneQ) ||
			IsRollerStatus(CMD_WORK_FLOW::RollerDoneUnQ))	{
					break;
		}

	}

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadPlcSimulatorClient::process_roller_done_end()
{
	if (IsRollerStatus(CMD_WORK_FLOW::RollerDoneEnd)) {
		CMD_CTRL::BodyRollerQualified qualified_status_t = this->wait4ImgProcessResult();
		this->emit_roller_done_qualified(qualified_status_t);
		this->printf_roller_status(mEvent);
	}
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadPlcSimulatorClient::SetBlock(bool _block)
{
	this->mBlock = _block;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/