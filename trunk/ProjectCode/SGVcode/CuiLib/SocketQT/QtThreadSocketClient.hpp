#pragma once

#include "cpp_stl.h"

#include <QtCore>
#include <QtNetwork>
#include <QSharedPointer>
#include <QThread>

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
public:
	void  connect2ServerIfNoConnected();
public:
	void write_ptr(qintptr p);
	
	void SetSocketDesp();
	
protected: 
	virtual void run();
public:


};