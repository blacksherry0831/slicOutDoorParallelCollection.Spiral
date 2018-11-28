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
	this->init_param();
#if 1
	mIpAddr = "192.168.100.101";
#else
	mIpAddr = "127.0.0.1";
#endif // 0
	m_socket = QSharedPointer<QtTcpClient>(new QtTcpClient());
	m_socket->moveToThread(this);

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
QtThreadSocketClient::QtThreadSocketClient(qintptr p)
{
	this->init_param();
	this->ptr_sd = p;
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
void QtThreadSocketClient::init_param()
{
	this->ptr_sd = -1;
	this->mSocketConnected = FALSE;
	this->mSleepTime = 0;
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
			
			m_socket->setSocketOption(QAbstractSocket::ReceiveBufferSizeSocketOption, 256*1024*1024);//这个没有任何作用
	}	

	this->closeSocket();

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadSocketClient::init_socket_client_session()
{
	if (ptr_sd >= 0) {
		m_socket = QSharedPointer<QtTcpClient>(new QtTcpClient());
		m_socket->setSocketDescriptor(ptr_sd);
		mSocketConnected=TRUE;
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
void QtThreadSocketClient::closeRunningServer()
{
	if (this->isRunning()) {
		this->closeServer();
	}
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
int QtThreadSocketClient::IsSocketConnectedThreadRunning()
{
	return  mSocketConnected && this->isRunning();
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
				mSocketConnected=TRUE;
				break;
			}else{
				mSocketConnected = FALSE;
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
void QtThreadSocketClient::closeSocket()
{
	if (!this->m_socket.isNull()) {

			if (this->m_socket->IsSocketAlive()) {
						this->m_socket->close();						
						this->emit_status_message(mStatusMessage = QString("Socket>> ").append("close: ").append(QString(mIpAddr.c_str())).append("Port:").append(QString::number(mPort)));
			}	

			mSocketConnected = FALSE;
			emit socket_connect_state(false);
	}

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadSocketClient::closeSocket4Server()
{	
	this->closeSocket();

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
int QtThreadSocketClient::IsSocketConnectedEx()
{

	if (m_socket.isNull()) {
		m_socket = QSharedPointer<QtTcpClient>(new QtTcpClient());
	}

	return m_socket->IsSocketConnected();

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
int QtThreadSocketClient::send_and_read_resp(QSharedPointer<CMD_CTRL> _cmd_send)
{
	QSharedPointer<CMD_CTRL> cmd_read_t = QSharedPointer<CMD_CTRL>(new CMD_CTRL());

	if (this->m_socket->Send_1_cmd(_cmd_send.data())==0) {
		return  SocketErrorMy;
	}

	if (0 == m_socket->Read_1_cmd(cmd_read_t.data())) {

			return  SocketErrorMy;

	}else{

			if (cmd_read_t->IsResp()) {
				return TRUE_MY;
			}else{
				return FALSE_MY;
			}

	}
	return TRUE_MY;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadSocketClient::SetMsg(QSharedPointer<CMD_CTRL> _msg)
{
	mCmdMsgQ.setCmd(_msg);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
QSharedPointer<CMD_CTRL> QtThreadSocketClient::GetMsg()
{
	return mCmdMsgQ.getCmd();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadSocketClient::ClearMsg()
{
	mCmdMsgQ.clear();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int  QtThreadSocketClient::Send_1_cmd(QSharedPointer<CMD_CTRL> _cmd)
{
	mSocketConnected = this->m_socket->Send_1_cmd(_cmd.data());
	return mSocketConnected;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int  QtThreadSocketClient::Read_1_cmd(QSharedPointer<CMD_CTRL> _cmd)
{
	mSocketConnected = this->m_socket->Read_1_cmd(_cmd.data());
	return mSocketConnected;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int QtThreadSocketClient::Send_Start_CMD(CMD_CTRL::CMD_TYPE_02_C _type_c, CMD_CTRL::WorkMode _wm)
{

	QSharedPointer<CMD_CTRL> qsp_cc_t = CMD_CTRL::getFpgaStartCmdEx(_type_c, _wm);

	return Send_1_cmd(qsp_cc_t);

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int QtThreadSocketClient::SendHearbeatCmd()
{
	QSharedPointer<CMD_CTRL> qsp_cc_t = QSharedPointer<CMD_CTRL>(new CMD_CTRL());
	qsp_cc_t->getHeartBeatCmd(0);
	return Send_1_cmd(qsp_cc_t);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
ResultMy QtThreadSocketClient::read_n_byte(int _n)
{
	return this->m_socket->read_n_byte(_n);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int QtThreadSocketClient::getByteTcpRead()
{
	return this->m_socket->getByteTcpRead();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
ResultMy QtThreadSocketClient::write_n_byte(const char * const _data, const int _size)
{
	return this->m_socket->write_n_byte(_data,_size);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadSocketClient::emit_thread_starting()
{
	emit thread_running_state(TRUE);
	this->emit_status_message(mStatusMessage = "Thread>> Ctrl Thread Start");
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadSocketClient::emit_thread_stopping()
{
	emit thread_running_state(FALSE);
	this->emit_status_message(mStatusMessage = "Thread>>  shutdown");
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
