//#include "stdafx.h"
#include "QtThreadClient.hpp"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/

/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/

/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
QtThreadClient::QtThreadClient(qintptr p)
{
	this->write_ptr(p);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
QtThreadClient::~QtThreadClient(void)
{
	qDebug() << "QtThreadClient is Release ! ";
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadClient::write_ptr(qintptr p)
{
	this->ptr_sd = p;
	m_socket = QSharedPointer<QtTcpClient>(new QtTcpClient());
	m_socket->moveToThread(this);
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
void QtThreadClient::run()
{
	BE_1105_Driver *be_1105 = BE_1105_Driver::getInstance(this);

#if defined(linux) || defined(__linux) || defined(__linux__)
	be_1105->open_ttyUSB();
#endif
#if  defined(_WIN32) || defined(_WIN64)
	be_1105->open(3);
#endif

	QSharedPointer<CMD_CTRL> cmd_t = QSharedPointer<CMD_CTRL>(new CMD_CTRL());
	
	qDebug() << "Client Thread Start";
	

	m_socket->setSocketDescriptor(ptr_sd);//客户端的初始化  
	
	if (m_socket->waitForConnected(MAX_MSECS)) {

		
/*-----------------------------*/		
		while (M_THREAD_RUN && m_socket->IsSocketAlive())
		{	

			be_1105->SendCmd4Done(BE_1105_RUN_NEG,55000);
			be_1105->SendCmd4Done(BE_1105_RUN_NEG,55000);

#if TRUE
//step 1
				
					std::cout << "Send Start" << std::endl;
					m_socket->Send_Start_CMD(TRUE);
												
					if (m_socket->Read_1_cmd(cmd_t.data()) == 0) {
						break;
					}
					if (cmd_t->IsResp()) {

							std::cout << "Rcv Start Resp !" << std::endl;

					}else {
						break;
					}
					QThread::sleep(10);
#endif // TRUE

#if TRUE
//step 2
					std::cout << "Send Stop" << std::endl;

					m_socket->Send_Start_CMD(FALSE);
					if (m_socket->Read_1_cmd(cmd_t.data()) == 0) {
						break;
					}
					if (cmd_t->IsResp()) {

						std::cout << "Rcv  Resp !" << std::endl;

					}
					else {
						break;
					}
					QThread::sleep(10);
#endif
	 	}
/*-----------------------------*/
				

		

		m_socket->close();


	}else {
		qDebug() << "Connect Fail";
	}


	qDebug() << "Client Thread Exit";

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