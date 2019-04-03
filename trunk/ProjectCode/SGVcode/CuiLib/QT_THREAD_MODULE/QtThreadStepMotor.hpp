#pragma once
/*-------------------------------------*/
#include "cpp_stl.h"
/*-------------------------------------*/
#include "qt_all.h"
/*-------------------------------------*/
#include "SocketQT/conf_ip.h"
/*-------------------------------------*/
#include "SocketQT/QtThreadBase.hpp"
#include "SocketQT/CMD_CTRL.hpp"
#include "SocketQT/QtThreadSocketClient.hpp"
/*-------------------------------------*/
#include "QT_THREAD_MODULE/QtThreadClientCtrl.hpp"
/*-------------------------------------*/
#include "SerialPort/BE_1105_Dirver.hpp"
/*-------------------------------------*/
#include "QT_THREAD_FLOW_CTRL/QtThreadFlowCtrlLocal.hpp"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
class QtThreadStepMotor :public QtThreadFlowCtrlLocal
{

public:
	QtThreadStepMotor();
	~QtThreadStepMotor(void);
protected:
	std::string mCurrentBord;
	QSharedPointer<BE_1105_Driver>	 mBE_1105;
private:
	static const int TIME_GAP;
	static const int TIME_15S;
	int RUN_MODE;
	int mBe1105RunDir;
protected:
	void StepMotorRun();
public:
	void Wait4ImgProcess(int _time);
public:
	void hardware_init();
	int  hardware_init_status();
	void hardware_roller_run();
	void hardware_roller_run_base();
	void hardware_roller_run_base_x5();
public:
	void  init_serial_port(QSharedPointer<BE_1105_Driver>	 _be_1105);
	int  init_serial_port_once(QSharedPointer<BE_1105_Driver>	 _be_1105);
public:
	void startServer();
	void closeServer();
	void closeServerAsync();
	void closeServerSync();
public:
	QtThreadStepMotor* SetBordIPaddr(QString _ipAddr);

};