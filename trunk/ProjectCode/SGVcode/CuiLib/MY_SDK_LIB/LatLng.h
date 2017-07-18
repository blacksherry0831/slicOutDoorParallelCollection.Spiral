#pragma once

#include "cpp_stl.h"

class LatLng
{
public:
	LatLng(double longitude,double latitude);
	~LatLng(void);
public:
	const static double Rc;  
	const static double Rj; 

	double m_LoDeg,m_LoMin,m_LoSec;  
	double m_LaDeg,m_LaMin,m_LaSec; 

	double m_Longitude,m_Latitude; 

	double m_RadLo,m_RadLa; 

	double Ec;  
	double Ed;  
public:
	static LatLng getMyLatLng(LatLng A,double distance,double angle);
	static double getAngle(LatLng A,LatLng B);

};

