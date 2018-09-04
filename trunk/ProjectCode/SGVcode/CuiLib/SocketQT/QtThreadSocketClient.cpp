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
void QtThreadSocketClient::init_socket()
{
	if (m_socket.isNull()) {
			m_socket = QSharedPointer<QtTcpClient>(new QtTcpClient());
	}	

	m_socket->close();

	emit socket_connect_state(false);
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
void QtThreadSocketClient::SetIpAddr(QString _ipAddr)
{
	this->mIpAddr = _ipAddr.toStdString();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadSocketClient::startServer()
{
	QtThreadBase::startServer();
	if (!m_socket.isNull()) {
		m_socket->startSocketRun();
	}
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadSocketClient::closeServer()
{
	int count = 0;

	QtThreadBase::closeServer();

	if (!m_socket.isNull()) {
		m_socket->stopSocketRun();
	}

	this->wait4ServerClose();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadSocketClient::wait4ServerClose()
{
	int count=0;
	while (this->isRunning()) {

		QThread::sleep(1);
		this->emit_status_message(QString("wait for thread done").append(QString::number(count++)));

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
	if (this->IsSocketAliveEx() == false) {

		do {
			this->init_socket();
			
			m_socket->connectToHost(QHostAddress(mIpAddr.c_str()), mPort);
			this->emit_status_message(mStatusMessage = QString("Socket>> ").append("Try Connect to IP : wait 30s,"));

			
			if (m_socket->waitForConnected(MAX_MSECS)==true) {
				
				emit socket_connect_state(true);
				this->emit_status_message(mStatusMessage = QString("Socket>> ").append("Connect  Success: "));
				
				break;
			}else{
				
				m_socket->disconnectFromHostMy();
				this->emit_status_message(mStatusMessage = QString("Socket>> ").append("Connect  fail: "));
				emit socket_connect_state(false);
			}

		} while (M_THREAD_RUN);
		
	}
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadSocketClient::disconnect4Server()
{
	this->m_socket->disconnectFromHostMy();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadSocketClient::closeSocket4Server()
{
	if (!this->m_socket.isNull()) {
			this->m_socket->close();
			this->emit_status_message(mStatusMessage = QString("Socket>> ").append("close: ").append(QString(mIpAddr.c_str())).append("Port:").append(QString::number(mPort)));
			emit socket_connect_state(false);
	}

	if (this->M_THREAD_RUN==false){
		this->m_socket.clear();
	}
	
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int QtThreadSocketClient::IsSocketAliveEx()
{
	if (m_socket.isNull()) {
		m_socket = QSharedPointer<QtTcpClient>(new QtTcpClient());
	}
	
	return m_socket->IsSocketAlive();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadSocketClient::emit_status_message(const QString & _msg)
{
	QString msg_t;

	msg_t.append(mThreadName).append(", ")
		.append(QString(mIpAddr.c_str())).append(", ")
		.append("Port:").append(QString::number(mPort))
		.append(_msg).append(" ");
		
	qDebug() << msg_t;

	emit status_message(msg_t);
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