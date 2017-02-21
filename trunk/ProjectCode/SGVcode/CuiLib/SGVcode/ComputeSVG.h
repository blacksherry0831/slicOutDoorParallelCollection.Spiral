#pragma once
#include "SGVcode/ImageMemData.h"
#include "modules.h"
/*-----------------------------------------------------------------------------*/
#define  Num_Hist 2048  /**能够进行超像素分割的最大值*/
#define  SKY_Force  0x04  /**天空强制聚类属性*/
#define  GROUND_Force 0x02 /**地面强制聚类属性*/
#define  VERTICAL_Force 0x01 /**立面强制聚类属性*/
/**********************/
#ifndef Ground
#define  Ground 1
#define  Vertical 2 
#define  Sky  3
#endif // Ground
/*-----------------------------------------------------------------------------*/
#define G_V_S_Num_Category  10 /**地面立面属性数组长度*/
#define SquareMeter	  (3)	/**面积索引*/
#define A_COLOR	  (6)	/**LAB A_Color索引*/
#define B_COLOR	  (7)	/**LAB B_Color索引*/
#define Lab_2_0_Dst (8)		/**距离原点属性*/
#define Lab_2_P_Dst (9)		/**与原点的距离*/
 //  X = 0.412453*R + 0.357580*G + 0.180423*B;
//Gray = R*0.299 + G*0.587 + B*0.114
/*-----------------------------------------------------------------------------*/
/**
*本类用于对天空、立面、地面的分类
*/
/*-----------------------------------------------------------------------------*/
#define USESTATIC TRUE
#define  UES_CUDA TRUE
/*-----------------------------------------------------------------------------*/
/**
*                                                                     
*
**/
/*-----------------------------------------------------------------------------*/
class ComputeSVG
{
public:
	ComputeSVG(void); 
	ComputeSVG(ImageMemData* MemData_t);
	~ComputeSVG(void);
private:
	ImageMemData* pMD;/**<需要的图像数据和图像计算用的中间变量*/
	CvMemStorage* storage;/**<opencv计算用的中间内存*/ 
	CvSeq* lines;/**<opencv的序列，占用CvMemStorage，内存空间*/
private:
	IplImage* gray_Color_contour_img;/**<灰度图像对应的轮廓图（调试用）*/
	IplImage* gray_img;/**<原图对应的灰度图像，单通道*/
	IplImage* gray_binary_img;/**<灰度经二值化后的图像*/
public:
void GetImgData(UINT32* ImgData,int Width,int Height,int*  Lables,int NumLabels);
void ClearMemory(void);
void separateSVG(void);	
double classifyOneSuperpixel(int sp);
/**********************/
void FillWeightArrayFM(void);
void FillWeightArrayZlm(double horizontal_line_scale);
void FillWeightArrayZlmParallel(double horizontal_line_scale);
void FillWeightArrayV(double horizontal_line, double n);

void FillWeightArrayG(double horizontal_line, double n);
void FillWeightArrayS(double horizontal_line, double n);

void FillWeightArrayG_InDoor(double horizontal_line, double n);
void FillWeightArrayS_InDoor(double horizontal_line, double n);

void separateSVG_Zlm(void);	
void zlm_ForceSkyGroundVertical(void);
void zlm_ForceSky(void);
void zlm_ForceGround(void);
void zlm_ForceVertical(void);
void zlm_ForceVertical_SG(void);
void CalcuateSpWeightByY();
float GetHs_InDoor(float DeflectionAngle);
float GetHg_InDoor(float DeflectionAngle);
private:
	double Confusion_M[3][3];/**<早期的模糊矩阵，现在不使用*/
	Energy_Harr  harr_sp_energy[Num_Hist];/**<各个超像素块的小波能量*/

static	double *cui_pYweight_GVS;/**<地面，立面，天空，在cui_Height上的权重数组*/

		double *cui_pYweight_G;/**<地面,在cui_Height上的权重数组*/
		double *cui_pYweight_V;/**<立面，在cui_Height上的权重数组*/
		double *cui_pYweight_S;/**<天空，在cui_Height上的权重数组*/


			IplImage * Cui_lab_hsv_Image;/**<Lab格式的图像文件*/
			IplImage *h_l_plane;/**<Lab格式中，L代表分量的图像文件*/
			IplImage *s_a_plane;/**<Lab格式中，A代表分量的图像文件*/
			IplImage *v_b_plane;/**<Lab格式中，B代表分量的图像文件*/
public:

