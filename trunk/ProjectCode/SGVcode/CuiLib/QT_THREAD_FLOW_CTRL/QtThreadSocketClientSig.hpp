#pragma once
/*-------------------------------------*/
#include "cpp_stl.h"
/*-------------------------------------*/
#include <QObject>
#include <QtCore>
#include <QtNetwork>
#include <QSharedPointer>
/*-------------------------------------*/
#include "SocketQT/QtThreadSocketClient.hpp"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
class QtThreadSocketClientSig :public QtThreadSocketClient
{
	Q_OBJECT

public:
	explicit QtThreadSocketClientSig(QObject *parent = Q_NULLPTR);
		
signals :
	void status_sjts(int, QString);

};