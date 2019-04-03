#pragma once

#include "opencv_stl.h"

#include "Line2Point.hpp"

class CrackDetection
{
public:
	CrackDetection(void);
	~CrackDetection(void);
private:
	IplImage* mImgOrgGray;
	IplImage* mImgBinary;
	IplImage* mImgBinaryLink;
	int mWidth;
	int mHeight;
	int mSpiralMatrix;
	int mNoiseTh;
	int mSmallBlock;
	std::string mImgBinaryFileName;
	std::string mImgBinaryLinkFileName;
	std::string mImgBinarySmoothFileName;
private:
	void MallocImage(CvSize _size_t);
public:
	void BinaryOneColumn(IplImage * image, IplImage * ColData, int IdxCol);

	static std::vector<float> getThresholdInCircleBlock();

	static std::vector<float> getThresholdOutCircleBlock();

	static std::vector<float> getThresholdInOutCircleBlock(std::string _in_out);

	static std::vector<float> getThresholdInCircleHoughBlock();

	static std::vector<float> getThresholdOutCircleHoughBlock();

	static std::vector<float> getThresholdInOutCircleHoughBlock(std::string _in_out);
	
	void LoadImg(std::string _file_name);

	void FillImgP2P(IplImage* _img_spiral_matrix,CvPoint _pt1,CvPoint _pt_cennter, int _matrix_size);
	int GetImgNum(IplImage* _img_spiral_matrix,const int _pixel_data);
	void FillMatrixImg2BinaryLink(IplImage * _img_spiral_matrix, IplImage * _img_bin_link, CvPoint _center_p,const int _matrix_size);

	std::vector<CvPoint> init_spiral_matrix(int _matrix_size);
	int cal_Noise_num(CvPoint _center_pt,std::vector<CvPoint> _spiral_point,IplImage* _img_bin,const int _spiral_matrix);
public:
	static const int POS_CLASS = 1;
	static const int NEG_CLASS = -1;
	static const std::string  CRACK_OUT;
	static const std::string  CRACK_IN;
public:
	void Cvt2Binary();
	void Cvt2Binary3Sigma();
	void Binary2Link();
	void Binary2Smooth();
	void RemoveConnectivity(IplImage* _img,int _size_remove);
	
	static void IMG_RemoveBorder(IplImage * _img_bin, std::vector<int> _row, std::vector<int> _col);
	static void Init_RemoveBorder(const int WIDTH, const int HEIGHT, std::vector<int>& _row, std::vector<int>& _col);

	void Binary2Noise(const int _matrix_size_out,const int _matrix_size_in,const int _c_step);
	void Binary2NoiseAdj(const int _matrix_size_out, const int _matrix_size_in);

	void CopyBinary2Link();

	std::vector<float> GetFeature(const int _matrix_size_out, const int _matrix_size_in);

	static std::vector<float> GetCrackImgHoughBlockFeatureBase(
		IplImage* _img_bin,
		IplImage* _img_block,
		std::vector<Line2Point>& _lines,
		IplImage* _img_output = nullptr,
		const CvScalar _color= CV_RGB(255, 255, 255),
		const int _thickness=3);

	static int predict_block(std::vector<float> _f, std::vector<float> _th);
	
	static int predict_in_out_block(
		std::vector<float> _f,
		std::string _in_out,
		std::vector<float> _th);

public:

	static void Gray2NoiseFast(IplImage* _img);
	static void Binary2Smooth_Line(IplImage* _img);

	static std::vector<float> GetFeatureFast(const IplImage* _img_binary);
	static std::vector<float> GetFeatureFastEx(IplImage* _img_binary, int _matrix_size_out, const int _matrix_size_in, const int _c_step);


};

