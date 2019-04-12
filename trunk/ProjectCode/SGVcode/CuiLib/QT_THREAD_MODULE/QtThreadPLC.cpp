//#include "stdafx.h"
#include "QtThreadPLC.hpp"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
#include "SerialPort/BE_1105_Dirver.hpp"
/*-------------------------------------*/
/**
*
PLC
59 6A 6B 6A  00 00  00 00 00 00   01 11  00 00  73 10  00 00  00 00 50 ready
59 6A 6B 6A 00 00 00 00 00 00  01 11 00 00 72 00     00 00   00 00 50工控机收到准备好
59 6A 6B 6A 00 00 00 00 00 00  01 11 00 00 63 20     00 00   00 01 60工控机发送位置
59 6A 6B 6A 00 00 00 00 00 00  01 11 00 00 63 20     00 00   00 02 63
59 6A 6B 6A 00 00 00 00 00 00  01 11 00 00 63 10     00 00   00 00 51工控机发送不合格判定
59 6A 6B 6A 00 00 00 00 00 00  01 11 00 00 63 10     00 00   00 01 50
###########################################################################
59 6A 6B 6A 00 00 00 00 00 00  01 11 00 00 72 00     02 00   00 00   72 resp
59 6A 6B 6A 00 00 00 00 00 00  01 11 00 00 63 20     02 00   01 00   42 into internal
59 6A 6B 6A 00 00 00 00 00 00  01 11 00 00 63 20     02 00   02 00   42 into internal
59 6A 6B 6A 00 00 00 00 00 00  01 11 00 00 63 10     02 00   01 00   72 ok合格

PLC send
IPC send
【2018-04-18 20:16:47收到数据(HEX),127.0.0.1:43557 Len:21resp】:59 6A 6B 6A 00 00 00 00 00 00 01 11 00 00 72 00 02 00 00 00 52
【2018-04-18 20:16:47收到数据(HEX),127.0.0.1:43557 Len:21into 5】:59 6A 6B 6A 00 00 00 00 00 00 01 11 00 00 63 20 02 00 05 00 66
【2018-04-18 20:17:43收到数据(HEX),127.0.0.1:43557 Len:21into 1】:59 6A 6B 6A 00 00 00 00 00 00 01 11 00 00 63 20 02 00 01 00 62
【2018-04-18 20:18:10收到数据(HEX),127.0.0.1:43557 Len:21 ok】:59 6A 6B 6A 00 00 00 00 00 00 01 11 00 00 63 10 02 00 01 00 52

*
*/
/*-------------------------------------*/


