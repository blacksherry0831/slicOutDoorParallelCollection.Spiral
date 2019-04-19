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
#include "QT_THREAD_MODULE/QtSigSjts.hpp"
#include "QT_THREAD_MODULE/CMD_WORK_FLOW.hpp"
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
class QtThreadSocketClientRoller :public QtThreadSocketClient
{
	Q_OBJECT

public:
	explicit QtThreadSocketClientRoller(QObject *parent = Q_NULLPTR);
public:
	void printf_roller_status(std::string _e);
protected:
	CMD_WORK_FLOW::SJTS_MACHINE_STATUS_ROLLER  mRollerStatus;

signals:
	void status_sjts_roller(int, QString);
protected:
	void emit_status_sjts_roller(QString _msg="");
	int IsRollerStatus(int _s);
protected:
	virtual void emit_roller_ready();
	virtual void emit_roller_pos_ready();
	virtual void emit_roller_into_inner_ready();
	virtual void emit_roller_done();

};