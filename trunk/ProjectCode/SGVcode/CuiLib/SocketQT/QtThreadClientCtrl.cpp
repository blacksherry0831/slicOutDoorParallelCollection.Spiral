//#include "stdafx.h"
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
	mIpAddr = "192.168.100.102";
#else
	mIpAddr = "127.0.0.1";
#endif // 0

	mPort = TCP_POET_CMD_CTRL;

	mWorkMode = CMD_CTRL::WorkMode( CMD_CTRL::WorkMode::WM_ORG_IMG| CMD_CTRL::WorkMode::WM_SIZE_FULL);
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
void QtThreadClientCtrl::run()
{
	this->init_socket();

	QSharedPointer<CMD_CTRL> cmd_t = QSharedPointer<CMD_CTRL>(new CMD_CTRL());
	
	qDebug() << "Ctrl Thread Start";
			
/*-----------------------------*/		
		while (M_THREAD_RUN)
		{	

			this->connect2ServerIfNoConnected();
				
			while (M_THREAD_RUN) {
			
#if TRUE
//step 1				
					std::cout << "Send Start" << std::endl;

					m_socket->Send_Start_CMD(TRUE,mWorkMode);
												
					if (m_socket->Read_1_cmd(cmd_t.data()) == 0) {
						break;
					}
					if (cmd_t->IsResp()) {

							std::cout << "Rcv Start Resp !" << std::endl;

					}else {
						break;
					}
					
#endif // TRUE

#if TRUE
					while(M_THREAD_RUN){
						
						if (this->SendCmdCtrl()) {
							//send success
						}else {
							//no cmd
							if (m_socket->SendHearbeatCmd()==0) {						
								break;
							}else {
								this->Sleep(1000);
							}
						
						}						

					}
#endif // TRUE





#if TRUE
//step 2
					std::cout << "Send Stop" << std::endl;

					m_socket->Send_Start_CMD(FALSE, mWorkMode);
					if (m_socket->Read_1_cmd(cmd_t.data()) == 0) {
						break;
					}
					if (cmd_t->IsResp()) {

						std::cout << "Rcv  Resp !" << std::endl;

					}
					else {
						break;
					}
#endif		
					this->Sleep(3*1000);
			
			}
		
			this->closeSocket4Server();

	 	}
/*-----------------------------*/

	

	
#if _DEBUG
	 qDebug() << "Image Ctrl Thread : shutdown !" ;
#endif // _DEBUG
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
void	QtThreadClientCtrl::SetWorkMode(CMD_CTRL::WorkMode _wm)
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

/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/