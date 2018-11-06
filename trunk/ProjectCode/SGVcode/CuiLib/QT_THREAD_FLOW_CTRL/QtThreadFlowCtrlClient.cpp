//#include "stdafx.h"
#include "QtThreadFlowCtrlClient.hpp"
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
QtThreadFlowCtrlClient::QtThreadFlowCtrlClient(qintptr p)
{
	m_socket = QSharedPointer<QtTcpClient>(new QtTcpClient());
#if 1
	mIpAddr = PLC_ADDR;
#else
	mIpAddr = "127.0.0.1";
#endif // 0

	mPort = 2001;

	m_socket->moveToThread(this);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
QtThreadFlowCtrlClient::~QtThreadFlowCtrlClient(void)
{
	qDebug() << "QtThreadClient is Release ! ";
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/