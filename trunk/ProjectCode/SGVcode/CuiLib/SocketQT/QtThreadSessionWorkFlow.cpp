//#include "stdafx.h"
#include "QtThreadSessionWorkFlow.hpp"
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
QtThreadSessionWorkFlow::QtThreadSessionWorkFlow(qintptr _socket):QtThreadSession(_socket)
{
	mWorkFlowStart = FALSE;
	mWorkFlowEnd = FALSE;
	mWorkFlowStep = 0;
	mWorkFlowResult=-1;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
QtThreadSessionWorkFlow::~QtThreadSessionWorkFlow(void)
{
	qDebug() << __func__;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int QtThreadSessionWorkFlow::IsWorkFlowDone()
{

	return (mWorkFlowStart == TRUE) && (mWorkFlowEnd == TRUE);

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int QtThreadSessionWorkFlow::getWorkFlowResult()
{
	return mWorkFlowResult;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void  QtThreadSessionWorkFlow::beforeSendMsg()
{

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
