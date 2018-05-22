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
	string pn_base;
	string org_gray_base;
	string org_gray_feature;
	string pn_sigma_base;
public:
	vector<float> feature_p1;
	vector<float> feature_n1;
	vector<vector<CvPoint>> frame_point_sets_out_p1;
	vector<vector<CvPoint>> frame_point_sets_out_n1;
public:
	void Process(IplImage* _image_gray, IplImage* avg, IplImage* sigma_scale_m);

	void Process2stepP(IplImage* diff, IplImage* sigma_scale_m);
	void Process2stepN(IplImage* diff, IplImage* sigma_scale_m);

	void SaveB(IplImage* _image_gray);
	static void cvThresholdMY(IplImage* src, IplImage* dst, const float T, const float MAX, const int RULE);
	static void SaveImage2Disk(string file_base, int CIRCLE, int CHANNEL, int FRAME, string ImageType, string MatrixType, IplImage* img);
	static void SaveMat2Disk(string file_base, int CIRCLE, int CHANNEL, int FRAME, string ImageType, string MatrixType, vector<float> vf);
	static void SaveVectorFloat2Disk(string file_base, int CIRCLE, int CHANNEL, int FRAME, string ImageType, string MatrixType, vector<vector<CvPoint>> vvc, int WIDTH, int HEIGHT);
private:
	void release();
};

