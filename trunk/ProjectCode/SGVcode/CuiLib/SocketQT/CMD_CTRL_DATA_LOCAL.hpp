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

	enum CMD_TYPE_LOCAL {

		CT_FPGA_START_00,
		CT_FPGA_START_01,

		CT_FPGA_STOP_00,
		CT_FPGA_STOP_01,

		CT_FPGA_START,
		CT_FPGA_STOP

	};

	enum CMD_TYPE_LOCAL_02 {

		CT_ROLLER_Q = 0x10
	};

public:
	int IsCmdRemote();
	int IsCmdLocal();
	int SetCmdRemote();
	int SetCmdLocal();

protected:
	int mCmdRemoteFlag;

	

};