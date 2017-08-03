#pragma once

#include "cpp_stl.h"

#include "SerialPort.h"

#include "pt_mutex.h"


/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
class SerialPortBase
{
protected:
	~SerialPortBase(void);
protected:
	ThreadMutex m_MUTEX;  
#if _MSC_VER
	CSerialPort m_sp;	
#endif
protected:
	 static  SerialPortBase* _instance;

public:
		SerialPortBase(void);
public:
	virtual void open(int com_num);
	virtual void init();
protected:
	void close_port();
	void close();
	int serial_write(const void* buffer,DWORD num);
};