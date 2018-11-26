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
QtThreadFlowCtrlClient::QtThreadFlowCtrlClient():QtThreadSocketClient()
{
	
#if 1
	mIpAddr = IPC_GUI_ADDR;
#else
	mIpAddr = "127.0.0.1";
#endif // 0

	mPort = TCP_PORT_IPC_FLOW_CTRL;
		

	mThreadName = __func__;
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
void QtThreadFlowCtrlClient::run()
{
	this->init_socket();	

	this->emit_status_message(mStatusMessage = "Thread>> Ctrl Thread Start");

	/*-----------------------------*/
	while (M_THREAD_RUN) {

			this->connect2ServerIfNoConnected();

						while (M_THREAD_RUN) {

							QSharedPointer<CMD_CTRL> cmd_t = QSharedPointer<CMD_CTRL>(new CMD_CTRL());
							
							cmd_t->SetCmdLocal();

							this->m_socket->Read_1_cmd(cmd_t.data());

							if (cmd_t->isHeartbeatCmd()){

							}else if (cmd_t->IsCmdLocal()) {
								
								QtThreadClientCtrl::SetCmd(cmd_t);
							
							}else{


							}

						

			
						}

			this->closeSocket4Server();

	}
	/*-----------------------------*/
	
	this->emit_status_message(mStatusMessage = "Thread>>  shutdown");

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/