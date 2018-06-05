#pragma once
/*---------------------------------------------------*/
#include <platform_cui.h>
/*---------------------------------------------------*/
#include <SGVcode/cui_GeneralImgProcess.h>
/*---------------------------------------------------*/
#include "SGVcode/ImageType.h"
/*---------------------------------------------------*/
/**
*本类实现对图像的小波变换和逆变换
*-可实现一、二层小波变换的能量计算\n
*-利用小波变换进行滤波，实现平滑处理
*
*/
/*---------------------------------------------------*/
class HarrTransformUser
{
public:
	HarrTransformUser(void);
	~HarrTransformUser(void);
private:
	IplImage* img_org_bgra;
	IplImage* img_org_gray;
	int mWIDTH;
	int mHEIGHT;
public:
	void setImageData(IplImage *_img_bgra);
	float CalculateEnergy(int _col_step);
	static float PixelSub(IplImage *_img_gary, int ci, int ri, int cj, int rj);
	static float PixelColSub(IplImage *_img_gary, int ci,  int cj,int ri);
	static float CalDiffEnergy(IplImage *_img_gary);
};

