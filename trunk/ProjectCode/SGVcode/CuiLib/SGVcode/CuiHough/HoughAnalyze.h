#pragma once
#include "../ImageMemData.h"

#define ContourImgMethod  (1)
#define OriginalImgMethod (2)
#define HarrImgMethod  (3)
/*-----------------------------------------------------------------------------*/
/**
*Hough变换参数选择
*/
/*-----------------------------------------------------------------------------*/
class HoughAnalyzeParam
{
public:
	int peProcessMethod;/**<图片预处理方式*/
	int HoughMethod;/**<hough变换方式*/
	int LineLengthMin;/**<直线最小长度*/
	int GapMax;/**<hough变换间隔*/
	int Threshold;/**<hough变换阈值*/
public:
	HoughAnalyzeParam(){
		this->peProcessMethod=-1;
		this->HoughMethod=CV_HOUGH_PROBABILISTIC;
		//累计值大于 threshold， 则函数返回这条线段
		this->Threshold=20;
		//它是最小线段长度.
		this->LineLengthMin=20;
		//碎线段连接的最大间隔值
		this->GapMax=2;

	}
	HoughAnalyzeParam(
		int LineLengthMin,
		int GapMax,
		int Threshold){
		this->peProcessMethod=HarrImgMethod;
		this->HoughMethod=CV_HOUGH_PROBABILISTIC;
		//累计值大于 threshold， 则函数返回这条线段
		this->Threshold=Threshold;
		//它是最小线段长度.
		this->LineLengthMin=LineLengthMin;
		//碎线段连接的最大间隔值
		this->GapMax=GapMax;

	}
	HoughAnalyzeParam(
		int LineLengthMin,
		int GapMax,
		int Threshold,
		int peProcessMethod){
			this->peProcessMethod=peProcessMethod;
			this->HoughMethod=CV_HOUGH_PROBABILISTIC;
			//累计值大于 threshold， 则函数返回这条线段
			this->Threshold=Threshold;
			//它是最小线段长度.
			this->LineLengthMin=LineLengthMin;
			//碎线段连接的最大间隔值
			this->GapMax=GapMax;

	}
	static  HoughAnalyzeParam GetDefaultHAParam(void)
	{
		static HoughAnalyzeParam hap;
		return hap;
	}
	static  HoughAnalyzeParam GetHarrHAParam(void)
	{
		static HoughAnalyzeParam hap(8,4,3);
		return hap;
	}
};
/*-----------------------------------------------------------------------------*/
/**
*对图像进行Hough变换分析
*/
/*-----------------------------------------------------------------------------*/
class HoughAnalyze
{
private:
	ImageMemData* pMD;/**<需要的图像数据和图像计算用的中间变量*/
	CvMemStorage* storage;/**<opencv计算用的中间内存*/ 
	CvSeq* lines;/**<opencv的序列，占用CvMemStorage，内存空间*/
	HoughAnalyzeParam HAParam;/**<cvhough变换参数*/
private:
	IplImage* gray_Color_img;/**<灰度图像对应的轮廓图（调试用）*/
	IplImage* gray_img;/**<原图对应的灰度图像，单通道*/
	IplImage* gray_binary_img;/**<灰度经二值化后的图像*/
	IplImage* src_img;/**<灰度经二值化后的图像*/
	IplImage *ImgMaskControus;/**<hough变换二值化图像的屏蔽罩*/
public:
	HoughAnalyze(
		ImageMemData* MemData_t,
		IplImage *ImgMask=nullptr,
		HoughAnalyzeParam* HAParam=NULL);
	~HoughAnalyze(void);
public:
	void ImgPreprocess(
	 IplImage **src_img_t,
	 int method);

	 void AdjustMaskImgSize(IplImage *ImgMask);
	
	 void AdjustLine(
	 LINEDATA *cui_line_data,
	 int cui_line_len);

static CvScalar GetGradientColor(double angle_color);

	 void GetAllhoughLineData(
		 IplImage **src_img_t,
		 IplImage **gray_Color_img_t,
		 LINEDATA **plinedata,
		 int *pline_len,
		 int method);

	  void GetHistogram180SVG(
		  string filename,
		  LINEDATA* linedata,
		  int linelen,
		  HistData180 *hist_data);

	  void GetHist180All(void);
	  HistData180 GetHist180VerticalAll(void);
	  HistData180 GetOneSPLineHistogram2(int sp);

	  void  GetAllHoughLineByContour_SP(
		const  UINT32* ImgData_ContoursSP,
		const  int sp,
		const  int WidthSP,
		const  int HeightSP,
		const UINT32 PosBorder);
public:
 HistData180 GetSaveALlImgHistogram_180(void);
public:
	  void GetAllHoughLineBySourceImg(void);
	  void GetAllHoughLineByHarrImg(void);
	  HistData180 GetALlImgHistogram_180(void);
	 
	  HistData180 GetSaveALlImgHistogram_180_0to100(void);
	  static void AdjustHistFrom0to100(HistData180 *histdata,int Dim);
	  void ImgPreprocessAdjustByMaskImg(void);
	
public:
	    bool DoubleIsInt(double n);
		bool FloatIsInt(float n);
		static void StatisticsHistogramByLine(
			 string filename,
			 LINEDATA* linedata,
			 int linelen,
			 HistData *hist_data,
			 int Dim);
		static void StatisticsHistogram180ByLine(
			string filename,
			LINEDATA* linedata,
			int linelen,
			HistData180 *hist_data);
};
/*-----------------------------------------------------------------------------*/
/**
*
*/
/*-----------------------------------------------------------------------------*/
