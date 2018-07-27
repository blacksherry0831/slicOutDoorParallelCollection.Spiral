#pragma once

#include "cpp_stl.h"

#include "qt_all.h"

#include "conf_ip.h"

/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
class QtLink :public QObject
{
	Q_OBJECT
signals:
	   void ssh_status(int);
	   void ping_status(int);
public:
	QtLink();	
	~QtLink(void);
protected:
	
	//定义自己需要的方法或变量  
private:
	QString m_ipAddr;
	
	QSharedPointer<QProcess> mPtrPingProcess;

	QSharedPointer<QTcpSocket> mTcpSocket;

public:

	void PingDevices(QString _ipStr, int _timeOut);


public  slots :
	   void ping_link_check();
	   void ssh_link_check();
	   void line_check();
	   void OnPing();
	   void StartSSH();
	   void OnPingEnded(int exitCode);
	   void OnSshStateChange(QAbstractSocket::SocketState _sst);
};