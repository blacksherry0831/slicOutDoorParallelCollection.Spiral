#pragma once

#include "opencv_stl.h"

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
	
	void LoadImg(std::string _file_name);

	void FillImgP2P(IplImage* _img_spiral_matrix,CvPoint _pt1,CvPoint _pt_cennter, int _matrix_size);
	int GetImgNum(IplImage* _img_spiral_matrix,const int _pixel_data);
	void FillMatrixImg2BinaryLink(IplImage * _img_spiral_matrix, IplImage * _img_bin_link, CvPoint _center_p,const int _matrix_size);

	std::vector<CvPoint> init_spiral_matrix(int _matrix_size);
	int cal_Noise_num(CvPoint _center_pt,std::vector<CvPoint> _spiral_point,IplImage* _img_bin,const int _spiral_matrix);
public:
	void Cvt2Binary();
	void Cvt2Binary3Sigma();
	void Binary2Link();
	void Binary2Smooth();
	void RemoveConnectivity(IplImage* _img,int _size_remove);



};

