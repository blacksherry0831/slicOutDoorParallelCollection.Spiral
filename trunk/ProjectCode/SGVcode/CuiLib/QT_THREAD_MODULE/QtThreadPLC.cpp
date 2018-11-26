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
	m_socket = QSharedPointer<QtTcpClient>(new QtTcpClient());
#if 1
	mIpAddr = PLC_ADDR;
#else
	mIpAddr = "127.0.0.1";
#endif // 0

	mPort = 2001;

	m_socket->moveToThread(this);

	
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
void QtThreadPLC::Run0()
{
	QSharedPointer<CMD_CTRL> cmd_t = QSharedPointer<CMD_CTRL>(new CMD_CTRL());
	QSharedPointer<BE_1105_Driver>	 be_1105 = QSharedPointer<BE_1105_Driver>(new BE_1105_Driver(Q_NULLPTR));
	int IS_SOCKET_OK = FALSE;

	this->init_serial_port(be_1105);


	while (M_THREAD_RUN) {


		this->connect2ServerIfNoConnected();

		while (m_socket->IsSocketAlive()) {

#if TRUE
			//rooler ready
			if (wait4PlcRoolerReady(cmd_t) == TRUE) {
				//roooler is ready !!!
				m_socket->SendPlcResp(TRUE);
				emit status_sjts(SJTS_MACHINE_STATUS::RoolerReady);
			}else{
				emit status_socket(FALSE);
				break;
			}
#endif // 0

			this->stepMotorRun(be_1105);


			emit status_sjts(SJTS_MACHINE_STATUS::RollerDone);


#if TRUE
			//rooler is ok or bad
			m_socket->SendPlcRollerQualified(CMD_CTRL::CT_OK);

			if (this->wait4PlcResp(cmd_t) == FALSE) {
				emit status_sjts(SJTS_MACHINE_STATUS::RollerDoneUnqualified);
				break;
			}else{
				emit status_sjts(SJTS_MACHINE_STATUS::RollerDoneQualified);
			}
#endif // TRUE

		}


	}
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadPLC::run1()
{
	QSharedPointer<CMD_CTRL> cmd_t = QSharedPointer<CMD_CTRL>(new CMD_CTRL());
	QSharedPointer<BE_1105_Driver>	 be_1105 = QSharedPointer<BE_1105_Driver>(new BE_1105_Driver(Q_NULLPTR));
	int IS_SOCKET_OK = FALSE;

	this->init_serial_port(be_1105);


	while (M_THREAD_RUN) {


		this->connect2ServerIfNoConnected();

		while (m_socket->IsSocketAlive()) {

			IS_SOCKET_OK = m_socket->Read_1_cmd(cmd_t.data());

			if (IS_SOCKET_OK == 0) {
				std::cout << "EVENT>>" << "Socket Error !" << std::endl;
				break;
			}

			if (cmd_t->IsRoolerReady()) {
				//roooler is ready !!!
				std::cout << "EVENT>>" << "Rooler Ready !" << std::endl;
								 

				stepMotorRun(be_1105);

				m_socket->SendPlcRollerQualified(CMD_CTRL::CT_OK);



				break;

			}else if (cmd_t->IsIntoInnerReady()){


			}else if (cmd_t->IsResp()) {
				std::cout << "Done !" << std::endl;
				break;
			}else {
				std::cout << "EVENT>>" << "Error Cmd!" << std::endl;
				continue;
			}

		}


	}
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int QtThreadPLC::MoveSlidingThenRunMotor(QSharedPointer<BE_1105_Driver>	 be_1105,int _pos, int _isRun)
{
	QSharedPointer<CMD_CTRL> cmd_t = QSharedPointer<CMD_CTRL>(new CMD_CTRL());
	int IS_SOCKET_OK = FALSE;
#if TRUE
	//Into Inter
	m_socket->SendPlcIntoInter(_pos);

	do {

		IS_SOCKET_OK = m_socket->Read_1_cmd(cmd_t.data());
		if (IS_SOCKET_OK == 0) {
			std::cout << "EVENT>>" << "Socket Error !" << std::endl;
			return FALSE;
		}

		if (cmd_t->IsIntoInnerReady()) {
				//roooler is ready !!!
			std::cout << "EVENT>> " << "Now into inter  !" <<_pos << std::endl;
			if (_isRun) {
				
				be_1105->SendCmd4Done(BE_1105_RUN_NEG, 
										BE_1105_RUN_SPEED_CRACK_DETECT, 
										BE_1105_RUN_CIRCLE_CRACK_DETECT);
				
			}
			return TRUE;

		}else if (cmd_t->IsRoolerReady()) {
			continue;
		}else {
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
	int IS_SOCKET_OK_t = m_socket->Read_1_cmd(_cmd.data());

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

						m_socket->SendPlcResp(TRUE);

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
	int IS_SOCKET_OK;

	do {

		IS_SOCKET_OK = m_socket->Read_1_cmd(_cmd.data());
		if (IS_SOCKET_OK == 0) {
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
	int IS_SOCKET_OK;

	do {

		IS_SOCKET_OK = m_socket->Read_1_cmd(_cmd.data());
		if (IS_SOCKET_OK == 0) {
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
		else {
			printf_event("ERROR", "Error Cmd!");
			break;
		}

	} while (m_socket->IsSocketAlive());



	return FALSE;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadPLC::run()
{
#if 1
	this->Run0();
#else
	this->run1();
#endif


}
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
			
			emit status_sjts(SJTS_MACHINE_STATUS::SerialPortIsOpen);
			break;
			
		}else{
			QThread::msleep(1000);
			std::cout << "EVENT>>" << "Cant Open Serial Port !" << std::endl;
			emit status_sjts(SJTS_MACHINE_STATUS::SerialPortError);
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

	for (size_t i = 0; i<STEP_NUM; i++) {
		
		int step_t = STEP[i];
		int run_t = TRUE;

		this->emit_step_motor_start(i);
		
		IsOK = this->MoveSlidingThenRunMotor(_be_1105, step_t, run_t);

		if (!IsOK) {
			return IsOK;
		}

		this->emit_step_motor_stop(i);

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
		emit status_sjts(SJTS_MACHINE_STATUS::StepMotorStart00);
	}
	else if (_circle==1)
	{
		emit status_sjts(SJTS_MACHINE_STATUS::StepMotorStart01);
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
		emit status_sjts(SJTS_MACHINE_STATUS::StepMotorStop00);
	}
	else if (_circle == 1)
	{
		emit status_sjts(SJTS_MACHINE_STATUS::StepMotorStop01);
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