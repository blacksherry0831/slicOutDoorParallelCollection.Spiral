#pragma once
/*-------------------------------------*/
#include "cpp_stl.h"
/*-------------------------------------*/
#include "QtThreadSocketClient.hpp"
#include "QtTcpClient.hpp"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
class QtThreadClientCtrlTest :public QtThreadSocketClient
{

public:
	QtThreadClientCtrlTest();
	QtThreadClientCtrlTest(qintptr p);
	~QtThreadClientCtrlTest(void);
protected:
	
	
public:

protected: 
	virtual void run();

};