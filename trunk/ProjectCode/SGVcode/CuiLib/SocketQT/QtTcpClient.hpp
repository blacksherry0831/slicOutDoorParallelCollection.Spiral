#pragma once

#include "cpp_stl.h"

#include <QtCore>
#include <QtNetwork>
#include <QSharedPointer>

/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
class QtTcpClient :public QTcpSocket
{
	
protected:
	int  MAX_MSECS;
public:
	QtTcpClient();
	~QtTcpClient(void);
public:
	int init();
	QByteArray readAllMy();
	int WriteMy(QByteArray _data);

};