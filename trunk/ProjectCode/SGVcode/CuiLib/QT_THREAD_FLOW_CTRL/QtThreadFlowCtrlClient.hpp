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
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
class QtThreadFlowCtrlClient :public QtThreadSocketClient
{

public:
	QtThreadFlowCtrlClient(qintptr p);
	~QtThreadFlowCtrlClient(void);
protected:
	
public:
		
protected: 
	

};