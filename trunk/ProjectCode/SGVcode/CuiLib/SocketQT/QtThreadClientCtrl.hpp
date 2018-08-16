#pragma once

#include "cpp_stl.h"

#include "qt_all.h"

#include "QtThreadSocketClient.hpp"
#include "QtTcpClient.hpp"
#include "CMD_CTRL_Q.hpp"

#include "conf_ip.h"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
class QtThreadClientCtrl :public QtThreadSocketClient
{
	Q_OBJECT
public:
	
public:
	QtThreadClientCtrl();
	QtThreadClientCtrl(qintptr p);
	~QtThreadClientCtrl(void);
protected:
	void initIpPort();
private:
	CMD_CTRL::WorkMode mWorkMode;
public:
	static CMD_CTRL_Q cmds;

	int SendCmdCtrl();

	void SetWorkMode(CMD_CTRL::WorkMode _wm);

	void SetWorkModeCmd(CMD_CTRL::WorkMode _wm);

public slots:
	
protected: 
	virtual void run();

};