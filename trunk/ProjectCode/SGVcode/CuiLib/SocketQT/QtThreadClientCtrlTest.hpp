#pragma once

#include "cpp_stl.h"


#include "QtThreadSocketClientCmdQ.hpp"
#include "QtTcpClient.hpp"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
class QtThreadClientCtrlTest :public QtThreadSocketClientCmdQ
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