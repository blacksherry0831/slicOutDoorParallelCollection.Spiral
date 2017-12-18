#pragma once

#include "opencv_stl.h"

class ImageProcess
{
public:
	ImageProcess(void);
	~ImageProcess(void);
public:
	void findTestArea(void);
	int	findMaxFloatIdx(float* arr, int N);
	void adjustFloatMatrix2_100000(int height, float* matrix);

	float findAvgFloat(float* arr, int N);
	static int GetMaxValueIndexdouble(
		double* data,
		double size);
public:
	void FilterByMeanStdDev(IplImage* image_org_rgb, char *show_win);
	IplImage* FilterByMeanStdDev(IplImage* image_org_rgb);
public:
	void SaveCutImage(IplImage* img_t, CvRect cut_t, string readfile_name_t);
	static void wait_for_show_image(string window_name, IplImage* img_t);
public:
	CvRect findTestArea(IplImage *image_gray, std::string readfile_name_t);
public:
	static CvSeq*	cvHoughCircles_Binary(CvArr* src_image, void* circle_storage,
		int method, double dp, double min_dist,
		double param1, double param2,
		int min_radius, int max_radius);

	static void icvHoughCirclesGradient(CvMat* img, float dp, float min_dist,
		int min_radius, int max_radius,
		int canny_threshold, int acc_threshold,
		CvSeq* circles, int circles_max);
public:
	static void zhangjiagang_hongbao_duanzao_rgb(string filename);
	static void zhangjiagang_hongbao_duanzao(string filename);

	static  IplImage* cut_image(IplImage* src_color_t, int method, CvRect& cut_t, IplImage* src_binary_t);
	static  IplImage* cut_image_01(CvMemStorage* memory, IplImage* src_color_cut, IplImage* src_binary_cut_part, CvRect& cut_t);

	static IplImage* use_lab2binary(IplImage* src_color_t);
	static  CvSeq* find_max_contour_adjust_binary(CvMemStorage* memory, IplImage* src_binary_t);

	static void equalizeHist_by_mask(IplImage* img_gary, CvRect mask_rect);
	static void equalizeHist_by_mask(IplImage* img_gary, IplImage* mask_img);
	static int GetHistogram(const IplImage* img_gary, const IplImage* mask_img, int *histogram);

	static IplImage* canny_by_mask(IplImage* src_color_t, IplImage* img_gary, IplImage* mask_img);
	static void Laplace_by_mask(IplImage* img_gary, IplImage* mask_img);

	static void threshold_binary(IplImage* img_gary, IplImage* mask_img);

	static CvPoint grayCenter(IplImage* TheImage_org, IplImage* mask_img);

	static IplImage* gary_by_angle(IplImage* src_color_t, IplImage* src_gary_t, IplImage* mask_img);
	static void gary_get_by_angle(IplImage* src_color_t, double*hist_gary_level);

	static  void hough_image(IplImage* src_color_t, int method);

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

	static Point  find_central_line(CvSeq* maxContour, IplImage* des);

	static void draw_duan_jian_result(IplImage* src_color, CvSeq* seq, Point3f circle1, Point3f circle0, IplImage* mask_img);
	static void draw_dash_line(IplImage* src_color, double k, double b, IplImage* mask_img);

	static void find_circle(IplImage* src_color_cut, IplImage* src_binary_cut_part);

	static void process_max_min_rect(CvMemStorage* memory, IplImage* src_color_cut, IplImage* src_gary_cut, CvSeq* seq, IplImage* src_binary_cut, Point3f& circle0, Point3f& circle1);
public:
	static void CannyAdaptiveFindThreshold(IplImage* pImg, double *low, double *high);
	static void HYAdaptiveFindThreshold(CvMat *dx, CvMat *dy, double *low, double *high);
public:
	static void HoughLine(IplImage* pImg, int *pR, int *pTh, int iThreshold, float rho_min, float rho_max, float theta_min, float theta_range);
	static void Draw_line_on_image(float rho, float theta, CvRect rect_cut, IplImage* iplimg_tmp, Point& pt1, Point& pt2);
public:
	static vector<float> crack_get_image_feature(IplImage *diff_org, string file_base, int frame_idx);
	static vector<float> crack_get_image_feature_gauss(IplImage *diff_org, string file_base, int CHANNEL, int frame_idx, IplImage *image_out,vector<float>& delta_out,boolean SAVE_FLAG);
	static void crack_get_long_crack(IplImage * diff_org,IplImage *image_4_delta,int delta_idx, vector<vector<CvPoint>>&   point_setsstring,string file_base, int CHANNEL, int frame_idx,boolean SAVE_FLAG);
	static std::string GetPath(std::string path_base, std::string path_sub, boolean CREATE_FLAG=true);
	static vector<float> process_histogram(vector<float>& histogram, vector<vector<CvPoint>>&  point_sets, vector<float>& delta_out, int HISTOGRAM_DIM, int width, int height);
	static int GetLineProperty(vector<CvPoint> point_set, vector<float> delta, float& sum_delta, int& idx);
public:
	static void Svm_Lean(vector<float> FeatureData, int FeatureDim, vector<INT32> FeatureClassify, int method, string path);
public:
	static void CuiResize(IplImage * src, IplImage * dst, const int m_step, const int n_step);
public:
	static void  DrawHistogram(float *data, int size, string file_base, int CHANNEL, int frame_idx, vector<float> feature);
public:
	static void  SaveArray2Disk(float *data, int size,int channel_t, int frame_count, string file_base);
	
	static float GetMaxValue(float* Data, long DataNum);
	static void  GetMaxValueIndex(
		float* data,
		float size,
		int* sort,
		int sort_num);
	static void  GetMinValueIndex(
		float* data,
		float size,
		int* sort,
		int sort_num);
};