#include "StepMotorSjts.hpp"
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
StepMotorSjts::StepMotorSjts()
{
	mStepMotor = QSharedPointer<StepMotorBe1105>(new StepMotorBe1105(nullptr));
	this->mBe1105RunDir = BE_1105_RUN_NEG;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
StepMotorSjts::~StepMotorSjts()
{
	
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void  StepMotorSjts::StepMotorRunOnce_Lab()
{
	this->mStepMotor->SendCmd4Done(mBe1105RunDir,
		BE_1105_RUN_SPEED_15S_BASE,
		BE_1105_RUN_ONE_CIRCLE_BASE);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void  StepMotorSjts::StepMotorRunOnce_kn()
{
	this->mStepMotor->SendCmd4Done(mBe1105RunDir,
		BE_1105_RUN_SPEED_CRACK_DETECT,
		BE_1105_RUN_CIRCLE_CRACK_DETECT);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void  StepMotorSjts::init_serial_port()
{
	do
	{

		if (this->init_serial_port_once()) {
			break;
		}
		else
		{
			//sleep
		}

	} while (0);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void StepMotorSjts::emit_init_serial_status(int _isOpen)
{
	
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int  StepMotorSjts::init_serial_port_once()
{
	
	const int open_status_t=this->StepMotor_OpenDefault();

	this->emit_init_serial_status(open_status_t);

	return this->StepMotor_IsOpen();

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int   StepMotorSjts::StepMotor_OpenDefault()
{
	this->mStepMotor->open_auto();

	return this->mStepMotor->IsOpen();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int  StepMotorSjts::StepMotor_IsOpen()
{
	return this->mStepMotor->IsOpen();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void  StepMotorSjts::StepMotor_Server_Start()
{
	this->mStepMotor->startSerialPortRun();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void StepMotorSjts::StepMotor_Server_Stop()
{
	this->mStepMotor->stopSerialPortRun();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int StepMotorSjts::do_StepMotor_sjts_Init()
{
	return this->init_serial_port_once();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void StepMotorSjts::do_StepMotor_sjts_Run_Once()
{
	StepMotorRunOnce_kn();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void StepMotorSjts::do_StepMotor_sjts_Run_Stop()
{
	this->mStepMotor->SendAutoCmd(BE_1105_STOP,
		BE_1105_RUN_SPEED_FASTEST);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void StepMotorSjts::do_StepMotor_sjts_Run_Fast()
{
	this->mStepMotor->SendAutoCmd(mBe1105RunDir,
		BE_1105_RUN_SPEED_FASTEST);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/