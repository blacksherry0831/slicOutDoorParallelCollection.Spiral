#include "stdafx.h"
#include "module_all_cui.h"
//#include "CuiHarrTransformLet.h"
/*---------------------------------------------------*/
/**
*��ʼ����Ա����
*
*/
/*---------------------------------------------------*/
CuiHarrTransformLet::CuiHarrTransformLet(void)
{	
	this->InitParam();
}
/*---------------------------------------------------*/
/**
*��ʼ����Ա����
*
*/
/*---------------------------------------------------*/
CuiHarrTransformLet::CuiHarrTransformLet(ImageMemData* MemData_t)
{	
	this->pMD=MemData_t;
	this->InitParam();
	this->CuiGetImageData(
		pMD->Src_ImgData,
		pMD->ImgWidth,
		pMD->ImgHeight);
}
/*---------------------------------------------------*/
/**
*�ͷż���ʱ������ڴ�
*
*
*/
/*---------------------------------------------------*/
void CuiHarrTransformLet::InitParam(void)
{
	//m_nColorTableLengthOut=0;
	m_nScale = 0;
	m_nBitCountOut=0;
	m_pImgDataOut=NULL;
	//m_pColorTableOut=NULL;
	pCoeff = NULL;
	/***********************/
	img_scr_color=NULL;
	img_scr_gray=NULL;
	img_dst_wavelet=NULL;
	img_original_size_color=NULL;
	this->cui_proportion=1;
	/***********************/
#if SHOW_HARR_IMG
	cvNamedWindow("HarrScr",CV_WINDOW_NORMAL);
	cvNamedWindow("HarrGray",CV_WINDOW_NORMAL);
	cvNamedWindow("HarrConvert",CV_WINDOW_NORMAL);
#endif
}
/*---------------------------------------------------*/
/**
*�ͷż���ʱ������ڴ�
*
*
*/
/*---------------------------------------------------*/

