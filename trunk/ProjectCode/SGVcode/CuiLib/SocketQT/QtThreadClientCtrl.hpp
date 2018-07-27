#pragma once

#include "cpp_stl.h"

#include <QtCore>
#include <QtNetwork>
#include <QSharedPointer>
#include <QThread>

#include "QtThreadSocketClient.hpp"
#include "QtTcpClient.hpp"

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

	void SetWorkMode(CMD_CTRL::WorkMode _wm);

public slots:
	
protected: 
	virtual void run();

};