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


	static IplImage* use_lab2binary(IplImage* src_color_t);
	static  CvSeq* find_max_contour_adjust_binary(IplImage* src_binary_t);
	
	static void equalizeHist_by_mask(IplImage* img_gary,CvRect mask_rect);
	static void equalizeHist_by_mask(IplImage* img_gary,IplImage* mask_img);
	static int GetHistogram(const IplImage* img_gary,const IplImage* mask_img,int *histogram);
	
	static void canny_by_mask(IplImage* src_color_t,IplImage* img_gary,IplImage* mask_img);
	static void Laplace_by_mask(IplImage* img_gary,IplImage* mask_img);

	static void threshold_binary(IplImage* img_gary,IplImage* mask_img);
	 
	static CvPoint grayCenter(IplImage* TheImage_org,IplImage* mask_img);

	static void gary_by_angle(IplImage* src_color_t,IplImage* src_gary_t,IplImage* mask_img);
	static void gary_get_by_angle(IplImage* src_color_t,double*hist_gary_level);

	static int GetMaxValueIndexdouble(
	double* data, 
	double size);
};

