#include "StdAfx.h"
#include "OpencvCamera.hpp"

/*-----------------------------------------*/
/**
*
*/
/*-----------------------------------------*/

/*-----------------------------------------*/
/**
*
*/
/*-----------------------------------------*/
OpencvCamera::OpencvCamera()
{  
	gloal_p_capture = NULL;//全局的图像指针
	global_cam_index = 0;//cvCaptureFromCAM的索引
}  
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
OpencvCamera::~OpencvCamera(void)
{
	this->release();
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
bool OpencvCamera::init()
{

	gloal_p_capture = cvCreateCameraCapture(global_cam_index);
	return true;
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
bool OpencvCamera::release()
{
	cvReleaseCapture(&gloal_p_capture);
	gloal_p_capture = NULL;
	return true;
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
IplImage* OpencvCamera::QueryFrame()
{
	return cvQueryFrame(gloal_p_capture);
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
std::string OpencvCamera::IntrinsicName()
{
	std::stringstream ss;

	ss << "Intrinsics";
	ss << "_";
	ss << this->global_cam_index;
	ss << ".xml";
		
	return ss.str();
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
std::string OpencvCamera::DistortionName()
{
	std::stringstream ss;
	ss << "Distortion";
	ss << "_";
	ss << this->global_cam_index;
	ss << ".xml";
	
	return ss.str();
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/