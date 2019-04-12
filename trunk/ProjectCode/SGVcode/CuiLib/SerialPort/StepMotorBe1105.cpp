#include "StdAfx.h"
#include "StepMotorBe1105.hpp"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
StepMotorBe1105::StepMotorBe1105(QObject *parent):BE_1105_Driver(parent)
{
	mCom = 4;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
StepMotorBe1105::~StepMotorBe1105(void)
{
	
	

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int StepMotorBe1105::init_serial_port_once()
{
	this->open_auto();
	
	return  this->IsOpen();

}

