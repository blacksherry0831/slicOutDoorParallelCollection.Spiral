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
#include "SocketQT/QtThreadClientCtrl.hpp"
#include "SocketQT/QtThreadSocketClient.hpp"
/*-------------------------------------*/
#include "SerialPort/BE_1105_Dirver.hpp"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
class QtThreadStepMotor :public QtThreadBase
{

public:
	QtThreadStepMotor();
	~QtThreadStepMotor(void);
protected:
	std::string mCurrentBord;
	QSharedPointer<BE_1105_Driver>	 mBE_1105;
	QSharedPointer <QtThreadSocketClient> mCmdCtrlPipe;

private:
	static const int TIME_GAP;
	static const int BLOCK_IN_STEP02;
	int RUN_MODE;
protected:
	void StepMotorRun();
public:
	void SetCmdCtrlPipe(QSharedPointer <QtThreadSocketClient> _cmdCtrlPipe);
	int IsCmdCtrlPipeOK();
	void Wait4ImgProcess(int _time);
	void blockInStep02();
protected: 
	virtual void run();
	void run_no_step_motor();
	void run_normal();
public:
	void startServer();
	void closeServer();
public:
	QtThreadStepMotor* SetBordIPaddr(QString _ipAddr);

};