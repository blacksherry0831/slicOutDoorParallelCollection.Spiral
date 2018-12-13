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
protected:
	QSharedPointer<QtTcpClient>  m_socket;//客户端的定义
protected:	
	//定义自己需要的方法或变量  
	qintptr ptr_sd;	

	std::string mIpAddr;
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

	void emit_thread_starting();
	void emit_thread_stopping();
protected:
	virtual void before_enter_thread();
	virtual void after_exit_thread();
protected:
	void enter_thread();
	void exit_thread();
	virtual int  socket_thread_run_condition();
	void init_socket_in_thread();
	void destory_socket_in_thread();
public:
	void write_ptr(qintptr p);
	void init_socket();
	
	void init_param();
	void SetIpAddr(QString _ipAddr);
	QString GetIpAddr();
public:
	void startServer();
	void closeServer();
	void closeRunningServer();
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
public:
	ResultMy read_n_byte(int _n);
	ResultMy write_n_byte(const char * const _data, const int _size);
	int getByteTcpRead();;
public:
	
signals:
void socket_connect_state(int);
void thread_running_state(int);
void running_client_sessions_change();
void client_session_work_state(int ,int);

public slots:

protected: 
	virtual void run();
	virtual void run_socket_work();

private:
	int SendHearbeatCmd(int _need_resp);
	int SendHeartBeatCmdReadResp();
private:
	CMD_CTRL_Q mCmdMsgQ;
public:
	virtual void beforeSendMsg();
public:
	void SetMsg(QSharedPointer<CMD_CTRL> _msg);
	QSharedPointer<CMD_CTRL> GetMsg();
	void ClearMsg();
protected:
	int mWorkFlowStart;
	int mWorkFlowEnd;
	
public:
	int IsWorkFlowDone();
};