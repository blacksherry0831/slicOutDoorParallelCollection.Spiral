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
*����ʵ�ֶ�ͼ���С���任����任
*-��ʵ��һ������С���任����������\n
*-����С���任�����˲���ʵ��ƽ������
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
	/**<ͼ������ָ��*/
	unsigned char * m_pImgData; 
	/**<ͼ��Ŀ�����Ϊ��λ*/
	int m_imgWidth;	 
	/**<ͼ��ĸߣ�����Ϊ��λ*/
	int m_imgHeight;
	/**<ÿ����ռ��λ��*/
	int m_nBitCount;
	/**<*/
	int m_nBitCountOut;
public:
	/**<С���任����������*/
	unsigned char * m_pImgDataOut;
	int* ImgLabels4Quarter;
	//LPRGBQUAD m_pColorTableOut;
	/*int m_nColorTableLengthOut;*/
	/**<*/
	double* pCoeff;
private:
	/**<С���任����*/
	int m_nScale;

public:
	//����������
	inline void MatrixRegionCopy(unsigned char *matrixRegion, int heightRegion, int widthRegion,
		unsigned char *matrixDest, int heightDest, int widthDest);

	//�������
	inline void MatrixSub(unsigned char *matrixA, unsigned char *matrixB, int height, int width, unsigned char *result);	

	//�в���
	inline void MatrixSampleRow(unsigned char *matrixInput, unsigned char *matrixOutputOdd,
		unsigned char *matrixOutputEven, int heightOutput, int widthOutput, int widthInput);

	//�в���
	inline void MatrixSampleLine(unsigned char *matrixInput, unsigned char *matrixOutputOdd,
		unsigned char *matrixOutputEven, int heightOutput, int widthOutput, int widthInput);

	//һ��С���ֽ�
	void Trans(unsigned char *inputData, int inputHeight, int inputWidth, int lineByte, int scale, unsigned char *outputData);
	void Trans_cpu(unsigned char *inputData, int inputHeight, int inputWidth, int lineByte, int scale, unsigned char *outputData);
	void Trans_gpu(unsigned char *inputData, int inputHeight, int inputWidth, int lineByte, int scale, unsigned char *outputData);

	//С���任
	void SimpleWaveletTrans(int scale);

	//�������
	inline void MatrixPlus(unsigned char *matrixA, unsigned char *matrixB, int height, int width, unsigned char *result);

	//�в��
	void MatrixInsertRow(unsigned char *matrixInputOdd, unsigned char *matrixInputEven,
		unsigned char *matrixOutput, int heightInput, int widthInput, int widthOutput);

	//�в��
	void MatrixInsertLine(unsigned char *matrixInputOdd, unsigned char *matrixInputEven,
		unsigned char *matrixOutput, int heightInput, int widthInput, int widthOutput);

	//һ��С���ع�
	void Reverse(unsigned char *inputData, int inputHeight, int inputWidth, int lineByte, int scale, unsigned char *outputData);

	//С���ع�
	void SimpleWaveletReverse();

	//��Ƶϵ�����㷽��ȥ����
	void H2Zero(double* coeff, int cHeight, int cWidth, 
		int staticHeight, int staticWidth);
	void SetH2Zero();

	//Ӳ��ֵ����ȥ����
	void Hard(double* coeff, int cHeight, int cWidth, double threshold,
		int staticHeight, int staticWidth);
	//Ӳ��ֵ����ȥ��Ķ���ӿ�
	void HardThreshold();

	int GetSgn(double number);
	//����ֵ����ȥ����
	void Soft(double* coeff, int cHeight, int cWidth, double threshold,
		int staticHeight, int staticWidth);
	//����ֵ����ȥ��Ķ���ӿ�
	void SoftThreshold();

	
	int Scale(void);
public:

	char filename_org[1024];	/**<�ļ���*/
	IplImage *img_original_size_color;/**<ԭʼ�ߴ��ͼ��*/ 
	IplImage *img_scr_color;/**<ԭʼͼ���Ӧ�Ĳ�ɫͼ�񣬿����8�ı���*/
	IplImage *img_scr_gray;/**<�Ҷ�ͼ�񣬿����8�ı���*/
	IplImage *img_dst_wavelet;/**<С���任���ͼ*/
	double   cui_proportion;/**<��Ҫ����ͼ��ռ����ͼ��ı���*/

	Energy_Harr harr_energy;/**<С������������*/
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
	ImageMemData* pMD;/**<��Ҫ��ͼ�����ݺ�ͼ������õ��м����*/
	void InitParam(void);
};

