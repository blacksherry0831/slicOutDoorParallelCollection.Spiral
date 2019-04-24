#pragma once
/*-------------------------------------*/
#include "cpp_stl.h"
/*-------------------------------------*/
#include "qt_all.h"
/*-------------------------------------*/
#include "QT_SDK_LIB/QtThreadBase.hpp"
/*-------------------------------------*/
#include "SocketQT/conf_ip.h"
/*-------------------------------------*/
#include "SocketQT/CMD_CTRL.hpp"
#include "SocketQT/QtThreadSocketClient.hpp"
/*-------------------------------------*/
#include "QtThreadClientCtrl.hpp"
#include "StepMotorSjts.hpp"
/*-------------------------------------*/
#include "SerialPort/BE_1105_Dirver.hpp"
/*-------------------------------------*/
#include "QT_THREAD_FLOW_CTRL/QtThreadFlowCtrlBase.hpp"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
class QtThreadStepMotor :public QtThreadFlowCtrlBase,public StepMotorSjts
{
	Q_OBJECT
public:
	QtThreadStepMotor();
	~QtThreadStepMotor(void);
signals:
	void status_sjts_motor(int, QString);
protected:
	std::string mCurrentBord;
	
private:
	static const int TIME_GAP;
	static const int TIME_15S;
	int RUN_MODE;

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
	void  init_serial_port();
	void  emit_init_serial_status(int _isOpen);
public:
	void startServer();
	void closeServer();
	void closeServerAsync();
	void closeServerSync();
public:
};