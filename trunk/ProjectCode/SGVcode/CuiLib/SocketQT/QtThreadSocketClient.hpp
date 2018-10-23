#pragma once

#include "cpp_stl.h"



#include "QtThreadBase.hpp"
#include "QtTcpClient.hpp"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
class QtThreadSocketClient :public QtThreadBase
{
	Q_OBJECT

public:
	
public:
	QtThreadSocketClient(qintptr p);
	QtThreadSocketClient();
	~QtThreadSocketClient(void);
protected:	
	//定义自己需要的方法或变量  
	qintptr ptr_sd;	
	QSharedPointer<QtTcpClient>  m_socket;//客户端的定义
	std::string mIpAddr;
	int mPort;
	int mSocketConnected;
public:
	int IsSocketConnectedThreadRunning();
public:
	void  connect2ServerIfNoConnected();
	void  disconnect4Server();
	void  closeSocket();
	void  closeSocket4Server();
	int   IsSocketAliveEx();
	int   IsSocketConnectedEx();
	void emit_status_message(const QString& _msg);
public:
	void write_ptr(qintptr p);
	void init_socket();
	void SetSocketDesp();
	void SetIpAddr(QString _ipAddr);
public:
	void startServer();
	void closeServer();
	void wait4ServerClose();

signals:
void socket_connect_state(int);


public slots:



protected: 
	virtual void run();
public:

};