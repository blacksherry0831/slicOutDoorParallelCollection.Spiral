#include "IStepMotorSjts.hpp"
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void IStepMotorSjts::init_serial_port()
{

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void IStepMotorSjts::emit_init_serial_status(int _isOpen)
{
	
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int  IStepMotorSjts::init_serial_port_once()
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
int   IStepMotorSjts::StepMotor_OpenDefault()
{
	return 0;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void IStepMotorSjts::StepMotor_Server_Start()
{

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void IStepMotorSjts::StepMotor_Server_Stop()
{

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int  IStepMotorSjts::StepMotor_IsOpen()
{
	return 0;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int IStepMotorSjts::do_StepMotor_sjts_Init()
{
	return this->init_serial_port_once();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int IStepMotorSjts::do_StepMotor_sjts_Run_Once()
{
	return 0;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int IStepMotorSjts::do_StepMotor_sjts_Run_Stop()
{
	return 0;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int IStepMotorSjts::do_StepMotor_sjts_Run_Fast()
{
	return 0;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/