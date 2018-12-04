//#include "stdafx.h"
#include "QtThreadClientCtrlTest.hpp"
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
QtThreadClientCtrlTest::QtThreadClientCtrlTest()
{
	
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
QtThreadClientCtrlTest::QtThreadClientCtrlTest(qintptr p)
{
	this->write_ptr(p);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
QtThreadClientCtrlTest::~QtThreadClientCtrlTest(void)
{
	qDebug() << "QtThreadClientCtrl is Release ! ";
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
void QtThreadClientCtrlTest::run()
{			
	this->init_socket();
		
		int data = 1;
				
		while (M_THREAD_RUN)
		{	

			this->connect2ServerIfNoConnected();
				
			while (M_THREAD_RUN) {
			
				QByteArray qba = QByteArray(1920 * 1080,(data++%255));
#if 0					
				this->m_socket->WriteMy(qba);
#endif
			}

	 	}

		this->close_destory_socket_4_server();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/