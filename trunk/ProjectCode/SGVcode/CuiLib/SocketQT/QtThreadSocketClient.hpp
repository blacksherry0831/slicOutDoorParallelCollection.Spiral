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
	QtThreadSocketClient(qintptr p);
	QtThreadSocketClient();
	~QtThreadSocketClient(void);
protected:
	
	//�����Լ���Ҫ�ķ��������  
	qintptr ptr_sd;
	
	QSharedPointer<QtTcpClient>  m_socket;//�ͻ��˵Ķ���
	std::string mIpAddr;
	int mPort;
public:
	void  connect2ServerIfNoConnected();
	void  disconnect4Server();
	void  closeSocket4Server();
	int   IsSocketAliveEx();
public:
	void write_ptr(qintptr p);
	void init_socket();
	void SetSocketDesp();
	

signals:
void socket_connect_state(int);

public slots:



protected: 
	virtual void run();
public:

};