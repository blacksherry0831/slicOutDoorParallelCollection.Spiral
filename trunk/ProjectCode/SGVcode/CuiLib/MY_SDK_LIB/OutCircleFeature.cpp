#include "StdAfx.h"
#include "OutCircleFeature.hpp"

/*-----------------------------------------*/
/**
*
*/
/*-----------------------------------------*/
OutCircleFeature::OutCircleFeature(int _width,int _height)
{  
	this->WIDTH = _width;
	this->HEIGHT = _height;
	image_gray_f = cvCreateImage(cvSize(WIDTH, HEIGHT), IPL_DEPTH_32F, 1);
	calc_temp_f = cvCreateImage(cvSize(WIDTH, HEIGHT), IPL_DEPTH_32F, 1);
	calc_temp_f_n = cvCreateImage(cvSize(WIDTH, HEIGHT), IPL_DEPTH_32F, 1);
	calc_temp_f_p = cvCreateImage(cvSize(WIDTH, HEIGHT), IPL_DEPTH_32F, 1);
	image_gray_binary_P = cvCreateImage(cvSize(WIDTH, HEIGHT), IPL_DEPTH_8U, 1);
	image_gray_binary_N = cvCreateImage(cvSize(WIDTH, HEIGHT), IPL_DEPTH_8U, 1);
	this->SAVE_FLAG=FALSE;
}  
/*-----------------------------------------*/
/**
*
*/
/*-----------------------------------------*/
OutCircleFeature::~OutCircleFeature(void)
{
	this->release();
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
void OutCircleFeature::release()
{
	
	cvReleaseImage(&image_gray_f);
	cvReleaseImage(&calc_temp_f);
	cvReleaseImage(&calc_temp_f_n);
	cvReleaseImage(&calc_temp_f_p);
	cvReleaseImage(&image_gray_binary_P);
	cvReleaseImage(&image_gray_binary_N);
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
void OutCircleFeature::Process(IplImage* _image_gray,IplImage* avg, IplImage* sigma_scale_m)
{

	vector<vector<CvPoint>> frame_point_sets;
	cvConvert(_image_gray, image_gray_f);
	//正值二值化，负值二值化;

	cvSub(image_gray_f, avg, calc_temp_f);
		
	{
		const int PN = 1;
		cvSub(calc_temp_f, sigma_scale_m, calc_temp_f_p);//大于0，有效点
		cvThresholdMY(calc_temp_f_p, image_gray_binary_P, COMP_MAX, TARGET_P, CV_THRESH_BINARY);//>0
		ImageProcess::CRACK_get_block_sets(image_gray_f, avg, image_gray_binary_P, SIGMA, TARGET_P, CIRCLE, CHANNEL, frame_count, frame_point_sets, "", PN);
		feature_p1 = ImageProcess::CRACK_get_block_feature(frame_point_sets, frame_point_sets_out_p1, calc_temp_f, PN);
	}

	{
		const int PN = -1;
		cvAdd(calc_temp_f, sigma_scale_m, calc_temp_f_n);//小于0，有效点
		cvThresholdMY(calc_temp_f_n, image_gray_binary_N, COMP_MAX, TARTET_N, CV_THRESH_BINARY_INV);//<0
		ImageProcess::CRACK_get_block_sets(image_gray_f, avg, image_gray_binary_N, SIGMA, TARTET_N, CIRCLE, CHANNEL, frame_count, frame_point_sets, "", PN);
		feature_n1 = ImageProcess::CRACK_get_block_feature(frame_point_sets, frame_point_sets_out_n1, calc_temp_f, PN);
	}

}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
void OutCircleFeature::SaveB(IplImage* _image_gray)
{
#if _DEBUG
	if (SAVE_FLAG) {
	
		SaveImage2Disk(pn_base, CIRCLE, CHANNEL, frame_count, "neg", "bin", image_gray_binary_N);
		SaveImage2Disk(pn_base, CIRCLE, CHANNEL, frame_count, "pos", "bin", image_gray_binary_P);
		SaveImage2Disk(org_gray_base, CIRCLE, CHANNEL, frame_count, "pos", "gary", _image_gray);
	}

	if (SAVE_FLAG) {
		SaveMat2Disk(org_gray_feature, CIRCLE, CHANNEL, frame_count, "pos", "mat", feature_p1);
		SaveVectorFloat2Disk(pn_sigma_base, CIRCLE, CHANNEL, frame_count, "pos", "binsigma", frame_point_sets_out_p1, WIDTH, HEIGHT);
	}

	if (SAVE_FLAG) {
		SaveMat2Disk(org_gray_feature, CIRCLE, CHANNEL, frame_count, "neg", "float", feature_n1);
		SaveVectorFloat2Disk(pn_sigma_base, CIRCLE, CHANNEL, frame_count, "neg", "binsigma", frame_point_sets_out_n1, WIDTH, HEIGHT);
	}
#endif // _DEBUG
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
void OutCircleFeature::cvThresholdMY(IplImage* src, IplImage* dst, const float T, const float MAX, const int RULE)
{

	const int WIDTH_COL = src->width;
	const int HEIGHT_ROW = src->height;

	for (size_t ci = 0; ci <WIDTH_COL; ci++) {
		for (size_t ri = 0; ri <HEIGHT_ROW; ri++) {
			const float data_src = cvGetReal2D(src, ri, ci);
			float data_dst;

			if (RULE == CV_THRESH_BINARY) {
				data_dst = data_src > T ? MAX : 0;
			}
			else if (RULE == CV_THRESH_BINARY_INV) {
				data_dst = data_src < T ? MAX : 0;
			}
			else {

			}

			cvSetReal2D(dst, ri, ci, data_dst);

		}
	}
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
void OutCircleFeature::SaveImage2Disk(string file_base, int CIRCLE, int CHANNEL, int FRAME, string ImageType, string MatrixType, IplImage* img)
{
	string ImageName;

	ImageName.append("circle").append(Base::int2str(CIRCLE)).append(".")
		.append("ch").append(Base::int2str(CHANNEL)).append(".")
		.append("frame").append(Base::int2str(FRAME)).append(".")
		.append(ImageType).append(".")
		.append(MatrixType).append(".")
		.append("png");

	const string ColSavePath_full = file_base + ImageName;//列号 作为文件名

	cvSaveImage(ColSavePath_full.c_str(), img);//在"H-S Histogtam"窗口中显示图像
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
void  OutCircleFeature::SaveMat2Disk(string file_base, int CIRCLE, int CHANNEL, int FRAME, string ImageType, string MatrixType, vector<float> vf)
{
	string ImageName;

	ImageName.append("circle").append(Base::int2str(CIRCLE)).append(".")
		.append("ch").append(Base::int2str(CHANNEL)).append(".")
		.append("frame").append(Base::int2str(FRAME)).append(".")
		.append(ImageType).append(".")
		.append(MatrixType).append(".")
		.append("xml");

	const string ColSavePath_full = file_base + ImageName;//列号 作为文件名

	ImageProcess::Opencv_SaveVector2CvMatrix(ColSavePath_full, vf);

}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
void OutCircleFeature::SaveVectorFloat2Disk(string file_base, int CIRCLE, int CHANNEL, int FRAME, string ImageType, string MatrixType, vector<vector<CvPoint>> vvc, int WIDTH, int HEIGHT)
{

	IplImage* img = cvCreateImage(cvSize(WIDTH, HEIGHT), IPL_DEPTH_8U, 1);


	ImageProcess::VVC2Image(vvc, img);

	OutCircleFeature::SaveImage2Disk(file_base, CIRCLE, CHANNEL, FRAME, ImageType, MatrixType, img);

	cvReleaseImage(&img);
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/