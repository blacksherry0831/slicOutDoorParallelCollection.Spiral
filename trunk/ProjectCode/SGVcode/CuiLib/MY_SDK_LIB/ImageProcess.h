 #pragma once

#include "opencv_stl.h"

#include "Line2Point.hpp"

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
	void SaveCutImage(IplImage* img_t, CvRect cut_t, std::string readfile_name_t);
	static void wait_for_show_image(std::string window_name, IplImage* img_t);
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
	static void zhangjiagang_hongbao_duanzao_rgb(std::string filename);
	static void zhangjiagang_hongbao_duanzao(std::string filename);

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

	static cv::Point3f hough_my(
		IplImage *img_data,
		IplImage *dist,
		double threshold, int minRadius, int maxRadius, double distance,
		IplImage *h_acc,
		IplImage *coins);

	static cv::Point3f hough_my_fast(
		IplImage *img_data,
		int method,
		double threshold, int minRadius, int maxRadius, double distance);

	static cv::Point3f hough_my_fast_small(IplImage *img_data);
	static cv::Point3f hough_my_fast_big(IplImage *img_data);

	static cv::Point  find_central_line(CvSeq* maxContour, IplImage* des);

	static void draw_duan_jian_result(IplImage* src_color, CvSeq* seq, cv::Point3f circle1, cv::Point3f circle0, IplImage* mask_img);
	static void draw_dash_line(IplImage* src_color, double k, double b, IplImage* mask_img);

	static void find_circle(IplImage* src_color_cut, IplImage* src_binary_cut_part);

	static void process_max_min_rect(CvMemStorage* memory, IplImage* src_color_cut, IplImage* src_gary_cut, CvSeq* seq, IplImage* src_binary_cut, cv::Point3f& circle0, cv::Point3f& circle1);
public:
	static void CannyAdaptiveFindThreshold(IplImage* pImg, double *low, double *high);
	static void HYAdaptiveFindThreshold(CvMat *dx, CvMat *dy, double *low, double *high);
public:
	static void HoughLine(IplImage* pImg, int *pR, int *pTh, int iThreshold, float rho_min, float rho_max, float theta_min, float theta_range);
	static void Draw_line_on_image(float rho, float theta, CvRect rect_cut, IplImage* iplimg_tmp, cv::Point& pt1, cv::Point& pt2);
public:
	static std::vector<float> crack_get_image_feature(IplImage *diff_org, std::string file_base, int frame_idx);
	static std::vector<float> crack_get_image_feature_gauss(IplImage *diff_org, std::string file_base, std::string file_name,int CIRCLE, int CHANNEL, int frame_idx, IplImage *image_out, std::vector<float>& delta_out,boolean SAVE_FLAG);
	
	static std::vector<float> crack_get_image_feature_one_line(std::string org);
	
	static void crack_get_long_crack(IplImage * diff_org,IplImage *image_4_delta,int delta_idx, std::vector<std::vector<CvPoint>>&   point_setsstring, std::string file_base,int CIRCLE ,int CHANNEL, int frame_idx,boolean SAVE_FLAG);
	static void VVC2Image(std::vector<std::vector<CvPoint>> vvc, IplImage * img);
	static void CRACK_get_block_sets(
		IplImage * image_binary,
		int TARGET,
		std::vector<std::vector<CvPoint>>& point_sets,
		boolean SAVE_FLAG);

	static std::vector<float> CRACK_get_histgram(
		IplImage * diff,
		std::vector<std::vector<CvPoint>> point_sets,
		int PN);

	static  std::vector<float> CRACK_get_histgram_feature(
		std::vector<float> histgram,
		int WIDTH,
		int  HEIGHT,
		int PN);

	static  std::vector<float> CRACK_get_block_feature(
		const std::vector<std::vector<CvPoint>> frame_point_sets,
		std::vector<std::vector<CvPoint>>& frame_point_sets_out,
		IplImage * diff,
		int PN);

	static 	void CRACK_get_block_property(
		IplImage * diff,
		std::vector<CvPoint> point_set,
		float& sum,
		unsigned long& count,
		int PN);
	
	static float CRACK_get_histgram_area(std::vector<float> histgram, int WIDTH, int HEIGHT, int PN);

	static std::vector<float> process_histogram(std::vector<float>& histogram, std::vector<std::vector<CvPoint>>&  point_sets, std::vector<float>& delta_out, int HISTOGRAM_DIM, int width, int height);
	
	static int GetLineProperty(std::vector<CvPoint> point_set, std::vector<float> delta, float& sum_delta, int& idx);