CuiHarrTransformLet::~CuiHarrTransformLet(void)
{
	if (m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
		m_pImgDataOut=NULL;
	}
	if (pCoeff != NULL)
	{
		delete[] pCoeff;
		pCoeff = NULL;
	}
#if SHOW_HARR_IMG 
	cvDestroyWindow("HarrScr");
	cvDestroyWindow("HarrGray");
	cvDestroyWindow("HarrConvert");
#endif
	

	if (img_scr_color!=NULL){
		cvReleaseImage(&img_scr_color);
	}
	if (img_scr_gray!=NULL){
		cvReleaseImage(&img_scr_gray);
	}
	if (img_dst_wavelet!=NULL){
		cvReleaseImage(&img_dst_wavelet);
	}
	if (img_original_size_color!=NULL){
		cvReleaseImage(&img_original_size_color);
	}
	delete[] ImgLabels4Quarter;
}
/*---------------------------------------------------*/
/**
*
*����������
* MatrixRegionCopy()
*
*    
*@param unsigned char *matrixRegion Դ��������ָ��
*@param int heightRegion ��������ĸ߶�
*@param int widthRegion ��������Ŀ��
*@param unsigned char *matrixDest Ŀ�ľ�������ָ��
*@param int heightDest Ŀ�ľ�������ĸ߶�
*@param int widthDest Ŀ�ľ�������Ŀ��
*
*@return
* void
*
* @note 
* ��Դ�����ָ���������ݸ��Ƶ�Ŀ�ľ����ָ������
*
*/
/*---------------------------------------------------*/
inline void CuiHarrTransformLet::MatrixRegionCopy(unsigned char *matrixRegion, 
	int heightRegion, 
	int widthRegion,
	unsigned char *matrixDest, 
	int heightDest,
	int widthDest)
{
	//��������߶ȺͿ��
	int heightMin = heightRegion>heightDest ? heightDest : heightRegion;
	int widthMin = widthRegion>widthDest ? widthDest : widthRegion;
	register int h, w;
	for (h=0; h<heightMin; h++)
	{
		for (w=0; w<widthMin; w++)
		{
			matrixDest[h * widthDest + w] = matrixRegion[h * widthRegion + w];
		}
	}
}
/*---------------------------------------------------*/
/**
*
*�������
*
* MatrixSub()
*
*    
*@param unsigned char *matrixA ��������A����ָ��
*@param unsigned char *matrixB ��������B����ָ��
*@param int height �߶�
*@param int width ���
*@param unsigned char *result ���������ָ��
*
* @return
* void
*
* @note 
* ���������������A��B���
*
*/
/*---------------------------------------------------*/
inline void CuiHarrTransformLet::MatrixSub(unsigned char *matrixA, 
	unsigned char *matrixB, 
	int height, 
	int width, 
	unsigned char *result)
{
//	if(gpu::getCudaEnabledDeviceCount()<0)
	
	{
		unsigned char temp;
		register int h, w;
		for (h=0; h<height; h++)
		{
			for (w=0; w<width; w++)
			{
				int index=h * width + w;
				temp = matrixA[index] - matrixB[index] + 128;
				result[index] = temp;
			}
		}
	}
	
	//else
	//{
	//	HarrTransformLet_MatrixSub_cuda(
	//				matrixA, 
	//				matrixB, 
	//				height, 
	//				width, 
	//				result);
	//}
	
}
/*---------------------------------------------------*/
/**
*
* 
*�в���
* MatrixSampleRow()
*
*    
*@param unsigned char *matrixInput ��������������ָ��
*@param unsigned char *matrixOutputOdd �����в������ɵľ���
*@param unsigned char *matrixOutputEven ż���в������ɵľ���
*@param int heightOutput �������߶�
*@param int widthOutput ���������
*@param int widthInput ���������
*
* @return
* void
*
* @note 
* �������������г�����
*
*/
/*---------------------------------------------------*/
inline void CuiHarrTransformLet::MatrixSampleRow(unsigned char *matrixInput, unsigned char *matrixOutputOdd,
										  unsigned char *matrixOutputEven, int heightOutput, int widthOutput, int widthInput)
{
	register int h, w;
	for (h=0; h<heightOutput; h++)
	{
		for (w=0; w<widthOutput; w++)
		{
			matrixOutputEven[h * widthOutput + w] = matrixInput[h * 2 * widthInput + w];
			matrixOutputOdd[h * widthOutput + w] = matrixInput[(h * 2 + 1) * widthInput + w];
		}
	}
}
/*---------------------------------------------------*/
/**
*�в���
* MatrixSampleLine()
*
*    
*@param unsigned char *matrixInput ��������������ָ��
*@param unsigned char *matrixOutputOdd �����в������ɵľ���
*@param unsigned char *matrixOutputEven ż���в������ɵľ���
*@param int heightOutput �������߶�
*@param int widthOutput ���������
*@param int widthInput ���������
*
* @return
* void
*
* @note 
* �������������г�����
*/
/*---------------------------------------------------*/
inline void CuiHarrTransformLet::MatrixSampleLine(
	unsigned char *matrixInput,
	unsigned char *matrixOutputOdd,
	unsigned char *matrixOutputEven,
	int heightOutput, 
	int widthOutput, 
	int widthInput)
{
	register int h, w;
	for (h=0; h<heightOutput; h++)
	{
		for (w=0; w<widthOutput; w++)
		{
			matrixOutputEven[h * widthOutput + w] = matrixInput[h * widthInput + w * 2];
			matrixOutputOdd[h * widthOutput + w] = matrixInput[h * widthInput + w * 2 + 1];
		}
	}
}
/*---------------------------------------------------*/
/**
*
* MatrixSub()
*
*    
*@param unsigned char *matrixA ����;���A����ָ��
*@param unsigned char *matrixB ����;���B����ָ��
*@param int height �߶�
*@param int width ���
*@param unsigned char *result �;�������ָ��
*
* @return
* void
*
* @note 
* ���������������A��B���
*/
/*---------------------------------------------------*/
inline void CuiHarrTransformLet::MatrixPlus(unsigned char *matrixA, 
	unsigned char *matrixB, 
	int height, 
	int width, 
	unsigned char *result)
{
	unsigned char temp;
	int h, w;
	for (h=0; h<height; h++)
	{
		for (w=0; w<width; w++)
		{
			temp = matrixA[h * width + w] + matrixB[h * width + w] - 128;
			result[h * width + w] = temp;
		}
	}
}
/*---------------------------------------------------*/
/**
*
*
* MatrixInsertRow()
*
*    
*@param unsigned char *matrixInputOdd ���������в������ɵľ���
*@param unsigned char *matrixInputEven ����ż���в������ɵľ���
*@param unsigned char *matrixOutput ���������������ָ��
*@param int heightInput �������߶�
*@param int widthInput ���������
*@param int widthOutput ���������
*
* @return
* void
*
* @note 
* �������������в�����
*
*/
/*---------------------------------------------------*/
void CuiHarrTransformLet::MatrixInsertRow(unsigned char *matrixInputOdd, unsigned char *matrixInputEven,
										 unsigned char *matrixOutput, int heightInput, int widthInput, int widthOutput)
{
	int h, w;
	for (h=0; h<heightInput*2; h++)
	{
		for (w=0; w<widthInput; w++)
		{
			if (h%2 == 0)
			{
				matrixOutput[h * widthOutput + w] = matrixInputEven[(int)(h/2) * widthInput + w];
			} 
			else
			{
				matrixOutput[h * widthOutput + w] = matrixInputOdd[(int)(h/2) * widthInput + w];
			}		
		}
	}
}
/*---------------------------------------------------*/
/**
*
*
* MatrixInsertLine()
*
*    
* @param  *matrixInputOdd ���������в������ɵľ���
* @param  *matrixInputEven ����ż���в������ɵľ���
* @param  *matrixOutput ���������������ָ��
* @param  heightInput �������߶�
* @param  widthInput ���������
* @param  widthOutput ���������
*
*@return
* void
*
* @note 
* �������������в�����
*/
/*---------------------------------------------------*/
void CuiHarrTransformLet::MatrixInsertLine(unsigned char *matrixInputOdd, unsigned char *matrixInputEven,
										  unsigned char *matrixOutput, int heightInput, int widthInput, int widthOutput)
{
	int h, w;
	for (h=0; h<heightInput; h++)
	{
		for (w=0; w<widthOutput; w++)
		{
			if (w%2 == 0)
			{
				matrixOutput[h * widthOutput + w] = matrixInputEven[h * widthInput + w/2];
			} 
			else
			{
				matrixOutput[h * widthOutput + w] = matrixInputOdd[h * widthInput + w/2];
			}		
		}
	}
}
/*---------------------------------------------------*/
/**
*һ��С���ֽ�
* Trans()
*
*    
* @param  *inputData �����������ָ��
* @param  inputHeight �������߶�
* @param  inputWidth ���������
* @param  lineByte �������ֽ���
* @param  scale �ֽ�߶�
* @param  *outputData �����������ָ��
*
* @return
* void
*
* @note 
* ����һ��С���ֽ�
*/
/*---------------------------------------------------*/
void CuiHarrTransformLet::Trans_cpu(
	unsigned char *inputData,
	int inputHeight,
	int inputWidth, 
	int lineByte,
	int scale,
	unsigned char *outputData)
{
	unsigned char *buff = new unsigned char[lineByte*m_imgHeight];

	int imageHeight = inputHeight;
	int imageWidth = inputWidth;
	
	unsigned char *temp1 = new unsigned char[imageWidth * (int)(imageHeight/2)];//������
	unsigned char *temp2 = new unsigned char[imageWidth * (int)(imageHeight/2)];//ż����
	//��ͼ���������в���
	this->MatrixSampleRow(inputData, temp1, temp2, (imageHeight/2), imageWidth, lineByte);
	//�в�֣��õ���Ƶ���Ƶ����
	this->MatrixSub(temp1, temp2, (imageHeight/2), imageWidth, temp1);//����-ż��
	unsigned char *temp00 = new unsigned char[(imageWidth/2) * (imageHeight/2)];//����
	unsigned char *temp01 = new unsigned char[(imageWidth/2) * (imageHeight/2)];//ż��
	//�Ե�Ƶ���ݽ����в���
	this->MatrixSampleLine(temp2, temp01, temp00, (imageHeight/2), (imageWidth/2), imageWidth);
	//�в�ֵõ�LL��LH
	this->MatrixSub(temp01, temp00, (imageHeight/2), (imageWidth/2), temp01);
	unsigned char *temp10 = new unsigned char[(imageWidth/2) * (imageHeight/2)];
	unsigned char *temp11 = new unsigned char[(imageWidth/2) * (imageHeight/2)];
	//�Ը�Ƶ���ݽ����в���
	this->MatrixSampleLine(temp1, temp11, temp10, (imageHeight/2), (imageWidth/2), imageWidth);
	//�в�֣��õ�HL��HH
	this->MatrixSub(temp11, temp10, (imageHeight/2), (imageWidth/2), temp11);
	
	this->MatrixRegionCopy(temp01, (imageHeight/2), (imageWidth/2),
		buff+lineByte*(int)(imageHeight/2)+(int)(imageWidth/2), imageHeight, lineByte);
	this->MatrixRegionCopy(temp10, (imageHeight/2), (imageWidth/2), buff, imageHeight, lineByte);
	this->MatrixRegionCopy(temp11, (imageHeight/2), (imageWidth/2), buff+(int)(imageWidth/2), imageHeight, lineByte);
	//�ͷſռ�
	delete[] temp1;
	delete[] temp2;
	delete[] temp01;
	delete[] temp10;
	delete[] temp11;
	
	scale--;
	//������LL���еݹ�ֽ�
	if (scale > 0)
	{
		this->Trans(temp00, imageHeight/2, imageWidth/2, imageWidth/2, scale, temp00);
	}
	this->MatrixRegionCopy(temp00, (imageHeight/2), (imageWidth/2), buff+lineByte*(int)(imageHeight/2),
		imageHeight, lineByte);
	delete[] temp00;

	//�����Ľ��������m_pImgDataOut
	memcpy(outputData, buff,  lineByte * inputHeight);
	delete[] buff;
}
/*---------------------------------------------------*/
/**
*һ��С���ֽ�
* Trans()
*
*    
* @param  *inputData �����������ָ��
* @param  inputHeight �������߶�
* @param  inputWidth ���������
* @param  lineByte �������ֽ���
* @param  scale �ֽ�߶�
* @param  *outputData �����������ָ��
*
* @return
* void
*
* @note 
* ����һ��С���ֽ�
*/
/*---------------------------------------------------*/
	void CuiHarrTransformLet::Trans_gpu(
	unsigned char *inputData,
	int inputHeight,
	int inputWidth, 
	int lineByte,
	int scale,
	unsigned char *outputData)
{
	unsigned char *buff = new unsigned char[lineByte*m_imgHeight];

	int imageHeight = inputHeight;
	int imageWidth = inputWidth;
	
	unsigned char *temp1 = new unsigned char[imageWidth * (int)(imageHeight/2)];
	unsigned char *temp2 = new unsigned char[imageWidth * (int)(imageHeight/2)];
	//��ͼ���������в���
	this->MatrixSampleRow(inputData, temp1, temp2, (imageHeight/2), imageWidth, lineByte);
	//�в�֣��õ���Ƶ���Ƶ����
	this->MatrixSub(temp1, temp2, (imageHeight/2), imageWidth, temp1);
	unsigned char *temp00 = new unsigned char[(imageWidth/2) * (imageHeight/2)];
	unsigned char *temp01 = new unsigned char[(imageWidth/2) * (imageHeight/2)];
	//�Ե�Ƶ���ݽ����в���
	this->MatrixSampleLine(temp2, temp01, temp00, (imageHeight/2), (imageWidth/2), imageWidth);
	//�в�ֵõ�LL��LH
	this->MatrixSub(temp01, temp00, (imageHeight/2), (imageWidth/2), temp01);
	unsigned char *temp10 = new unsigned char[(imageWidth/2) * (imageHeight/2)];
	unsigned char *temp11 = new unsigned char[(imageWidth/2) * (imageHeight/2)];
	//�Ը�Ƶ���ݽ����в���
	this->MatrixSampleLine(temp1, temp11, temp10, (imageHeight/2), (imageWidth/2), imageWidth);
	//�в�֣��õ�HL��HH
	this->MatrixSub(temp11, temp10, (imageHeight/2), (imageWidth/2), temp11);
	
	this->MatrixRegionCopy(temp01, (imageHeight/2), (imageWidth/2),
		buff+lineByte*(int)(imageHeight/2)+(int)(imageWidth/2), imageHeight, lineByte);
	this->MatrixRegionCopy(temp10, (imageHeight/2), (imageWidth/2), buff, imageHeight, lineByte);
	this->MatrixRegionCopy(temp11, (imageHeight/2), (imageWidth/2), buff+(int)(imageWidth/2), imageHeight, lineByte);
	//�ͷſռ�
	delete[] temp1;
	delete[] temp2;
	delete[] temp01;
	delete[] temp10;
	delete[] temp11;
	
	scale--;
	//������LL���еݹ�ֽ�
	if (scale > 0)
	{
		this->Trans(temp00, imageHeight/2, imageWidth/2, imageWidth/2, scale, temp00);
	}
	this->MatrixRegionCopy(temp00, (imageHeight/2), (imageWidth/2), buff+lineByte*(int)(imageHeight/2),
		imageHeight, lineByte);
	delete[] temp00;

	//�����Ľ��������m_pImgDataOut
	memcpy(outputData, buff,  lineByte * inputHeight);
	delete[] buff;
}
/*---------------------------------------------------*/
/**
*һ��С���ֽ�
* Trans()
*
*    
* @param  *inputData �����������ָ��
* @param  inputHeight �������߶�
* @param  inputWidth ���������
* @param  lineByte �������ֽ���
* @param  scale �ֽ�߶�
* @param  *outputData �����������ָ��
*
* @return
* void
*
* @note 
* ����һ��С���ֽ�
*/
/*---------------------------------------------------*/
void CuiHarrTransformLet::Trans(
	unsigned char *inputData,
	int inputHeight,
	int inputWidth, 
	int lineByte,
	int scale,
	unsigned char *outputData)
{
#if _MSC_VER &&_DEBUG
	LARGE_INTEGER litmp;
	LONGLONG QPart1,QPart2;
	double dfMinus, dfFreq, dfTim;
	QueryPerformanceFrequency(&litmp);
	dfFreq = (double)litmp.QuadPart;// ��ü�������ʱ��Ƶ��
	QueryPerformanceCounter(&litmp);
	QPart1 = litmp.QuadPart;// ��ó�ʼֵ
#endif
	{
			this->Trans_cpu(inputData,inputHeight,inputWidth,lineByte,scale,outputData);				
	}
#if _MSC_VER &&_DEBUG
	QueryPerformanceCounter(&litmp);
	QPart2 = litmp.QuadPart;//�����ֵֹ
	dfMinus = (double)(QPart2-QPart1);
	dfTim = dfMinus / dfFreq;// ��ö�Ӧ��ʱ��ֵ����λΪ��
#endif
}
/*---------------------------------------------------*/
/**
*
* SimpleWaveletTrans()
*
*    
* @param  scale �ֽ�߶ȣ�Ĭ��Ϊ1
*
* @return
* void
*
* @note 
* ��ͼ�����ݽ��и����߶ȵ�С���ֽ�
*/
/*---------------------------------------------------*/
void CuiHarrTransformLet::SimpleWaveletTrans(int scale = 1)
{
	if (scale<1 || scale>5)
	{
		return;
	}
	if(m_pImgDataOut!=NULL){
		delete[] m_pImgDataOut;
		m_pImgDataOut=NULL;
	}
	this->m_nScale = scale;
	int lineByte=(m_imgWidth*m_nBitCount/8+3)/4*4;
	m_pImgDataOut=new unsigned char[lineByte*m_imgHeight];

	this->Trans(m_pImgData, m_imgHeight, m_imgWidth, lineByte, scale, m_pImgDataOut);
}
/*---------------------------------------------------*/
/**
*С���ع�\n
* Reverse()
*
*    
* @param  *inputData �����������ָ��
* @param  inputHeight �������߶�
* @param  inputWidth ���������
* @param  lineByte �������ֽ���
* @param  scale �ֽ�߶�
* @param  *outputData �����������ָ��
*
* @return
* void
*
* @note 
* ����һ��С���ع�
*/
/*---------------------------------------------------*/
void CuiHarrTransformLet::Reverse(unsigned char *inputData, int inputHeight, int inputWidth, int lineByte,
								 int scale, unsigned char *outputData)
{
	unsigned char *buff = new unsigned char[lineByte*m_imgHeight];

	int imageHeight = inputHeight;
	int imageWidth = inputWidth;


	unsigned char *temp10 = new unsigned char[(imageWidth/2) * (imageHeight/2)];
	unsigned char *temp11 = new unsigned char[(imageWidth/2) * (imageHeight/2)];
	this->MatrixRegionCopy(inputData, imageHeight, lineByte, temp10, (imageHeight/2), (imageWidth/2));
	this->MatrixRegionCopy(inputData+(int)(imageWidth/2), imageHeight, lineByte, temp11, (imageHeight/2), (imageWidth/2));
	this->MatrixPlus(temp11, temp10, (imageHeight/2), (imageWidth/2), temp11);
	unsigned char *temp1 = new unsigned char[imageWidth * (int)(imageHeight/2)];
	this->MatrixInsertLine(temp11, temp10, temp1, (imageHeight/2), (imageWidth/2), imageWidth);
	
	unsigned char *temp00 = new unsigned char[(imageWidth/2) * (imageHeight/2)];
	unsigned char *temp01 = new unsigned char[(imageWidth/2) * (imageHeight/2)];
	this->MatrixRegionCopy(inputData+lineByte*(int)(imageHeight/2), imageHeight, lineByte, temp00, (imageHeight/2), (imageWidth/2));
	this->MatrixRegionCopy(inputData+lineByte*(int)(imageHeight/2)+(int)(imageWidth/2),
		imageHeight, lineByte, temp01, (imageHeight/2), (imageWidth/2));
	
	scale--;
	//�ݹ��LL���ݽ����ع�
	if (scale > 0)
	{
		this->Reverse(temp00, imageHeight/2, imageWidth/2, imageWidth/2, scale, temp00);
	}
	this->MatrixPlus(temp01, temp00, (imageHeight/2), (imageWidth/2), temp01);
	unsigned char *temp2 = new unsigned char[imageWidth * (int)(imageHeight/2)];
	this->MatrixInsertLine(temp01, temp00, temp2, (imageHeight/2), (imageWidth/2), imageWidth);

	this->MatrixPlus(temp1, temp2, (imageHeight/2), imageWidth, temp1);
	this->MatrixInsertRow(temp1, temp2, buff, (imageHeight/2), imageWidth, lineByte);
	
	//�ͷſռ�
	delete[] temp1;
	delete[] temp2;
	delete[] temp01;
	delete[] temp10;
	delete[] temp11;
	delete[] temp00;

	//�����Ľ��������m_pImgDataOut
	memcpy(outputData, buff,  lineByte * inputHeight);
	delete[] buff;
}
/*---------------------------------------------------*/
/**
*
*
* SimpleWaveletReverse()
*
*    
* void
*
* @return
* void
*
* @note 
* ����ӦС���ֽ�ϵ������С���ع�
*/
/*---------------------------------------------------*/
void CuiHarrTransformLet::SimpleWaveletReverse()
{
	if (m_nScale <= 0)
	{
		return;
	}
	if (m_pImgDataOut==NULL)
	{
		return;
	}
	int lineByte=(m_imgWidth*m_nBitCount/8+3)/4*4;

	this->Reverse(m_pImgDataOut, m_imgHeight, m_imgWidth, lineByte, m_nScale, m_pImgDataOut);
}
/*---------------------------------------------------*/
/**
*
* H2Zero()
*
*    
*@param coeff ��������磺С��ϵ���ĸ�Ƶ����
*@param cHeight �������������߶ȣ�
*@param cWidth ��������������ȣ�
*@param staticHeight ԭʼͼ��̶��߶�
*@param staticWidth ԭʼͼ��Ĺ̶����
*
* @return
* void
*
* @note 
* �Ը�Ƶϵ������
*/
/*---------------------------------------------------*/
void CuiHarrTransformLet::H2Zero(double* coeff, int cHeight, int cWidth, 
						  int staticHeight, int staticWidth)
{
/*	if (m_nScale <= 0)
	{
		return;
	}
	if (m_pImgDataOut==NULL)
	{
		return;
	}
	int lineByte=(m_imgWidth * m_nBitCount / 8+3)/4*4;


	int k = 1 << m_nScale;
	int h, w, s, d, m = 0;
	for (s = 1, d = 0; s <= m_nScale; s++)
	{
			d = m * lineByte + (m_imgWidth / (1 << s));
			for (h=0; h < m_imgHeight / (1 << s); h++)
			{
				for (w=0; w < m_imgWidth / (1 << s); w++)
				{
					//
					m_pImgDataOut[d + h * lineByte + w] = (BYTE)0;
				}
			}
			m += (m_imgHeight / (1 << s));
	}*/
	int i,j;
	for(i = 0; i < cHeight; i++)
	{         
		for(j = 0; j < cWidth; j++)
		{
			*(coeff + i * staticWidth + j) =0;
		}
	}
}
/*---------------------------------------------------*/
/**
*
* Hard()
*
*    
*@param coeff ��������磺С��ϵ���ĸ�Ƶ����
*@param cHeight �������������߶ȣ�
*@param cWidth ��������������ȣ�
*@param threshold ��ֵ
*@param staticHeight ԭʼͼ��̶��߶�
*@param staticWidth ԭʼͼ��Ĺ̶����
*
* @return
* void
*
* @note 
* ���������ֵʹ��Ӳ��ֵ����������ľ����з�
*/
/*---------------------------------------------------*/
void CuiHarrTransformLet::Hard(double* coeff, int cHeight, int cWidth, double threshold,
						  int staticHeight, int staticWidth)
{
	
	int i,j;
	for(i = 0; i < cHeight; i++)
	{         
		for(j = 0; j < cWidth; j++)
		{
			if((*(coeff + i * staticWidth + j)>=0)&&(*(coeff + i * staticWidth + j) <= threshold) ||
				(*(coeff + i * staticWidth + j)<0)&&(*(coeff + i * staticWidth + j)*(-1) <= threshold))
			{
				*(coeff + i * staticWidth + j) =0;
			}
		}
	}
}
/*---------------------------------------------------*/
/**
*����ֵ����ȥ����
* Soft()
*
*    
*@param coeff ��������磺С��ϵ���ĸ�Ƶ����
*@param cHeight �������������߶ȣ�
*@param cWidth ��������������ȣ�
*@param threshold ��ֵ
*@param staticHeight ԭʼͼ��̶��߶�
*@param staticWidth ԭʼͼ��Ĺ̶����
*
* @return
* void
*
* @note 
* ���������ֵʹ������ֵ����������ľ����з�
*/
/*---------------------------------------------------*/
int CuiHarrTransformLet::GetSgn(double number)
{
	if(number ==0)
	{
		return 0;
	}
	else if(number >0)
	{
		return 1;
	}
	else 
	{
		return (-1);
	}
}
void CuiHarrTransformLet::Soft(double* coeff, int cHeight, int cWidth, double threshold,
						  int staticHeight, int staticWidth)
{
	
	int i,j;
	double temp;
	for(i = 0; i < cHeight; i++)
	{         
		for(j = 0; j < cWidth; j++)
		{
			if((*(coeff + i * staticWidth + j)>=0)&&(*(coeff + i * staticWidth + j) <= threshold) ||
				(*(coeff + i * staticWidth + j)<0)&&(*(coeff + i * staticWidth + j)*(-1) <= threshold))
			{
				*(coeff + i * staticWidth + j) =0;
			}
			else
			{
				temp =*(coeff + i * staticWidth + j);
				if(temp<0)
				{
					temp *=(-1);
				}
				temp -=threshold;
				*(coeff + i * staticWidth + j) = GetSgn(*(coeff + i * staticWidth + j))*temp;
			}
		}
	}
}










