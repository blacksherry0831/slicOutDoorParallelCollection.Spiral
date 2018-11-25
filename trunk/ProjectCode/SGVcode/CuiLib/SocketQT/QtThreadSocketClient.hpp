#pragma once
/*-------------------------------------*/
#include "cpp_stl.h"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
#include "QtThreadBase.hpp"
#include "QtTcpClient.hpp"
#include "CMD_CTRL_Q.hpp"
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
	void init_socket_client_session();
	
	void SetIpAddr(QString _ipAddr);
public:
	void startServer();
	void closeServer();
	void closeRunningServer();
	void wait4ServerClose();
public:
	int send_and_read_resp(QSharedPointer<CMD_CTRL> _cmd_send);
signals:
void socket_connect_state(int);

public slots:

protected: 
	virtual void run();
private:
	CMD_CTRL_Q mCmdMsgQ;
public:
	void SetMsg(QSharedPointer<CMD_CTRL> _msg);
	QSharedPointer<CMD_CTRL> GetMsg();
	void ClearMsg();
};