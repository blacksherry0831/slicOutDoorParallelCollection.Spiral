//#include "stdafx.h"
#include "QtThreadPlcSocket.hpp"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
#include "SerialPort/BE_1105_Dirver.hpp"
/*-------------------------------------*/
/**
*
PLC
59 6A 6B 6A  00 00  00 00 00 00   01 11  00 00  73 10  00 00  00 00 50 ready
59 6A 6B 6A 00 00 00 00 00 00  01 11 00 00 72 00     00 00   00 00 50工控机收到准备好
59 6A 6B 6A 00 00 00 00 00 00  01 11 00 00 63 20     00 00   00 01 60工控机发送位置
59 6A 6B 6A 00 00 00 00 00 00  01 11 00 00 63 20     00 00   00 02 63
59 6A 6B 6A 00 00 00 00 00 00  01 11 00 00 63 10     00 00   00 00 51工控机发送不合格判定
59 6A 6B 6A 00 00 00 00 00 00  01 11 00 00 63 10     00 00   00 01 50
###########################################################################
59 6A 6B 6A 00 00 00 00 00 00  01 11 00 00 72 00     02 00   00 00   72 resp
59 6A 6B 6A 00 00 00 00 00 00  01 11 00 00 63 20     02 00   01 00   42 into internal
59 6A 6B 6A 00 00 00 00 00 00  01 11 00 00 63 20     02 00   02 00   42 into internal
59 6A 6B 6A 00 00 00 00 00 00  01 11 00 00 63 10     02 00   01 00   72 ok合格

PLC send
IPC send
【2018-04-18 20:16:47收到数据(HEX),127.0.0.1:43557 Len:21resp】:59 6A 6B 6A 00 00 00 00 00 00 01 11 00 00 72 00 02 00 00 00 52
【2018-04-18 20:16:47收到数据(HEX),127.0.0.1:43557 Len:21into 5】:59 6A 6B 6A 00 00 00 00 00 00 01 11 00 00 63 20 02 00 05 00 66
【2018-04-18 20:17:43收到数据(HEX),127.0.0.1:43557 Len:21into 1】:59 6A 6B 6A 00 00 00 00 00 00 01 11 00 00 63 20 02 00 01 00 62
【2018-04-18 20:18:10收到数据(HEX),127.0.0.1:43557 Len:21 ok】:59 6A 6B 6A 00 00 00 00 00 00 01 11 00 00 63 10 02 00 01 00 52

*
*/
/*-------------------------------------*/


/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
QtThreadPlcSocket::QtThreadPlcSocket(QObject *parent ):QtThreadFlowCtrlBase(parent)
{
	
#if 1
	mIpAddr = PLC_ADDR;
#else
	mIpAddr = "127.0.0.1";
#endif // 0

	mPort = 2001;
	
	this->InitIntoStep();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
QtThreadPlcSocket::~QtThreadPlcSocket(void)
{
	qDebug() << this->objectName()<< "is Release ! ";
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int QtThreadPlcSocket::Read_1_plc_cmd_process_hearbeat(QSharedPointer<CMD_CTRL> _cmd)
{

	if (Read_1_cmd_process_hearbeat(_cmd)) {
			this->print_cmd(_cmd);
			this->process_fatal_error(_cmd);
	}
	else {
			this->print_socket_connected();	
	}

	return this->GetSocketConnected();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int QtThreadPlcSocket::SendPlcIntoInterEx()
{
	const int IN_STEP = this->GetIntoStep();
		
	return SendPlcIntoInter(IN_STEP);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
QString QtThreadPlcSocket::GetIntoStepStr()
{
	return QString::number(this->mCurrentStep);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int     QtThreadPlcSocket::GetIntoStep()
{
	Q_ASSERT(this->mCurrentStep == 1 || this->mCurrentStep == 2);

	if (this->mCurrentStep>2)
	{
		this->mCurrentStep = 2;
	}

	if (this->mCurrentStep<0)
	{
		this->mCurrentStep = 0;
	}
	
	return this->mCurrentStep;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void    QtThreadPlcSocket::IncIntoStep()
{
	this->mCurrentStep++;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadPlcSocket::InitIntoStep()
{
	this->mCurrentStep=1;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadPlcSocket::print_socket_connected()
{
	if (GetSocketConnected() == 0) {
		printf_event("EVENT", "Socket Error !");
	}
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void  QtThreadPlcSocket::process_fatal_error_msg(QString _msg)
{
	this->process_machine_error(_msg);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadPlcSocket::do_run_work()
{

	

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadPlcSocket::run_socket_work()
{

	while (socket_thread_run_condition()) {
		this->do_run_work();
	}

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadPlcSocket::process_plc_cmd(QSharedPointer<CMD_CTRL> _cmd)
{

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int  QtThreadPlcSocket::process_plc_cmd_async(QSharedPointer<CMD_CTRL> _cmd)
{
	return 0;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/