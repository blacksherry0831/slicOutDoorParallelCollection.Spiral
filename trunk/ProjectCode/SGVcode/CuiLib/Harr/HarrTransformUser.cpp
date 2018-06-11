#include "stdafx.h"
/*---------------------------------------------------*/
#include "HarrTransformUser.hpp"
/*---------------------------------------------------*/
/**
*初始化成员变量
*6
*/
/*---------------------------------------------------*/
HarrTransformUser::HarrTransformUser(void)
{	
	img_org_bgra = NULL;
	img_org_gray = NULL;
}
/*---------------------------------------------------*/
/**
*
*
*/
/*---------------------------------------------------*/
HarrTransformUser::~HarrTransformUser(void)
{
	cvReleaseImage(&img_org_bgra);
	cvReleaseImage(&img_org_gray);
}
/*---------------------------------------------------*/
/**
*
*
*/
/*---------------------------------------------------*/
void HarrTransformUser::setImageData(IplImage *_img_bgra) 
{
	const int img_width	 =mWIDTH  = _img_bgra->width;
	const int img_height =mHEIGHT = _img_bgra->height;

	ASSERT(img_width % 4 == 0);
	ASSERT(img_height % 4 == 0);
	ASSERT(_img_bgra->widthStep == _img_bgra->width*_img_bgra->nChannels);
	
	if (img_org_bgra!=NULL){
		cvReleaseImage(&img_org_bgra);
	}
	
	img_org_bgra = cvCloneImage(_img_bgra);

	if (img_org_gray != NULL) {
		cvReleaseImage(&img_org_gray);
	}
	img_org_gray = cvCreateImage(cvGetSize(img_org_bgra),IPL_DEPTH_8U,1);
	cvCvtColor(img_org_bgra, img_org_gray,CV_BGRA2GRAY);
}
/*---------------------------------------------------*/
/**
*
*
*/
/*---------------------------------------------------*/
float HarrTransformUser::CalculateEnergyCol(int _col_step)
{

	IplImage* img_diff = cvCreateImage(cvGetSize(img_org_bgra),IPL_DEPTH_32F,1);
	float feature=0;
	float sub_v = 0;
	unsigned int  effect_t = 0;
				//cvZero(&img_diff);
					{
						

								for (size_t ri = 0; ri <mHEIGHT ; ri++){

										for (size_t ci = 0; ci <mWIDTH; ci++){

											if (ci + _col_step < mWIDTH) {
								
												sub_v=PixelColSub(img_org_gray, ci+ _col_step, ci , ri);
												effect_t++;
																		
											}
											else {
											
												sub_v = 0;
											}
											
											
											cvSetReal2D(img_diff,ri,ci,sub_v);	
							
										}
								}


	
	
					}

		feature=CalDiffEnergy(img_diff,effect_t);


	cvReleaseImage(&img_diff);

	return feature;

}
/*---------------------------------------------------*/
/**
*
*
*/
/*---------------------------------------------------*/
float HarrTransformUser::CalculateEnergyRow(int _row_step)
{
	IplImage* img_diff = cvCreateImage(cvGetSize(img_org_bgra), IPL_DEPTH_32F, 1);
	float feature = 0;
	float sub_v = 0;
	unsigned int  effect_t=0;
	//cvZero(&img_diff);
	{


		for (size_t ri = 0; ri <mHEIGHT; ri++) {

			for (size_t ci = 0; ci <mWIDTH; ci++) {

				if (ri + _row_step < mHEIGHT) {

					sub_v = PixelRowSub(img_org_gray, ri + _row_step, ri, ci);
					effect_t++;
				}
				else {

					sub_v = 0;
				}


				cvSetReal2D(img_diff, ri, ci, sub_v);

			}
		}




	}

	feature = CalDiffEnergy(img_diff,effect_t);


	cvReleaseImage(&img_diff);

	return feature;
}
/*---------------------------------------------------*/
/**
*
*
*/
/*---------------------------------------------------*/
float HarrTransformUser::PixelSub(IplImage *_img_gary,int ci,int ri,int cj,int rj)
{

	float data1 = cvGetReal2D(_img_gary, ri, ci);
	float data0 = cvGetReal2D(_img_gary, rj, cj );
	
	return data1-data0;
}
/*---------------------------------------------------*/
/**
*
*
*/
/*---------------------------------------------------*/
float HarrTransformUser::PixelColSub(IplImage *_img_gary, int ci, int cj, int row)
{
	return PixelSub(_img_gary, ci, row,cj,row);
}
/*---------------------------------------------------*/
/**
*
*
*/
/*---------------------------------------------------*/
float HarrTransformUser::PixelRowSub(IplImage * _img_gary, int ri, int rj, int col)
{
	return PixelSub(_img_gary, col, ri, col, rj);
}
/*---------------------------------------------------*/
/**
*
*
*/
/*---------------------------------------------------*/
float  HarrTransformUser::CalDiffEnergy(IplImage *_img_gary,float effect)
{
	const int WIDTH = _img_gary->width;
    const int HEIGHT = _img_gary->height;
 
	ASSERT(effect>0 && effect < WIDTH*HEIGHT);

	float sum = 0;


	for (size_t ri = 0; ri < HEIGHT; ri++) {

		for (size_t ci = 0; ci < WIDTH; ci++) {
			
			float diff=cvGetReal2D(_img_gary, ri, ci);

			sum += diff*diff;

		}

	}

	sum = sum /effect;
	
	return sum;

}
/*---------------------------------------------------*/
/**
*
*
*/
/*---------------------------------------------------*/

/*---------------------------------------------------*/
/**
*
*
*/
/*---------------------------------------------------*/
/*---------------------------------------------------*/
/**
*
*
*/
/*---------------------------------------------------*/
/*---------------------------------------------------*/
/**
*
*
*/
/*---------------------------------------------------*/