	/*--------------------------------------------------------------------*/
	UINT32* cui_ImgData; /**<图像数据*/
	int cui_Width;		 /**<图像宽度*/
	int cui_Height;		 /**<图像高度*/
	int*  cui_ImgLables;	 /**<图像的Lables数据*/
	int*  cui_ImgLables_SVG;	 /**<图像的天空，立面，地面分类数据*/
	int cui_NumLabels;	 /**<当前cui_ImgLables中图块的数目*/
	double* cui_G_V_S_Num_Category;	 /**<图像在天空、立面、地面的分类数据*/
	LabColorCategory* Lab_Color_Category;	 /**<每个超像素对应的天空分类属性*/
	double cui_HorizontalLine;		 /**<水平线的位置*/
	INT32*  Category_Force;		 /**<超像素具有的强类型*/
	INT32 * cui_Matrix_Category_Lable;		 /**<超像素具有的普通分类*/
	UINT32* E_matrix;		 /**<相邻矩阵*/
	SP_PROPERTY *p_SpProperty;		 /**<超像素在图像上的范围及其所属分类*/
	/*--------------------------------------------------------------------*/
public:
	///////////////////////////////////////
	void CalculateBlockEnergy(int sp);
	void CalculateBlockEnergy2(int sp);
static	void CalculateBlockEnergy2_S(
	int sp,
	Energy_Harr* harr_sp_energy,
	SP_PROPERTY *p_SpProperty,
	int*  cui_ImgLables,
	UINT32* cui_ImgData,
	int cui_Width,
	int cui_Height);
	///////////////////////////////////////
	void RemoveSkyBySquareMeter(void);
	void RemoveGroundByPosition(void);
	void RemoveGroundByPosition2(void);
	void RemoveSomeUnreasonableClassification(void);
	//////////////////////////////////////////////

static	void FindandSetNeighbour(INT32 *Category,INT32 spnum,int sp_start,double* GVS_Category,UINT32* E_matrix);
void CalculateSkyVHarrEnergy(void);
static void CalculateSkyVHarrEnergy_S(
	int cui_NumLabels,
	Energy_Harr* harr_sp_energy,
	SP_PROPERTY *p_SpProperty,
	int*  cui_ImgLables,
	UINT32* cui_ImgData,
	int cui_Width,
	int cui_Height,
	double* cui_G_V_S_Num_Category,
	INT32*  Category_Force);

int  CalculateMostEffectiveSky(double sky_Light_Threshold,double sky_energy);

static int  CalculateMostEffectiveSky_S(
	double sky_Light_Threshold,
	double sky_energy,
	int cui_NumLabels,
	double* cui_G_V_S_Num_Category,
	Energy_Harr* harr_sp_energy,
	INT32 * cui_Matrix_Category_Lable,
	INT32*  Category_Force);

void DivideSkyV(double sky_energy, double sky_Light_Threshold, double L_distance, double AB_distance);
void DivideSkyV_S(
	double sky_energy,
	double sky_Light_Threshold,
	double L_distance, 
	double AB_distance,
	int* cui_ImgLables,
	int cui_NumLabels,
	ImageMemData* pMD,
	Energy_Harr* harr_sp_energy,
	UINT32* E_matrix,
	double* cui_G_V_S_Num_Category);
void RemoveUnEffectiveSkySP(ULONGLONG sky_Effect_Index,float L_distance);
static void RemoveUnEffectiveSkySP_S(
	ULONGLONG sky_Effect_Index,
	float L_distance,
	int cui_NumLabels,
	double* cui_G_V_S_Num_Category,
	Energy_Harr* harr_sp_energy,
	LabColorCategory* Lab_Color_Category,
	INT32 * cui_Matrix_Category_Lable,
	INT32*  Category_Force);
static int ClassifyOneSPlCategory(int *lables,int width,int height,int sp);
void RemoveVerticalByNeighbor(void);
/*-------------------------------*/
bool Is_Sky_Block(
	int spj,
	float sky_Light_Threshold,
	float sky_energy_Threshold,
	float AB_distance,
	float L_distance);
/*-------------------------------*/
static bool Is_Sky_Block_S(
	int spj,
	float sky_Light_Threshold,
	float sky_energy_Threshold,
	float AB_distance,
	float L_distance,
	Energy_Harr* harr_sp_energy,
	double* cui_G_V_S_Num_Category,
	LabColorCategory* Lab_Color_Category);
/*-------------------------------*/
void InitParam(void);
void CalculateUpandDownSquarebyHor(float* UpSquare, float* DownSquare, int  sp);
#if TRUE

void ForceSky_InDoor(void);
void ForceGround_InDoor(void);

void separateSVG_InDoor(void);
void zlm_ForceSkyGroundVertical_InDoor(void);
void zlm_ForceSkyGroundVertical_InDoor3_31(void);
void zlm_ForceSkyGroundVertical_InDoor4_16(void);
void zlm_ForceSkyGroundVertical_InDoor4_26(void);

static void InitDFSLinkConnection_Ground(ImageMemData* pMD);
void InitDFSLinkConnection_Sky(void);


static void ForceGround_InDoor_DFS(ImageMemData* pMD);
void ForceSky_InDoor_DFS(void);
/*-----------------------------------------*/
void InitDFSLinkConnection_Vertical(void);
void ForceVertical_InDoor_DFS(void);
void DFS_4Vertical2UporDown(int spi);

void DFS_4Vertical2UporDown_ByColor(int spi);
/*-----------------------------------------*/
static void DFS_4Ground2Up(int spi,ImageMemData* pMD);
void DFS_4Sky2Down(int spi);

void InitSPLabProperty(void); 
void DFS_RemoveVonG(int spi);
void RemoveSunpendVonG_MustHaveS(void);
void VerticalMustHaveSupport_Pixel(void);
void VerticalMustHaveSupport_SpPixel(void);
void OverHorLineMustVertical(void);
void DecideFloatingSkySP2V_Init(void);
void DecideFloatingSky_InDoor_DFS(void);
void DFS_4FloatingSky2Down(int spi);
#endif
public:	
	double Gx_InDoor(int x,int n,float posHor,float Pg,int Height);
	double Sx_InDoor(int x,int n,float posHor,float Ps,int Height);
	static double Pow_Odd(double b,double w);//奇函数
	static double Pow_Even(double b,double w);//偶函数
public:
	void InitSPForceCategory(void);
	void FuzzyClassification(bool SaveNumInfo=false);
	void GetPsPgIteration(void);
	void GetPsPgIteration_NearBigFarSmall_5m4d(void);
	void GetPsPg_DFS(void);
	bool IsLabelsHaveSky(void);
public:
	//定义立面
	void CrossEyeLineIsVertical(void);
	UINT32 StripIsVertical(void);
	double CalculateVGDifference(int spi);
public:	
};

