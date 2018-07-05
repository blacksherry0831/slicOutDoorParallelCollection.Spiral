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
#include "QtThread8Video.hpp"
#include "QtTcpClient.hpp"
#include "ChannelsData.hpp"
#include "conf_ip.h"
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