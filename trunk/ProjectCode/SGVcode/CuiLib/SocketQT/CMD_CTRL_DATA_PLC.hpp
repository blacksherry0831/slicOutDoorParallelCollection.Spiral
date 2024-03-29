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
class CMD_CTRL_DATA_PLC
{
public:
	CMD_CTRL_DATA_PLC();
	~CMD_CTRL_DATA_PLC();
public:
	/*-----------------------------------*/

	enum CMD_PLC_TYPE {
		CT_STATUS = 's',
		CT_CMD = 'c',
		CT_RESP = 'r',
	};
	
	/*-----------------------------------*/

	enum CMD_PLC_STATUS_TYPE {
		CS_READY_ERROR =0x11,//this is a error 
		CS_READY = 0x10,
		CS_MOVE2POS =0x20,
		CS_ABORT= 0x30,
		CS_POS_READY = 0x40,
		
		CS_MODE_CURRENT_MANUAL = 0x50,
		CS_MODE_CURRENT_AUTO = 0x51,
	};

	/*-----------------------------------*/


};