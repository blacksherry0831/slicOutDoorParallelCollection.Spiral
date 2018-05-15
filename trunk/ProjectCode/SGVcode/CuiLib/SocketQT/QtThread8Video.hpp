#pragma once

#include "cpp_stl.h"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
#include <QtCore>
#include <QtNetwork>
#include <QSharedPointer>
#include <QThread>
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
#include "QtThreadSocketClient.hpp"
#include "QtTcpClient.hpp"
#include "ChannelsData.hpp"
#include "conf_ip.h"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
class QtThread8Video :public QtThreadSocketClient
{

public:
	QtThread8Video(qintptr p);
	QtThread8Video();
	~QtThread8Video(void);
protected:
	void initIpPort();
public:
		
	void Run0();
	void run1();
	void ProcessCmd(QSharedPointer<CMD_CTRL> cmd_t);
protected: 
	virtual void run();

};