/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
QtThreadPLC::QtThreadPLC(QObject *parent ):QtThreadPlcSocket(parent)
{
		
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
QtThreadPLC::~QtThreadPLC(void)
{
	
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/

/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadPLC::do_run_work()
{

	QSharedPointer<CMD_CTRL> cmd_t = QSharedPointer<CMD_CTRL>(new CMD_CTRL());
	
	this->wait4WorkFlowStart();	
	
	if (wait4PlcRoolerReady(cmd_t) == TRUE) {
				//rooler ready		
				this->do_StepMotor_sjts_Run_Fast();
					
				if (wait4PlcRoolerPosReady(cmd_t) == TRUE) {
		
							this->doPlcStepMotorRun();
								
							CMD_CTRL::BodyRollerQualified qualified_status_t = this->wait4ImgProcessResult();

							//rooler is ok or bad
							sendPlcRollerQualifiedEx(qualified_status_t);
		
					}
					else
					{
						this->do_StepMotor_sjts_Run_Stop();
					}
									
	}
	 
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/

/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int QtThreadPLC::MoveSlidingThenRunMotor(const int _pos, int _isRun)
{
	QSharedPointer<CMD_CTRL> cmd_t = QSharedPointer<CMD_CTRL>(new CMD_CTRL());
	
#if TRUE
	//Into Inter
	SendPlcIntoInter(_pos);

	do {

		this->Read_1_plc_cmd_process_hearbeat(cmd_t);
				
		if (0==this->GetSocketConnected()) {
			return FALSE;
		}
		
		if (cmd_t->IsIntoInnerReady()) {
				//roooler is ready !!!
			
			if (_isRun) {


				this->do_sjts_roller_run(_pos - 1);

				
				
			}
			return TRUE;

		}
		else if (cmd_t->isHeartbeatCmd()) {
			
		}else if (cmd_t->IsRoolerReady()) {
			return FALSE;
		}else if (cmd_t->IsAbortStop()) {
			return FALSE;
		}else{
			
			return FALSE;
		}

	} while (this->socket_thread_run_condition());
	
#endif // TRUE
	return this->GetSocketConnected();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/

/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadPLC::process_plc_cmd(QSharedPointer<CMD_CTRL> _cmd)
{
	do {

				if (_cmd->IsRoolerReady()) {
						//rooler is ready !!!
						printf_event("EVENT", "Rooler is Ready !");

						sendPlcResp(CMD_CTRL::CMD_TYPE_02_RESP::CT_OK);

						this->emit_roller_ready();

						this->stepMotorRun();

						m_socket->SendPlcRollerQualified(CMD_CTRL::CT_OK);


						break;

					}else if (_cmd->IsIntoInnerReady()) {
						printf_event("EVENT", "RESP");

					}else if (_cmd->IsResp()) {
						printf_event("EVENT", "RESP");
						break;
					}else {						
						printf_event("ERROR","Error Cmd!");
						continue;
					}


	} while (0);
	
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int QtThreadPLC::wait4PlcResp(QSharedPointer<CMD_CTRL> _cmd)
{
	
	do {

		this->Read_1_plc_cmd_process_hearbeat(_cmd);
		
		if (GetSocketConnected() == 0) {
			return FALSE;
		}

		
		if (_cmd->IsResp()) {
			std::cout << "Done !" << std::endl;
			return TRUE;
		}
		else if(_cmd->isHeartbeatCmd()){

		}else if (_cmd->IsRoolerReady()) {
			return FALSE; 
		}
		else {
			return FALSE;
		}

	} while (this->socket_thread_run_condition());
		

	return FALSE;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int QtThreadPLC::wait4PlcRoolerReady(QSharedPointer<CMD_CTRL> _cmd)
{
	
	do {

		this->Read_1_plc_cmd_process_hearbeat(_cmd);
			   		
		if (GetSocketConnected() == 0) {		
			return FALSE;
		}
		
		if (_cmd->IsResp()) {
			
		}else if (_cmd->IsRoolerReady()) {
			
			//roooler is ready !!!
			this->do_sjts_roller_ready();
			return TRUE;

		}
		else if (_cmd->IsIntoInnerReady()){
			return FALSE;
		}
		else if (_cmd->IsRoolerReadyError()) {
			//roooler is ready !!!
			sendPlcResp(CMD_CTRL::CMD_TYPE_02_RESP::CT_OK);
			return TRUE;

		}else if (_cmd->IsAbortStop()){
			return FALSE;
		}else if (_cmd->isHeartbeatCmd()) {
		
		}else if (_cmd->IsOperationMode()) {
		
		}else{
			
		}

	} while (this->socket_thread_run_condition());
	
	return FALSE;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int QtThreadPLC::wait4PlcRoolerPosReady(QSharedPointer<CMD_CTRL> _cmd)
{
	do {
		
		this->do_StepMotor_sjts_Run_Fast();

		this->Read_1_plc_cmd_process_hearbeat(_cmd);

		if (GetSocketConnected() == 0) {
			return FALSE;
		}

		if (_cmd->IsResp()) {

		}else if (_cmd->IsRoolerReady()) {

			//roooler is ready !!!
			sendPlcResp(CMD_CTRL::CMD_TYPE_02_RESP::CT_OK);
			this->emit_roller_ready();
			return FALSE;

		}
		else if (_cmd->IsIntoInnerReady()) {
			return FALSE;
		}
		else if (_cmd->IsRoolerReadyError()) {
			sendPlcResp(CMD_CTRL::CMD_TYPE_02_RESP::CT_OK);
			this->do_StepMotor_sjts_Run_Stop();
			return FALSE;

		}else if (_cmd->IsAbortStop()) {
			this->do_StepMotor_sjts_Run_Stop();
			return FALSE;

		}else if (_cmd->isHeartbeatCmd()) {

		}else if(_cmd->IsRoolerPosReady()){
			
			this->do_sjts_roller_pos_ready();
			
			return TRUE;
		}
		else {

		}

	} while (this->socket_thread_run_condition());

	return FALSE;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadPLC::do_sjts_roller_ready()
{
	sendPlcResp(CMD_CTRL::CMD_TYPE_02_RESP::CT_OK);
	this->emit_roller_ready();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadPLC::do_sjts_roller_pos_ready()
{
	sendPlcResp(CMD_CTRL::CMD_TYPE_02_RESP::CT_OK);
	this->do_StepMotor_sjts_Run_Stop();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int QtThreadPLC::doPlcStepMotorRun()
{
	int result_t=this->stepMotorRun();
	 	
	this->emit_roller_done();

	return result_t;

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int QtThreadPLC::stepMotorRun()
{
#if TRUE

	const  int STEP[2] = { 1,2 };
	const  int STEP_NUM = sizeof(STEP) / sizeof(int);
	int IsOK = FALSE;

	for (size_t i = STEP[0]; i<=STEP[STEP_NUM-1]; i++) {
		
		int step_t = i;
		int run_t = TRUE;	
		
		IsOK = this->MoveSlidingThenRunMotor(step_t, run_t);

		if (FALSE==IsOK) {
			return IsOK;
		}

		
	}

#endif // TRUE

	return IsOK;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadPLC::run_socket_work()
{

	this->do_StepMotor_sjts_Init();

	while (socket_thread_run_condition()){
		this->do_run_work();
	}

	this->do_StepMotor_sjts_Run_Stop();

}

/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadPLC::init_serial_port()
{

	do
	{

		if (this->init_serial_port_once()) {
			break;
		}
		else
		{
			this->SleepMy(1000);
		}

	} while (M_THREAD_RUN);

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int QtThreadPLC::sendPlcRollerQualifiedEx(int _qualified)
{

#if 1
	_qualified = CMD_CTRL::BodyRollerQualified::Qualified;
	//全部设置为合格
#endif

	QSharedPointer<CMD_CTRL> cmd_t = QSharedPointer<CMD_CTRL>(new CMD_CTRL());
	//rooler is ok or bad
	m_socket->SendPlcRollerQualified(_qualified);

	if (this->wait4PlcResp(cmd_t) == FALSE) {

		return FALSE;
	}
	else {

	}
	
	this->emit_roller_done_qualified((CMD_CTRL::BodyRollerQualified)_qualified);

	return TRUE;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadPLC::do_sjts_roller_run(const int _step)
{
	this->emit_step_motor_start(_step);

	do_StepMotor_sjts_Run_Once();

	this->emit_step_motor_stop(_step);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/