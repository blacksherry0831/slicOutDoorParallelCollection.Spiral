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
public:
	static CvSeq*	cvHoughCircles_Binary( CvArr* src_image, void* circle_storage,
                int method, double dp, double min_dist,
                double param1, double param2,
                int min_radius, int max_radius );

	static void icvHoughCirclesGradient( CvMat* img, float dp, float min_dist,
                         int min_radius, int max_radius,
                         int canny_threshold, int acc_threshold,
                         CvSeq* circles, int circles_max );
public:
	static void zhangjiagang_hongbao_duanzao_rgb(string filename);
	static void zhangjiagang_hongbao_duanzao(string filename);
	static void wait_for_show_image(string window_name,IplImage* img_t);


	static void use_lab2binary(IplImage* src_color_t);

};

