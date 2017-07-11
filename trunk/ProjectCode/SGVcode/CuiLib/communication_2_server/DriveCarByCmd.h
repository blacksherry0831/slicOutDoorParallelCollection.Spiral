#pragma once

#include "CommData.h"
#include "ResponseData.h"

#include <SerialPort/AGV_Dirver.h>

#include "cpp_stl.h"
/*----------------------------------*/
/**
*	boost_1_64_0-msvc-10.0-32.exe
*	BOOST_ROOT=C:\local\boost_1_64_0
*	b2 --toolset=msvc-10.0 --build-type=complete stage
* 
*/
/*----------------------------------*/
class DriveCarByCmd
{
private:
	AGV_Dirver* m_AGV;
private:

public:
	DriveCarByCmd(void);
	~DriveCarByCmd(void);
private:
	
public:
	 void ExeCommand(ResponseData cmd_t);
	 void ExeCommandManual(ResponseData cmd_t);
	 void ExeCommandAuto(ResponseData cmd_t);
};

