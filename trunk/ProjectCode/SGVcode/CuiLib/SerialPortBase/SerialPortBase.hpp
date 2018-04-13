#pragma once

#include "cpp_stl.h"
#include <QtCore>
#include <QSerialPort>  
#include <QSerialPortInfo>


/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
class SerialPortBase :public QObject
{
	Q_OBJECT
public:
	SerialPortBase(void);
protected:
	QSharedPointer<QSerialPort> m_qsp;
	QSharedPointer<QTimer> m_timer;
	vector<QSerialPortInfo> m_serialPorts;
	QQueue<unsigned char>  m_buffer;

protected:
	~SerialPortBase(void);
protected:
	QMutex m_MUTEX;  


	DWORD m_baudrate;
	char	buffer_result[1024];//Ω” ‹buffer
	char	buffer_cmd[1024];//∑¢ÀÕbuffer
protected:
	void close_port();
	void close();
	int serial_write(const void* buffer,DWORD num);
	int serial_read_all();

	int serial_read(void* _data_out, int _size);
	int serial_process_data(const char* _data,int _len);
public:
	void PrintAllSerialPort(void);
private:
	void initSerialPort();
public:
	int open_ttyUSB();

	virtual int open(int com_num);
	virtual int open_s(string com_name);
	virtual int open_q(QSerialPortInfo _qspi);
	virtual int init();
	void StartTimer();
public slots :
	int readComDataSlot();
};