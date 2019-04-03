#include "StdAfx.h"
#include "Base.h"
#include "BlockAnalysis.hpp"
/*-----------------------------------------*/
/**
*
*/
/*-----------------------------------------*/
#ifdef max
#undef max
#endif // max
/*-----------------------------------------*/
#ifdef min
#undef min
#endif // max
/*-----------------------------------------*/
/**
*
*/
/*-----------------------------------------*/
BlockAnalysis::BlockAnalysis(void)
{
	this->init();
	mImgBinary = nullptr;

}
/*-----------------------------------------*/
/**
*
*/
/*-----------------------------------------*/
BlockAnalysis::~BlockAnalysis(void)
{
	this->release_memory();
}
/*-----------------------------------------*/
/**
*
*/
/*-----------------------------------------*/
BlockAnalysis::BlockAnalysis(IplImage *_img_bin)
{
	this->init();

	assert(	_img_bin->nChannels==1 &&
			_img_bin->width%4==0);

	mImgBinary = cvCloneImage(_img_bin);

}
/*-----------------------------------------*/
/**
*
*/
/*-----------------------------------------*/
void BlockAnalysis::create_memory()
{
	mStorage = cvCreateMemStorage(0);
}
/*-----------------------------------------*/
/**
*
*/
/*-----------------------------------------*/
void BlockAnalysis::release_memory()
{
	cvReleaseMemStorage(&mStorage);
	cvReleaseImage(&mImgBinary);
	mBlocksProperty.clear();
}
/*-----------------------------------------*/
/**
*
*/
/*-----------------------------------------*/
int BlockAnalysis::FindContoursIn()
{
	
	return mContoursNum = FindContours(mImgBinary);
	
}
/*-----------------------------------------*/
/**
*
*/
/*-----------------------------------------*/
int BlockAnalysis::FindContours(IplImage * _img_bin)
{
	return mContoursNum = cvFindContours(_img_bin, mStorage, &m_pContourHeader, sizeof(CvContour), mMode, mMethod);
}
/*-----------------------------------------*/
/**
*
*/
/*-----------------------------------------*/
void BlockAnalysis::GetAllBlockProperty(int _method)
{
	CvSeq * pContour = m_pContourHeader;  //提取轮廓的序列指针
										  //--------------画轮廓----------------
	if (mContoursNum >= 1) {
		for (; pContour != 0; pContour = pContour->h_next)
		{

			if (CV_IS_SEQ_CLOSED(pContour)) {
				BlockProperty bp(pContour);
				
				if (0==_method)
				{
					bp.GetFeatureAll();
				}
				else if(1==_method)
				{
					bp.GetFeatureArea();
				}
				
				mBlocksProperty.push_back(bp);
			}
			else {
				ASSERT(0);
			}
		}
	}

}
/*-----------------------------------------*/
/**
*
*/
/*-----------------------------------------*/
void BlockAnalysis::GetAllBlockProperty()
{
	GetAllBlockProperty(0);
}
/*-----------------------------------------*/
/**
*
*/
/*-----------------------------------------*/
void BlockAnalysis::GetAllBlockProperty_Area()
{
	GetAllBlockProperty(1);
}
/*-----------------------------------------*/
/**
*
*/
/*-----------------------------------------*/
BlockProperty BlockAnalysis::GetMaxBlock()
{

	if (mMaxAreaBlockIdx < 0 )
	{
		BlockProperty bp_max_t(nullptr);
		return bp_max_t;		
	}
	else
	{
		return mBlocksProperty[mMaxAreaBlockIdx];
	}
	
}
/*-----------------------------------------*/
/**
*
*/
/*-----------------------------------------*/
CvBox2D BlockAnalysis::GetMaxBlockBox()
{

	if (mMaxAreaBlockIdx < 0)
	{
			BlockProperty bp_max_t(nullptr);
			return bp_max_t.Box();	
	}
	else
	{

		return mBlocksProperty[mMaxAreaBlockIdx].GetMinAreaRect();
	}

}
/*-----------------------------------------*/
/**
*
*/
/*-----------------------------------------*/
CvRect BlockAnalysis::GetMaxBlockRect()
{
	if (mMaxAreaBlockIdx < 0)
	{
		BlockProperty bp_max_t(nullptr);
		return bp_max_t.Rect();
	}
	else
	{

		return mBlocksProperty[mMaxAreaBlockIdx].getBoundingRect();
	}
}
/*-----------------------------------------*/
/**
*
*/
/*-----------------------------------------*/
void BlockAnalysis::analyzeAllBlock()
{
	BlockProperty bp_max_t(nullptr);

	mAllPixelNums = 0;

	for (size_t i = 0; i < mBlocksProperty.size(); i++) {

		const BlockProperty& bp_t = mBlocksProperty[i];
		const float area_t = bp_t.Area();
		mAllPixelNums += area_t;

		if (area_t >= bp_max_t.Area()) {
			bp_max_t = bp_t;
			mMaxAreaBlockIdx = i;
		}


	}



}
/*-----------------------------------------*/
/**
*
*/
/*-----------------------------------------*/
std::vector<float> BlockAnalysis::GetMaxBlockProperty()
{
	BlockProperty bp_t=GetMaxBlock();
	
	std::vector<float> bp_v_t;
	
	bp_v_t.push_back(bp_t.Area());

	bp_v_t.push_back(ellipseEccentricity(bp_t.Box().size,0));
	
	return bp_v_t;
}
/*-----------------------------------------*/
/**
*
*/
/*-----------------------------------------*/
std::vector<float> BlockAnalysis::GetAllBlockFeature()
{
	std::vector<float> feature_t = GetMaxBlockProperty();

	feature_t.push_back(mAllPixelNums);

	return  feature_t;
}
/*-----------------------------------------*/
/**
*
*/
/*-----------------------------------------*/
void BlockAnalysis::DrawContoursAll(IplImage* _img,int _thickness)
{
	CvSeq * pContour_t = m_pContourHeader;  //提取轮廓的序列指针
	const int max_level = 1;//绘制轮廓及在其后的相同的级别下轮廓
	for (; pContour_t != 0; pContour_t = pContour_t->h_next)
	{
		cvDrawContours(_img, pContour_t,mExternalColor, mHoleColor, max_level, _thickness);
	}
}
/*-----------------------------------------*/
/**
*
*/
/*-----------------------------------------*/
void BlockAnalysis::DrawMaxContour(IplImage * _img,int _thickness,int _max_level)
{

	const int max_level = 0;//如果等级为0，绘制单独的轮廓。
	if (mMaxAreaBlockIdx!=-1){
		cvDrawContours(_img,mBlocksProperty[mMaxAreaBlockIdx].Contour(), mExternalColor, mHoleColor, _max_level, _thickness);
	}

}
/*-----------------------------------------*/
/**
*
*/
/*-----------------------------------------*/
void BlockAnalysis::DrawMaxContourBlock(IplImage * _img)
{

	DrawMaxContour(_img, CV_FILLED);

}
/*-----------------------------------------*/
/**
*
*/
/*-----------------------------------------*/
void BlockAnalysis::DrawBox(IplImage * _img,CvBox2D _box, const CvScalar _color, int _thickness)
{
		ImageProcess::DrawBox(_box, _img,_color);
}
/*-----------------------------------------*/
/**
*
*/
/*-----------------------------------------*/
std::vector<float> BlockAnalysis::getFeatureMaxBlockAll(IplImage * _img_bin, 
	IplImage * _img_draw,
	const CvScalar _color,
	const int _thickness)
{
	std::vector<float> f_t;

	this->FindContours(_img_bin);
	this->GetAllBlockProperty_Area();
	this->analyzeAllBlock();

	CvBox2D box_t = this->GetMaxBlockBox();
	CvRect  rect_t = this->GetMaxBlockRect();


	if (this->ContoursNum()>0) {
		if (_img_draw != nullptr)
		{
			this->DrawContoursAll(_img_draw, CV_FILLED);
			this->DrawBox(_img_draw, box_t,_color,_thickness);
#if 0
			cvRectangleR(_img_draw, rect_t, CV_RGB(255, 255, 255), thickness);
#endif // 0

		}

	}

	f_t = this->GetAllBlockFeature();

	return f_t;
}
/*-----------------------------------------*/
/**
*
*/
/*-----------------------------------------*/
std::vector<float> BlockAnalysis::getFeatureBlock(IplImage * _img_bin, IplImage * _img_draw, const CvScalar _color,const int _thickness)
{	

	BlockAnalysis ba;
	
	std::vector<float> f_t = ba.getFeatureMaxBlockAll(_img_bin,_img_draw);

	return f_t;

}
/*-----------------------------------------*/
/**
*
*/
/*-----------------------------------------*/
float BlockAnalysis::ellipseEccentricity(const CvSize2D32f& _size,float _default)
{
	const  float  a = std::max(_size.width / 2, _size.height / 2);
	
	const  float  b = std::min(_size.width / 2, _size.height / 2);

	assert(a >= 0);

	if (a==0){

		return	 _default;

	}else {
				const float c = sqrt(a*a - b*b);

				const float e = c / a;
				assert(e >= 0 && e <= 1);
				return e;
	
	}

}
/*-----------------------------------------*/
/**
*
*/
/*-----------------------------------------*/
void BlockAnalysis::init()
{
	this->init_param();
	this->create_memory();
}
/*-----------------------------------------*/
/**
*
*/
/*-----------------------------------------*/
void BlockAnalysis::init_param()
{
	const int ALPHA = 255;
	mMode = CV_RETR_EXTERNAL;
	mMethod = CV_CHAIN_APPROX_NONE;
	mHoleColor = cvScalar(ALPHA,0,255,255);
	mExternalColor = cvScalar(ALPHA,255,255,0);
	mMaxAreaBlockIdx =-1;
	mAllPixelNums = 0;
}
/*-----------------------------------------*/
/**
*
*/
/*-----------------------------------------*/
int BlockAnalysis::ContoursNum()
{
	return mContoursNum;
}
/*-----------------------------------------*/
/**
*
*/
/*-----------------------------------------*/