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

#include "QT_THREAD_MODULE/QtSigSjts.hpp"

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
signals:
	void status_sjts_roller(int, QString);

protected:
	virtual void emit_roller_ready();
	virtual void emit_roller_pos_ready();
	virtual void emit_roller_into_inner_ready();
	virtual void emit_roller_done();

};