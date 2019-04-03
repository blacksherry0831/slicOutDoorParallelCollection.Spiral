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
const int QtThreadStepMotor::TIME_15S = 15 * 1000;
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

/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThreadStepMotor::hardware_roller_run()
{
	if (this->hardware_init_status()){

#if 0
	this->hardware_roller_run_base_x5();
#else
	this->hardware_roller_run_base();
#endif

	}
	else
	{
		this->SleepMy(TIME_15S);
	}

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void  QtThreadStepMotor::hardware_roller_run_base()
{

#if 1
	this->mBE_1105->SendCmd4Done(mBe1105RunDir,
		BE_1105_RUN_SPEED_15S_BASE,
		BE_1105_RUN_ONE_CIRCLE_BASE);
#endif

#if 0
	this->mBE_1105->SendCmd4Done(mBe1105RunDir,
		35000,
		BE_1105_RUN_ONE_CIRCLE_BASE);
#endif // 0


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

	if (this->init_serial_port_once(this->mBE_1105) == FALSE) {
		this->printf_event("Serial Port", "open fali");
	}

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int QtThreadStepMotor::hardware_init_status()
{
	return this->mBE_1105->init();
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
void  QtThreadStepMotor::init_serial_port(QSharedPointer<BE_1105_Driver>	 _be_1105)
{
	do {
		
			if (init_serial_port_once(_be_1105) == TRUE) {

				break;

			}
			else {
				this->SleepMy(1000);
			}

	} while (M_THREAD_RUN);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int QtThreadStepMotor::init_serial_port_once(QSharedPointer<BE_1105_Driver> _be_1105)
{
	_be_1105->open_auto();

	if (_be_1105->init() == TRUE) {

		emit status_sjts(CMD_CTRL::SJTS_MACHINE_STATUS::SerialPortIsOpen, "");
	
	}else {
		emit status_sjts(CMD_CTRL::SJTS_MACHINE_STATUS::SerialPortError, "");
	}

	return _be_1105->IsOpen();

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/