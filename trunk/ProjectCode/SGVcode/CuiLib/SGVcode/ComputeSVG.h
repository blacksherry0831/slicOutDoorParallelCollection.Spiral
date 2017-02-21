#pragma once
#include "SGVcode/ImageMemData.h"
#include "modules.h"
/*-----------------------------------------------------------------------------*/
#define  Num_Hist 2048  /**�ܹ����г����طָ�����ֵ*/
#define  SKY_Force  0x04  /**���ǿ�ƾ�������*/
#define  GROUND_Force 0x02 /**����ǿ�ƾ�������*/
#define  VERTICAL_Force 0x01 /**����ǿ�ƾ�������*/
/**********************/
#ifndef Ground
#define  Ground 1
#define  Vertical 2 
#define  Sky  3
#endif // Ground
/*-----------------------------------------------------------------------------*/
#define G_V_S_Num_Category  10 /**���������������鳤��*/
#define SquareMeter	  (3)	/**�������*/
#define A_COLOR	  (6)	/**LAB A_Color����*/
#define B_COLOR	  (7)	/**LAB B_Color����*/
#define Lab_2_0_Dst (8)		/**����ԭ������*/
#define Lab_2_P_Dst (9)		/**��ԭ��ľ���*/
 //  X = 0.412453*R + 0.357580*G + 0.180423*B;
//Gray = R*0.299 + G*0.587 + B*0.114
/*-----------------------------------------------------------------------------*/
/**
*�������ڶ���ա����桢����ķ���
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
	ImageMemData* pMD;/**<��Ҫ��ͼ�����ݺ�ͼ������õ��м����*/
	CvMemStorage* storage;/**<opencv�����õ��м��ڴ�*/ 
	CvSeq* lines;/**<opencv�����У�ռ��CvMemStorage���ڴ�ռ�*/
private:
	IplImage* gray_Color_contour_img;/**<�Ҷ�ͼ���Ӧ������ͼ�������ã�*/
	IplImage* gray_img;/**<ԭͼ��Ӧ�ĻҶ�ͼ�񣬵�ͨ��*/
	IplImage* gray_binary_img;/**<�ҶȾ���ֵ�����ͼ��*/
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
	double Confusion_M[3][3];/**<���ڵ�ģ���������ڲ�ʹ��*/
	Energy_Harr  harr_sp_energy[Num_Hist];/**<���������ؿ��С������*/

static	double *cui_pYweight_GVS;/**<���棬���棬��գ���cui_Height�ϵ�Ȩ������*/

		double *cui_pYweight_G;/**<����,��cui_Height�ϵ�Ȩ������*/
		double *cui_pYweight_V;/**<���棬��cui_Height�ϵ�Ȩ������*/
		double *cui_pYweight_S;/**<��գ���cui_Height�ϵ�Ȩ������*/


			IplImage * Cui_lab_hsv_Image;/**<Lab��ʽ��ͼ���ļ�*/
			IplImage *h_l_plane;/**<Lab��ʽ�У�L���������ͼ���ļ�*/
			IplImage *s_a_plane;/**<Lab��ʽ�У�A���������ͼ���ļ�*/
			IplImage *v_b_plane;/**<Lab��ʽ�У�B���������ͼ���ļ�*/
public:

	/*--------------------------------------------------------------------*/
	UINT32* cui_ImgData; /**<ͼ������*/
	int cui_Width;		 /**<ͼ����*/
	int cui_Height;		 /**<ͼ��߶�*/
	int*  cui_ImgLables;	 /**<ͼ���Lables����*/
	int*  cui_ImgLables_SVG;	 /**<ͼ�����գ����棬�����������*/
	int cui_NumLabels;	 /**<��ǰcui_ImgLables��ͼ�����Ŀ*/
	double* cui_G_V_S_Num_Category;	 /**<ͼ������ա����桢����ķ�������*/
	LabColorCategory* Lab_Color_Category;	 /**<ÿ�������ض�Ӧ����շ�������*/
	double cui_HorizontalLine;		 /**<ˮƽ�ߵ�λ��*/
	INT32*  Category_Force;		 /**<�����ؾ��е�ǿ����*/
	INT32 * cui_Matrix_Category_Lable;		 /**<�����ؾ��е���ͨ����*/
	UINT32* E_matrix;		 /**<���ھ���*/
	SP_PROPERTY *p_SpProperty;		 /**<��������ͼ���ϵķ�Χ������������*/
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
	static double Pow_Odd(double b,double w);//�溯��
	static double Pow_Even(double b,double w);//ż����
public:
	void InitSPForceCategory(void);
	void FuzzyClassification(bool SaveNumInfo=false);
	void GetPsPgIteration(void);
	void GetPsPgIteration_NearBigFarSmall_5m4d(void);
	void GetPsPg_DFS(void);
	bool IsLabelsHaveSky(void);
public:
	//��������
	void CrossEyeLineIsVertical(void);
	UINT32 StripIsVertical(void);
	double CalculateVGDifference(int spi);
public:	
};

