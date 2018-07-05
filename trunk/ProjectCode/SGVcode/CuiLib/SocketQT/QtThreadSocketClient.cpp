//#include "stdafx.h"
#include "QtThreadSocketClient.hpp"
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
QtThreadSocketClient::QtThreadSocketClient()
{
#if 1
	mIpAddr = "192.168.100.101";
#else
	mIpAddr = "127.0.0.1";
#endif // 0
	m_socket = QSharedPointer<QtTcpClient>(new QtTcpClient());
	m_socket->moveToThread(this);
	this->ptr_sd =-1;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
QtThreadSocketClient::QtThreadSocketClient(qintptr p)
{
	this->write_ptr(p);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
QtThreadSocketClient::~QtThreadSocketClient(void)
{
	qDebug() << "QtThreadClient is Release ! ";
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadSocketClient::write_ptr(qintptr p)
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
void QtThreadSocketClient::SetSocketDesp()
{
	if (ptr_sd>=0) {
		m_socket = QSharedPointer<QtTcpClient>(new QtTcpClient());
		m_socket->setSocketDescriptor(ptr_sd);
	}
	
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadSocketClient::run()
{
	

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void  QtThreadSocketClient::connect2ServerIfNoConnected()
{
	if (m_socket->IsSocketAlive() == false) {

		do {
			m_socket = QSharedPointer<QtTcpClient>(new QtTcpClient());
			m_socket->connectToHost(QHostAddress(mIpAddr.c_str()), mPort);
			std::cout << "Try Connect to IP: " << mIpAddr << "Port:" << mPort << std::endl;
			QThread::sleep(1);
		} while (m_socket->waitForConnected(MAX_MSECS) == false);
		std::cout << "Connect  Success: " << mIpAddr << "Port:" << mPort << std::endl;
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