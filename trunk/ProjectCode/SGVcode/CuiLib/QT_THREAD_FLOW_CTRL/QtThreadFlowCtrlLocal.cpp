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
	this->SleepMy(5000);
	
	while (M_THREAD_RUN)
	{
		emit status_sjts(CMD_CTRL::SJTS_MACHINE_STATUS::RoolerReady);
		

		{
				emit status_sjts(CMD_CTRL::SJTS_MACHINE_STATUS::StepMotorStart00);
				this->SleepMy(15*1000);
				emit status_sjts(CMD_CTRL::SJTS_MACHINE_STATUS::StepMotorStop00);
				this->SleepMy(100);
		}
	
		{
				emit status_sjts(CMD_CTRL::SJTS_MACHINE_STATUS::StepMotorStart01);
				while (M_THREAD_RUN) this->SleepMy(100);
				emit status_sjts(CMD_CTRL::SJTS_MACHINE_STATUS::StepMotorStop01);
				this->SleepMy(100);
		}

		emit status_sjts(CMD_CTRL::SJTS_MACHINE_STATUS::RollerDone);
		this->SleepMy(100);
		emit status_sjts(CMD_CTRL::SJTS_MACHINE_STATUS::RollerDoneQualified);
		this->SleepMy(100);
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

