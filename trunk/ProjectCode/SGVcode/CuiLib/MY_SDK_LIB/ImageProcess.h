#pragma once

#include "opencv_stl.h"

class ImageProcess
{
public:
	ImageProcess(void);
	~ImageProcess(void);
	void findTestArea(void);
	int	findMaxFloatIdx(float* arr, int N);
	void adjustFloatMatrix2_100000(int height,float* matrix);
	CvRect findTestArea(IplImage *image_gray,std::string readfile_name_t);
	float findAvgFloat(float* arr, int N);
public:
	
	void SaveCutImage(IplImage* img_t,CvRect cut_t,string readfile_name_t);
public:
	void FilterByMeanStdDev(IplImage* image_org_rgb,char *show_win);

	IplImage* FilterByMeanStdDev(IplImage* image_org_rgb);
};

