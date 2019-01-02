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
QtThreadPLC::QtThreadPLC(QObject *parent ):QtThreadFlowCtrlBase(parent)
{
	
#if 1
	mIpAddr = PLC_ADDR;
#else
	mIpAddr = "127.0.0.1";
#endif // 0

	mPort = 2001;

	

	
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
QtThreadPLC::~QtThreadPLC(void)
{
	qDebug() << "QtThreadClient is Release ! ";
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
void QtThreadPLC::do_run_work(QSharedPointer<BE_1105_Driver>	 _be_1105)
{
	QSharedPointer<CMD_CTRL> cmd_t = QSharedPointer<CMD_CTRL>(new CMD_CTRL());
	
	this->wait4WorkFlowStart();	
	
	if (wait4PlcRoolerReady(cmd_t) == TRUE) {
				//rooler ready		

				this->doPlcStepMotorRun(_be_1105);
								
				int qualified_status_t = this->wait4ImgProcessResult();

				//rooler is ok or bad
				sendPlcRollerQualifiedEx(qualified_status_t);



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
int QtThreadPLC::MoveSlidingThenRunMotor(QSharedPointer<BE_1105_Driver>	 be_1105,const int _pos, int _isRun)
{
	QSharedPointer<CMD_CTRL> cmd_t = QSharedPointer<CMD_CTRL>(new CMD_CTRL());
	
#if TRUE
	//Into Inter
	m_socket->SendPlcIntoInter(_pos);

	do {

		this->Read_1_plc_cmd_process_hearbeat(cmd_t);
				
		if (this->GetSocketConnected()) {
			return FALSE;
		}
		
		if (cmd_t->IsIntoInnerReady()) {
				//roooler is ready !!!
			
			if (_isRun) {

				this->emit_step_motor_start(_pos-1);

				be_1105->SendCmd4Done(BE_1105_RUN_NEG, 
										BE_1105_RUN_SPEED_CRACK_DETECT, 
										BE_1105_RUN_CIRCLE_CRACK_DETECT);

				this->emit_step_motor_stop(_pos-1);
				
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
void QtThreadPLC::process_plc_cmd(QSharedPointer<CMD_CTRL> _cmd, QSharedPointer<BE_1105_Driver>	 _be_1105)
{
	do {

				if (_cmd->IsRoolerReady()) {
						//rooler is ready !!!
						printf_event("EVENT", "Rooler is Ready !");

						m_socket->SendPlcResp(CMD_CTRL::CMD_TYPE_02_RESP::CT_OK);

						this->stepMotorRun(_be_1105);

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
int QtThreadPLC::process_plc_cmd_easy(QSharedPointer<CMD_CTRL> _cmd)
{
	this->print_cmd(_cmd);

	do {

		if (_cmd->IsRoolerReady()) {
			//rooler is ready !!!		
			break;

		}else if (_cmd->IsIntoInnerReady()) {
			
		}else if (_cmd->IsResp()) {
			
			break;

		}else {
			
			return FALSE;

		}


	} while (0);

	return TRUE;
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
			sendPlcResp(CMD_CTRL::CMD_TYPE_02_RESP::CT_OK);
			return TRUE;

		}
		else if (_cmd->IsRoolerReadyError()) {
			//roooler is ready !!!
			sendPlcResp(CMD_CTRL::CMD_TYPE_02_RESP::CT_OK);
			return TRUE;

		}else if (_cmd->IsAbortStop()){
			return FALSE;
		}else if (_cmd->isHeartbeatCmd()) {
		
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

/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void  QtThreadPLC::init_serial_port(QSharedPointer<BE_1105_Driver>	 _be_1105)
{
	do {
		_be_1105->open_auto();

		if (_be_1105->init() == TRUE) {
			
			emit status_sjts(CMD_CTRL::SJTS_MACHINE_STATUS::SerialPortIsOpen);
			break;
			
		}else{
			this->SleepMy(1000);
			emit status_sjts(CMD_CTRL::SJTS_MACHINE_STATUS::SerialPortError);
		}

	} while (_be_1105->init() == FALSE && M_THREAD_RUN);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int QtThreadPLC::doPlcStepMotorRun(QSharedPointer<BE_1105_Driver> _be_1105)
{
	int result_t=this->stepMotorRun(_be_1105);
	 	
	this->emit_roller_done();

	return result_t;

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int QtThreadPLC::stepMotorRun(QSharedPointer<BE_1105_Driver>	 _be_1105)
{
#if TRUE

	const  int STEP[2] = { 1,2 };
	const  int STEP_NUM = sizeof(STEP) / sizeof(int);
	int IsOK = FALSE;

	for (size_t i = STEP[0]; i<=STEP[STEP_NUM-1]; i++) {
		
		int step_t = i;
		int run_t = TRUE;	
		
		IsOK = this->MoveSlidingThenRunMotor(_be_1105, step_t, run_t);

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
void QtThreadPLC::emit_step_motor_start(int _circle)
{
	if (_circle==0){
		emit status_sjts(CMD_CTRL::SJTS_MACHINE_STATUS::StepMotorStart00);
	}
	else if (_circle==1)
	{
		emit status_sjts(CMD_CTRL::SJTS_MACHINE_STATUS::StepMotorStart01);
	}else
	{
		Q_ASSERT(FALSE);
	}
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadPLC::emit_step_motor_stop(int _circle)
{
	if (_circle == 0) {
		emit status_sjts(CMD_CTRL::SJTS_MACHINE_STATUS::StepMotorStop00);
	}
	else if (_circle == 1)
	{
		emit status_sjts(CMD_CTRL::SJTS_MACHINE_STATUS::StepMotorStop01);
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
void QtThreadPLC::emit_roller_done_qualified(CMD_CTRL::BodyRollerQualified _qualified)
{

	CMD_CTRL::SJTS_MACHINE_STATUS sjts_status_t;

	if (_qualified == CMD_CTRL::BodyRollerQualified::Qualified) {
		sjts_status_t = CMD_CTRL::SJTS_MACHINE_STATUS::RollerDoneQualified;
	}
	else if(_qualified == CMD_CTRL::BodyRollerQualified::UnQualified) {
		sjts_status_t = CMD_CTRL::SJTS_MACHINE_STATUS::RollerDoneUnqualified;
	}
	else
	{
		Q_ASSERT(0);
	}
	emit status_sjts(sjts_status_t);


}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadPLC::run_socket_work()
{

	m_socket->SetReadTimeOutMy(1000*60*2);//2 min

	QSharedPointer<BE_1105_Driver>	 be_1105 = QSharedPointer<BE_1105_Driver>(new BE_1105_Driver(Q_NULLPTR));

	this->init_serial_port(be_1105);

	while (socket_thread_run_condition())
	{
		this->do_run_work(be_1105);
	}

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadPLC::print_undefined_cmd(QSharedPointer<CMD_CTRL> _cmd)
{
	printf_event("ERROR", "Error Cmd!");
	printf_event("ERROR", "this is Error Cmd ! do you know why ????????????????");
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadPLC::print_cmd(QSharedPointer<CMD_CTRL> _cmd)
{
	if (_cmd->IsRoolerReady()) {
		//rooler is ready !!!
		printf_event("EVENT", "Rooler is Ready !");
		
	}
	else if (_cmd->IsIntoInnerReady()) {
		
		printf_event("EVENT", "Now into inter  !");
	}
	else if (_cmd->isHeartbeatCmd()) {
		printf_event("EVENT", "Hearbeat@rcv !");
	}
	else if (_cmd->IsResp()) {
		printf_event("EVENT", "RESP");		
	}else if (_cmd->IsRoolerReadyError()) {
		printf_event("EVENT", "Rooler is Ready ERROR !");
	}else if (_cmd->IsAbortStop()){
		printf_event("EVENT", "Rooler is Abort Stop !");
	}else {
		print_undefined_cmd(_cmd);
	}

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadPLC::print_socket_connected()
{
	if (GetSocketConnected() == 0) {
		printf_event("EVENT", "Socket Error !");
	}
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int QtThreadPLC::sendPlcResp(CMD_CTRL::CMD_TYPE_02_RESP _type)
{
	int result_t=m_socket->SendPlcResp(CMD_CTRL::CMD_TYPE_02_RESP::CT_OK);
	emit status_sjts(CMD_CTRL::SJTS_MACHINE_STATUS::RoolerReady);
	return result_t;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int QtThreadPLC::Read_1_plc_cmd_process_hearbeat(QSharedPointer<CMD_CTRL> _cmd)
{

	if (Read_1_cmd_process_hearbeat(_cmd)) {
	
			this->print_cmd(_cmd);
	}
	else {
			this->print_socket_connected();	
	
	}

	return this->GetSocketConnected();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int QtThreadPLC::sendPlcRollerQualifiedEx(int _qualified)
{
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
