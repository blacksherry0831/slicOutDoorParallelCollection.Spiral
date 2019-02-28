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
#include "QT_THREAD_FLOW_CTRL/QtThreadFlowCtrlBase.hpp"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
class QtThreadStepMotor :public QtThreadFlowCtrlBase
{

public:
	QtThreadStepMotor();
	~QtThreadStepMotor(void);
protected:
	std::string mCurrentBord;
	QSharedPointer<BE_1105_Driver>	 mBE_1105;
	QSharedPointer <QtThreadSocketClient> mCmdCtrlPipe;
private:
	bool mBlock;
private:
	static const int TIME_GAP;
	static const int BLOCK_IN_STEP02;
	int RUN_MODE;
	int mBe1105RunDir;
protected:
	void StepMotorRun();
public:
	void SetCmdCtrlPipe(QSharedPointer <QtThreadSocketClient> _cmdCtrlPipe);
	int IsCmdCtrlPipeOK();
	void Wait4ImgProcess(int _time);
	void blockInStep02();
public:
	void hardware_init();
	
	void hardware_roller_run();

	void hardware_roller_run_base();
	void hardware_roller_run_base_x5();
protected: 
	virtual void run();
	void run_no_step_motor();
	void run_normal();
public:
	void  init_serial_port(QSharedPointer<BE_1105_Driver>	 _be_1105);
public:
	void startServer();
	void closeServer();
	void closeServerAsync();
	void closeServerSync();
	void SetBlock(bool _block);
public:
	QtThreadStepMotor* SetBordIPaddr(QString _ipAddr);

};