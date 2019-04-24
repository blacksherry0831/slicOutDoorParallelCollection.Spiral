//#include "stdafx.h"
#include "QtThreadSocketClientRoller.hpp"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
QtThreadSocketClientRoller::QtThreadSocketClientRoller(QObject *_parent) :QtThreadSocketClientPlc(_parent)
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
	this->mRollerStatus = CMD_WORK_FLOW::SJTS_MACHINE_STATUS_ROLLER::RollerPosReady;
	this->emit_status_sjts_roller(this->GetIntoStepStr());
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadSocketClientRoller::emit_roller_ready()
{
	const QString circle_seq = CircleSeq();
	this->mRollerStatus = CMD_WORK_FLOW::SJTS_MACHINE_STATUS_ROLLER::RollerReadyStart;
	this->emit_status_sjts_roller(circle_seq);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadSocketClientRoller::emit_roller_into_inner_ready()
{
	this->mRollerStatus = CMD_WORK_FLOW::SJTS_MACHINE_STATUS_ROLLER::RollerIntoInnerReady;
	this->emit_status_sjts_roller(this->GetIntoStepStr());
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadSocketClientRoller::emit_roller_done()
{
	this->mRollerStatus = CMD_WORK_FLOW::SJTS_MACHINE_STATUS_ROLLER::RollerDoneEnd;
	this->emit_status_sjts_roller();
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
QString  QtThreadSocketClientRoller::CircleSeq()
{
	const uint time_t = QDateTime::currentDateTime().toTime_t();

	return QString::number(time_t);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadSocketClientRoller::emit_roller_abort()
{
	this->mRollerStatus = CMD_WORK_FLOW::SJTS_MACHINE_STATUS_ROLLER::MachineAbort;
	this->emit_status_sjts_roller();
	this->M_THREAD_RUN = FALSE;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadSocketClientRoller::do_sjts_roller_ready()
{
	sendPlcResp(CMD_CTRL::CMD_TYPE_02_RESP::CT_OK);
	
	this->InitIntoStep();
	this->emit_roller_ready();

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadSocketClientRoller::do_sjts_roller_pos_ready()
{
	sendPlcResp(CMD_CTRL::CMD_TYPE_02_RESP::CT_OK);

	this->emit_roller_pos_ready();
	SendPlcIntoInterEx();

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/

