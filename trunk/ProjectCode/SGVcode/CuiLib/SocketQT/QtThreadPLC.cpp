//#include "stdafx.h"
#include "QtThreadPLC.hpp"
/*-------------------------------------*/
/**
*
###########  XXXXX  ###########   XXXXX  ?????  CCCCC  ZZZZZ  DDDDD  CRC
PLC 59 6A 6B 6A  00 00  00 00 00 00   01 11  00 00  73 10  00 00  00 00  50 ready
PC  59 6A 6B 6A  00 00  00 00 00 00   01 11  00 00  72 00  00 00  00 00  50工控机收到准备好
PC  59 6A 6B 6A  00 00  00 00 00 00   01 11  00 00  63 20  00 00  00 01  60工控机发送位置
PLC 59 6A 6B 6A  00 00  00 00 00 00   01 11  00 00  73 20  00 00  00 02  63
59 6A 6B 6A  00 00  00 00 00 00   01 11  00 00  63 10  00 00  00 00  51工控机发送不合格判定
59 6A 6B 6A  00 00  00 00 00 00   01 11  00 00  63 10  00 00  00 01  50
*
*/
/*-------------------------------------*/

/*-------------------------------------*/
/**
*
PLC
59 6A 6B 6A  00 00  00 00 00 00   01 11  00 00  73 10  00 00  00 00 50  R ready
59 6A 6B 6A  00 00  00 00 00 00   01 11  00 00  73 20  00 00  00 00 50  R into
59 6A 6B 6A  00 00  00 00 00 00   01 11  00 00  73 20  00 00  00 00 50  R into
79 6A 6B 6A 00 00 00 00 00 00 01 11 00 00 72 00 02 00 00 00 72

IPC
79 6A 6B 6A 00 00 00 00 00 00 01 11 00 00 72 00 02 00 00 00 72 resp
79 6A 6B 6A 00 00 00 00 00 00 01 11 00 00 63 20 02 00 01 00 42 into internal
79 6A 6B 6A 00 00 00 00 00 00 01 11 00 00 63 20 02 00 01 00 42 into internal
79 6A 6B 6A 00 00 00 00 00 00 01 11 00 00 63 10 02 00 00 01 72 ok合格
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
	mIpAddr = "127.0.0.1";
	mPort = 20000;

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

/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadPLC::run()
{
	QSharedPointer<CMD_CTRL> cmd_t = QSharedPointer<CMD_CTRL>(new CMD_CTRL());
	QSharedPointer<BE_1105_Driver>	 be_1105 = QSharedPointer<BE_1105_Driver>(new BE_1105_Driver(Q_NULLPTR));
	
	do {
	
#if defined(linux) || defined(__linux) || defined(__linux__)
		be_1105->open_ttyUSB();
#endif
#if  defined(_WIN32) || defined(_WIN64)
		be_1105->open(3);
#endif	
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


			if(m_socket->IsSocketAlive()==false) {
			
					do {
					
						m_socket->connectToHost(QHostAddress(mIpAddr.c_str()),mPort);
						std::cout <<"Try Connect to IP: "<<mIpAddr <<"Port:"<<mPort<< std::endl;
					
					}while (m_socket->waitForConnected(MAX_MSECS) == false);	

			}

			while (m_socket->IsSocketAlive()) {
#if TRUE
//rooler ready
						if (m_socket->Read_1_cmd(cmd_t.data()) == 0) {
							std::cout << "EVENT>>" << "Socket Error !" << std::endl;
							break;
						}

						if (cmd_t->IsRoolerReady()) {
							//roooler is ready !!!
							std::cout <<"EVENT>>"<<"Rooler Ready !" << std::endl;
							m_socket->SendPlcResp(TRUE);
							
						}else{
							std::cout << "EVENT>>" << "Error Cmd!" << std::endl;
							continue;
						}
#endif // 0
#if TRUE

						//Into Inter
						m_socket->SendPlcIntoInter(2);
						
						if (m_socket->Read_1_cmd(cmd_t.data()) == 0) {
							std::cout << "EVENT>>" << "Socket Error !" << std::endl;
							break;
						}

						if (cmd_t->IsIntoInnerReady()) {
							//roooler is ready !!!
							std::cout <<"EVENT>> "<<"Now into inter  !" << std::endl;
							be_1105->SendCmd4Done(BE_1105_RUN_NEG, 55000);
							be_1105->SendCmd4Done(BE_1105_RUN_NEG, 55000);

						}
						else {
							std::cout << "EVENT>>" << "Error Cmd!" << std::endl;
							continue;
						}

#endif // TRUE
#if TRUE
						//Into Inter
						m_socket->SendPlcIntoInter(1);

						if (m_socket->Read_1_cmd(cmd_t.data()) == 0) {
							std::cout << "EVENT>>" << "Socket Error !" << std::endl;
							break;
						}

						if (cmd_t->IsIntoInnerReady()) {
							//roooler is ready !!!
							std::cout << "EVENT>> " << "Now out inter  !" << std::endl;
							

						}
						else {
							std::cout << "EVENT>>" << "Error Cmd!" << std::endl;
							continue;
						}
#endif // TRUE

#if TRUE
						//rooler is ok or bad
						m_socket->SendPlcRollerQualified(TRUE);
						if (m_socket->Read_1_cmd(cmd_t.data()) == 0) {
							std::cout << "EVENT>>" << "Socket Error !" << std::endl;
							break;
						}

						if (cmd_t->IsResp()) {
							
							std::cout << "Done !" << std::endl;


						}
						else
						{
							std::cout << "EVENT>>" << "Error Cmd!" << std::endl;
							continue;
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