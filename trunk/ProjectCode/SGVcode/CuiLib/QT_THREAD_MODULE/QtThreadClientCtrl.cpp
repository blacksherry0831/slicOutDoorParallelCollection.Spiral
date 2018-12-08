#include "QtThreadClientCtrl.hpp"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
CMD_CTRL_Q QtThreadClientCtrl::cmds;
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
QtThreadClientCtrl::QtThreadClientCtrl()
{
	mHeartBeatFreq=5000;
	initIpPort();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
QtThreadClientCtrl::QtThreadClientCtrl(qintptr p)
{
	initIpPort();
	this->write_ptr(p);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
QtThreadClientCtrl::~QtThreadClientCtrl(void)
{
	qDebug() << "QtThreadClientCtrl is Release ! ";
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadClientCtrl::initIpPort()
{
#if 1
	mIpAddr = BORD_VIDEO_OUT;
#else
	mIpAddr = "127.0.0.1";
#endif // 0

	mPort = TCP_POET_CMD_CTRL;

	mWorkMode = CMD_CTRL::WorkMode( CMD_CTRL::WorkMode::WM_ORG_IMG| CMD_CTRL::WorkMode::WM_SIZE_FULL);

	this->mThreadName = "Ctrl Thread";
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadClientCtrl::SetDataPipe(QSharedPointer <QtThreadSocketClient> _dataPipe)
{
	this->mDataPipe = _dataPipe;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int QtThreadClientCtrl::IsDataPipeOK()
{

	Q_ASSERT(FALSE);
	if (!this->mDataPipe.isNull()) {
	
		return this->mDataPipe->IsSocketConnectedThreadRunning();

	}

	return FALSE;

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
int QtThreadClientCtrl::SendCmdCtrl()
{

	if (cmds.IsValid()) {
		
		QSharedPointer<CMD_CTRL> cmd_ctrl_t=cmds.getCmd();
		
		return	Send_1_cmd(cmd_ctrl_t);
	
	}
			
	return FALSE;	

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int QtThreadClientCtrl::ProcessCmds()
{
	static const int SleepInterval=100;
	static int SleepTimes = 0;
	int result_t=INIT_MY;
	if (cmds.IsValid()) {

				QSharedPointer<CMD_CTRL> cmd_ctrl_t = cmds.getCmd();
		
				if (cmd_ctrl_t->IsCmdRemote()) {
					result_t =this->ProcessRemoteCmds(cmd_ctrl_t);
				}else if (cmd_ctrl_t->IsCmdLocal()) {				
					result_t =this->ProcessLocalCmds(cmd_ctrl_t);
				}else {					
					Q_ASSERT(0);
				}

	}else{

		this->SleepMy(100);

	}
	
	return result_t;

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int QtThreadClientCtrl::ProcessLocalCmds(QSharedPointer<CMD_CTRL> cmd_ctrl_t)
{
	QSharedPointer<CMD_CTRL> cmd_t = QSharedPointer<CMD_CTRL>(new CMD_CTRL());
	int	cmd_status_t =INIT_MY;

	if (cmd_ctrl_t->IsCmdLocal()){
		
					if (cmd_ctrl_t->IsThisCmd00(CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_START)){

						cmd_status_t = SendCmd2FPGA(CMD_CTRL::CMD_TYPE_02_C::CT_START);

					}else if (cmd_ctrl_t->IsThisCmd00(CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_START_00)) {
					
						cmd_status_t = SendCmd2FPGA(CMD_CTRL::CMD_TYPE_02_C::CT_START_00);

					}else if (cmd_ctrl_t->IsThisCmd00(CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_STOP_00)) {
					
						cmd_status_t = SendCmd2FPGA(CMD_CTRL::CMD_TYPE_02_C::CT_STOP_00);

					}else if (cmd_ctrl_t->IsThisCmd00(CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_START_01)) {

						cmd_status_t = SendCmd2FPGA(CMD_CTRL::CMD_TYPE_02_C::CT_START_01);
					
					}else if (cmd_ctrl_t->IsThisCmd00(CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_STOP_01)) {

						cmd_status_t = SendCmd2FPGA(CMD_CTRL::CMD_TYPE_02_C::CT_STOP_01);
					
					}else if (cmd_ctrl_t->IsThisCmd00(CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_STOP)) {
						
						cmd_status_t = SendCmd2FPGA(CMD_CTRL::CMD_TYPE_02_C::CT_STOP);

					}else {
				
					}

	}
	return 	cmd_status_t;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int QtThreadClientCtrl::ProcessRemoteCmds(QSharedPointer<CMD_CTRL> cmd_ctrl_t)
{
	QSharedPointer<CMD_CTRL> cmd_resp_t = QSharedPointer<CMD_CTRL>(new CMD_CTRL());
	
	Q_ASSERT(cmd_ctrl_t->IsCmdRemote());

	if (cmd_ctrl_t->IsCmdRemote()){
		//cmd is remote cmd
		 return this->send_and_read_cmd(cmd_ctrl_t, cmd_resp_t);

	}
	return INIT_MY;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadClientCtrl::SetWorkMode(CMD_CTRL::WorkMode _wm)
{
	mWorkMode = _wm;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/

void	QtThreadClientCtrl::SetWorkModeCmd(CMD_CTRL::WorkMode _wm)
{
	SetWorkMode(_wm);
	
	QSharedPointer<CMD_CTRL> cmd_t = QSharedPointer<CMD_CTRL>(new CMD_CTRL());
		
	cmd_t->getModeChangeCmd(_wm);
	
	if (this->isRunning()) {
		QtThreadClientCtrl::SetCmd(cmd_t);

	}
	
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadClientCtrl::SetImgSigmaCmd(int _sigma)
{
	QSharedPointer<CMD_CTRL> cmd_t = QSharedPointer<CMD_CTRL>(new CMD_CTRL());

	cmd_t->getSigmaChangeCmd(_sigma);

	if (this->isRunning()) {
		QtThreadClientCtrl::cmds.setCmd(cmd_t);
	}
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int QtThreadClientCtrl::SendCmd2FPGA(CMD_CTRL::CMD_TYPE_02_C _start_stop)
{
	QSharedPointer<CMD_CTRL> cmd_t = QSharedPointer<CMD_CTRL>(new CMD_CTRL());
	QSharedPointer<CMD_CTRL> qsp_cc_t = CMD_CTRL::getFpgaStartCmdEx(_start_stop, mWorkMode);

	if (_start_stop){
		this->emit_status_message(mStatusMessage = QString("CMD>> Send Start cmd"));		
	}else {
		this->emit_status_message(mStatusMessage = QString("CMD>> Send Stop "));
	}
	
	int status_t=this->send_and_read_cmd(qsp_cc_t, cmd_t);
	
	if (TRUE_MY==status_t)
	{
		
			if (cmd_t->IsResp()) {
				this->emit_status_message(mStatusMessage = QString("CMD>> Rcv Start Resp "));
				return TRUE_MY;
			}else {
				return FALSE_MY;
			}

	}
	
	return status_t;

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
void QtThreadClientCtrl::SetLocalCmd(int cmd_00)
{

	QSharedPointer<CMD_CTRL> cmd_t = CMD_CTRL::getLocalCmdEx(cmd_00);

	QtThreadClientCtrl::cmds.setCmd(cmd_t);

#if _DEBUG
	
#endif // _DEBUG

	
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadClientCtrl::SetCmd(QSharedPointer<CMD_CTRL> _cmd)
{
	QtThreadClientCtrl::cmds.setCmd(_cmd);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadClientCtrl::ClearCmd()
{
	QtThreadClientCtrl::cmds.clear();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int QtThreadClientCtrl::IsCmdLocalFPGA(QSharedPointer<CMD_CTRL> _cmd)
{

	if (_cmd->IsThisCmd00(CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_START)) {

		printf_event("CMD_CTRL::CMD_TYPE_LOCAL::","CT_FPGA_START");
		return TRUE;

	}else if (_cmd->IsThisCmd00(CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_START_00)) {

		printf_event("CMD_CTRL::CMD_TYPE_LOCAL::", "CT_FPGA_START_00");
		return TRUE;

	}
	else if (_cmd->IsThisCmd00(CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_STOP_00)) {

		printf_event("CMD_CTRL::CMD_TYPE_LOCAL::", "CT_FPGA_STOP_00");
		return TRUE;

	}
	else if (_cmd->IsThisCmd00(CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_START_01)) {

		printf_event("CMD_CTRL::CMD_TYPE_LOCAL::", "CT_FPGA_START_01");
		return TRUE;

	}
	else if (_cmd->IsThisCmd00(CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_STOP_01)) {

		printf_event("CMD_CTRL::CMD_TYPE_LOCAL::", "CT_FPGA_STOP_01");
		return TRUE;

	}
	else if (_cmd->IsThisCmd00(CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_STOP)) {

		printf_event("CMD_CTRL::CMD_TYPE_LOCAL::", "CT_FPGA_STOP");
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
void QtThreadClientCtrl::run_socket_work()
{
	if (SocketErrorMy == this->ProcessCmds()) {
		return;
	}

	if (SocketErrorMy == this->SendHeartBeatCmdReadResp5s()) {
		return;
	}

	if (!this->IsSocketConnectedEx()) {
		return;
	}
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
#if _DEBUG

void QtThreadClientCtrl::run()
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
