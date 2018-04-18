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
class QtThreadPLC :public QtThreadBase
{

public:
	QtThreadPLC(qintptr p);
	~QtThreadPLC(void);
protected:
	std::string mIpAddr;
	int mPort;
		
	QSharedPointer<QtTcpClient>  m_socket;//客户端的定义
public:
		
	void Run0();
	void run1();
protected: 
	virtual void run();

};