 #pragma once

#include "opencv_stl.h"

#include "Line2Point.hpp"

#include "BlockProperty.hpp"

#include "ImageProcess.h"

class BlockAnalysis
{
public:
	BlockAnalysis();
	BlockAnalysis(IplImage *_img_bin);
	~BlockAnalysis(void);
private:
	void init();
	void init_param();
	void create_memory();
	void release_memory();
public:
	int FindContoursIn();
	int FindContours(IplImage * _img_bin);
	void GetAllBlockProperty(int _method);
	void GetAllBlockProperty();
	void GetAllBlockProperty_Area();
	
	BlockProperty GetMaxBlock();

	CvBox2D GetMaxBlockBox();
	CvRect  GetMaxBlockRect();
	void analyzeAllBlock();

	std::vector<float>	GetMaxBlockProperty();
	std::vector<float>	GetAllBlockFeature();
	
	int ContoursNum();

public:
	void DrawContoursAll(IplImage* _img, int _thickness=1);
	void DrawMaxContour(IplImage* _img, int _thickness = 1,int _max_level=0);
	void DrawMaxContourBlock(IplImage * _img);
	void DrawBox(IplImage * _img, CvBox2D _box, const CvScalar _color=CV_RGB(255,255,255), int _thickness=1);
public:
	std::vector<float>  getFeatureMaxBlockAll(IplImage* _img_bin,
		IplImage* _img_draw = nullptr,
		const CvScalar _color=CV_RGB(255,255,255),
		const int _thickness=1);

	static std::vector<float> getFeatureBlock(IplImage* _img_bin,
		IplImage* _img_draw=nullptr,
		const CvScalar _color=CV_RGB(255,255,255),
		const int _thickness=1);
private:
	CvMemStorage * mStorage;    //提取轮廓需要的储存容量 0为默认64KB
	CvScalar mExternalColor;
	CvScalar mHoleColor;
	CvSeq * m_pContourHeader;  //提取轮廓的序列指针	
	int mMode;
	int mMethod;
	int mContoursNum;//提取轮廓的数目
	int mMaxAreaBlockIdx;
	int mAllPixelNums;
	static float  ellipseEccentricity(const CvSize2D32f& _size, float _default);
private:
	std::vector<BlockProperty> mBlocksProperty;
private:

	IplImage* mImgBinary;
	
};