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
QtThreadPLC::QtThreadPLC(qintptr p)
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

#if TRUE
	//rooler ready
	if (wait4PlcRoolerReady(cmd_t) == TRUE) {
		//roooler is ready !!!
		m_socket->SendPlcResp(CMD_CTRL::CMD_TYPE_02_RESP::CT_OK);
		emit status_sjts(CMD_CTRL::SJTS_MACHINE_STATUS::RoolerReady);
	}
	else {
		emit status_socket(FALSE);
		return;
	}
#endif // 0

	this->stepMotorRun(_be_1105);

	emit status_sjts(CMD_CTRL::SJTS_MACHINE_STATUS::RollerDone);


	int qualified_status_t = this->wait4ImgProcessResult();

	CMD_CTRL::SJTS_MACHINE_STATUS sjts_status;

	




#if TRUE
	//rooler is ok or bad
	m_socket->SendPlcRollerQualified(qualified_status_t);

	if (this->wait4PlcResp(cmd_t) == FALSE) {
		
		return;
	}
	else {
	
	}
#endif // TRUE

#if  1

	if (qualified_status_t == CMD_CTRL::BodyRollerQualified::Qualified) {
		sjts_status = CMD_CTRL::SJTS_MACHINE_STATUS::RollerDoneQualified;
	}
	else {
		sjts_status = CMD_CTRL::SJTS_MACHINE_STATUS::RollerDoneUnqualified;
	}
	emit status_sjts(sjts_status);

#endif //  1

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
	int IS_SOCKET_OK = FALSE;
#if TRUE
	//Into Inter
	m_socket->SendPlcIntoInter(_pos);

	do {

		IS_SOCKET_OK = Read_1_cmd_process_hearbeat(cmd_t);
		if (IS_SOCKET_OK == 0) {
			std::cout << "EVENT>>" << "Socket Error !" << std::endl;
			return FALSE;
		}

		if (cmd_t->IsIntoInnerReady()) {
				//roooler is ready !!!
			std::cout << "EVENT>> " << "Now into inter  !" <<_pos << std::endl;
			if (_isRun) {

				this->emit_step_motor_start(_pos-1);

				be_1105->SendCmd4Done(BE_1105_RUN_NEG, 
										BE_1105_RUN_SPEED_CRACK_DETECT, 
										BE_1105_RUN_CIRCLE_CRACK_DETECT);

				this->emit_step_motor_stop(_pos-1);
				
			}
			return TRUE;

		}else if (cmd_t->IsRoolerReady()) {
			continue;
		}else if (cmd_t->IsAbortStop()) {
			printf_event("EVENT", "Rooler is Abort Stop !");

			return FALSE;
		}else{
			std::cout << "EVENT>>" << "Error Cmd!" << std::endl;
			return FALSE;
		}

	} while (m_socket->IsSocketAlive());
	
#endif // TRUE
	return IS_SOCKET_OK;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int QtThreadPLC::read_plc_cmd(QSharedPointer<CMD_CTRL> _cmd)
{
	int IS_SOCKET_OK_t = Read_1_cmd_process_hearbeat(_cmd);

	return IS_SOCKET_OK_t;
}
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
	do {

		if (_cmd->IsRoolerReady()) {
			//rooler is ready !!!
			printf_event("EVENT", "Rooler is Ready !");
			break;

		}else if (_cmd->IsIntoInnerReady()) {
			printf_event("EVENT", "RESP");

		}else if (_cmd->IsResp()) {
			printf_event("EVENT", "RESP");
			break;

		}else {
			printf_event("ERROR", "Error Cmd!");
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
void QtThreadPLC::printf_event(std::string _event, std::string _msg)
{
	std::cout << _event<< ">>" << _msg<< std::endl;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int QtThreadPLC::wait4PlcResp(QSharedPointer<CMD_CTRL> _cmd)
{
	

	do {

		Read_1_cmd_process_hearbeat(_cmd);
		if (GetSocketConnected() == 0) {
			std::cout << "EVENT>>" << "Socket Error !" << std::endl;
			return FALSE;
		}

		if (_cmd->IsResp()) {
			std::cout << "Done !" << std::endl;
			return TRUE;
		}
		else if (_cmd->IsRoolerReady()) {
			return FALSE; 
		}
		else {
			printf_event("ERROR", "Error Cmd!");
			return FALSE;
		}

	} while (m_socket->IsSocketAlive());

	

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

	    Read_1_cmd_process_hearbeat(_cmd);
		if (GetSocketConnected() == 0) {
			printf_event("EVENT", "Socket Error !");
			return FALSE;
		}

		if (_cmd->IsResp()) {
			printf_event("EVENT", "RESP");
			break;
		}
		else if (_cmd->IsRoolerReady()) {
			printf_event("EVENT", "Rooler is Ready !");
			return TRUE;
		}
		else if (_cmd->IsAbortStop())
		{
			printf_event("EVENT", "Rooler is Abort Stop !");

		}else{
			printf_event("ERROR", "Error Cmd!");
			printf_event("ERROR", "this is Error Cmd ! do you know why ????????????????");
			//Q_ASSERT(FALSE);
			break;
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
int QtThreadPLC::wait4ImgProcessResult()
{
	CMD_CTRL::BodyRollerQualified qualified_t = CMD_CTRL::BodyRollerQualified::UnQualified;

	

	return qualified_t;
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