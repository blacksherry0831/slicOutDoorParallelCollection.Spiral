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
public:
	int init();
	int ReadAllMy();
	int ReadMy();
	int WriteMy(QByteArray _data);
	int WriteMy(const char* const _data,const int _size);
public:
	int IsSocketAlive();
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
public:
	int Send_Start_CMD(int _type, CMD_CTRL::WorkMode _wm);
	int SendHearbeatCmd();
public:
	int SendPlcResp(int _type);
	int SendPlcIntoInter(int _step);
	int SendPlcRollerQualified(int _qualified);

signals:
	void socket_connect_state(int);
public:

};