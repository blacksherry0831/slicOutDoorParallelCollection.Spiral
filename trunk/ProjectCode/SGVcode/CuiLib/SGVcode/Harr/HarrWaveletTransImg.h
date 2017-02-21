#pragma once
/*---------------------------------------------------*/
/**
*Harr分解后得到的图片
*@deprecated 弃用
*
*/
/*---------------------------------------------------*/
class HarrWaveletTransImg
{
public:
	IplImage *src_argb;
	IplImage *ImgWavelet;
	int *src_mask;
	int ImgWidth;
	int ImgHeight;
#if TRUE
	IplImage *imgHH_1;
	IplImage *imgHL_1;
	IplImage *imgLH_1;
	IplImage *imgLL_1;
#endif
#if TRUE
	IplImage *imgHH_2;
	IplImage *imgHL_2;
	IplImage *imgLH_2;
	IplImage *imgLL_2;
#endif
#if TRUE
	IplImage *imgHH_3;
	IplImage *imgHL_3;
	IplImage *imgLH_3;
	IplImage *imgLL_3;
#endif
public:
	HarrWaveletTransImg(IplImage *src_img,IplImage *trans_img,int* maskImg,int scale);
	~HarrWaveletTransImg(void);
private:
	int scale;
	void InitParam(void);
	void ReleaseMem(void);
	
	void Parse();
	void Parse_1();
	void Parse_2();
	void Parse_3();
	void CopyRectImg(CvRect rect,IplImage*src,IplImage** dst);
public:
	void SaveImg(void);
};

