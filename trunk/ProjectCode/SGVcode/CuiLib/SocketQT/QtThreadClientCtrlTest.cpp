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
		this->SetSocketDesp();
		int data = 1;
		
		
		while (M_THREAD_RUN)
		{	



			this->connect2ServerIfNoConnected();
				
			while (M_THREAD_RUN) {
			
				QByteArray qba = QByteArray(1920 * 1080,(data++%255));
									
				this->m_socket->WriteMy(qba);
				
				
			
			}
		


	 	}

		m_socket->close();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/