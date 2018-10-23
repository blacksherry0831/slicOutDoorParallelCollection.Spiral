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
	Q_OBJECT
public:
	QtThread8Video(qintptr p);
	QtThread8Video();
	~QtThread8Video(void);
protected:
	void initIpPort();
public:
	void save_record(int _is_save);
	void Run0();
	void run1();
	static void ProcessCmd(QSharedPointer<CMD_CTRL> cmd_t);
private:
	void emit_img_signals(QSharedPointer<CMD_CTRL> cmd_t);
signals:
	void img_stat(int _p_stat,int _channel,int _frames);
protected: 
	virtual void run();

};