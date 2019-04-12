#pragma once
/*-------------------------------------*/
#include "cpp_stl.h"
/*-------------------------------------*/
#include "BE_1105_Dirver.hpp"
/*-------------------------------------*/
#include <QtCore>
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
class StepMotorBe1105 :public BE_1105_Driver
{
	Q_OBJECT

public:
	StepMotorBe1105(QObject *parent = nullptr);
	~StepMotorBe1105(void);
public:
	
	int   init_serial_port_once();

};