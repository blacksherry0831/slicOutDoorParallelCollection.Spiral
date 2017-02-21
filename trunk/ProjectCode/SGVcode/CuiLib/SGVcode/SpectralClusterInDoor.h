#pragma once
#include <platform_cui.h>
/*------------------------------------------------------------------------*/
#define  C_Used2FindVP	 22222
#define  InDoorThreshold  (0.9)
#define  Iteration__Threshold__InDoor   (0.9)
/*----------------------------------------------------------------------------------------------------------------*/
/** 
*  class SpectralClusterInDoor
*  ��̬�׾����㷨
*@see  bool SpectralClusterInDoor::Spectral_Clustering_B()
*@note�׾������ ���� ���� ���ܴ�ͨ\n
*  �����ص�����
*  
/*----------------------------------------------------------------------------------------------------------------*/
class SpectralClusterInDoor
{
private:
	ImageMemData* pMD;/**<ͼ�����ݼ�������м����*/
	CvMemStorage* storage;/**<opencv�����õ��м��ڴ�*/ 
	CvSeq* lines;/**<opencv�����У�ռ��CvMemStorage���ڴ�ռ�*/
public:
	SpectralClusterInDoor(ImageMemData* pMD_t);
	~SpectralClusterInDoor(void);
private:
	IplImage* gray_Color_contour_img;/**<�Ҷ�ͼ���Ӧ������ͼ�������ã�*/
	IplImage* gray_img;/**<ԭͼ��Ӧ�ĻҶ�ͼ�񣬵�ͨ��*/
	IplImage* gray_binary_img;/**<�ҶȾ���ֵ�����ͼ��*/
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

