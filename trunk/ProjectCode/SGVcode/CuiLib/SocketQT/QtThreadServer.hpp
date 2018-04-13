#pragma once

#include "cpp_stl.h"

#include <QtCore>
#include <QtNetwork>
#include <QSharedPointer>

#include "QtTcpServer.hpp"

/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
class QtThreadServer :public QtThreadBase
{
private:
	
protected:
	
public:
	QtThreadServer();
	QtThreadServer(int _port,QSharedPointer<QtTcpServer> _TcpServer);
	~QtThreadServer(void);
private:
	QSharedPointer<QtTcpServer> m_TcpServer;
	int m_port;
public:	
	void run();

};