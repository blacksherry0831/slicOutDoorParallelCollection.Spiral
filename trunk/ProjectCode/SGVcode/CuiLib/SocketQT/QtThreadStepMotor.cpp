#include "QtThreadStepMotor.hpp"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
#define  DEBUG_TEST	 do{}while(0)
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
QtThreadStepMotor::QtThreadStepMotor()
{
	
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
QtThreadStepMotor::~QtThreadStepMotor(void)
{
	qDebug() << "QtThreadStepMotor is Release ! ";
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadStepMotor::StepMotorRun()
{
	const float FpgaConvertMaxTime=12;
	QTime time;
	time.start(); //��ʼ��ʱ����msΪ��λ
	{	
		mBE_1105->SendCmd4Done(BE_1105_RUN_NEG, 55000);
	}

	int time_Diff = time.elapsed(); //���ش��ϴ�start()��restart()��ʼ������ʱ����λms

			//���·����ǽ�msתΪs
	float f = time_Diff / 1000.0;

	Q_ASSERT(f>FpgaConvertMaxTime);//this time greater than 12s


}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadStepMotor::SetCmdCtrlPipe(QSharedPointer<QtThreadSocketClient> _cmdCtrlPipe)
{
	this->mCmdCtrlPipe = _cmdCtrlPipe;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int QtThreadStepMotor::IsCmdCtrlPipeOK()
{

	if (!this->mCmdCtrlPipe.isNull()) {
	
		return this->mCmdCtrlPipe->IsSocketConnectedThreadRunning();
	}

	return FALSE;

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void  QtThreadStepMotor::run()
{
	QSharedPointer<CMD_CTRL> cmd_t = QSharedPointer<CMD_CTRL>(new CMD_CTRL());

	mBE_1105 = QSharedPointer<BE_1105_Driver>(new BE_1105_Driver(Q_NULLPTR));
	/*-------------------------------------*/
	
	do {

		mBE_1105->open_auto();

		if (mBE_1105->init()==TRUE) {
			break;
		}else{
			this->SleepMy(1000);
			std::cout << "EVENT>>" << "Cant Open Serial Port !" << std::endl;
		}

		if (M_THREAD_RUN==FALSE){
			break;
		}

	} while (mBE_1105->init()==FALSE);
	/*-------------------------------------*/

	while (M_THREAD_RUN){


		if (this->IsCmdCtrlPipeOK()){

#if _DEBUG
					this->SleepMy(5000);//wait
#endif // _DEBUG

					QtThreadClientCtrl::SetLocalCmd(CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_START);{
							DEBUG_TEST;
									QtThreadClientCtrl::SetLocalCmd(CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_START_00); {
										DEBUG_TEST;
										this->StepMotorRun();
			
									}QtThreadClientCtrl::SetLocalCmd(CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_STOP_00);
							DEBUG_TEST;
							this->SleepMy(5000);//wait
									QtThreadClientCtrl::SetLocalCmd(CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_START_01); {
										DEBUG_TEST;
										this->StepMotorRun();

									}QtThreadClientCtrl::SetLocalCmd(CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_STOP_01);
							DEBUG_TEST;
		
					}QtThreadClientCtrl::SetLocalCmd(CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_STOP);
					DEBUG_TEST;
				}


		}


	/*-------------------------------------*/
	mBE_1105->close();
	mBE_1105.clear();

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadStepMotor::startServer()
{
	QtThreadBase::startServer();
	if (!mBE_1105.isNull()){
		mBE_1105->startSerialPortRun();
	}
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadStepMotor::closeServer()
{
	QtThreadBase::closeServer();
	if (!mBE_1105.isNull()){
		mBE_1105->stopSerialPortRun();
	}
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
QtThreadStepMotor* QtThreadStepMotor::SetBordIPaddr(QString _ipAddr)
{
	this->mCurrentBord = _ipAddr.toStdString();
	return this;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/