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
QtThreadFlowCtrlBase::QtThreadFlowCtrlBase(QObject *parent):QtThreadSocketClientSig(parent)
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
void QtThreadFlowCtrlBase::setWorkFlowDones(int _work_flow,int _result)
{

	this->mWorkFlowDoneClientThreads = _work_flow;
	this->mWorkFlowDoneClientThreadsResult = _result;

#if _DEBUG

	if (this->mWorkFlowDoneClientThreads) {
		
				printf_event("WORK FLOW","mWorkFlowDones==TRUE");

				if (this->mWorkFlowDoneClientThreadsResult == CMD_CTRL::BodyRollerQualified::Qualified) {
					printf_event("WORK FLOW RESULT", "==Qualified");
				}
				else
				{
					printf_event("WORK FLOW RESULT", "==Unqualified");
				}

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
	this->setWorkFlowDones(0,CMD_CTRL::CMD_TYPE_02_RESP::CT_ERROR);

	do
	{
		if (this->getClientSessionCount() >= CLIENT_SESSION_TOTAL)
		{
			return TRUE;
		}
		else {
			this->SleepMy(1000);
			emit status_sjts(CMD_CTRL::SJTS_MACHINE_STATUS::ClientError,"NoClient");
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
CMD_CTRL::BodyRollerQualified QtThreadFlowCtrlBase::wait4ImgProcessResult()
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
		//time out error
		qualified_t = CMD_CTRL::BodyRollerQualified::UnQualified;//time out 
	}
	else
	{
		qualified_t = this->getWorkFLowQualified();
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
	this->setWorkFlowDones(0,CMD_CTRL::CMD_TYPE_02_RESP::CT_ERROR);

	emit status_sjts(CMD_CTRL::SJTS_MACHINE_STATUS::RollerDone,"");
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int QtThreadFlowCtrlBase::getWorkFLowResult()
{
	return this->mWorkFlowDoneClientThreadsResult;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
CMD_CTRL::BodyRollerQualified QtThreadFlowCtrlBase::getWorkFLowQualified()
{
	CMD_CTRL::BodyRollerQualified  qualified_t =(CMD_CTRL::BodyRollerQualified) this->getWorkFLowResult();
		
	Q_ASSERT(qualified_t== CMD_CTRL::BodyRollerQualified::Qualified || qualified_t == CMD_CTRL::BodyRollerQualified::UnQualified);
		
	return qualified_t;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
CMD_CTRL::SJTS_MACHINE_STATUS QtThreadFlowCtrlBase::BodyRollerQualified_2_SJTS_MACHINE_STATUS(CMD_CTRL::BodyRollerQualified _qualified)
{

	CMD_CTRL::SJTS_MACHINE_STATUS sjts_status_t;

	if (_qualified == CMD_CTRL::BodyRollerQualified::Qualified) {
		sjts_status_t = CMD_CTRL::SJTS_MACHINE_STATUS::RollerDoneQualified;
	}
	else if (_qualified == CMD_CTRL::BodyRollerQualified::UnQualified) {
		sjts_status_t = CMD_CTRL::SJTS_MACHINE_STATUS::RollerDoneUnqualified;
	}
	else
	{
		Q_ASSERT(0);
	}
	return sjts_status_t;

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void  QtThreadFlowCtrlBase::emit_roller_done_qualified(CMD_CTRL::BodyRollerQualified _qualified)
{

	CMD_CTRL::SJTS_MACHINE_STATUS sjts_status_t = this->BodyRollerQualified_2_SJTS_MACHINE_STATUS(_qualified);
	
	emit status_sjts(sjts_status_t,"");
	
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
QString  QtThreadFlowCtrlBase::CircleSeq()
{
	const uint time_t = QDateTime::currentDateTime().toTime_t();
		
	return QString::number(time_t);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int QtThreadFlowCtrlBase::sendPlcResp(CMD_CTRL::CMD_TYPE_02_RESP _type)
{
	int result_t = this->m_socket->SendPlcResp(CMD_CTRL::CMD_TYPE_02_RESP::CT_OK);

	return result_t;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void  QtThreadFlowCtrlBase::emit_roller_ready()
{
	emit status_sjts(CMD_CTRL::SJTS_MACHINE_STATUS::RoolerReady, CircleSeq());
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void  QtThreadFlowCtrlBase::process_machine_error(QString _msg)
{
	this->emit_machine_error(_msg);
	this->M_THREAD_RUN = false;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void  QtThreadFlowCtrlBase::emit_machine_error(QString _msg)
{
	emit status_sjts(CMD_CTRL::SJTS_MACHINE_STATUS::SjtsMachineInnerError,_msg);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
