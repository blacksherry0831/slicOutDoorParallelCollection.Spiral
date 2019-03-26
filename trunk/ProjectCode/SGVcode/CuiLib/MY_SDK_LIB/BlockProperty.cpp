#include "StdAfx.h"
#include "Base.h"
#include "BlockProperty.hpp"

/*-----------------------------------------*/
/**
*
*/
/*-----------------------------------------*/
BlockProperty::BlockProperty(CvSeq * _pContour)
{
	this->m_pContour = _pContour;
	this->init_param();
}
/*-----------------------------------------*/
/**
*
*/
/*-----------------------------------------*/
BlockProperty::~BlockProperty(void)
{
	
}
/*-----------------------------------------*/
/**
*
*/
/*-----------------------------------------*/
void BlockProperty::init_param()
{
	mArcLen=0;
	mArea=0;
	memset(&mBox, 0, sizeof(CvBox2D));
}
/*-----------------------------------------*/
/**
*
*/
/*-----------------------------------------*/
float BlockProperty::Area() const
{
	return mArea;
}
/*-----------------------------------------*/
/**
*
*/
/*-----------------------------------------*/
CvSeq * BlockProperty::Contour()
{
	return m_pContour;
}
/*-----------------------------------------*/
/**
*
*/
/*-----------------------------------------*/
CvBox2D BlockProperty::Box()
{
	return mBox;
}
/*-----------------------------------------*/
/**
*
*/
/*-----------------------------------------*/
void BlockProperty::GetFeatureAll()
{

	const CvSeq* pContour_t=m_pContour;

	if (CV_IS_SEQ_CLOSED(pContour_t)) {
		
		mArcLen += cvArcLength(pContour_t);
		mArea += cvContourArea(pContour_t);
		GetMinAreaRect();
		
	}
	else {
		ASSERT(0);
	}

}
/*-----------------------------------------*/
/**
*
*/
/*-----------------------------------------*/
void BlockProperty::GetFeatureArea()
{

	const CvSeq* pContour_t = m_pContour;

	if (CV_IS_SEQ_CLOSED(pContour_t)) {
		
		mArea += cvContourArea(pContour_t);
		
	}
	else {
		ASSERT(0);
	}

}
/*-----------------------------------------*/
/**
*
*/
/*-----------------------------------------*/
CvBox2D BlockProperty::GetMinAreaRect()
{
	//CvMemStorage* storage = cvCreateMemStorage();
	
	mBox= cvMinAreaRect2(m_pContour);

	//cvReleaseMemStorage(&storage);

	return mBox;
}
/*-----------------------------------------*/
/**
*
*/
/*-----------------------------------------*/