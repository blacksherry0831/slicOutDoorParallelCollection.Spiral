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