#include "StdAfx.h"
#include "LatLng.h"
/*-----------------------------------------*/
/**
*
*/
/*-----------------------------------------*/
const double LatLng::Rc=6378137; 
const double LatLng::Rj=6356725; 
/*-----------------------------------------*/
/**
*
*/
/*-----------------------------------------*/
LatLng::LatLng(double longitude,double latitude)
{  
	m_LoDeg=(int)longitude;  
	m_LoMin=(int)((longitude-m_LoDeg)*60);  
	m_LoSec=(longitude-m_LoDeg-m_LoMin/60.)*3600;  

	m_LaDeg=(int)latitude;  
	m_LaMin=(int)((latitude-m_LaDeg)*60);  
	m_LaSec=(latitude-m_LaDeg-m_LaMin/60.)*3600;  

	m_Longitude=longitude;  
	m_Latitude=latitude;  
	m_RadLo=longitude*M_PI/180.;  
	m_RadLa=latitude*M_PI/180.;  
	Ec=Rj+(Rc-Rj)*(90.-m_Latitude)/90.;  
	Ed=Ec*cos(m_RadLa);  
}  
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
LatLng::~LatLng(void)
{

}
/*-----------------------------------------*/
/** 
* 求B点经纬度 
* @param A 已知点的经纬度， 
* @param distance   AB两地的距离  单位km 
* @param angle  AB连线与正北方向的夹角（0~360） 
* @return  B点的经纬度 
*/ 
/*-----------------------------------------*/ 
LatLng LatLng::getMyLatLng(LatLng A,double distance,double angle)
{  
	double dx = distance*1000*sin((angle)*M_PI/180);  
	double dy= distance*1000*cos((angle)*M_PI/180);  

	double bjd=(dx/A.Ed+A.m_RadLo)*180./M_PI;  
	double bwd=(dy/A.Ec+A.m_RadLa)*180./M_PI;  
	return LatLng(bjd, bwd);  
}  
 /*-----------------------------------------*/      
/** 
* 获取AB连线与正北方向的角度 
* @param A  A点的经纬度 
* @param B  B点的经纬度 
* @return  AB连线与正北方向的角度（0~360） 
*/  
/*-----------------------------------------*/ 
double LatLng::getAngle(LatLng A,LatLng B)
{  
    double dx=(B.m_RadLo-A.m_RadLo)*A.Ed;  
    double dy=(B.m_RadLa-A.m_RadLa)*A.Ec;  
    double angle=0.0;  
    angle=atan(fabs(dx/dy))*180./M_PI;    
    double dLo=B.m_Longitude-A.m_Longitude;  
    double dLa=B.m_Latitude-A.m_Latitude;  
    if(dLo>0&&dLa<=0){  
        angle=(90.-angle)+90;  
    }  
    else if(dLo<=0&&dLa<0){  
        angle=angle+180.;  
    }else if(dLo<0&&dLa>=0){  
        angle= (90.-angle)+270;  
    }  
    return angle;  
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/