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
	
	//�����Լ���Ҫ�ķ��������  
	qintptr ptr_sd;
	
	QSharedPointer<QtTcpClient>  m_socket;//�ͻ��˵Ķ���
public:
	void write_ptr(qintptr p);
	
	
	
protected: 
	virtual void run();

};