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

						while (M_THREAD_RUN && mSocketConnected) {

							QSharedPointer<CMD_CTRL> cmd_t = QSharedPointer<CMD_CTRL>(new CMD_CTRL());
							
							cmd_t->SetCmdLocal();


								if (Read_1_cmd(cmd_t)){
									if (cmd_t->IsCmdLocal()){
										if (cmd_t->isHeartbeatCmd()){
											cmd_t.clear();
										}else{
											QtThreadClientCtrl::SetCmd(cmd_t);
										}
									}
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