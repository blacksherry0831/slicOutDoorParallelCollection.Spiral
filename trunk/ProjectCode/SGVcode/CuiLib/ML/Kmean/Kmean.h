#pragma once
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/
#include "module_all_cui.h"
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/
#ifndef  KM_ShowImage
#define  KM_ShowImage   (FALSE)
#endif

/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/
#define  KMean_Method_BLOCK        (1)
#define  KMean_Method_ORGColor     (2)
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/
class Kmean
{
public:
	ImageMemData* pMD;
	IplImage* img3ch;
	IplImage* imgmask;
	IplImage* mainColor;
public:
	CvMat *lablesMat;//分类后的矩阵
	CvMat *points3ch;//分类前的样例浮点矩阵
	vector<CvPoint> pointsPos;
	IplImage* lablesImg;
	vector<LabColor>  ColorCategory;
public:
	Kmean(IplImage *img,ImageMemData* pMD,IplImage* imgmask=nullptr);
	~Kmean(void);
private:
	void  FillAreaInOrgImg(int NumClusters,int method);
	void  FillPointsMat(void);
	int NumCluster;
	bool ChangetoLab;
public:
	void ChangeColor2LabModule();
	void GetImgLables(int NumClusters,int method);
	void ShowImg(void);
	LabColor  GetMaxAreaClassifications();
	unsigned long GetEffectiveColorNumber();
};
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/