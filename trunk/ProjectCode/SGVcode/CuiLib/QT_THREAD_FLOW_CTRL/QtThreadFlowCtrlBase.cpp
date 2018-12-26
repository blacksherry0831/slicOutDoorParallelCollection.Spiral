//#include "stdafx.h"
#include "QtThreadFlowCtrlBase.hpp"
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
QtThreadFlowCtrlBase::QtThreadFlowCtrlBase(QObject *parent): QtThreadSocketClient(parent)
{


	
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
QtThreadFlowCtrlBase::~QtThreadFlowCtrlBase(void)
{
	
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int QtThreadFlowCtrlBase::getWorkFlowDones()
{
	return this->mWorkFlowDoneClientThreads;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadFlowCtrlBase::setWorkFlowDones(int _work_flow)
{

	this->mWorkFlowDoneClientThreads = _work_flow;

#if _DEBUG

	if (this->mWorkFlowDoneClientThreads) {
		printf_event("WORK FLOW","mWorkFlowDones==TRUE");
	}

#endif // _DEBUG

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadFlowCtrlBase::setClientSessionCount(int _count)
{
	this->mClientSessionCount = _count;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int QtThreadFlowCtrlBase::wait4WorkFlowStart()
{
	this->setWorkFlowDones(0);

	do
	{
		if (this->getClientSessionCount() >= CLIENT_SESSION_TOTAL)
		{
			return TRUE;
		}
		else {
			this->SleepMy();
		}

	} while (socket_thread_run_condition());

	return FALSE;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int QtThreadFlowCtrlBase::getClientSessionCount()
{
	return this->mClientSessionCount;
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
int QtThreadFlowCtrlBase::wait4ImgProcessResult()
{
	CMD_CTRL::BodyRollerQualified qualified_t = CMD_CTRL::BodyRollerQualified::Qualified;
	int WAIT_TIME_MS=15*1000;
	const int SLEEP_TIME = 100;
	TimeMeasure tm(__func__);
	
	while (this->socket_thread_run_condition() && WAIT_TIME_MS>0)
	{

		if (this->getWorkFlowDones()) {
			break;
		}
		else
		{
			this->SleepMy(SLEEP_TIME);
			WAIT_TIME_MS -= SLEEP_TIME;
		}

	}
	
	if (WAIT_TIME_MS<=0)
	{
		qualified_t = CMD_CTRL::BodyRollerQualified::UnQualified;//time out 
	}

	return qualified_t;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadFlowCtrlBase::emit_roller_done()
{
	this->setWorkFlowDones(0);

	emit status_sjts(CMD_CTRL::SJTS_MACHINE_STATUS::RollerDone);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/