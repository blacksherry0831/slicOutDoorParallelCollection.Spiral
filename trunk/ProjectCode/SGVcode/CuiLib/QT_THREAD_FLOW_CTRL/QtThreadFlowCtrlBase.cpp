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
int QtThreadFlowCtrlBase::getWorkFlowDone()
{
	return this->mWorkFlowDone;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadFlowCtrlBase::setWorkFlowDone(int _work_flow)
{
	this->mWorkFlowDone = _work_flow;
#if _DEBUG
	if (this->mWorkFlowDone == TRUE) {
		std::cout <<""<<std::endl;
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
	this->setWorkFlowDone(FALSE);

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

	TimeMeasure tm;
	tm.start(__func__);

	while (this->socket_thread_run_condition())
	{

		if (this->getWorkFlowDone() == TRUE) {
			break;
		}
		else
		{
			this->SleepMy();
		}

	}

	tm.stop();

	this->setWorkFlowDone(FALSE);
	
	return qualified_t;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/