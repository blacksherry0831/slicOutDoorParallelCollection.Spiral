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
#include "SocketQT/QtTcpClient.hpp"
#include "SocketQT/ChannelsData.hpp"
#include "SocketQT/conf_ip.h"
#include "SocketQT/QtThreadSocketClient.hpp"
/*-------------------------------------*/

#include "QtThread8Video.hpp"

/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
class QtThread8VideoRaw :public QtThreadSocketClient
{

public:
	
	QtThread8VideoRaw();
	~QtThread8VideoRaw(void);
protected:
	void initIpPort();
public:	
	
protected: 
	virtual void run();

};