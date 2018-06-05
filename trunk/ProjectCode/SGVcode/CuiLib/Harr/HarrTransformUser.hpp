#pragma once
/*---------------------------------------------------*/
#include <platform_cui.h>
/*---------------------------------------------------*/
#include <SGVcode/cui_GeneralImgProcess.h>
/*---------------------------------------------------*/
#include "SGVcode/ImageType.h"
/*---------------------------------------------------*/
/**
*����ʵ�ֶ�ͼ���С���任����任
*-��ʵ��һ������С���任����������\n
*-����С���任�����˲���ʵ��ƽ������
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

