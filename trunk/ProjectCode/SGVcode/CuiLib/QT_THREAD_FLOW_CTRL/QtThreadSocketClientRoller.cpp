//#include "stdafx.h"
#include "QtThreadSocketClientRoller.hpp"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
QtThreadSocketClientRoller::QtThreadSocketClientRoller(QObject *_parent) :QtThreadSocketClient(_parent)
{
	this->mRollerStatus = CMD_WORK_FLOW::SJTS_MACHINE_STATUS_ROLLER::RollerInit;
	this->mEvent = "Roller";
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadSocketClientRoller::emit_roller_pos_ready()
{
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadSocketClientRoller::emit_roller_ready()
{
	
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadSocketClientRoller::emit_roller_into_inner_ready()
{

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadSocketClientRoller::emit_roller_done()
{
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void  QtThreadSocketClientRoller::emit_status_sjts_roller(QString _msg)
{
	emit status_sjts_roller(this->mRollerStatus,_msg);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int QtThreadSocketClientRoller::IsRollerStatus(int _s)
{
	return _s==this->mRollerStatus;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadSocketClientRoller::printf_roller_status(std::string _e)
{

	if (IsRollerStatus(CMD_WORK_FLOW::RollerDoneQ)) {
		printf_event(_e, "Q");
	}
	else	if (IsRollerStatus(CMD_WORK_FLOW::RollerDoneUnQ)) {
		printf_event(_e, "UnQ");
	}
	else {

	}

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/



