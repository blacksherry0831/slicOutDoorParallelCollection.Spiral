#pragma once
#include "../ImageMemData.h"

#define ContourImgMethod  (1)
#define OriginalImgMethod (2)
#define HarrImgMethod  (3)
/*-----------------------------------------------------------------------------*/
/**
*Hough�任����ѡ��
*/
/*-----------------------------------------------------------------------------*/
class HoughAnalyzeParam
{
public:
	int peProcessMethod;/**<ͼƬԤ����ʽ*/
	int HoughMethod;/**<hough�任��ʽ*/
	int LineLengthMin;/**<ֱ����С����*/
	int GapMax;/**<hough�任���*/
	int Threshold;/**<hough�任��ֵ*/
public:
	HoughAnalyzeParam(){
		this->peProcessMethod=-1;
		this->HoughMethod=CV_HOUGH_PROBABILISTIC;
		//�ۼ�ֵ���� threshold�� �������������߶�
		this->Threshold=20;
		//������С�߶γ���.
		this->LineLengthMin=20;
		//���߶����ӵ������ֵ
		this->GapMax=2;

	}
	HoughAnalyzeParam(
		int LineLengthMin,
		int GapMax,
		int Threshold){
		this->peProcessMethod=HarrImgMethod;
		this->HoughMethod=CV_HOUGH_PROBABILISTIC;
		//�ۼ�ֵ���� threshold�� �������������߶�
		this->Threshold=Threshold;
		//������С�߶γ���.
		this->LineLengthMin=LineLengthMin;
		//���߶����ӵ������ֵ
		this->GapMax=GapMax;

	}
	HoughAnalyzeParam(
		int LineLengthMin,
		int GapMax,
		int Threshold,
		int peProcessMethod){
			this->peProcessMethod=peProcessMethod;
			this->HoughMethod=CV_HOUGH_PROBABILISTIC;
			//�ۼ�ֵ���� threshold�� �������������߶�
			this->Threshold=Threshold;
			//������С�߶γ���.
			this->LineLengthMin=LineLengthMin;
			//���߶����ӵ������ֵ
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
*��ͼ�����Hough�任����
*/
/*-----------------------------------------------------------------------------*/
class HoughAnalyze
{
private:
	ImageMemData* pMD;/**<��Ҫ��ͼ�����ݺ�ͼ������õ��м����*/
	CvMemStorage* storage;/**<opencv�����õ��м��ڴ�*/ 
	CvSeq* lines;/**<opencv�����У�ռ��CvMemStorage���ڴ�ռ�*/
	HoughAnalyzeParam HAParam;/**<cvhough�任����*/
private:
	IplImage* gray_Color_img;/**<�Ҷ�ͼ���Ӧ������ͼ�������ã�*/
	IplImage* gray_img;/**<ԭͼ��Ӧ�ĻҶ�ͼ�񣬵�ͨ��*/
	IplImage* gray_binary_img;/**<�ҶȾ���ֵ�����ͼ��*/
	IplImage* src_img;/**<�ҶȾ���ֵ�����ͼ��*/
	IplImage *ImgMaskControus;/**<hough�任��ֵ��ͼ���������*/
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
