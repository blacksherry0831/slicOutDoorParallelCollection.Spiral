#include "stdafx.h"
#include "module_all_cui.h"
//#include <SGVcode/Harr/HarrWaveletTransImg.h>
/*---------------------------------------------------*/
/**
*初始化成员变量
*
*/
/*---------------------------------------------------*/
HarrWaveletTransImg::HarrWaveletTransImg(IplImage *src_img,IplImage *trans_img,int* maskImg,int scale)
{	
	this->InitParam();
	this->src_argb=cvCloneImage(src_img);
	this->ImgWavelet=cvCloneImage(trans_img);
	this->ImgWidth=src_img->width;
	this->ImgHeight=src_img->height;
	this->scale=scale;
	this->src_mask=new int[ImgHeight*ImgWidth];
	memcpy(src_mask,maskImg,sizeof(int)*ImgWidth*ImgHeight);
	this->Parse();
}
/*---------------------------------------------------*/
/**
*初始化成员变量
*
*/
/*---------------------------------------------------*/
HarrWaveletTransImg::~HarrWaveletTransImg(void)
{	
	this->ReleaseMem();
}
/*---------------------------------------------------*/
/**
*
*/
/*---------------------------------------------------*/
void HarrWaveletTransImg::InitParam()
{
	src_argb=nullptr;
	src_mask=nullptr;
	ImgWavelet=nullptr;
#if TRUE
	imgHH_1=nullptr;
	imgHL_1=nullptr;
	imgLH_1=nullptr;
	imgLL_1=nullptr;
#endif
#if TRUE
	imgHH_2=nullptr;
	imgHL_2=nullptr;
	imgLH_2=nullptr;
	imgLL_2=nullptr;
#endif
#if TRUE
	imgHH_3=nullptr;
	imgHL_3=nullptr;
	imgLH_3=nullptr;
	imgLL_3=nullptr;
#endif
}
/*---------------------------------------------------*/
/**
*根据分解层数。将小波分解得到的图像保存的本类
*/
/*---------------------------------------------------*/
void HarrWaveletTransImg::Parse()
{
	if (scale>=1){
		this->Parse_1();
	}
	if (scale>=2){
		this->Parse_2();
	}
	if (scale>=3){
		this->Parse_3();
	}
	ASSERT(scale<=3);
}
/*---------------------------------------------------*/
/**
*
*/
/*---------------------------------------------------*/
void HarrWaveletTransImg::Parse_1()
{	
	IplImage *ImgSrc=this->ImgWavelet;
	ASSERT(scale>=1);
	int width=ImgSrc->width;
	int height=ImgSrc->height;
	CopyRectImg(cvRect(width/2,height/2,width/2,height/2),ImgSrc,&this->imgHH_1);
	CopyRectImg(cvRect(width/2,0,width/2,height/2),ImgSrc,&this->imgHL_1);
	CopyRectImg(cvRect(0,height/2,width/2,height/2),ImgSrc,&this->imgLH_1);
	CopyRectImg(cvRect(0,0,width/2,height/2),ImgSrc,&this->imgLL_1);

}
/*---------------------------------------------------*/
/**
*
*/
/*---------------------------------------------------*/
void HarrWaveletTransImg::Parse_2()
{
	IplImage *ImgSrc=this->imgLL_1;
	ASSERT(scale>=2);
	int width=ImgSrc->width;
	int height=ImgSrc->height;
	CopyRectImg(cvRect(width/2,height/2,width/2,height/2),ImgSrc,&this->imgHH_2);
	CopyRectImg(cvRect(width/2,0,width/2,height/2),ImgSrc,&this->imgHL_2);
	CopyRectImg(cvRect(0,height/2,width/2,height/2),ImgSrc,&this->imgLH_2);
	CopyRectImg(cvRect(0,0,width/2,height/2),ImgSrc,&this->imgLL_2);
}
/*---------------------------------------------------*/
/**
*
*/
/*---------------------------------------------------*/
void HarrWaveletTransImg::Parse_3()
{
	IplImage *ImgSrc=this->imgLL_2;
	ASSERT(scale>=3);
	int width=ImgSrc->width;
	int height=ImgSrc->height;
	CopyRectImg(cvRect(width/2,height/2,width/2,height/2),ImgSrc,&this->imgHH_3);
	CopyRectImg(cvRect(width/2,0,width/2,height/2),ImgSrc,&this->imgHL_3);
	CopyRectImg(cvRect(0,height/2,width/2,height/2),ImgSrc,&this->imgLH_3);
	CopyRectImg(cvRect(0,0,width/2,height/2),ImgSrc,&this->imgLL_3);
}
/*---------------------------------------------------*/
/**
*
*/
/*---------------------------------------------------*/
void HarrWaveletTransImg::ReleaseMem()
{
	cvReleaseImage(&src_argb);
	cvReleaseImage(&ImgWavelet);
	delete[]src_mask;
#if TRUE
	cvReleaseImage(&imgHH_1);
	cvReleaseImage(&imgHL_1);
	cvReleaseImage(&imgLH_1);
	cvReleaseImage(&imgLL_1);
#endif
#if TRUE
	cvReleaseImage(&imgHH_2);
	cvReleaseImage(&imgHL_2);
	cvReleaseImage(&imgLH_2);
	cvReleaseImage(&imgLL_2);
#endif
#if TRUE
	cvReleaseImage(&imgHH_3);
	cvReleaseImage(&imgHL_3);
	cvReleaseImage(&imgLH_3);
	cvReleaseImage(&imgLL_3);
#endif
}
/*---------------------------------------------------*/
/**
*
*/
/*---------------------------------------------------*/
void HarrWaveletTransImg::CopyRectImg(CvRect src_roi,IplImage*src,IplImage** dst)
{
	if (*dst!=nullptr){
		cvReleaseImage(dst);
	}
	*dst=cvCreateImage(cvSize(src_roi.width,src_roi.height),src->depth,src->nChannels);
	CvRect dst_roi = cvRect(0, 0, src_roi.width, src_roi.height); 

	cvSetImageROI(src, src_roi);  
	cvSetImageROI(*dst, dst_roi);  

	cvCopy(src, *dst);  

	cvResetImageROI(src);  
	cvResetImageROI(*dst);  

}
/*---------------------------------------------------*/
/**
*
*/
/*---------------------------------------------------*/
void HarrWaveletTransImg::SaveImg(void)
{
	cvSaveImage("HarrWaveletTransImg/src_argb.png",src_argb);
	cvSaveImage("HarrWaveletTransImg/ImgWavelet.png",ImgWavelet);
	src_mask;
#if TRUE
	cvSaveImage("HarrWaveletTransImg/imgHH_1.png",imgHH_1);
	cvSaveImage("HarrWaveletTransImg/imgHL_1.png",imgHL_1);
	cvSaveImage("HarrWaveletTransImg/imgLH_1.png",imgLH_1);
	cvSaveImage("HarrWaveletTransImg/imgLL_1.png",imgLL_1);
#endif
#if TRUE
	cvSaveImage("HarrWaveletTransImg/imgHH_2.png",imgHH_2);
	cvSaveImage("HarrWaveletTransImg/imgHL_2.png",imgHL_2);
	cvSaveImage("HarrWaveletTransImg/imgLH_2.png",imgLH_2);
	cvSaveImage("HarrWaveletTransImg/imgLL_2.png",imgLL_2);
#endif
#if TRUE
	cvSaveImage("HarrWaveletTransImg/imgHH_3.png",imgHH_3);
	cvSaveImage("HarrWaveletTransImg/imgHL_3.png",imgHL_3);
	cvSaveImage("HarrWaveletTransImg/imgLH_3.png",imgLH_3);
	cvSaveImage("HarrWaveletTransImg/imgLL_3.png",imgLL_3);
#endif	
}