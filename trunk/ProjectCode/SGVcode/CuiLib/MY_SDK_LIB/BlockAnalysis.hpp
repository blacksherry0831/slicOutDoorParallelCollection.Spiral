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
	void init_param();
	void create_memory();
	void release_memory();
public:
	void FindContours();
	void GetAllBlockProperty(int _method);
	void GetAllBlockProperty();
	void GetAllBlockProperty_Area();
	
	BlockProperty GetMaxBlock();

	CvBox2D GetMaxBlockBox();

	void analyzeAllBlock();

	std::vector<float>	GetMaxBlockProperty();
	std::vector<float>	GetAllBlockFeature();
	
	int ContoursNum();

public:
	void DrawContoursAll(IplImage* _img, int _thickness=1);
	void DrawMaxContour(IplImage* _img, int _thickness = 1);
	void DrawMaxContourBlock(IplImage * _img);
	void DrawBox(IplImage * _img, CvBox2D _box, int _thickness=1);
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