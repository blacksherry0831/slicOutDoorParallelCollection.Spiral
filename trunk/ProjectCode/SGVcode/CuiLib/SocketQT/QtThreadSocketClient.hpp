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
	explicit QtThreadSocketClient(QObject *parent = Q_NULLPTR);
	~QtThreadSocketClient(void);

signals:
	void socket_connect_state(int);
	void client_session_work_state(int, int, int);

protected:
	QSharedPointer<QtTcpClient>  m_socket;//客户端的定义
protected:	
	//定义自己需要的方法或变量  
	qintptr ptr_sd;	

	std::string mIpAddr;
	std::string mIpAddrLocal;
	std::string mIpAddrRemote;
	int mPort;
private:
	int mSocketConnected;
	
public:
	int GetSocketConnected();
	int IsSocketConnectedThreadRunning();
public:
	void  connect2ServerIfNoConnected();
	void  disconnect4Server();
	
	void  close_destory_socket_4_server();
	int   IsSocketAliveEx();
	int   IsSocketConnectedEx();
	void emit_status_message(const QString& _msg);
protected:
	virtual void emit_thread_starting();
	virtual void emit_thread_stopping();
protected:
	virtual void enter_thread();
	virtual void exit_thread();
protected:

	virtual int  socket_thread_run_condition();
	void init_socket_in_thread();
	void destory_socket_in_thread();
public:
	void write_ptr(qintptr p);
	void init_socket();
	
	void init_param();
	void SetIpAddr(QString _ipAddr);
	QString GetIpAddr();
	QString GetClientSessionIpAddr();
public:
	void startServer();
	void closeServer();
	void closeServerAsync();
	void closeServerSync();
	void wait4ServerClose();
private:

public:
	int Send_1_cmd(QSharedPointer<CMD_CTRL> _cmd);
	int Send_1_cmd_resp(CMD_CTRL::CMD_TYPE_02_RESP _resp= CMD_CTRL::CMD_TYPE_02_RESP::CT_OK,int _resp_value=0);
	int Read_1_cmd(QSharedPointer<CMD_CTRL> _cmd);

	int Read_1_cmd_process_hearbeat(QSharedPointer<CMD_CTRL> _cmd);
	
	int send_and_read_cmd_resp(QSharedPointer<CMD_CTRL> _cmd_send, QSharedPointer<CMD_CTRL> _cmd_resp = Q_NULLPTR);
	int send_and_read_cmd(QSharedPointer<CMD_CTRL> _cmd_send, QSharedPointer<CMD_CTRL> _cmd_resp = Q_NULLPTR);
	
	int SendHearbeatCmd5s(int _need_resp);
	int SendHeartBeatCmdReadResp5s();
	int SleepMy_HeartBeatReadResp5s(int _100ms=100);
public:
	ResultMy read_n_byte(int _n);
	ResultMy write_n_byte(const char * const _data, const int _size);
	int getByteTcpRead();;
public:
	


public slots:

protected: 
	virtual void run();
	virtual void run_socket_work();

private:
	int SendHearbeatCmd(int _need_resp);
	int SendHeartBeatCmdReadResp();
	void SendHearbeatResp(QSharedPointer<CMD_CTRL> _cmd);
public:
	virtual void beforeSendMsg();
protected:
	int mWorkFlowStart;
	int mWorkFlowEnd;
	int mWorkFlowResult;
public:
	int IsWorkFlowDone();
	int getWorkFlowResult();
};