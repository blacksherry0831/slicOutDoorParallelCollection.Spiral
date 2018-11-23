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
class CMD_CTRL_DATA_ARM_FPGA
{
public:
	enum WorkMode
	{
		WM_SIZE_FULL = 0x80,
		WM_SIZE_CUT = 0x40,
		WM_ORG_IMG = 0x20,
		WM_DIFF_IMG = 0x10,
		RESP = 0x01
	};
public:
	CMD_CTRL_DATA_ARM_FPGA();
	~CMD_CTRL_DATA_ARM_FPGA();
public:



};