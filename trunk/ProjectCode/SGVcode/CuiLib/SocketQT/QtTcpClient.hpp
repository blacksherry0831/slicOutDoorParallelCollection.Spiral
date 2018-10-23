#pragma once

#include "cpp_stl.h"

#include "qt_all.h"

#include "CMD_CTRL.hpp"

#include "SerialPort/BE_1105_Dirver.hpp"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
enum ResultMy
{
	INIT_MY = 255,
	SocketErrorMy = -1,
	SocketErrotTimeout=-2,
	TRUE_MY = 1,
	FALSE_MY = 0,
};
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
class QtTcpClient :public QTcpSocket
{
	Q_OBJECT
protected:
	int  MAX_MSECS;
public:
	explicit QtTcpClient(QObject *parent = Q_NULLPTR);
	~QtTcpClient(void);
private:
	QByteArray m_buffer;
	int m_buffer_Length;
protected:
	int mSocketRun;
	int mSocketConnected;
	int mSocketErrorOccur;
public:
	int init();
	ResultMy read_n_byte(int _n);
	ResultMy write_n_byte(const char* const _data, const int _size);
	int ReadAllMy(const int _size);
	int ReadMy(const int _size);
	int ReadMy_seg(const int _size);
	int ReadMy_all(const int _size);
	int WriteMy(QByteArray _data);
	int WriteMy(const char* const _data,const int _size);
public:
	int IsSocketAlive();
	int IsSocketConnected();
	int IsSocketError();
	
	void disconnectFromHostMy();
public:
	void startSocketRun();
	void stopSocketRun();
public:
	int Send_1_cmd(CMD_CTRL *_cmd);
	int Read_1_cmd(CMD_CTRL *_cmd);
	int Read_1_cmd_fast(CMD_CTRL *_cmd);
	int Read_nSize_2_body(CMD_CTRL *_cmd);
	int getByteTcpRead();
public:
	int Send_Start_CMD(CMD_CTRL::CMD_TYPE_02_C _type_c, CMD_CTRL::WorkMode _wm);
	int SendHearbeatCmd();
public:
	int SendPlcResp(int _type);
	int SendPlcIntoInter(int _step);
	int SendPlcRollerQualified(int _qualified);

signals:
	void socket_connect_state(int);
public:

};