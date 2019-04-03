 #pragma once

#include "opencv_stl.h"

class BlockProperty
{
public:
	BlockProperty(CvSeq * _pContour);
	~BlockProperty(void);
private:
	CvSeq * m_pContour;  //提取轮廓的序列指针	
	float mArcLen;
	float mArea;
	CvRect mRect;
	CvBox2D mBox;
private:
	void init_param();
public:
	float Area() const;
	CvSeq * Contour();  //提取轮廓的序列指针	
	CvBox2D Box();
	CvRect Rect();
	void GetFeatureAll();
	void GetFeatureArea();
	CvBox2D GetMinAreaRect();
	CvRect  getBoundingRect();
};