#pragma once

#include "cpp_stl.h"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
#include "qt_all.h"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
#include "../SocketQT/QtThreadSocketClient.hpp"
#include "../SocketQT/QtTcpClient.hpp"
#include "../SocketQT/conf_ip.h"
#include "SocketQT/QtThreadClientCtrl.hpp"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
class QtThreadFlowCtrlClient :public QtThreadSocketClient
{

public:
	QtThreadFlowCtrlClient();
	~QtThreadFlowCtrlClient(void);
protected:
	
public:
	void run();
protected: 
	

};