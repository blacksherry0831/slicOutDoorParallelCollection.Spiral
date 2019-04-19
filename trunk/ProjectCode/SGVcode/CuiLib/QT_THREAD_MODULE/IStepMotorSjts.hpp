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

/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
class IStepMotorSjts
{
	/*---------------------------------------*/
public:
	virtual int     do_StepMotor_sjts_Init();
	virtual int		do_StepMotor_sjts_Run_Once();
	virtual int		do_StepMotor_sjts_Run_Stop();
	virtual int     do_StepMotor_sjts_Run_Fast();
public:
	virtual void  emit_init_serial_status(int _isOpen);
public:
	virtual void  init_serial_port();
	virtual int   init_serial_port_once();
public:
	virtual int StepMotor_IsOpen();
	virtual int StepMotor_OpenDefault();
public:
	virtual void  StepMotor_Server_Start();
	virtual void  StepMotor_Server_Stop();
	/*---------------------------------------*/
};