#pragma once

#include "cpp_stl.h"

#include "qt_all.h"

#include "CMD_CTRL.hpp"

#include "SerialPort/BE_1105_Dirver.hpp"

#include "QtTcpClientBase.hpp"
/*-------------------------------------*/
#define HEART_BEAT_FREQUENCY (5)
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
#define USE_PLC_SOCKET (1)
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
class QtTcpClient :public QtTcpClientBase
{
	Q_OBJECT
public:
	enum SOCKET_STATUS {
		Error=-1,
		DisConnected = 0,
		Connected = 1,
		ReadWriteTimeOut =1,
		ErrorOccur=1,
		
	};
protected:
	int  MAX_MSECS;
public:
	explicit QtTcpClient(QObject *parent = Q_NULLPTR);
	~QtTcpClient(void);

public:
	int Send_1_cmd(CMD_CTRL *_cmd);
	int Read_1_cmd(CMD_CTRL *_cmd);
	int Read_1_cmd_fast(CMD_CTRL *_cmd);
	int Read_nSize_2_body(CMD_CTRL *_cmd);
	int getByteTcpRead();
public:
	int Send_Start_CMD(CMD_CTRL::CMD_TYPE_02_C _type_c, CMD_CTRL::WorkMode _wm,uint _circle_seq);
	int SendHearbeatCmd(int _need_resp);
public:

#if USE_PLC_SOCKET
	int SendPlcResp(CMD_CTRL::CMD_TYPE_02_RESP _type);
	int SendPlcIntoInter(int _step);
	int SendPlcRollerQualified(int _qualified);
#endif

public:

};