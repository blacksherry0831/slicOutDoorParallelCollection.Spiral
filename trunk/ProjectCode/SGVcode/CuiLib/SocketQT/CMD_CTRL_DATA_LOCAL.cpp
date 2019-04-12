//#include "stdafx.h"
#include "CMD_CTRL_DATA_LOCAL.hpp"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
CMD_CTRL_DATA_LOCAL::CMD_CTRL_DATA_LOCAL()
{
	this->SetCmdRemote();
}
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
CMD_CTRL_DATA_LOCAL::~CMD_CTRL_DATA_LOCAL()
{
		
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int CMD_CTRL_DATA_LOCAL::IsCmdRemote()
{
	return mCmdRemoteFlag;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int CMD_CTRL_DATA_LOCAL::IsCmdLocal()
{
	return mCmdRemoteFlag == FALSE;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int CMD_CTRL_DATA_LOCAL::SetCmdRemote()
{
	return mCmdRemoteFlag = TRUE;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int CMD_CTRL_DATA_LOCAL::SetCmdLocal()
{
	return mCmdRemoteFlag = FALSE;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/