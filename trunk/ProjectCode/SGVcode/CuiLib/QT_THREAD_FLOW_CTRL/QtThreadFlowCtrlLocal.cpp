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

	mBlock = true;
	
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
	const int TIME_INTERVAL = 5000;
	const int TIME_VIDEO = 20*1000;
	while (M_THREAD_RUN)
	{
		this->SleepMy(TIME_INTERVAL);
		this->wait4WorkFlowStart();
		
		emit status_sjts(CMD_CTRL::SJTS_MACHINE_STATUS::RoolerReady);
		this->SleepMy(TIME_INTERVAL);

		{
				emit status_sjts(CMD_CTRL::SJTS_MACHINE_STATUS::StepMotorStart00);
				this->SleepMy(TIME_VIDEO);
				emit status_sjts(CMD_CTRL::SJTS_MACHINE_STATUS::StepMotorStop00);
				this->SleepMy(TIME_INTERVAL);
		}
	
		{
				emit status_sjts(CMD_CTRL::SJTS_MACHINE_STATUS::StepMotorStart01);
				do {
					this->SleepMy(TIME_VIDEO); 
				} while (M_THREAD_RUN && mBlock);
				emit status_sjts(CMD_CTRL::SJTS_MACHINE_STATUS::StepMotorStop01);
				this->SleepMy(TIME_INTERVAL);
		}

	
		this->emit_roller_done();
		

		int qualified_status_t = this->wait4ImgProcessResult();
		
		if (qualified_status_t)	{
				emit status_sjts(CMD_CTRL::SJTS_MACHINE_STATUS::RollerDoneQualified);
		}else {
				emit status_sjts(CMD_CTRL::SJTS_MACHINE_STATUS::RollerDoneUnqualified);
		}
	
		this->SleepMy(TIME_INTERVAL);
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
void QtThreadFlowCtrlLocal::SetBlock(bool _block)
{
	this->mBlock = _block;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/

