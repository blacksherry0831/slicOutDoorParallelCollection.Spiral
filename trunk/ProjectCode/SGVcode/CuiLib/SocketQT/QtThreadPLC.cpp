//#include "stdafx.h"
#include "QtThreadPLC.hpp"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/

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

/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/

void QtThreadPLC::Run0()
{
}

void QtThreadPLC::run1()
{
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
				
				be_1105->SendCmd4Done(BE_1105_RUN_NEG, 55000);
				
			}
			return TRUE;

		}
		else if (cmd_t->IsRoolerReady()) {
			continue;
		}
		else {
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
void QtThreadPLC::run()
{
	QSharedPointer<CMD_CTRL> cmd_t = QSharedPointer<CMD_CTRL>(new CMD_CTRL());
	QSharedPointer<BE_1105_Driver>	 be_1105 = QSharedPointer<BE_1105_Driver>(new BE_1105_Driver(Q_NULLPTR));
	int IS_SOCKET_OK = FALSE;
	do {

		be_1105->open_auto();

		if (be_1105->init()==TRUE) {
			break;
		}
		else
		{
			QThread::msleep(1000);
			std::cout << "EVENT>>" << "Cant Open Serial Port !" << std::endl;
		}

	} while (be_1105->init()==FALSE);


	while (M_THREAD_RUN){


			this->connect2ServerIfNoConnected();

			while (m_socket->IsSocketAlive()) {
#if TRUE
//rooler ready
				do
				{
						IS_SOCKET_OK = m_socket->Read_1_cmd(cmd_t.data());
						if ( IS_SOCKET_OK== 0) {
							std::cout << "EVENT>>" << "Socket Error !" << std::endl;
							break;
						}
						if (cmd_t->IsRoolerReady()) {
							//roooler is ready !!!
							std::cout <<"EVENT>>"<<"Rooler Ready !" << std::endl;
							m_socket->SendPlcResp(TRUE);
							break;
						}else{
							std::cout << "EVENT>>" << "Error Cmd!" << std::endl;
							continue;
						}

				} while (m_socket->IsSocketAlive());
				if (IS_SOCKET_OK == 0) break;						
#endif // 0

#if TRUE
				const  int STEP[2] = { 1,2};
				const  int STEP_NUM = sizeof(STEP) / sizeof(int);
				int IsOK=FALSE;
				for (size_t i = 0; i<STEP_NUM ; i++){
					int step_t = STEP[i];
					int run_t =TRUE;
					IsOK=this->MoveSlidingThenRunMotor(be_1105, step_t, run_t);
					if (!IsOK) break;
					
				}
				if (!IsOK) break;

#endif // TRUE

#if TRUE
						//rooler is ok or bad
						m_socket->SendPlcRollerQualified(CMD_CTRL::CT_OK);
							
						do {

							IS_SOCKET_OK = m_socket->Read_1_cmd(cmd_t.data());
							if (IS_SOCKET_OK == 0) {
								std::cout << "EVENT>>" << "Socket Error !" << std::endl;
								break;
							}

							if (cmd_t->IsResp()) {
								std::cout << "Done !" << std::endl;
								break;
							}else if (cmd_t->IsRoolerReady()) {
								continue;
							}
							else {
								std::cout << "EVENT>>" << "Error Cmd!" << std::endl;
								break;
							}
						} while (m_socket->IsSocketAlive());
						if (IS_SOCKET_OK == FALSE) break;
#endif // TRUE
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