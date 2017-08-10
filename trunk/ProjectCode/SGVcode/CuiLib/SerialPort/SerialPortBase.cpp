#include "stdafx.h"
#include "SerialPortBase.hpp"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/

/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/

SerialPortBase::SerialPortBase(void)
{
	

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
SerialPortBase::~SerialPortBase(void)
{

	this->close_port();

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
bool SerialPortBase::open(int com_num)
{
	if(m_sp.IsOpen()==false){
		m_sp.Open(com_num,9600);
		{
			COMMTIMEOUTS Timeouts;//unit ms 
			Timeouts.ReadIntervalTimeout = 100;//ms
			Timeouts.ReadTotalTimeoutConstant=100;
			Timeouts.ReadTotalTimeoutMultiplier=100;
			Timeouts.WriteTotalTimeoutConstant=100;
			Timeouts.WriteTotalTimeoutMultiplier=100;
			m_sp.SetTimeouts(Timeouts);
		}
#if _MSC_VER
		//HANDLE handle_t=::CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)readCompassThread,this,0,NULL);
#endif
	}

	return m_sp.IsOpen();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void SerialPortBase::init()
{
	if (m_sp.IsOpen()==TRUE){
		return;
	}

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/

/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void SerialPortBase::close_port()
{	
	if (m_sp.IsOpen() == TRUE) {
		m_sp.Close();	
	}
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void SerialPortBase::close()
{
	this->close_port();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int SerialPortBase::serial_write(const void * buffer, DWORD num)
{	
	int WriteNum = -1;

	if (m_sp.IsOpen()) {
		return m_sp.Write(buffer, num);
	}

	return WriteNum;
}


/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/

/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/

/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/

/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/

/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/