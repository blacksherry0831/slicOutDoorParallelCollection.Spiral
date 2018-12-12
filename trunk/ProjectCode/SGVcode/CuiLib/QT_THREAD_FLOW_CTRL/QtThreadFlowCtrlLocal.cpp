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
QtThreadFlowCtrlLocal::QtThreadFlowCtrlLocal(QObject *parent):QtThreadFlowCtrlBase(parent)
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
	
	while (M_THREAD_RUN)
	{
		this->wait4WorkFlowStart();
		
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

		this->setWorkFlowDone(FALSE);

		emit status_sjts(CMD_CTRL::SJTS_MACHINE_STATUS::RollerDone);
		

		int qualified_status_t = this->wait4ImgProcessResult();
		
		if (qualified_status_t)	{
				emit status_sjts(CMD_CTRL::SJTS_MACHINE_STATUS::RollerDoneQualified);
		}else {
				emit status_sjts(CMD_CTRL::SJTS_MACHINE_STATUS::RollerDoneUnqualified);
		}
	
		this->SleepMy(2000);
	}

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int QtThreadFlowCtrlLocal::socket_thread_run_condition()
{
	return M_THREAD_RUN ;
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

