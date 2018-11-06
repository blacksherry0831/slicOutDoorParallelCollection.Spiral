#include "QtThreadStepMotor.hpp"

#include "SocketQT/ChannelsData.hpp"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
#define  DEBUG_TEST	 do{}while(0)
/*-------------------------------------*/
const int QtThreadStepMotor::TIME_GAP=5*1000;
const int QtThreadStepMotor::BLOCK_IN_STEP02= TRUE;
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
QtThreadStepMotor::QtThreadStepMotor()
{
	this->mThreadName = "Step Motor Thread ";
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
	time.start(); //开始计时，以ms为单位
	{	
		mBE_1105->SendCmd4Done(BE_1105_RUN_NEG, 55000);
	}

	int time_Diff = time.elapsed(); //返回从上次start()或restart()开始以来的时间差，单位ms

			//以下方法是将ms转为s
	float f = time_Diff / 1000.0;

	if (this->M_THREAD_RUN)
	{
		Q_ASSERT(f>FpgaConvertMaxTime);//this time greater than 12s
	}
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
void QtThreadStepMotor::Wait4ImgProcess(const int _time)
{
	const int TIME_MAX = 60*1000;
	const int time_interval = 500;
	int time_sleep = 0;


	do {

		QThread::msleep(time_interval);

		time_sleep += time_interval;

		if (0==ChannelsData::getInstance()->QueueSize())
		{
			if (time_sleep>_time)
			{
				break;
			}

		}

		Q_ASSERT(time_sleep<TIME_MAX);

		if (time_sleep>TIME_MAX)	{	
			break;
		}


	} while (M_THREAD_RUN);

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadStepMotor::run_no_step_motor()
{
	QSharedPointer<CMD_CTRL> cmd_t = QSharedPointer<CMD_CTRL>(new CMD_CTRL());
		
	/*-------------------------------------*/
	this->emit_status_message(mStatusMessage = "Thread>> Ctrl Thread Start");
	/*-------------------------------------*/
	
	/*-------------------------------------*/
	QtThreadClientCtrl::ClearCmd();
	/*-------------------------------------*/

	while (M_THREAD_RUN) {


		if (this->IsCmdCtrlPipeOK()) {

#if _DEBUG
			this->SleepMy(1000);//wait
#endif // _DEBUG

			QtThreadClientCtrl::SetLocalCmd(CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_START);
			this->emit_status_message(mStatusMessage = "CT_FPGA_START");
			this->SleepMy(1000);
			{

			
				QtThreadClientCtrl::SetLocalCmd(CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_START_00);
				this->emit_status_message(mStatusMessage = "CT_FPGA_START_00");
				{
					this->SleepMy(TIME_GAP*3);
				}
				QtThreadClientCtrl::SetLocalCmd(CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_STOP_00);
				this->emit_status_message(mStatusMessage = "CT_FPGA_STOP_00");
				
				this->SleepMy(TIME_GAP);

				QtThreadClientCtrl::SetLocalCmd(CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_START_01);
				this->emit_status_message(mStatusMessage = "CT_FPGA_START_01");
				{
					this->SleepMy(TIME_GAP * 3);
					this->blockInStep02();
				}
				QtThreadClientCtrl::SetLocalCmd(CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_STOP_01);
				this->emit_status_message(mStatusMessage = "CT_FPGA_STOP_01");
				
				


			}QtThreadClientCtrl::SetLocalCmd(CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_STOP);
			this->emit_status_message(mStatusMessage = "CT_FPGA_STOP");
			this->Wait4ImgProcess(TIME_GAP);
		}


	}


	/*-------------------------------------*/
	
#if _DEBUG
	this->emit_status_message(mStatusMessage = "Thread>>  shutdown");
#endif // _DEBUG

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadStepMotor::run_normal()
{
	
	QSharedPointer<CMD_CTRL> cmd_t = QSharedPointer<CMD_CTRL>(new CMD_CTRL());

	mBE_1105 = QSharedPointer<BE_1105_Driver>(new BE_1105_Driver(Q_NULLPTR));

	/*-------------------------------------*/
	this->emit_status_message(mStatusMessage = "Thread>> Ctrl Thread Start");
	/*-------------------------------------*/
	do {

		mBE_1105->open_auto();

		if (mBE_1105->init() == TRUE) {
			break;
		}
		else {
			this->SleepMy(1000);
			std::cout << "EVENT>>" << "Cant Open Serial Port !" << std::endl;
		}

		if (M_THREAD_RUN == FALSE) {
			break;
		}

	} while (mBE_1105->init() == FALSE);
	/*-------------------------------------*/
	QtThreadClientCtrl::ClearCmd();
	/*-------------------------------------*/

	while (M_THREAD_RUN) {


		if (this->IsCmdCtrlPipeOK()) {



			QtThreadClientCtrl::SetLocalCmd(CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_START);
			this->emit_status_message(mStatusMessage = "CT_FPGA_START");

			{

				DEBUG_TEST;
				QtThreadClientCtrl::SetLocalCmd(CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_START_00);
				this->emit_status_message(mStatusMessage = "CT_FPGA_START_00");
				{
					DEBUG_TEST;
					this->StepMotorRun();
				}
				QtThreadClientCtrl::SetLocalCmd(CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_STOP_00);
				this->emit_status_message(mStatusMessage = "CT_FPGA_STOP_00");
#if 0
				this->Wait4ImgProcess(TIME_GAP);
#endif // 0



				QtThreadClientCtrl::SetLocalCmd(CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_START_01);
				this->emit_status_message(mStatusMessage = "CT_FPGA_START_01");
				{
					DEBUG_TEST;
					this->StepMotorRun();
					this->blockInStep02();
				}
				QtThreadClientCtrl::SetLocalCmd(CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_STOP_01);
				this->emit_status_message(mStatusMessage = "CT_FPGA_STOP_01");
#if 0
				this->Wait4ImgProcess(TIME_GAP);
#endif // 0

				

			}QtThreadClientCtrl::SetLocalCmd(CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_STOP);
			this->emit_status_message(mStatusMessage = "CT_FPGA_STOP");
			DEBUG_TEST;
		}


	}


	/*-------------------------------------*/
	mBE_1105->close();
	mBE_1105.clear();
#if _DEBUG
	this->emit_status_message(mStatusMessage = "Thread>>  shutdown");
#endif // _DEBUG
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void  QtThreadStepMotor::run()
{	
#if 1
		this->run_normal();
#endif

#if 0
		this->run_no_step_motor();
#endif
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
	this->wait4ServerClose();
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
void QtThreadStepMotor::blockInStep02()
{
	if (BLOCK_IN_STEP02) {

		while (this->M_THREAD_RUN) {
			this->SleepMy(200);
			if (!this->IsCmdCtrlPipeOK()) {
				break;
			}
		}

	}
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/