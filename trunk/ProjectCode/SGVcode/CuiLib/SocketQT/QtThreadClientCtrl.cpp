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
void QtThreadClientCtrl::run_00()
{
	this->init_socket();

	QSharedPointer<CMD_CTRL> cmd_t = QSharedPointer<CMD_CTRL>(new CMD_CTRL());

	this->emit_status_message(mStatusMessage = "Thread>> Ctrl Thread Start");

	/*-----------------------------*/
	while (M_THREAD_RUN)
	{

		this->connect2ServerIfNoConnected();

		while (M_THREAD_RUN) {

#if TRUE
			//step 1				
			this->emit_status_message(mStatusMessage = QString("CMD>> Send Start cmd"));


			m_socket->Send_Start_CMD(CMD_CTRL::CMD_TYPE_02_C::CT_START, mWorkMode);

			if (m_socket->Read_1_cmd(cmd_t.data()) == 0) {
				break;
			}

			if (cmd_t->IsResp()) {
				this->emit_status_message(mStatusMessage = QString("CMD>> Rcv Start Resp "));

			}else {
				break;
			}

#endif // TRUE

#if TRUE
			while (M_THREAD_RUN) {

				if (this->SendCmdCtrl()) {
					//send success
				}
				else {
					//no cmd
					if (m_socket->SendHearbeatCmd() == 0) {
						break;
					}
					else {
						this->Sleep(1000);
					}

				}

			}
#endif // TRUE

#if TRUE
			//step 2
			this->emit_status_message(mStatusMessage = QString("CMD>> Send Stop "));
			m_socket->Send_Start_CMD(CMD_CTRL::CMD_TYPE_02_C::CT_STOP, mWorkMode);

			if (m_socket->Read_1_cmd(cmd_t.data()) == 0) {
				break;
			}
			else {
				if (cmd_t->IsResp()) {
					this->emit_status_message(mStatusMessage = QString("CMD>> Rcv  Resp "));
				}
				else {
					break;
				}
			}

#endif		

#if TRUE
			this->Sleep(3 * 1000);
#endif // TRUE



		}

		this->closeSocket4Server();

	}
	/*-----------------------------*/

#if _DEBUG
	this->emit_status_message(mStatusMessage = "Thread>>  shutdown");
#endif // _DEBUG
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadClientCtrl::run_01()
{
	this->init_socket();

	QSharedPointer<CMD_CTRL> cmd_t = QSharedPointer<CMD_CTRL>(new CMD_CTRL());

	this->emit_status_message(mStatusMessage = "Thread>> Ctrl Thread Start");

	/*-----------------------------*/
	while (M_THREAD_RUN){

		this->connect2ServerIfNoConnected();

		while (M_THREAD_RUN) {

			if (this->IsDataPipeOK()) {
					if (SocketErrorMy==this->ProcessCmds()) {
						break;
					}
			}

			if (SocketErrorMy == this->SendHearbeatCmd()) {
					break;
			}

		}

		this->closeSocket4Server();

	}
	/*-----------------------------*/

#if _DEBUG
	this->emit_status_message(mStatusMessage = "Thread>>  shutdown");
#endif // _DEBUG
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadClientCtrl::run()
{
	this->run_01();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int QtThreadClientCtrl::SendCmdCtrl()
{

	if (cmds.IsValid()) {
		
		QSharedPointer<CMD_CTRL> cmd_ctrl_t=cmds.getCmd();
		
		return	m_socket->Send_1_cmd(cmd_ctrl_t.data());
	
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
	if (cmd_ctrl_t->IsCmdRemote()){
		//cmd is remote cmd

		if (0==m_socket->Send_1_cmd(cmd_ctrl_t.data())) {
			return SocketErrorMy;
		}
		else {
			return TRUE_MY;
		}

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
	mWorkMode = _wm;
	
	QSharedPointer<CMD_CTRL> cmd_t = QSharedPointer<CMD_CTRL>(new CMD_CTRL());
		
	cmd_t->getModeChangeCmd(_wm);
	
	if (this->isRunning()) {
		QtThreadClientCtrl::cmds.setCmd(cmd_t);
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

	if (_start_stop){
		this->emit_status_message(mStatusMessage = QString("CMD>> Send Start cmd"));		
	}else {
		this->emit_status_message(mStatusMessage = QString("CMD>> Send Stop "));
	}

	if (0 == m_socket->Send_Start_CMD(_start_stop, mWorkMode)) {
	
		return  SocketErrorMy;
	}

	if (0 == m_socket->Read_1_cmd(cmd_t.data())) {

		return  SocketErrorMy;
	
	}else{

			if (cmd_t->IsResp()) {
				this->emit_status_message(mStatusMessage = QString("CMD>> Rcv Start Resp "));
				return TRUE_MY;
			}else {
				return FALSE_MY;
			}

	}



}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int QtThreadClientCtrl::SendHearbeatCmd()
{
	static const int SleepInterval = 100;
	static int SleepTimes = 0;
	int result_t = INIT_MY;

	this->Sleep(SleepInterval);

	SleepTimes++;

	if (SleepTimes % (mHeartBeatFreq / SleepInterval) == 0)
	{
#if 1
		//no cmd
		if (0 == m_socket->SendHearbeatCmd()) {
			result_t = SocketErrorMy;
		}
		else {
			result_t = TRUE_MY;
		}
#endif // 0		
	}
	return result_t;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadClientCtrl::SetLocalCmd(int cmd_00)
{

	QSharedPointer<CMD_CTRL> cmd_t = QSharedPointer<CMD_CTRL>(new CMD_CTRL());

	cmd_t->getLocalCmd(cmd_00);

	QtThreadClientCtrl::cmds.setCmd(cmd_t);

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