#pragma once
/*-----------------------------------*/
/**
*
*/
/*-----------------------------------*/
#include "cpp_stl.h"
/*-----------------------------------*/
/**
*
*/
/*-----------------------------------*/
#include "qt_all.h"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
class CMD_CTRL_DATA_LOCAL
{
public:
	CMD_CTRL_DATA_LOCAL();
	~CMD_CTRL_DATA_LOCAL();
public:
	enum SJTS_MACHINE_STATUS
	{
		RoolerReady = 1,

		StepMotorStart00 = 2,
		StepMotorStop00 = 3,

		StepMotorStart01 = 4,
		StepMotorStop01 = 5,

		RollerDone = 6,


		RollerDoneUnqualified = 7,
		RollerDoneQualified = 8,

		SerialPortIsOpen = 10,
		SerialPortError = 11,

		ClientError = 12,

		SjtsMachineInnerError = 13,

	};

	enum CMD_TYPE_LOCAL {

		CT_FPGA_START=1,

		CT_FPGA_START_00=2,
		CT_FPGA_START_01=3,

		CT_FPGA_STOP_00=4,
		CT_FPGA_STOP_01=5,
		
		CT_FPGA_STOP=6,

	};
	
public:
	int IsCmdRemote();
	int IsCmdLocal();
	int SetCmdRemote();
	int SetCmdLocal();

protected:
	int mCmdRemoteFlag;

	

};