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
	int WriteMy(char* _data, int _size);

public:
	void Send_Start_CMD(int _type);

	void SendPlcResp(int _type);
	void SendPlcIntoInter(int _step);
	void SendPlcRollerQualified(int _qualified);

	void Send_1_cmd(CMD_CTRL *_cmd);
	int Read_1_cmd(CMD_CTRL *_cmd);

	int IsSocketAlive();

};