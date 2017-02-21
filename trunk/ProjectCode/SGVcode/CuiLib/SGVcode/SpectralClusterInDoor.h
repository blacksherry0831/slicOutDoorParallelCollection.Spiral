#pragma once
#include <platform_cui.h>
/*------------------------------------------------------------------------*/
#define  C_Used2FindVP	 22222
#define  InDoorThreshold  (0.9)
#define  Iteration__Threshold__InDoor   (0.9)
/*----------------------------------------------------------------------------------------------------------------*/
/** 
*  class SpectralClusterInDoor
*  形态谱聚类算法
*@see  bool SpectralClusterInDoor::Spectral_Clustering_B()
*@note谱聚类天空 立面 地面 不能穿通\n
*  超像素的轮廓
*  
/*----------------------------------------------------------------------------------------------------------------*/
class SpectralClusterInDoor
{
private:
	ImageMemData* pMD;/**<图像数据及计算的中间变量*/
	CvMemStorage* storage;/**<opencv计算用的中间内存*/ 
	CvSeq* lines;/**<opencv的序列，占用CvMemStorage，内存空间*/
public:
	SpectralClusterInDoor(ImageMemData* pMD_t);
	~SpectralClusterInDoor(void);
private:
	IplImage* gray_Color_contour_img;/**<灰度图像对应的轮廓图（调试用）*/
	IplImage* gray_img;/**<原图对应的灰度图像，单通道*/
	IplImage* gray_binary_img;/**<灰度经二值化后的图像*/
public:
/*--------------------------------*/
	void GetMatrix_E(void);
	void GetMatrix_D(void);
	void GetMatrix_L(void);
	void GetMatrix_W(void);
	void GetMatrix_W_1_44_84_96_136_180(void);
/*--------------------------------*/	
static	float Calculate2HistBLikely(
		HistData180 histOne,
		HistData180 histTwo,
		int Dim);
/*--------------------------------*/	
static	float Calculate2HistBLikely(
		 ManhattanHistogram histOne,
		 ManhattanHistogram histTwo);
/*--------------------------------*/	
static	float Calculate2HistBLikelyByManhattan(
		HistData180 histOne,
		HistData180 histTwo,
		int Dim);
/*--------------------------------*/
static	float Calculate2HistBLikelyByManhattan(
	    ManhattanHistogram histOne,
	    ManhattanHistogram histTwo);
/*--------------------------------*/	
	bool Spectral_Clustering_B(
		double EigenvectorNumPercent=0.1,
		double ClusterPercent=0.1,
		double Threshold=Spectral_Clustering_B_Threshold);
	void Get_L_Eigenvalue(void);
/*--------------------------------*/
	void B_Cluster(
		UINT EigenvectorNum, 
		UINT ClusterNum,
		double Threshold);
/*--------------------------------*/
	void DrawHistogramOnImg(void);
	double Find_MaxSimilar(void);
};
/*----------------------------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------------------------*/

