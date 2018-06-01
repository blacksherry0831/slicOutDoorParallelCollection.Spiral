#pragma once

#include "cpp_stl.h"

#include "ICamera.hpp"

class OpencvCamera :public ICamera
{
public:
	OpencvCamera(void);
	~OpencvCamera(void);
public:
	bool init();
	bool release();
	IplImage* QueryFrame();
	std::string IntrinsicName();
	std::string DistortionName();
private:
	CvCapture* gloal_p_capture;//ȫ�ֵ�ͼ��ָ��
	int  global_cam_index;//cvCaptureFromCAM������



};

