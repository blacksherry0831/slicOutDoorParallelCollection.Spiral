#pragma once
/*-------------------------------------*/
#include "cpp_stl.h"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
#include "QtThreadBase.hpp"
#include "QtTcpClient.hpp"
#include "CMD_CTRL_Q.hpp"
#include "QtThreadSocketClient.hpp"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
class QtThreadSocketClientCmdQ :public QtThreadSocketClient
{
	Q_OBJECT
public:
	QtThreadSocketClientCmdQ(qintptr p);
	explicit QtThreadSocketClientCmdQ(QObject *parent = Q_NULLPTR);
	~QtThreadSocketClientCmdQ(void);

private:
	CMD_CTRL_Q mCmdMsgQ;

public:
	void SetMsgWhileRunning(QSharedPointer<CMD_CTRL> _msg);
	void SetMsg(QSharedPointer<CMD_CTRL> _msg);
	QSharedPointer<CMD_CTRL> GetMsg();
	void ClearMsg();

};