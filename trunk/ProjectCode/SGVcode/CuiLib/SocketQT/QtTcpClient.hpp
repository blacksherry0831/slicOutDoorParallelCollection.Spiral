#pragma once

#include "cpp_stl.h"

#include <QtCore>
#include <QtNetwork>
#include <QSharedPointer>

#include "CMD_CTRL.hpp"

#include "SerialPort/BE_1105_Dirver.hpp"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
#define TCP_PORT_VIDEO_TRANS (10001)
#define TCP_POET_CMD_CTRL    (10000)
#define TCP_PORT_VIDEO_RAW	 (8888)
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
class QtTcpClient :public QTcpSocket
{
	
protected:
	int  MAX_MSECS;
public:
	QtTcpClient();
	~QtTcpClient(void);
private:
	QByteArray m_buffer;
public:
	int init();
	QByteArray readAllMy();
	int WriteMy(QByteArray _data);
	int WriteMy(const char* const _data,const int _size);
public:
	int IsSocketAlive();
public:
	int Send_1_cmd(CMD_CTRL *_cmd);
	int Read_1_cmd(CMD_CTRL *_cmd);
	int Read_nSize_2_body(CMD_CTRL *_cmd);
public:
	int Send_Start_CMD(int _type);
	int SendHearbeatCmd();
public:
	int SendPlcResp(int _type);
	int SendPlcIntoInter(int _step);
	int SendPlcRollerQualified(int _qualified);
public:

};