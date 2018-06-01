#pragma once
/*-----------------------------------------*/
/**
*
*/
/*-----------------------------------------*/
#include "cpp_stl.h"
/*-----------------------------------------*/
/**
*
*/
/*-----------------------------------------*/
#include "Base.h"
#include "ImageProcess.h"
/*-----------------------------------------*/
/**
*
*/
/*-----------------------------------------*/
class OutCircleFeature
{
public:
	OutCircleFeature(int _width, int _height);
	~OutCircleFeature(void);
private:
	int WIDTH;
	int HEIGHT;
private:	
	IplImage* image_gray_f;
	IplImage* calc_temp_f;
	IplImage* calc_temp_f_n;
	IplImage* calc_temp_f_p;
	IplImage* image_gray_binary_P;
	IplImage* image_gray_binary_N;
public:
	const int TARGET_P = 255;
	const int TARTET_N = 128;
	const float COMP_MAX = 0;
public:
	int CIRCLE;
	int CHANNEL;
	int SIGMA;
	int frame_count;
	int SAVE_FLAG;
	std::string pn_base;
	std::string org_gray_base;
	std::string org_gray_feature;
	std::string pn_sigma_base;
public:
	std::vector<float> feature_p1;
	std::vector<float> feature_n1;
	std::vector<std::vector<CvPoint>> frame_point_sets_out_p1;
	std::vector<std::vector<CvPoint>> frame_point_sets_out_n1;
public:
	void Process(IplImage* _image_gray, IplImage* avg, IplImage* sigma_scale_m);

	void Process2stepP(IplImage* diff, IplImage* sigma_scale_m);
	void Process2stepN(IplImage* diff, IplImage* sigma_scale_m);

	void SaveB(IplImage* _image_gray);
	static void cvThresholdMY(IplImage* src, IplImage* dst, const float T, const float MAX, const int RULE);
	static void SaveImage2Disk(std::string file_base, int CIRCLE, int CHANNEL, int FRAME, std::string ImageType, std::string MatrixType, IplImage* img);
	static void SaveMat2Disk(std::string file_base, int CIRCLE, int CHANNEL, int FRAME, std::string ImageType, std::string MatrixType, std::vector<float> vf);
	static void SaveVectorFloat2Disk(std::string file_base, int CIRCLE, int CHANNEL, int FRAME, std::string ImageType, std::string MatrixType, std::vector<std::vector<CvPoint>> vvc, int WIDTH, int HEIGHT);
private:
	void release();
};

