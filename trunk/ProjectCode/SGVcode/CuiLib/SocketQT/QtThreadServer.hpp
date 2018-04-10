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
	QtThreadServer(QSharedPointer<QtTcpServer> _TcpServer);
	~QtThreadServer(void);
private:
	QSharedPointer<QtTcpServer> m_TcpServer;
public:	
	void run();

};