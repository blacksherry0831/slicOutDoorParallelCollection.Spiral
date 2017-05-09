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
	static  CvSeq* find_max_contour_adjust_binary(CvMemStorage* memory,IplImage* src_binary_t);
	
	static void equalizeHist_by_mask(IplImage* img_gary,CvRect mask_rect);
	static void equalizeHist_by_mask(IplImage* img_gary,IplImage* mask_img);
	static int GetHistogram(const IplImage* img_gary,const IplImage* mask_img,int *histogram);
	
	static IplImage* canny_by_mask(IplImage* src_color_t,IplImage* img_gary,IplImage* mask_img);
	static void Laplace_by_mask(IplImage* img_gary,IplImage* mask_img);

	static void threshold_binary(IplImage* img_gary,IplImage* mask_img);
	 
	static CvPoint grayCenter(IplImage* TheImage_org,IplImage* mask_img);

	static IplImage* gary_by_angle(IplImage* src_color_t,IplImage* src_gary_t,IplImage* mask_img);
	static void gary_get_by_angle(IplImage* src_color_t,double*hist_gary_level);

	static int GetMaxValueIndexdouble(
	double* data, 
	double size);


	static  IplImage* cut_image(IplImage* src_color_t,int method,CvRect& cut_t,IplImage* src_binary_t);
	static  IplImage* cut_image_01(CvMemStorage* memory,IplImage* src_color_cut,IplImage* src_binary_cut_part,CvRect& cut_t);
	
	static  void hough_image(IplImage* src_color_t,int method);

	static Point3f hough_my(
	IplImage *img_data,
	IplImage *dist,
	double threshold, int minRadius, int maxRadius, double distance,
	IplImage *h_acc,
	IplImage *coins);

	static Point3f hough_my_fast(
	IplImage *img_data,
	int method,
	double threshold, int minRadius, int maxRadius, double distance);

	static Point3f hough_my_fast_small(IplImage *img_data);
	static Point3f hough_my_fast_big(IplImage *img_data);

	static Point  find_central_line(CvSeq* maxContour,IplImage* des);
	
	static void draw_duan_jian_result(IplImage* src_color,CvSeq* seq,Point3f circle1,Point3f circle0,IplImage* mask_img);
	static void draw_dash_line(IplImage* src_color,double k,double b,IplImage* mask_img);


	


	static void find_circle(IplImage* src_color_cut,IplImage* src_binary_cut_part);

	static void process_max_min_rect(CvMemStorage* memory,IplImage* src_color_cut,IplImage* src_gary_cut,CvSeq* seq,IplImage* src_binary_cut,Point3f& circle0,Point3f& circle1);


};

