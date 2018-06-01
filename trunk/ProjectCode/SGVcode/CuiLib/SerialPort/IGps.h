#pragma once

#include "cpp_stl.h"
#include "MY_SDK_LIB/LatLng.h"

class IGps
{


public:
	double g_Lat_float;
	double g_Lon_float;

public:
	virtual ~IGps(void){}



public:
	virtual std::string GetLatLonStr()=0;
	virtual std::string GetLatStr()=0;
	virtual std::string GetLonStr()=0;
	virtual LatLng get()=0;


		
};