/*---------------------------------------------------*/
/**
*�ͷ�ͼ������ڴ�
*
*
*/
/*---------------------------------------------------*/
void CuiHarrTransformLet::ReleaseImgData(void)
{
	if (img_original_size_color!=NULL){
		cvReleaseImage(&img_original_size_color);
	}
	if (img_scr_color!=NULL){
		cvReleaseImage(&img_scr_color);
	}
	if (img_scr_gray!=NULL){
		cvReleaseImage(&img_scr_gray);
	}
	if (img_dst_wavelet!=NULL){
		cvReleaseImage(&img_dst_wavelet);
	}
	delete[] ImgLabels4Quarter;
}
/*---------------------------------------------------*/
/**
*����ͼ�����
*
*
*/
/*---------------------------------------------------*/
void CuiHarrTransformLet::CuiLoadImgBuffer(UINT32* ImgData,int Width,int Height)
{
	  this->ReleaseImgData();
	  img_original_size_color=cvCreateImage(cvSize(Width,Height),IPL_DEPTH_8U,4);
	  memcpy(img_original_size_color->imageData,ImgData,sizeof(UINT32)*Width*Height);
	  this->PreproccessImg();

}
/*---------------------------------------------------*/
/**
*���ļ�·������ͼ�����
*
*@param filename �ļ�·��+�ļ���
*/
/*---------------------------------------------------*/
IplImage* CuiHarrTransformLet::CuiLoadFile(const char *filename)
{
	this->ReleaseImgData();

	img_original_size_color=cvLoadImage(filename);
	this->cui_proportion=1;
	strcpy_s(filename_org,sizeof(filename_org),filename);
	//////////////////////////////////////////////////////////	
    this->PreproccessImg();
	return img_original_size_color;
}
/*---------------------------------------------------*/
/**
*����ͼ�����
*
*
*@param	[in] imgBuffer  ͼ�����
*@param		width   ͼ�������
*@param		height  ͼ�����߶�
*@param	 proportion С����������ʵ����ռ�ı���
*
*/
/*---------------------------------------------------*/
void CuiHarrTransformLet::CuiGetImageData(
	UINT32*		imgBuffer,
	int					width,
	int					height,
	double            proportion)
{
	IplImage *img_t=NULL;  	
	if (img_scr_color!=NULL){
		cvReleaseImage(&img_scr_color);
	}
	if (img_scr_gray!=NULL){
		cvReleaseImage(&img_scr_gray);
	}
	if (img_dst_wavelet!=NULL){
		cvReleaseImage(&img_dst_wavelet);
	}
	img_t=cvCreateImage(cvSize(width,height),IPL_DEPTH_8U,4);
	memcpy(img_t->imageData,imgBuffer,img_t->imageSize);
	this->cui_proportion=proportion;
	strcpy_s(filename_org,sizeof(filename_org),"memory");
#if 1
	//////////////////////////////////////////////////////////	
	CvSize C2bmpsize=cvGetSize(img_t);
	/*if (C2bmpsize.width%8!=0){
		C2bmpsize.width=C2bmpsize.width-C2bmpsize.width%8+8;
	}
	if (C2bmpsize.height%8!=0){
		C2bmpsize.height=C2bmpsize.height-C2bmpsize.height%8+8;	
	}	*/
	img_scr_color=cvCreateImage(C2bmpsize,img_t->depth,img_t->nChannels);
	img_scr_gray=cvCreateImage(C2bmpsize,IPL_DEPTH_8U,1);//�Ҷ�		
	img_dst_wavelet=cvCloneImage(img_scr_gray);
	cvResize(img_t,img_scr_color);
#endif
	cvCvtColor(img_scr_color, img_scr_gray, CV_BGR2GRAY);  
#if 0
cvFlip(img_scr_gray);
#endif
	//ͼ������ָ��
	m_pImgData=(unsigned char*)img_scr_gray->imageData; 
	//ͼ��Ŀ�����Ϊ��λ
	m_imgWidth=img_scr_gray->width;	 
	//ͼ��ĸߣ�����Ϊ��λ
	m_imgHeight=img_scr_gray->height;
	//ÿ����ռ��λ��
	m_nBitCount=m_nBitCountOut=img_scr_gray->nChannels*img_scr_gray->depth;
#if SHOW_HARR_IMG
	cvNamedWindow("HarrScr");
	cvNamedWindow("HarrGray");
	cvNamedWindow("HarrConvert");
#endif
	
	cvReleaseImage(&img_t);

	ImgLabels4Quarter=new int[m_imgWidth*m_imgHeight];
}
/*---------------------------------------------------*/
/**
*HighGUI��ʾС��������ͼ��
*
*
*/
/*---------------------------------------------------*/
void CuiHarrTransformLet::CuiShowImage(void)
{
#if SHOW_HARR_IMG
	cvShowImage("HarrScr",img_scr_color);
	cvShowImage("HarrGray",img_scr_gray);
	cvShowImage("HarrConvert",img_dst_wavelet);
#endif

	
}
/*---------------------------------------------------*/
/**
*С���任
*
*@param  scale С���任����
*/
/*---------------------------------------------------*/
void CuiHarrTransformLet::CuiWaveletTrans(int scale)
{
	cvFlip(img_scr_gray);
	this->SimpleWaveletTrans(scale);
	if (m_pImgDataOut!=NULL){	
		memcpy(img_dst_wavelet->imageData,m_pImgDataOut,img_dst_wavelet->imageSize);
		cvFlip(img_dst_wavelet);
	}
	cvFlip(img_scr_gray);
}
/*---------------------------------------------------*/
/**
*С����任
*
*
*/
/*---------------------------------------------------*/
void CuiHarrTransformLet::CuiWaveletReverse(void)
{
	cvFlip(img_scr_gray);
	this->SimpleWaveletReverse();
	if (m_pImgDataOut!=NULL){	
		memcpy(img_dst_wavelet->imageData,m_pImgDataOut,img_dst_wavelet->imageSize);
		cvFlip(img_dst_wavelet);
	}
	cvFlip(img_scr_gray);
}
/*---------------------------------------------------*/
/**
*����С���任��ͼ����н��봦��
*
*@param  scale ������� Ĭ����1
*/
/*---------------------------------------------------*/
IplImage* CuiHarrTransformLet::HarrSmooth(int scale)
{
   this->CuiWaveletTrans(scale);
   /************�˲�*****************/
	  IplImage img_convert;
	  cvInitImageHeader(&img_convert,cvSize(img_scr_gray->width,img_scr_gray->height),IPL_DEPTH_8U,1);
	  img_convert.imageData=(char *)m_pImgDataOut;
	  //С������Ϊ�Ҷ�ͼ
	  cvFlip(&img_convert);
#if 1
	  //ȫ���˲�
	  for (int i=0;i<img_convert.width;i++){	//colm
		  for (int j=0;j<img_convert.height;j++){	//row
			  if ((i<img_convert.width/powl(2,scale))&&
				  (j<img_convert.height/powl(2,scale))){
			  }else{
				  cvSetReal2D(&img_convert,j,i,128);
			  }

		  }

	  }
#else
	  //��Ƶ�˲�
	  for (int i=0;i<img_convert.width;i++){	//colm
		  for (int j=0;j<img_convert.height;j++){	//row
			  if ((i>img_convert.width/2)&&
				  (j>img_convert.height/2)){
			  	     cvSetReal2D(&img_convert,j,i,128);
			  
			  }

		  }

	  }
#endif
	  cvFlip(&img_convert);
/******************************************/
	  if (m_pImgDataOut!=NULL){	
		  memcpy(img_dst_wavelet->imageData,m_pImgDataOut,img_dst_wavelet->imageSize);
		  cvFlip(img_dst_wavelet);
	  }
/******************************************/

#if SHOW_HARR_IMG
   this->CuiShowImage();
	 cvWaitKey(0);
#endif
	
   /*********************************/
    this->CuiWaveletReverse();
	return img_dst_wavelet;
}
/*---------------------------------------------------*/
/**
*����С���任������������ֲ�
*
*@param rect  ���������ľ�������
*@param light   ���������Ƿ�ΪС���任�ĵ�Ƶ��
*- TRUE Ϊ��ƵLL��
*- FALSE Ϊ�ǵ�Ƶ��,��Ҫ��ȥ128 
*@return С������
*/
/*---------------------------------------------------*/
double CuiHarrTransformLet::CuiCalculateEnergy(CvRect rect,int light)
{
	
#if 1
double Energy=0;
	double t=0;
	
	  cvSetImageROI(img_dst_wavelet,rect);
	 //cvThreshold(img_dst_wavelet,img_dst_wavelet,128,255,CV_THRESH_OTSU);
	 //IplImage *temp_t=cvCloneImage(img_dst_wavelet);
	  //cvAdaptiveThreshold(temp_t,img_dst_wavelet,255,CV_ADAPTIVE_THRESH_MEAN_C,CV_THRESH_BINARY,25,10);
      //cvReleaseImage(&temp_t);
	 for (register int i=0;i<rect.width;i++){	//colm
		 for (register int j=0;j<rect.height;j++){	//row
				 if (light){
				    t=cvGetReal2D(img_dst_wavelet,j,i);
				 }else{
				    t=cvGetReal2D(img_dst_wavelet,j,i)-128;
				 }			 
				 Energy+=t*t;
		 }
		 
	 }
	 cvResetImageROI(img_dst_wavelet);
	 Energy=Energy/(rect.width*rect.height*cui_proportion);//ƽ������
	 //printf("E_LH=%e",Energy);
	return Energy;
#endif

	
#if 0
	/*��δ�����BUG*/
double Energy=0;
	unsigned char t=0;	
	 int x_top=rect.width+rect.x;
	 int y_top=rect.height+rect.y;
	for (register int i=rect.x;i<x_top;i++){	//colm
		for (register int j=rect.y;j<y_top;j++){	//row
				 if (light){
				    t=((uchar *) (img_dst_wavelet->imageData + j*img_dst_wavelet->widthStep))[i];
				 }else{
				    t=((uchar *) (img_dst_wavelet->imageData + j*img_dst_wavelet->widthStep))[i]-128;
				 }			 
				 Energy+=t*t;
		 }		 
	 }
	 Energy=Energy/(rect.width*rect.height*cui_proportion);//ƽ������
	return Energy;
#endif

	
}
/*---------------------------------------------------*/
/**
*����С���任���м���
*
*
*/
/*---------------------------------------------------*/
void CuiHarrTransformLet::CuiSaveImg(void){

	if (cui_GeneralImgProcess::SAVE_DEBUG_2DISK==TRUE){
		char filename[1024];
			sprintf(filename,"%s_Harr.jpg",filename_org);
			cvSaveImage(filename,img_dst_wavelet);
			/************************************/
			{
			  ofstream result_flie;
			  result_flie.open("CuiHarrTransformLet.txt",ios::app|ios::out);
			  if (result_flie.is_open()){
				   result_flie<<filename_org<<endl;
				   result_flie<<"  L_LL: "<<harr_energy.EnergyL_LL
							  <<"  L_LH: "<<harr_energy.EnergyL_LH
							  <<"  L_HL: "<<harr_energy.EnergyL_HL
							  <<"  L_HH: "<< harr_energy.EnergyL_HH

							  <<"   LL:    "<<harr_energy.Energy_LL
							  <<"   LH:    "<<harr_energy.Energy_LH
							  <<"   HL:    "<<harr_energy.Energy_HL
							  <<"   HH:    "<<harr_energy.Energy_HH<<endl;
			  }else{

				  cout<<"Open Fail"<<endl;
			  }
	 
			  result_flie.close();
			}	           
			/*************************************/
	}else{

	}

	
}
/*---------------------------------------------------*/
/**
*����С���任�������������
*
*
*/
/*---------------------------------------------------*/
void CuiHarrTransformLet::CuiCalculateEnergyALL(void)
{
	harr_energy.harrDepth=this->m_nScale;;
	if (this->m_nScale==2){
		harr_energy.EnergyL_LL=CuiCalculateEnergy(cvRect(0,0,
															img_dst_wavelet->width/2/2,img_dst_wavelet->height/2/2),1);
		harr_energy.EnergyL_LH=CuiCalculateEnergy(cvRect(0,img_dst_wavelet->height/2/2,
															img_dst_wavelet->width/2/2,img_dst_wavelet->height/2/2));
		harr_energy.EnergyL_HL=CuiCalculateEnergy(cvRect(img_dst_wavelet->width/2/2,0,
															img_dst_wavelet->width/2/2,img_dst_wavelet->height/2/2));
		harr_energy.EnergyL_HH=CuiCalculateEnergy(cvRect(img_dst_wavelet->width/2/2,img_dst_wavelet->height/2/2,
			                                               img_dst_wavelet->width/2/2,img_dst_wavelet->height/2/2));
		harr_energy.Energy_LH=CuiCalculateEnergy(cvRect(0,img_dst_wavelet->height/2,
															img_dst_wavelet->width/2,img_dst_wavelet->height/2));
		harr_energy.Energy_HL=CuiCalculateEnergy(cvRect(img_dst_wavelet->width/2,0,
															img_dst_wavelet->width/2,img_dst_wavelet->height/2));
		harr_energy.Energy_HH=CuiCalculateEnergy(cvRect(img_dst_wavelet->width/2,img_dst_wavelet->height/2,
															img_dst_wavelet->width/2,img_dst_wavelet->height/2));
		
	}
	if (this->m_nScale==1){
		harr_energy.EnergyL_LL=0;
		harr_energy.EnergyL_LH=0;
		harr_energy.EnergyL_HL=0;
		harr_energy.EnergyL_HH=0;
		harr_energy.Energy_LL=CuiCalculateEnergy(cvRect(0,0,
			img_dst_wavelet->width/2,img_dst_wavelet->height/2),1);
		harr_energy.Energy_LH=CuiCalculateEnergy(cvRect(0,img_dst_wavelet->height/2,
			img_dst_wavelet->width/2,img_dst_wavelet->height/2));
		harr_energy.Energy_HL=CuiCalculateEnergy(cvRect(img_dst_wavelet->width/2,0,
			img_dst_wavelet->width/2,img_dst_wavelet->height/2));
		harr_energy.Energy_HH=CuiCalculateEnergy(cvRect(img_dst_wavelet->width/2,img_dst_wavelet->height/2,
			img_dst_wavelet->width/2,img_dst_wavelet->height/2));

	}
}
/*---------------------------------------------------*/
/**
*
*
*/
/*---------------------------------------------------*/
void CuiHarrTransformLet::CalculateEnergyAllApBlock(SpSetProperty& SpSet)
{

	if (this->m_nScale==1){
		//LL
		double t=0;
		for (register int x=0;x<m_imgWidth/2;x++){
			for(register int y=0;y<m_imgHeight/2;y++){
				int spIdx=ImgLabels4Quarter[x+y*m_imgWidth];
					t=cvGetReal2D(img_dst_wavelet,y,x);
					SpSet.SpPropertySet[spIdx].harr_energy.Energy_LL+=fabs(t);	
					SpSet.SpPropertySet[spIdx].harr_energy.Size_LL++;
			}
		}
		//LH
		for (register int x=0;x<m_imgWidth/2;x++){
			for(register int y=m_imgHeight/2;y<m_imgHeight;y++){
				int spIdx=ImgLabels4Quarter[x+y*m_imgWidth];
				t=cvGetReal2D(img_dst_wavelet,y,x)-128;
				SpSet.SpPropertySet[spIdx].harr_energy.Energy_LH+=fabs(t);
				SpSet.SpPropertySet[spIdx].harr_energy.Size_LH++;
			}
		}
		//HL
		for (register int x=m_imgWidth/2;x<m_imgWidth;x++){
			for(register int y=0;y<m_imgHeight/2;y++){
				int spIdx=ImgLabels4Quarter[x+y*m_imgWidth];
				t=cvGetReal2D(img_dst_wavelet,y,x)-128;
				SpSet.SpPropertySet[spIdx].harr_energy.Energy_HL+=fabs(t);
				SpSet.SpPropertySet[spIdx].harr_energy.Size_HL++;
			}
		}
		//HH
		for (register int x=m_imgWidth/2;x<m_imgWidth;x++){
			for(register int y=m_imgHeight/2;y<m_imgHeight;y++){
				int spIdx=ImgLabels4Quarter[x+y*m_imgWidth];
				t=cvGetReal2D(img_dst_wavelet,y,x)-128;
				SpSet.SpPropertySet[spIdx].harr_energy.Energy_HH+=fabs(t);
				SpSet.SpPropertySet[spIdx].harr_energy.Size_HH++;
			}
		}	
		//sigema
		for (register int spi=0;spi<SpSet.SpPropertySet.size();spi++){
			////////////////////////////////////////////////////////
			SpSet.SpPropertySet[spi].harr_energy.Energy_LL/=SpSet.SpPropertySet[spi].harr_energy.Size_LL;
			SpSet.SpPropertySet[spi].harr_energy.Energy_HH/=SpSet.SpPropertySet[spi].harr_energy.Size_HH;
			SpSet.SpPropertySet[spi].harr_energy.Energy_HL/=SpSet.SpPropertySet[spi].harr_energy.Size_HL;
			SpSet.SpPropertySet[spi].harr_energy.Energy_LH/=SpSet.SpPropertySet[spi].harr_energy.Size_LH;
			////////////////////////////////////////////////////////
			SpSet.SpPropertySet[spi].harr_energy.EnergyHighsigma=SpSet.SpPropertySet[spi].harr_energy.Energy_HH
				+SpSet.SpPropertySet[spi].harr_energy.Energy_HL
				+SpSet.SpPropertySet[spi].harr_energy.Energy_LH;
			///////////////////////////////////////////////////////
			SpSet.SpPropertySet[spi].harr_energy.EnergyL_LL=SpSet.SpPropertySet[spi].harr_energy.Energy_LL;
			///////////////////////////////////////////////////////
		}
	}
	
}
/*---------------------------------------------------*/
/**
*��ȡ�����С������
*
*@param [out]energy С���������ݽṹ
*/
/*---------------------------------------------------*/
void CuiHarrTransformLet::CuiGetAllEnergy(Energy_Harr* energy)
{
	//harr_energy.Category_Sky=energy->Category_Sky;
	//harr_energy.Sky_Candidate=energy->Sky_Candidate;
	
	
	if (this->m_nScale==2){
		harr_energy.Energy_LL=harr_energy.EnergyL_LL=sqrtl(harr_energy.EnergyL_LL);	
	}
	if (this->m_nScale==1){
	    harr_energy.Energy_LL=harr_energy.EnergyL_LL=sqrtl(harr_energy.Energy_LL);
	}
	harr_energy.EnergyHighsigma=sqrtl(harr_energy.Energy_HH+harr_energy.Energy_HL+harr_energy.Energy_LH);
	memcpy(energy,&harr_energy,sizeof(harr_energy));

}
/*---------------------------------------------------*/
/**
*ȥ��ͼ�������߽��ϣ�������С������
*
*@param [in] img_boder ����ȥ���߽�����������ģ��
*@deprecated ������
*/
/*---------------------------------------------------*/
void CuiHarrTransformLet::CuiAdjustBoder(IplImage *img_boder)
{
	if (this->m_nScale==2){

		for (register int i=0;i<img_dst_wavelet->width;i++){	//colm
			for (register int j=0;j<img_dst_wavelet->height;j++){	//row
				if ((i<img_dst_wavelet->width/4)&&(j<img_dst_wavelet->height/4)){
					 // ��Ƶ����ֵ����
				}else{
					if (cvGetReal2D(img_boder,j,i)!=128){
						 cvSetReal2D(img_dst_wavelet,j,i,128);
					}
					 
				}
			}
			;
		}
	}
	if (this->m_nScale==1){

		for (register int i=0;i<img_dst_wavelet->width;i++){	//colm
			for (register int j=0;j<img_dst_wavelet->height;j++){	//row
				if ((i<img_dst_wavelet->width/2)&&(j<img_dst_wavelet->height/2)){
					// ��Ƶ����ֵ����
				}else{
					if (cvGetReal2D(img_boder,j,i)!=128){
						cvSetReal2D(img_dst_wavelet,j,i,128);
					}

				}
			}
			;
		}
	}
}
/*---------------------------------------------------*/
/**
*ȥ��ͼ�������߽��ϣ�������С������\n
*(ȥ��Ӧͼ���Ե��������α����)
*@param [in] img_boder ����ȥ���߽�����������ģ��
*
*
*/
/*---------------------------------------------------*/
void CuiHarrTransformLet::CuiAdjustBoderByContour(IplImage *img_boder)
{
	if (this->m_nScale==2){

		for (register int i=0;i<img_dst_wavelet->width;i++){	//colm
			for (register int j=0;j<img_dst_wavelet->height;j++){	//row
				if ((i<img_dst_wavelet->width/4)&&(j<img_dst_wavelet->height/4)){
					// ��Ƶ����ֵ����
				}else{
					if (cvGetReal2D(img_boder,j,i)!=0){
						cvSetReal2D(img_dst_wavelet,j,i,128);
					}

				}
			}
			;
		}
	}
#if 0
    cvShowImage("img_boder",img_boder);
	
	cvShowImage("img_dst_wavelet",img_dst_wavelet);
	cvWaitKey(10);
#endif
	ASSERT(this->m_nScale==1);

	if (this->m_nScale==1){

		/*CvScalar  color_no_remove=cvScalar(0,0,0,0);
		CvScalar  color_t;*/
		for (register int i=0;i<img_dst_wavelet->width;i++){	//colm
			for (register int j=0;j<img_dst_wavelet->height;j++){	//row
				if ((i<img_dst_wavelet->width/2)&&(j<img_dst_wavelet->height/2)){
					// ��Ƶ����ֵ����
				}else{					
					//color_t=cvGet2D(img_boder,j,i);
					//if (color_t.val[0]!=color_no_remove.val[0]){
					//	cvSetReal2D(img_dst_wavelet,j,i,128);
					//}
					unsigned char color_td=((uchar *) (img_boder->imageData + j*img_boder->widthStep))[i];
					if(color_td!=0){
						((uchar *)(img_dst_wavelet->imageData + j*img_dst_wavelet->widthStep))[i]=128;
					}
					

				}
			}
		}
	}
#if 0
	cvShowImage("img_boder",img_boder);
	cvSaveImage("boder.jpg",img_boder);
	cvShowImage("img_dst_wavelet",img_dst_wavelet);
	cvSaveImage("img_dst_wavelet.jpg",img_dst_wavelet);
	cvWaitKey(10);
#endif
}
/*---------------------------------------------------*/
/**
*Ԥ���������ͼ��\n
*
*@note ͼ���ȱ�����8������������[a r g b]��ͨ��
*/
/*---------------------------------------------------*/
void CuiHarrTransformLet::PreproccessImg(void)
{
	//////////////////////////////////////////////////////////	
	CvSize C2bmpsize=cvGetSize(img_original_size_color);

	C2bmpsize.width=(C2bmpsize.width+3)/4*4;
	C2bmpsize.height=(C2bmpsize.height+3)/4*4;

	img_scr_color=cvCreateImage(C2bmpsize,img_original_size_color->depth,img_original_size_color->nChannels);
	img_scr_gray=cvCreateImage(C2bmpsize,IPL_DEPTH_8U,1);//�Ҷ�		
	
	img_dst_wavelet=cvCloneImage(img_scr_gray);
	cvResize(img_original_size_color,img_scr_color);
	cvCvtColor(img_scr_color, img_scr_gray, CV_BGR2GRAY);  
	//ͼ������ָ��
	m_pImgData=(unsigned char*)img_scr_gray->imageData; 
	//ͼ��Ŀ�����Ϊ��λ
	m_imgWidth=img_scr_gray->width;	 
	//ͼ��ĸߣ�����Ϊ��λ
	m_imgHeight=img_scr_gray->height;
	//ÿ����ռ��λ��
	m_nBitCount=m_nBitCountOut=img_scr_gray->nChannels*img_scr_gray->depth;

#if SHOW_HARR_IMG
	cvNamedWindow("HarrScr");
	cvNamedWindow("HarrGray");
	cvNamedWindow("HarrConvert");
#endif


}
/*---------------------------------------------------*/
/**
*����߽�ͼ��\n
*
*@param С���任����
*@note  �߽�ͼ��Ϊ�ڰ�ͼ��\n
*��ԭʼ���������
*/
/*---------------------------------------------------*/
void CuiHarrTransformLet::CalculateBoderImg_Block(int scale)
{
	ASSERT(scale==1);
	/////////////////////////////
	UINT32 color=0x00ffffff;
	IplImage *boder_img=cvCreateImage(cvSize(m_imgWidth,m_imgHeight),IPL_DEPTH_8U,4);
	IplImage *boder_img_quarter=cvCreateImage(cvSize(m_imgWidth/2,m_imgHeight/2),IPL_DEPTH_8U,4);
	IplImage *contour_img_quarter=cvCreateImage(cvSize(m_imgWidth/2,m_imgHeight/2),IPL_DEPTH_8U,4);
	CvScalar WhiteColor=cvScalar(color&0xff,(color>>8)&0xff,(color>>16)&0xff,(color>>24)&0xff);
	CvScalar BlackColor=cvScalar(0,0,0,0);
//#if 0
//cvFlip(this->img_scr_color);
//#endif
	
	cvCopyImage(this->img_scr_color,boder_img);
	cvResize(this->img_scr_color,boder_img_quarter);
//#if 0
//cvFlip(this->img_scr_color);
//#endif
	
	cvZero(contour_img_quarter);
	//cvNot(contour_img_quarter,contour_img_quarter);
#if 1

	if (1==scale){
		int i=0;
		int mode=CV_RETR_CCOMP;        //��ȡ������ģʽ
		int contoursNum=0;            //��ȡ��������Ŀ
		CvScalar externalColor;
		CvScalar holeColor;
		CvMemStorage * storage=cvCreateMemStorage(0);    //��ȡ������Ҫ�Ĵ������� 0ΪĬ��64KB
		CvSeq * pcontour=0;  //��ȡ����������ָ��	
		IplImage * src=cvCloneImage(boder_img_quarter);
		IplImage * pImg=cvCreateImage(cvGetSize(src),src->depth,1);

		cvCvtColor(src,pImg,CV_RGB2GRAY);    //��ͼ��ת��Ϊ�Ҷ�
#if 0		
		cvNamedWindow("src",CV_WINDOW_AUTOSIZE);
		cvNamedWindow("pcontour",CV_WINDOW_AUTOSIZE);
		cvShowImage("src",src);
#endif

		//	cvThreshold(pImg,pImg,180,255,CV_THRESH_BINARY);    //��ֵ��
		//--------------��������----------------
		mode=CV_RETR_LIST;
		contoursNum=cvFindContours(pImg,storage,&  pcontour,sizeof(CvContour),mode,CV_CHAIN_APPROX_NONE);
		cout<<contoursNum<<" "<<endl;
		//--------------������----------------
		if (contoursNum>0){
			
				if (pcontour!=NULL){
					
#if 0
					holeColor=WhiteColor;
					externalColor=BlackColor;
#else
					holeColor=BlackColor;
					externalColor= WhiteColor;
#endif					
					cvDrawContours(contour_img_quarter,pcontour,externalColor,holeColor,1,CV_FILLED,8);
					cvDrawContours(contour_img_quarter,pcontour,BlackColor,BlackColor,1,4,8);
				}

		}
#if 0
	cvDrawRect(contour_img_quarter,cvPoint(1,1),cvPoint(m_imgWidth/2-1,m_imgHeight/2-1),WhiteColor,4);
#endif
	
		/**********************************************/
		int p_x[4]={0,0,m_imgWidth/2,m_imgWidth/2};
		int p_y[4]={0,m_imgHeight/2,0,m_imgHeight/2};
		cvZero(boder_img);
		for (int i=0;i<4;i++)
		{
			cvSetImageROI(boder_img,cvRect(p_x[i],p_y[i],m_imgWidth/2,m_imgHeight/2));
			cvCopyImage(contour_img_quarter,boder_img);
			cvResetImageROI(boder_img);
		}
		cvNot(boder_img,boder_img);
		cvConvertImage(boder_img,this->img_dst_wavelet,CV_BGRA2GRAY);
		/***********************************************/
#if 0
		cvShowImage("boder_img",img_dst_wavelet);
		cvShowImage("contour_img_quarter",contour_img_quarter);
		cvWaitKey(0);
#endif

		cvReleaseImage(&src);
		cvReleaseImage(&pImg);
		cvReleaseMemStorage(&storage);
	}

#endif
	cvReleaseImage(&contour_img_quarter);
	cvReleaseImage(&boder_img);
	cvReleaseImage(&boder_img_quarter);

}
/*---------------------------------------------------*/
/**
*
*
*/
/*---------------------------------------------------*/
int CuiHarrTransformLet::CalculateBoderContours(int width,int height)
{
	int S=width*height;
	int Scale=10000;
	S/=Scale;
	if (S<4){
		return 9;
	}else if (S>=4&&S<8){
		return 12;
	}else if (S>=8&&S<20){
		return 14;
	}else if (S>=20&&S<40){
		return 17;
	}else if (S>=40){
		return 22;
	}else{
		ASSERT(0);
		return 9;
	}
	ASSERT(0);
}
/*---------------------------------------------------*/
/**
*
*
*/
/*---------------------------------------------------*/
int CuiHarrTransformLet::CalculateBoderContoursSC(float S,float C)
{
	float Thinkness=0;
	float P=0.3;
	Thinkness=2*S*P/C;
	return (int)(Thinkness+1);
}
/*---------------------------------------------------*/
/**
*
*
*/
/*---------------------------------------------------*/
float   CuiHarrTransformLet::CalculateImgArea(IplImage *contour_img_quarter)
{
	float  area=0;
	UINT32 color=0x00ffffff;
	CvScalar WhiteColor=cvScalar(color&0xff,(color>>8)&0xff,(color>>16)&0xff,(color>>24)&0xff);
	for (int x=0;x<contour_img_quarter->width;x++){
		for (int y=0;y<contour_img_quarter->height;y++){
			CvScalar PixelColor=cvGet2D(contour_img_quarter,y,x);
			if ((PixelColor.val[0]==WhiteColor.val[0])&&
				(PixelColor.val[1]==WhiteColor.val[1])&&
				(PixelColor.val[2]==WhiteColor.val[2])&&
				(PixelColor.val[3]==WhiteColor.val[3])){
					area++;
			}
		}
	}
	return area;
}
/*---------------------------------------------------*/
/**
*
*
*/
/*---------------------------------------------------*/
int CuiHarrTransformLet::CalculateBoderContoursThinkness(CvSeq * pcontour,int width,int height)
{
	UINT32 color=0x00ffffff;
	UINT32 color_B=0x00000000;
	int ContoursThickness=1;
	CvScalar BlackColor=cvScalar(color_B&0xff,(color_B>>8)&0xff,(color_B>>16)&0xff,(color_B>>24)&0xff);
	CvScalar WhiteColor=cvScalar(color&0xff,(color>>8)&0xff,(color>>16)&0xff,(color>>24)&0xff);
	IplImage *contour_img_quarter=cvCreateImage(cvSize(width,height),IPL_DEPTH_8U,4);
	IplImage *contour_img_Block=cvCreateImage(cvSize(width,height),IPL_DEPTH_8U,4);
	CvScalar externalColor;
	CvScalar holeColor;
    CvSeq*	 pcontourH=pcontour;
	float areaAll,areaReduce,RemovePer;
	do {
		ContoursThickness+=4;
		areaAll=0;
		areaReduce=0;
		cvSet(contour_img_quarter,BlackColor);
		cvSet(contour_img_Block,BlackColor);
		for (pcontour=pcontourH;pcontour!=0;pcontour=pcontour->h_next){			
			holeColor=WhiteColor;
			externalColor=WhiteColor;
			cvDrawContours(contour_img_Block,pcontour,externalColor,holeColor,1,CV_FILLED);//��ɫ����
			cvDrawContours(contour_img_quarter,pcontour,externalColor,holeColor,1,CV_FILLED);//��ɫ����
			////////////////////////////////////////////////////
			holeColor=BlackColor;
			externalColor=BlackColor;
			cvDrawContours(contour_img_Block,pcontour,externalColor,holeColor,1,ContoursThickness);//��ɫ����	
		}
		areaAll=CalculateImgArea(contour_img_quarter);//ԭʼͼ���С����
		areaReduce=CalculateImgArea(contour_img_Block);//ԭʼͼ���С����
#if 0	

		cvSaveImage("contour_img_quarter.jpg",contour_img_quarter);
		cvSaveImage("contour_img_Block.jpg",contour_img_Block);

#endif
		if (areaAll==0) return 1;
		 RemovePer=1-areaReduce/areaAll;

	}while(RemovePer<0.2);	
	cvReleaseImage(&contour_img_quarter);
	cvReleaseImage(&contour_img_Block);
	return ContoursThickness;
}
/*---------------------------------------------------*/
/**
*
*
*/
/*---------------------------------------------------*/
void CuiHarrTransformLet::CalculateBoderImg(int scale)
{
	ASSERT(scale==1);
	/////////////////////////////
	UINT32 color=0x00ffffff;
	//int ContoursThickness=CalculateBoderContours(m_imgWidth,m_imgHeight);
	IplImage *boder_img=cvCreateImage(cvSize(m_imgWidth,m_imgHeight),IPL_DEPTH_8U,4);
	IplImage *boder_img_quarter=cvCreateImage(cvSize(m_imgWidth/2,m_imgHeight/2),IPL_DEPTH_8U,4);
	IplImage *contour_img_quarter=cvCreateImage(cvSize(m_imgWidth/2,m_imgHeight/2),IPL_DEPTH_8U,4);
	CvScalar WhiteColor=cvScalar(color&0xff,(color>>8)&0xff,(color>>16)&0xff,(color>>24)&0xff);
	/*cvFlip(this->img_scr_color);*/
	cvCopyImage(this->img_scr_color,boder_img);
	cvResize(this->img_scr_color,boder_img_quarter);
	/*cvFlip(this->img_scr_color);*/
	cvZero(contour_img_quarter);
#if 1

	if (1==scale){
		int i=0;
		int mode=CV_RETR_CCOMP;        //��ȡ������ģʽ
		int contoursNum=0;            //��ȡ��������Ŀ
		CvScalar externalColor;
		CvScalar holeColor;
		CvMemStorage * storage=cvCreateMemStorage(0);    //��ȡ������Ҫ�Ĵ������� 0ΪĬ��64KB
		CvSeq * pcontour=0;  //��ȡ����������ָ��	
		IplImage * src=cvCloneImage(boder_img_quarter);
		IplImage * pImg=cvCreateImage(cvGetSize(src),src->depth,1);

		cvCvtColor(src,pImg,CV_RGB2GRAY);    //��ͼ��ת��Ϊ�Ҷ�
#if 0		
		cvNamedWindow("src",CV_WINDOW_AUTOSIZE);
		cvNamedWindow("pcontour",CV_WINDOW_AUTOSIZE);
		cvShowImage("src",src);
#endif

		//	cvThreshold(pImg,pImg,180,255,CV_THRESH_BINARY);    //��ֵ��
		//--------------��������----------------
		mode=CV_RETR_LIST;
		contoursNum=cvFindContours(pImg,storage,&  pcontour,sizeof(CvContour),mode,CV_CHAIN_APPROX_NONE);
#if 0
		cout<<contoursNum<<" "<<endl;
#endif
		
#if 0
		//--------------������----------------
		if (contoursNum>0){

			if (pcontour!=NULL){
				UINT32 color_B=0x00000000;
				holeColor=cvScalar(color_B&0xff,(color_B>>8)&0xff,(color_B>>16)&0xff,(color_B>>24)&0xff);
				externalColor= WhiteColor;
				float S=cvContourArea(pcontour);
				float C=cvArcLength(pcontour);
				int ContoursThickness=CalculateBoderContoursSC(fabs(S),C);
				cvDrawContours(contour_img_quarter,pcontour,externalColor,holeColor,1,ContoursThickness);
			}

		}
#endif
#if 0
		float S=0,C=0;
		CvSeq *headcontour=pcontour;
		for (;pcontour!=0;pcontour=pcontour->h_next)
		{	
			S+=fabs(cvContourArea(pcontour));
			C+=cvArcLength(pcontour);			
		}
		int ContoursThickness=CalculateBoderContoursSC(S,C);
		pcontour=headcontour;
#endif
		int ContoursThickness=CalculateBoderContoursThinkness(pcontour,m_imgWidth/2,m_imgHeight/2);
		for (;pcontour!=0;pcontour=pcontour->h_next)
		{
			holeColor=WhiteColor;
			externalColor=WhiteColor;
			cvDrawContours(contour_img_quarter,pcontour,externalColor,holeColor,1,ContoursThickness);
		}
		
#if 0
		cvDrawRect(contour_img_quarter,cvPoint(1,1),cvPoint(m_imgWidth/2-1,m_imgHeight/2-1),WhiteColor,4);
#endif

		/**********************************************/
		int p_x[4]={0,0,m_imgWidth/2,m_imgWidth/2};
		int p_y[4]={0,m_imgHeight/2,0,m_imgHeight/2};
		cvZero(boder_img);
		for (register int i=0;i<4;i++)
		{
			cvSetImageROI(boder_img,cvRect(p_x[i],p_y[i],m_imgWidth/2,m_imgHeight/2));
			cvCopyImage(contour_img_quarter,boder_img);
			cvResetImageROI(boder_img);
		}
		cvConvertImage(boder_img,this->img_dst_wavelet,CV_BGRA2GRAY);
		/***********************************************/
#if 0
		cvShowImage("boder_img",img_dst_wavelet);
		cvShowImage("contour_img_quarter",contour_img_quarter);
		cvWaitKey(0);
#endif
		cvReleaseImage(&src);
		cvReleaseImage(&pImg);
		cvReleaseMemStorage(&storage);
	}

#endif
	cvReleaseImage(&contour_img_quarter);
	cvReleaseImage(&boder_img);
	cvReleaseImage(&boder_img_quarter);
}
/*---------------------------------------------------*/
/**
*
*@return ����С�����ͼ  
*/
/*---------------------------------------------------*/
int CuiHarrTransformLet::Scale(void)
{
	return this->m_nScale;
}
/*---------------------------------------------------*/
/**
*
*
*/
/*---------------------------------------------------*/
void  CuiHarrTransformLet::SetSuperPixelLabels(int* ImgLabels,int Width,int Height)
{
	ASSERT(sizeof(int)==4);
	ASSERT(Width==m_imgWidth);
	ASSERT(Height==m_imgHeight);
	int WidthQuarter=Width/2;
	int HeightQuarter=Height/2;
	int* ImgLabelsQuarter=new int[WidthQuarter*HeightQuarter];
	for (register int x=0;x<Width;x+=2){
		for (register int y=0;y<Height;y+=2){
			int xq=x/2;
			int yq=y/2;
			ImgLabelsQuarter[xq+yq*WidthQuarter]=ImgLabels[x+y*Width];
		}
	}
	/////////////////
	int X_base[4]={0,WidthQuarter,0,WidthQuarter};
	int Y_base[4]={0,0,HeightQuarter,HeightQuarter};
	for (register int i=0;i<4;i++){
		int x_org=X_base[i];
		int y_org=Y_base[i];
		for (register int x=0;x<WidthQuarter;x++){
			for (register int y=0;y<HeightQuarter;y++){
				int x_new=x_org+x;
				int y_new=y_org+y;
				ImgLabels4Quarter[x_new+y_new*Width]=ImgLabelsQuarter[x+y*WidthQuarter];
			}
		}
	}
#if 0
	cui_GeneralImgProcess::ShowImgLabels(ImgLabels,Width,Height);
	cui_GeneralImgProcess::ShowImgLabels(ImgLabelsQuarter,WidthQuarter,HeightQuarter);
	cui_GeneralImgProcess::ShowImgLabels(ImgLabels4Quarter,Width,Height);
#endif
	delete[] ImgLabelsQuarter;
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
/*---------------------------------------------------*/
/**
*
*
*/
/*---------------------------------------------------*/