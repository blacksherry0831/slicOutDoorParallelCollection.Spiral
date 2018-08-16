#pragma once

#include "cpp_stl.h"
/*-----------------------------------*/
/**
*
*/
/*-----------------------------------*/
#include <QtCore>
#include <QtNetwork>
#include <QSharedPointer>
#include <QThread>
/*-----------------------------------*/
/**
*
*/
/*-----------------------------------*/
#include "CMD_CTRL.hpp"
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/

/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
class CMD_CTRL_Q
{
public:
	
public:
	CMD_CTRL_Q();
	~CMD_CTRL_Q();
private:
	QMutex mutex;
	QQueue<QSharedPointer<CMD_CTRL>> mCmds;
protected:

private:
	void clear();
	void init();
	void destory();

public:
	int QueueSize();
	int IsValid();
public:
	QSharedPointer<CMD_CTRL> getCmd();
	void setCmd(QSharedPointer<CMD_CTRL> cmd_ctrl);

};