public:
	static int Save_Feature2TXT(std::vector<float> features,
		std::vector<INT32>  classifies,
		int feature_dimension,
		std::string _feature_name,
		std::string file_base);
public:

	static void Svm_Lean(std::vector<float> FeatureData,
		int FeatureDim,
		std::vector<INT32> FeatureClassify,
		int method,
		std::string path,
		std::string _module_xml);

	static void Svm_Lean_M(CvMat* _data_mat,
		CvMat* _classify_mat,
		int _method,
		std::string _path,
		std::string _module_xml);

	static void Svm_Lean_SAVE_MAT(CvMat* _data_mat,
		CvMat* _classify_mat,
		const std::string data_file,
		const std::string classify_file);

	static void Svm_Lean_SAVE_VECTOR(std::vector<float> FeatureData,
		int FeatureDim,
		std::vector<INT32> FeatureClassify,
		const std::string data_file,
		const std::string classify_file);

	static void Svm_Lean_F(std::string _featureFile,
						 std::string _classifyFile,
						int _method,
						std::string _path,
						std::string _module_xml);

	static int Svm_Lean_Predict(CvSVM &_svm,
		std::vector<float> _feature);

	static int Svm_Lean_Save_Feature2XML(CvMat* _feature,
		CvMat* _classify,std::string _filename);

	
	static int Svm_Lean_Save_Feature2TXT(std::vector<float> features,
		std::vector<INT32>  classifies,
		int feature_dimension,
		std::string file_base);
	
	static float Svm_Lean_Precision(int tp,int fp,int fn);
	static float Svm_Lean_Recall(int tp, int fp, int fn);
	static float Svm_Lean_SAVE_Recall_Precision(int tp, int fp, int fn, std::string _full_path);
	
	static int SampleCalPos(int& _tp, int& _fn,int _sample,int _result);
	static int SampleCalNeg(int& _tn, int& _fp, int _sample, int _result);

public:
	static int Hough_Line_CV_STANDARD_TEST(CvArr* image,
		IplImage *color_dst,
		double rho=1,
		double theta=CV_PI/180,
		int threshold=150);

	static int Hough_Line_CV_PROBABILISTIC_TEST(CvArr* image,
		IplImage *color_dst,
		double rho=1,
		double theta=CV_PI/180,
		int threshold=150,
		double _lineLenMin = 10,
		double _lineGap = 2);

	static std::vector<Line2Point> Hough_Line_CV_PROBABILISTIC_GetLine(CvArr* image,
		double rho = 1,
		double theta = CV_PI / 180,
		int threshold = 150,
		double _lineLenMin = 10,
		double _lineGap = 2);
	
	static void LineCalAngle(std::vector<Line2Point>& _lines);

	static std::vector<Line2Point> LineRemove(
					std::vector<Line2Point>& _lines,
					double _theta_c,
					double _theta_r);

	static void LineDraw(std::vector<Line2Point>& _lines,
		IplImage * img_t,
		CvScalar _color= CV_RGB(255, 0, 0),
		int _thickness=1);

	static std::vector<Line2Point> getHoughLines(IplImage* _img_bin,
		IplImage* _img_draw = nullptr,
		CvScalar _color = CV_RGB(255, 0, 0),
		int _thickness = 3);

	static void IMG_RemoveBorder(IplImage* _img_bin,std::vector<int> row,std::vector<int> col);

public:
	static void CuiResize(IplImage * src, IplImage * dst, const int m_step, const int n_step);
public:
	static void  DrawHistogram(float *data, int size, std::string file_base, int CHANNEL, int frame_idx, std::vector<float> feature);
	static void  DrawHistogram_fromImage(IplImage * img, std::string file_base, int CIRCLE, int CHANNEL, int frame_idx, int ColIdx, int hist_bar_height=255, int hist_bar_width=1);
	static void  DrawBox(CvBox2D rect, IplImage * _img, const CvScalar color=CV_RGB(255,0,0));
public:
	static void  SaveArray2Disk(float *data, int size,int channel_t, int frame_count, std::string file_base);
	
	static void  Opencv_SaveVector2CvMatrix(std::string file_name, std::vector<float> vf);

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
	public:
		static int CRACK_MAX_SIZE;
		static int CRACK_MAX_WIDTH;
public:
	static int GetOneColumn(IplImage *image, IplImage* ColData,int IdxCol);
public:
	static void VIDEO_GetWidthHeight(std::string video_full_path,int* WIDTH,int* HEIGHT);
};