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
	
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadSocketClient::init_socket()
{
	if (m_socket.isNull())
	{
		m_socket = QSharedPointer<QtTcpClient>(new QtTcpClient());

			if (ptr_sd >= 0) {
				m_socket->setSocketDescriptor(ptr_sd);
				mSocketConnected = TRUE;//client is connected
				mIpAddr = m_socket->peerAddress().toString().toStdString();

			}else if (ptr_sd==-1){
				m_socket = QSharedPointer<QtTcpClient>(new QtTcpClient());
								
			}else {
				
				Q_ASSERT(FALSE);
			
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
	this->before_enter_thread();
	this->enter_thread();

				this->init_socket_in_thread();

							while (M_THREAD_RUN){
									
									this->connect2ServerIfNoConnected();

											while (M_THREAD_RUN && mSocketConnected){

												this->run_socket_work();

											}

									this->close_destory_socket_4_server();

							}

				this->destory_socket_in_thread();

	this->exit_thread();
	this->after_exit_thread();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadSocketClient::run_socket_work()
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
	if (ptr_sd >= 0) {
		return;
	}

	if (this->IsSocketAliveEx() == false) {

		do {
			this->init_socket();
#if _DEBUG
			this->IsSocketConnectedEx();
#endif

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
int QtThreadSocketClient::send_and_read_cmd(QSharedPointer<CMD_CTRL> _cmd_send, QSharedPointer<CMD_CTRL> _cmd_resp)
{
	QSharedPointer<CMD_CTRL> cmd_read_t = _cmd_resp;

	if (0==Send_1_cmd(_cmd_send)) {
		return  SocketErrorMy;
	}
	else
	{
			//send cmd success
			if (0 == Read_1_cmd(cmd_read_t)) {

						return  SocketErrorMy;

			}else{

				return TRUE_MY;

			}

	}

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
void QtThreadSocketClient::enter_thread()
{
	this->emit_thread_starting();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadSocketClient::exit_thread()
{
	this->emit_thread_stopping();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadSocketClient::init_socket_in_thread()
{
	this->init_socket();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadSocketClient::destory_socket_in_thread()
{
	this->close_destory_socket_4_server();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadSocketClient::before_enter_thread()
{

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadSocketClient::after_exit_thread()
{

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadSocketClient::close_destory_socket_4_server()
{

	if (!this->m_socket.isNull()) {

		if (this->m_socket->IsSocketAlive()) {
			this->m_socket->close();
			this->emit_status_message(mStatusMessage = QString("Socket>> ").append("close: ").append(QString(mIpAddr.c_str())).append("Port:").append(QString::number(mPort)));
			
			this->m_socket.clear();
			mSocketConnected = FALSE;
			emit socket_connect_state(false);		
		
		}
	}

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
QString QtThreadSocketClient::GetIpAddr()
{
	return   QString::fromStdString(mIpAddr);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
