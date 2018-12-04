//#include "stdafx.h"
#include "QtThreadFlowCtrlLocal.hpp"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/


/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
QtThreadFlowCtrlLocal::QtThreadFlowCtrlLocal(QObject *parent):QtThreadBase(parent)
{


	
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
QtThreadFlowCtrlLocal::~QtThreadFlowCtrlLocal(void)
{
	
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/

/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/

/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/

/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadFlowCtrlLocal::run()
{
	this->SleepMy(2000);
	
	while (M_THREAD_RUN)
	{
		emit status_sjts(CMD_CTRL::SJTS_MACHINE_STATUS::RoolerReady);
		this->SleepMy(2000);

		{
				emit status_sjts(CMD_CTRL::SJTS_MACHINE_STATUS::StepMotorStart00);
				this->SleepMy(15*1000);
				emit status_sjts(CMD_CTRL::SJTS_MACHINE_STATUS::StepMotorStop00);
				this->SleepMy(2000);
		}
	
		{
				emit status_sjts(CMD_CTRL::SJTS_MACHINE_STATUS::StepMotorStart01);
				do { this->SleepMy(15 * 1000); } while (M_THREAD_RUN && 0);
				emit status_sjts(CMD_CTRL::SJTS_MACHINE_STATUS::StepMotorStop01);
				this->SleepMy(2000);
		}

		emit status_sjts(CMD_CTRL::SJTS_MACHINE_STATUS::RollerDone);
		this->SleepMy(2000);
		emit status_sjts(CMD_CTRL::SJTS_MACHINE_STATUS::RollerDoneQualified);
		this->SleepMy(2000);
	}

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/

/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/

/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/

