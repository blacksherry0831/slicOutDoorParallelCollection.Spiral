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
	this->RUN_MODE = 1;
	this->mBe1105RunDir = BE_1105_RUN_NEG;
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
		mBE_1105->SendCmd4Done(BE_1105_RUN_NEG, BE_1105_RUN_SPEED_CRACK_DETECT);
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
void QtThreadStepMotor::hardware_roller_run()
{
#if 0
	this->hardware_roller_run_base_x5();
#else
	this->hardware_roller_run_base();
#endif
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void  QtThreadStepMotor::hardware_roller_run_base()
{
	this->mBE_1105->SendCmd4Done(mBe1105RunDir,
		BE_1105_RUN_SPEED_15S_BASE,
		BE_1105_RUN_ONE_CIRCLE_BASE);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void  QtThreadStepMotor::hardware_roller_run_base_x5()
{
	this->mBE_1105->SendCmd4Done(mBe1105RunDir,
		BE_1105_RUN_SPEED_CRACK_DETECT,
		BE_1105_RUN_CIRCLE_CRACK_DETECT);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadStepMotor::hardware_init()
{
	this->mBE_1105 = QSharedPointer<BE_1105_Driver>(new BE_1105_Driver(Q_NULLPTR));

	this->init_serial_port(mBE_1105);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadStepMotor::run_normal()
{
	const int TIME_INTERVAL = 2000;
	const int TIME_VIDEO = 15 * 1000;

	this->hardware_init();	

	while (M_THREAD_RUN)
	{
		this->SleepMy(TIME_INTERVAL);
		this->wait4WorkFlowStart();

		this->SleepMy(TIME_INTERVAL);
		
		emit status_sjts(CMD_CTRL::SJTS_MACHINE_STATUS::RoolerReady, CircleSeq());

		{
			this->SleepMy(TIME_INTERVAL);
			emit status_sjts(CMD_CTRL::SJTS_MACHINE_STATUS::StepMotorStart00, "");			
			this->hardware_roller_run();			
			emit status_sjts(CMD_CTRL::SJTS_MACHINE_STATUS::StepMotorStop00, "");
		}

		{
			this->SleepMy(TIME_INTERVAL);
			emit status_sjts(CMD_CTRL::SJTS_MACHINE_STATUS::StepMotorStart01, "");			
			this->hardware_roller_run();
			emit status_sjts(CMD_CTRL::SJTS_MACHINE_STATUS::StepMotorStop01, "");
		}


		this->emit_roller_done();


		CMD_CTRL::BodyRollerQualified qualified_status_t = this->wait4ImgProcessResult();

		this->emit_roller_done_qualified(qualified_status_t);

		this->SleepMy(TIME_INTERVAL);
	}
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void  QtThreadStepMotor::run()
{	

	if (RUN_MODE){
		this->run_normal();
	}else{
		this->run_no_step_motor();
	}

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
	this->closeServerSync();
}
/*-------------------------------------*/
/**
*@note 同步
*/
/*-------------------------------------*/
void QtThreadStepMotor::closeServerSync()
{
	this->closeServerAsync();
	this->wait4ServerClose();
}
/*-------------------------------------*/
/**
*@note 异步
*/
/*-------------------------------------*/
void QtThreadStepMotor::closeServerAsync()
{
	QtThreadBase::closeServer();
	if (!mBE_1105.isNull()) {
		mBE_1105->stopSerialPortRun();
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
void QtThreadStepMotor::SetBlock(bool _block)
{
	this->mBlock = _block;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void  QtThreadStepMotor::init_serial_port(QSharedPointer<BE_1105_Driver>	 _be_1105)
{
	do {
		_be_1105->open_auto();

		if (_be_1105->init() == TRUE) {

			emit status_sjts(CMD_CTRL::SJTS_MACHINE_STATUS::SerialPortIsOpen, "");
			break;

		}
		else {
			this->SleepMy(1000);
			emit status_sjts(CMD_CTRL::SJTS_MACHINE_STATUS::SerialPortError, "");
		}

	} while (_be_1105->init() == FALSE && M_THREAD_RUN);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/