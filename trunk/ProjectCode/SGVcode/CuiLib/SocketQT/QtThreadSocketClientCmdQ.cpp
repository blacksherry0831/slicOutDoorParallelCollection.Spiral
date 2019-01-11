//#include "stdafx.h"
#include "QtThreadSocketClientCmdQ.hpp"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
QtThreadSocketClientCmdQ::QtThreadSocketClientCmdQ(QObject *parent):QtThreadSocketClient(parent)
{

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
QtThreadSocketClientCmdQ::QtThreadSocketClientCmdQ(qintptr p) :QtThreadSocketClient(p)
{
	
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
QtThreadSocketClientCmdQ::~QtThreadSocketClientCmdQ(void)
{
	qDebug() << "QtThreadSocketClientCmdQ ! ";
}

/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadSocketClientCmdQ::SetMsg(QSharedPointer<CMD_CTRL> _msg)
{
	mCmdMsgQ.setCmd(_msg);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
QSharedPointer<CMD_CTRL> QtThreadSocketClientCmdQ::GetMsg()
{
	return mCmdMsgQ.getCmd();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadSocketClientCmdQ::ClearMsg()
{
	mCmdMsgQ.clear();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadSocketClientCmdQ::SetMsgWhileRunning(QSharedPointer<CMD_CTRL> _msg)
{
	if (this->isRunning()) {
		this->SetMsg(_msg);
	}
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/