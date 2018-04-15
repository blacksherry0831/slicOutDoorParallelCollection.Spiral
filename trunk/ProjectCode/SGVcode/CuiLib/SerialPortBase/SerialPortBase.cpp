//#include "stdafx.h"
#include "SerialPortBase.hpp"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
SerialPortBase::SerialPortBase(QObject *parent):QObject(parent)
{
	
	this->initSerialPort();
	this->m_timer =QSharedPointer<QTimer>(new QTimer());
	this->m_qsp = QSharedPointer<QSerialPort>(new QSerialPort());
	this->m_buffer.clear();
}
/*-------------------------------------*/
/**
*
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
void SerialPortBase::PrintAllSerialPort(void)
{	
	qDebug() << "seril port can use : " << QSerialPortInfo::availablePorts().size();

	foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
		qDebug() << "Name : " << info.portName();
		qDebug() << "Description : " << info.description();
		qDebug() << "Manufacturer: " << info.manufacturer();
		qDebug() << "Serial Number: " << info.serialNumber();
		qDebug() << "System Location: " << info.systemLocation();
		
	}
	
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void SerialPortBase::initSerialPort()
{
	foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
	
		this->m_serialPorts.push_back(info);
	}
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int SerialPortBase::open_ttyUSB()
{

	for (size_t i = 0; i <m_serialPorts.size(); i++){
		string ttysusb = m_serialPorts[i].portName().toStdString();
		if (ttysusb.find("ttyUSB") != std::string::npos) {
			if (this->open_s(ttysusb)==false) {
				std::cout << "sudo gedit /etc/udev/rules.d/70-ttyusb.rules" << std::endl
					<< " KERNEL==\"ttyUSB[0 - 9] * \", MODE=\"0666\" " << std::endl;
			}else{
				
			}
		}
	}
	return init();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int SerialPortBase::open(int com_num)
{
	std::string com_name;
	
	com_name.append(std::to_string(com_num));
		
	return open_s(com_name);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int SerialPortBase::open_s(string com_name)
{
	
	for (size_t i = 0; i <m_serialPorts.size(); i++) {
		string ttysusb = m_serialPorts[i].portName().toStdString();
		if (ttysusb.find(com_name) != std::string::npos) {
			this->open_q(m_serialPorts[i]);
		}
	}

	if (init()==false) {
		std::cout << "Cant Open: " <<com_name<< std::endl;
	}
	else {
		

	}


	return init();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int SerialPortBase::open_q(QSerialPortInfo _qspi)
{
	const string com_name = _qspi.portName().toStdString();

	std::cout << "ready to open serial port: " << com_name<< std::endl;
	
	

	m_qsp->setPort(_qspi);

	m_qsp->setBaudRate(m_baudrate, QSerialPort::AllDirections);
	m_qsp->setDataBits(QSerialPort::Data8);
	m_qsp->setParity(QSerialPort::NoParity);
	m_qsp->setStopBits(QSerialPort::OneStop);
	m_qsp->setFlowControl(QSerialPort::NoFlowControl);

	bool com = m_qsp->open(QIODevice::ReadWrite);//打

	if (com)
	{
		//"串口打开成功"
		std::cout << "open serial port：" << com_name << std::endl;
		/*connect(
		m_qsp,
		SIGNAL(readyRead()),
		this,
		SLOT(dealdata()));*/
	}
	else {
		std::cout << "open fail !" << std::endl;
	}

	return init();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int SerialPortBase::init()
{
	return m_qsp->isOpen()==true ? TRUE: FALSE;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void SerialPortBase::StartTimer()
{
	connect(m_timer.data(), SIGNAL(timeout()), this, SLOT(readComDataSlot()));
	m_timer->start(1000);
	std::cout << "Read Timer is Starting: " << std::endl;
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

	if (m_qsp->isOpen()) {
		m_qsp->close();
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

	if (m_qsp->isOpen()) {
	
		WriteNum=m_qsp->write((char*)buffer, num);

		m_qsp->waitForBytesWritten();
	
	}

	return WriteNum;
	
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int SerialPortBase::serial_read_all()
{
	
	
	m_qsp->waitForReadyRead();

	QByteArray showdata = m_qsp->readAll();

	const char* DATA = showdata.data();
	const int LEN = showdata.size();

	serial_process_data(DATA,LEN);
	
	return LEN;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int SerialPortBase::readComDataSlot()
{

	//读取串口数据  
	QByteArray readComData = m_qsp->readAll();

	//将读到的数据显示到数据接收区的te中  
	if (readComData.size()>0){
		
	}

	//清除缓冲区  
	readComData.clear();
	return 0;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int SerialPortBase::serial_read(void * _data_out, int _size)
{
	int size_t= m_qsp->read((char*)_data_out, _size);
	
	return size_t;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int SerialPortBase::serial_process_data(const char * _data, int _len)
{
	return 0;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void SerialPortBase::moveToThreadQSP(QThread* _thread)
{
	m_qsp->moveToThread(_thread);
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