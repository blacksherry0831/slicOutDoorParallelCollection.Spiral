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
	QSharedPointer<QtTcpClient>  m_socket;//�ͻ��˵Ķ���
protected:	
	//�����Լ���Ҫ�ķ��������  
	qintptr ptr_sd;	

	std::string mIpAddr;
	int mPort;
	int mSocketConnected;
	
public:
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
public:
	int Send_1_cmd(QSharedPointer<CMD_CTRL> _cmd);
	int Read_1_cmd(QSharedPointer<CMD_CTRL> _cmd);
	//int Send_Start_CMD(CMD_CTRL::CMD_TYPE_02_C _type_c, CMD_CTRL::WorkMode _wm);
	int SendHearbeatCmd();
	ResultMy read_n_byte(int _n);
	ResultMy write_n_byte(const char * const _data, const int _size);
	int getByteTcpRead();;
public:
	int send_and_read_cmd(QSharedPointer<CMD_CTRL> _cmd_send,QSharedPointer<CMD_CTRL> _cmd_resp=Q_NULLPTR);
signals:
void socket_connect_state(int);
void thread_running_state(int);

public slots:

protected: 
	virtual void run();
	virtual void run_socket_work();
private:
	CMD_CTRL_Q mCmdMsgQ;
public:
	void SetMsg(QSharedPointer<CMD_CTRL> _msg);
	QSharedPointer<CMD_CTRL> GetMsg();
	void ClearMsg();
};