#pragma once
#if FALSE
#pragma comment(lib, "wavelet_3_8.lib")
#endif
#include "SGVcode/ImageType.h"
#include "WaveletApi.h"
#include "FilterApi.h"
#include "DIBapi.h"
//#include <math.h>
//#include <iostream>
//#include <fstream> 
//using namespace std;
#include <platform_cui.h>
#define SHOW_HARR_IMG FALSE
/*---------------------------------------------------*/
/**
*本类实现对图像的小波变换和逆变换
*-可实现一、二层小波变换的能量计算\n
*-利用小波变换进行滤波，实现平滑处理
*
*/
/*---------------------------------------------------*/
class CuiHarrTransformLet
{
public:
	CuiHarrTransformLet(void);
	CuiHarrTransformLet(ImageMemData* MemData_t);
	~CuiHarrTransformLet(void);
public:
	/**<图像数据指针*/
	unsigned char * m_pImgData; 
	/**<图像的宽，像素为单位*/
	int m_imgWidth;	 
	/**<图像的高，像素为单位*/
	int m_imgHeight;
	/**<每像素占的位数*/
	int m_nBitCount;
	/**<*/
	int m_nBitCountOut;
public:
	/**<小波变换结果输出矩阵*/
	unsigned char * m_pImgDataOut;
	int* ImgLabels4Quarter;
	//LPRGBQUAD m_pColorTableOut;
	/*int m_nColorTableLengthOut;*/
	/**<*/
	double* pCoeff;
private:
	/**<小波变换层数*/
	int m_nScale;

public:
	//矩阵区域复制
	inline void MatrixRegionCopy(unsigned char *matrixRegion, int heightRegion, int widthRegion,
		unsigned char *matrixDest, int heightDest, int widthDest);

	//矩阵相减
	inline void MatrixSub(unsigned char *matrixA, unsigned char *matrixB, int height, int width, unsigned char *result);	

	//行采样
	inline void MatrixSampleRow(unsigned char *matrixInput, unsigned char *matrixOutputOdd,
		unsigned char *matrixOutputEven, int heightOutput, int widthOutput, int widthInput);

	//列采样
	inline void MatrixSampleLine(unsigned char *matrixInput, unsigned char *matrixOutputOdd,
		unsigned char *matrixOutputEven, int heightOutput, int widthOutput, int widthInput);

	//一次小波分解
	void Trans(unsigned char *inputData, int inputHeight, int inputWidth, int lineByte, int scale, unsigned char *outputData);
	void Trans_cpu(unsigned char *inputData, int inputHeight, int inputWidth, int lineByte, int scale, unsigned char *outputData);
	void Trans_gpu(unsigned char *inputData, int inputHeight, int inputWidth, int lineByte, int scale, unsigned char *outputData);

	//小波变换
	void SimpleWaveletTrans(int scale);

	//矩阵相加
	inline void MatrixPlus(unsigned char *matrixA, unsigned char *matrixB, int height, int width, unsigned char *result);

	//行插点
	void MatrixInsertRow(unsigned char *matrixInputOdd, unsigned char *matrixInputEven,
		unsigned char *matrixOutput, int heightInput, int widthInput, int widthOutput);

	//列插点
	void MatrixInsertLine(unsigned char *matrixInputOdd, unsigned char *matrixInputEven,
		unsigned char *matrixOutput, int heightInput, int widthInput, int widthOutput);

	//一次小波重构
	void Reverse(unsigned char *inputData, int inputHeight, int inputWidth, int lineByte, int scale, unsigned char *outputData);

	//小波重构
	void SimpleWaveletReverse();

	//高频系数置零方法去噪声
	void H2Zero(double* coeff, int cHeight, int cWidth, 
		int staticHeight, int staticWidth);
	void SetH2Zero();

	//硬阈值方法去噪声
	void Hard(double* coeff, int cHeight, int cWidth, double threshold,
		int staticHeight, int staticWidth);
	//硬阈值方法去噪的对外接口
	void HardThreshold();

	int GetSgn(double number);
	//软阈值方法去噪声
	void Soft(double* coeff, int cHeight, int cWidth, double threshold,
		int staticHeight, int staticWidth);
	//软阈值方法去噪的对外接口
	void SoftThreshold();

	
	int Scale(void);
public:

	char filename_org[1024];	/**<文件名*/
	IplImage *img_original_size_color;/**<原始尺寸的图像*/ 
	IplImage *img_scr_color;/**<原始图像对应的彩色图像，宽度是8的倍数*/
	IplImage *img_scr_gray;/**<灰度图像，宽度是8的倍数*/
	IplImage *img_dst_wavelet;/**<小波变换结果图*/
	double   cui_proportion;/**<需要计算图块占整个图块的比例*/

	Energy_Harr harr_energy;/**<小波能量计算结果*/
public:
	IplImage* CuiLoadFile(const char *);
	IplImage*  HarrSmooth(int scale);

	void CuiShowImage(void);
	void CuiWaveletTrans(int scale);
	double CuiCalculateEnergy(CvRect rect,int light=0);
	void CuiSaveImg(void);
	void CuiCalculateEnergyALL(void);
	void CalculateEnergyAllApBlock(SpSetProperty& SpSet);

	void CuiGetImageData(
		UINT32*	imgBuffer,
		int	width,int height,
		double proportion=1);

	void CuiGetAllEnergy(Energy_Harr* energy);
	void CuiAdjustBoder(IplImage *img_boder);
	void CuiAdjustBoderByContour(IplImage *img_boder);
	void CuiWaveletReverse(void);
	

	void CuiLoadImgBuffer(
		UINT32* ImgData,
		int Width,
		int Height);

	void PreproccessImg(void);
	void ReleaseImgData(void);
	void CalculateBoderImg(int scale);
	int  CalculateBoderContours(int width,int height);
	int  CalculateBoderContoursSC(float S,float C);
	int  CalculateBoderContoursThinkness(
		CvSeq * pcontour,
		int width,
		int height);
	float  CalculateImgArea(IplImage *contour_img_quarter);
	void   CalculateBoderImg_Block(int scale);
	void   SetSuperPixelLabels(int* ImgLabels,int Width,int Height);
private:
	ImageMemData* pMD;/**<需要的图像数据和图像计算用的中间变量*/
	void InitParam(void);
};

