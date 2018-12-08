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
#include "SocketQT/QtThreadSocketClient.hpp"
#include "SocketQT/QtTcpClient.hpp"
#include "SocketQT/ChannelsData.hpp"
#include "SocketQT/ChannelsData4Show.hpp"
#include "SocketQT/conf_ip.h"
#include "SocketQT/type_socket_qt.h"
/*-------------------------------------*/
#include "MY_SDK_LIB/CrackDetection.hpp"
#include "QT_SDK_LIB/QBase.h"
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
	
	static void ProcessCmd(QSharedPointer<CMD_CTRL> _cmd);
	
	void setPriorityMy();
public:
	void set_record_time(QSharedPointer<CMD_CTRL> _cmd);
private:
	void emit_img_signals(QSharedPointer<CMD_CTRL> cmd_t);

	std::string mTimeCurrent;
signals:
	void img_stat(int _p_stat,int _channel,int _frames);
protected: 
	virtual void before_enter_thread();
#if _DEBUG
	virtual void run();
#endif
	virtual void run_socket_work();

};