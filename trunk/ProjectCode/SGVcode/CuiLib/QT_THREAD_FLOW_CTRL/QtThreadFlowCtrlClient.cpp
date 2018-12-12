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
	this->setRemoteServer();
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
void QtThreadFlowCtrlClient::setRemoteServer()
{

	mIpAddr = IPC_GUI_ADDR;

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadFlowCtrlClient::setLocalServer()
{

	mIpAddr = "127.0.0.1";

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadFlowCtrlClient::run_socket_work()
{
	QSharedPointer<CMD_CTRL> cmd_t = QSharedPointer<CMD_CTRL>(new CMD_CTRL());
	
	cmd_t->SetCmdLocal();
	
	if (this->Read_1_cmd_process_hearbeat(cmd_t)) {
		
		Q_ASSERT(cmd_t->IsCmdLocal());

		if (cmd_t->IsCmdLocal()) {

				if (cmd_t->isHeartbeatCmd()) {
					cmd_t.clear();
				}else{
					this->emit_work_flow_status_sjts(cmd_t);//emit work fflow
					
					QtThreadClientCtrl::SetCmd(cmd_t);//set work flow 2 

					if (cmd_t->IsThisCmd00(CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_STOP)) {					
							this->wait_4_inner_done();					
					}
					
					this->Send_1_cmd_resp(CMD_CTRL::CMD_TYPE_02_RESP::CT_OK);
				}

		}

		
	}

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int QtThreadFlowCtrlClient::emit_work_flow_status_sjts(QSharedPointer<CMD_CTRL> _cmd)
{
	int sig_t = _cmd->GetCmd00();
	
	emit status_sjts(sig_t);


	if (_cmd->IsThisCmd00(CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_START)) {
		printf_event("---------------------------------------------");
		printf_event("@ CMD_CTRL::CMD_TYPE_LOCAL::", "CT_FPGA_START");
		return TRUE;

	}
	else if (_cmd->IsThisCmd00(CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_START_00)) {

		printf_event("@ CMD_CTRL::CMD_TYPE_LOCAL::", "CT_FPGA_START_00");
		return TRUE;

	}
	else if (_cmd->IsThisCmd00(CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_STOP_00)) {

		printf_event("@ CMD_CTRL::CMD_TYPE_LOCAL::", "CT_FPGA_STOP_00");
		return TRUE;

	}
	else if (_cmd->IsThisCmd00(CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_START_01)) {

		printf_event("@ CMD_CTRL::CMD_TYPE_LOCAL::", "CT_FPGA_START_01");
		return TRUE;

	}
	else if (_cmd->IsThisCmd00(CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_STOP_01)) {

		printf_event("@ CMD_CTRL::CMD_TYPE_LOCAL::", "CT_FPGA_STOP_01");
		return TRUE;

	}
	else if (_cmd->IsThisCmd00(CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_STOP)) {

		printf_event("@ CMD_CTRL::CMD_TYPE_LOCAL::", "CT_FPGA_STOP");
		printf_event("---------------------------------------------");
		return TRUE;

	}
	else {

		Q_ASSERT(FALSE);
	}

	return FALSE;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
#if _DEBUG
void QtThreadFlowCtrlClient::run()
{
	this->before_enter_thread();
	this->enter_thread();

	this->init_socket_in_thread();

	while (M_THREAD_RUN) {

		this->connect2ServerIfNoConnected();

		while (socket_thread_run_condition()) {

			this->run_socket_work();

		}

		this->close_destory_socket_4_server();

	}

	this->destory_socket_in_thread();

	this->exit_thread();
	this->after_exit_thread();
}
#endif
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int QtThreadFlowCtrlClient::wait_4_inner_done()
{
	int wait_time = 10 * 1000;
	int result_t = FALSE;
	TimeMeasure tm;

	tm.start(__func__);

	do
	{

		if (this->IsImgProcessDone())
		{
			result_t=TRUE;
			break;
		}

		this->SleepMy(100);
		wait_time -= 100;
		this->SendHeartBeatCmdReadResp5s();

	} while (socket_thread_run_condition() &&wait_time>0);

	tm.stop();

	return  result_t;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int  QtThreadFlowCtrlClient::IsImgProcessDone()
{
	if (ChannelsData::getInstance()->QueueSize()>0) {
		return FALSE;
	}

	return TRUE;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/