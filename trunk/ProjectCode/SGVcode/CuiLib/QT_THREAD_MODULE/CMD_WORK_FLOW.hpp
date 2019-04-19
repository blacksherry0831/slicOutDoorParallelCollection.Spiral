#pragma once
/*-----------------------------------*/
/**
*
*/
/*-----------------------------------*/
#include "cpp_stl.h"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
class CMD_WORK_FLOW
{
private:
	CMD_WORK_FLOW();
	~CMD_WORK_FLOW();
public:
	
	enum SJTS_MACHINE_STATUS_ROLLER
	{
		RollerInit=0,
		RollerReadyStart = 1,
		RollerPosReady,
		RollerIntoInnerReady,
		RollerDoneEnd,
		RollerDoneUnQ,
		RollerDoneQ,
		MachineAbort,
	};

	enum SJTS_MACHINE_STATUS_MOTOR
	{
		MotorStart00=10,
		MotorStop00,
		MotorStart01,
		MotorStop01,

		SerialPortOpen = 20,
		SerialPortError ,
	};

	enum SJTS_MACHINE_STATUS_SERVER
	{
		NoClient = 30,		
	};

};