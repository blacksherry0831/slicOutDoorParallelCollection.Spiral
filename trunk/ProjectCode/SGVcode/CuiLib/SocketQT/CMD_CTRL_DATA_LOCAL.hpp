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
		
public:
	int IsCmdRemote();
	int IsCmdLocal();
	int SetCmdRemote();
	int SetCmdLocal();

protected:
	int mCmdRemoteFlag;

	

};