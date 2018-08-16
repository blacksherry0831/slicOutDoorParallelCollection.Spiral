//#include "stdafx.h"
#include "QtLink.hpp"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
QtLink::QtLink(void)
{
	mRun = TRUE;
	m_ipAddr = BORD_VIDEO_OUT;
}
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
QtLink::~QtLink(void)
{
	mTcpSocket.clear();
	mPtrPingProcess.clear();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtLink::ping_link_check(void)
{
	if (mRun==TRUE){
			if (mPtrPingProcess.isNull()) {
					mPtrPingProcess = QSharedPointer<QProcess>(new QProcess(this));
			}
	 }

	if (!mPtrPingProcess.isNull()) {
	
		QProcess::ProcessState pState= mPtrPingProcess->state();

			if (pState == QProcess::ProcessState::NotRunning) {
				
				if (mRun == TRUE) {
					OnPing();
				}else{
					mPtrPingProcess.clear();

				}				

			}else if (pState == QProcess::ProcessState::Running) {
	
			}else if (pState == QProcess::ProcessState::Starting) {
	
			}else{

			}
	
	}
			

	


}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void  QtLink ::PingDevices(QString _ipStr,int _timeOut)
{
	QProcess *cmd = new QProcess;
	#ifdef _TTY_ARMV4_  
	QString strArg = "ping -s 1 -c 1 " + b_tmpIpStr;        //linux平台下的格式  
	#else  
	QString strArg = "ping " + _ipStr + " -n 1 -w " + QString::number(_timeOut);  //windows下的格式  
	#endif  
	cmd->start(strArg);
	cmd->waitForReadyRead();
	cmd->waitForFinished();
	QString retStr = cmd->readAll();
	if (retStr.indexOf("TTL") != -1)
	{
		qDebug() << _ipStr << "is online!\n";
	}
	retStr.clear();

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtLink::Stop()
{
	mRun = FALSE;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtLink::OnPing()
{
	connect(mPtrPingProcess.data(), SIGNAL(finished(int)), this, SLOT(OnPingEnded(int)),Qt::ConnectionType::UniqueConnection);
#ifdef __linux__
	mPingProcess.start("ping", QStringList() << "-c" << "1" << m_ipAddr);
#else
	mPtrPingProcess->start("ping", QStringList() << "-n" << "1" << m_ipAddr);
#endif
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void  QtLink::OnPingEnded(int exitCode)
{
	QByteArray output = mPtrPingProcess->readAllStandardOutput();
	if (!output.isEmpty())
	{

		if(exitCode!=0)	qDebug() << QString::fromLocal8Bit(output);
		
		if (-1 != QString(output).indexOf("ttl", 0, Qt::CaseInsensitive))
		{
			if (exitCode != 0) qDebug() << "PING OK";
			emit ping_status(TRUE);
		}
		else
		{
			if (exitCode != 0) qDebug() << "PING ERROR";
			emit ping_status(FALSE);

		}
	}
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtLink::ssh_link_check()
{
	if (mTcpSocket.isNull()) {
		mTcpSocket = QSharedPointer<QTcpSocket>(new QTcpSocket(this));
	}
	
	QAbstractSocket::SocketState sockstate_t=  mTcpSocket->state();

	if (sockstate_t == QAbstractSocket::SocketState::UnconnectedState) {
		StartSSH();
	}else if (sockstate_t ==QAbstractSocket::SocketState::ConnectedState) {
	
		mTcpSocket->write("root");
		mTcpSocket->waitForBytesWritten(100);
		mTcpSocket->waitForReadyRead(100);
		QByteArray output = mTcpSocket->readAll();
		if(!output.isEmpty())
			qDebug() << QString::fromLocal8Bit(output);
	}


}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtLink::StartSSH()
{
	if (mTcpSocket.isNull()) {
		mTcpSocket = QSharedPointer<QTcpSocket>(new QTcpSocket(this));
	}


	connect(mTcpSocket.data(), SIGNAL(stateChanged(QAbstractSocket::SocketState)),this,SLOT(OnSshStateChange(QAbstractSocket::SocketState)),Qt::ConnectionType::UniqueConnection);

	mTcpSocket->connectToHost(m_ipAddr, TCP_PORT_SSH);


}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtLink::OnSshStateChange(QAbstractSocket::SocketState _sst)
{

	if (_sst == QAbstractSocket::SocketState::ConnectedState) {
		emit ssh_status(TRUE);
	}else {
		emit ssh_status(FALSE);
	}

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtLink::line_check()
{
#if 1
ping_link_check();

ssh_link_check();
#endif // 0
	
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/