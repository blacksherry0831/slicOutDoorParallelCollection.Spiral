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
class QtThreadClient :public QtThreadBase
{

public:
	QtThreadClient(qintptr p);
	~QtThreadClient(void);
protected:
	
	//定义自己需要的方法或变量  
	qintptr ptr_sd;
	
	QSharedPointer<QtTcpClient>  m_socket;//客户端的定义
public:
	void write_ptr(qintptr p);
	
	
	
protected: 
	virtual void run();

};