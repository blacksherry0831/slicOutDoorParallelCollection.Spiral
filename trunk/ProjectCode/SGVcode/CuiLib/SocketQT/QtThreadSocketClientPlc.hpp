#pragma once
/*-------------------------------------*/
#include "cpp_stl.h"
/*-------------------------------------*/
#include "QT_SDK_LIB/QtThreadBase.hpp"
/*-------------------------------------*/
#include "QtTcpClient.hpp"
#include "CMD_CTRL_Q.hpp"
#include "QtThreadSocketClient.hpp"
/*-------------------------------------*/
#include "QT_THREAD_MODULE/IPlcSjts.hpp"
/*-------------------------------------*/
#include "SocketQT/conf_ip.h"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
class QtThreadSocketClientPlc :public  QtThreadSocketClient, public IPlcSjts
{
	Q_OBJECT
public:
	explicit QtThreadSocketClientPlc(QObject *parent = Q_NULLPTR);
	~QtThreadSocketClientPlc(void);
private:
	int mCurrentStep;
protected:
	QString GetIntoStepStr();
	int     GetIntoStep();
	void    IncIntoStep();
	void    InitIntoStep();
protected:
	int sendPlcRollerQualifiedEx(CMD_CTRL::BodyRollerQualified _qualified);
	int sendPlcResp(CMD_CTRL::CMD_TYPE_02_RESP _type);
	int SendPlcIntoInter(int _step);
	int SendPlcIntoInterEx();
protected:
	virtual void print_socket_connected();
protected:
	virtual void process_fatal_error_msg(QString _msg);
public:
	virtual int  Read_1_plc_cmd_process_hearbeat(QSharedPointer<CMD_CTRL> _cmd);
	virtual int  process_plc_cmd_async(QSharedPointer<CMD_CTRL> _cmd);
protected:

};