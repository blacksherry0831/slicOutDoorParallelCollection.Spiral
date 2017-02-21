#pragma once
#include "SGVcode/ImageData.h"
/*-----------------------------------------------------------------------------*/
/**
*                                                                     
*
**/
/*-----------------------------------------------------------------------------*/
#define  TestOutCompute TRUE
/*-----------------------------------------------------------------------------*/
/**
*                                                                     
*
**/
/*-----------------------------------------------------------------------------*/
class ComputeSVG2
{
public:
	ComputeSVG2(void); 
	ComputeSVG2(ImageData* MemData_t);
	~ComputeSVG2(void); 
public:
	ImageData* pIMD;
    vector<double>	Lab2Point_AB_Dst;
	vector<double>  Lab2Zero_L_Dst;
	vector<double>  Lab_L_Dst;
public:
void InitParam(void);
void ClearMemory(void);
void FillWeightArrayZlmParallel(void);
void FillWeightArrayZlm(void);
void separateSVG_Zlm(void);
void ForceSkyGroundVertical(void);
//////////////////////////////////
void zlm_ForceSky(void);
void RemoveSomeUnreasonableClassification(void);
void RemoveSkyBySquareMeter(void);
void zlm_ForceVertical(void);
void zlm_ForceGround(void);
void zlm_ForceVertical_SG(void);
void zlm_ForceVertical_De(void);
void RemoveGroundByPosition2(void);
///////////////////////////////////
double GetSkyHarrLightThreshold(void);
double GetSkyHarrEnergyThreshold(void);
void RemoveUnEffectiveSkySP(
	ULONGLONG sky_Effect_Index,
	float L_distance);
void DivideSkyV(
	double sky_energy,
	double sky_Light_Threshold,
	double L_distance, 
	double AB_distance);
void Init2SkyEffective(int sky_Effect_Index);
int  CalculateMostEffectiveSky(
	double sky_Light_Threshold,
	double sky_energy_threshold);
///////////////////////////////
bool Is_Sky_Block(
	int spj,
	float sky_Light_Threshold,
	float sky_energy_Threshold,
	float AB_distance,
	float L_distance);
///////////////////////////////
double CalculateSpSimilar(int spi,int spj);
double CalculateVGDifference(int spi);
void zlm_ForceVertical_WidthHeight(void); 
/////////////////////////////////
void CalculateSpClassification(int* Matrix_Category_Lable_host);
///////////////////////////////
void  FillWeightArrayS_InDoor20150603(double horizontal_line, double n);
void  FillWeightArrayG_InDoor20150603(double horizontal_line, double n);
double Gx_InDoor20150603(int y,int n,float H0,float Hg,int H);
double Sx_InDoor20150603(int y,int n,float H0,float Hg,int H);
/*---------------------------------------------------------------------------------------------------*/
void FillWeightArrayV_New(double horizontal_line, double n);
void FillWeightArrayV_Gaussian(double horizontal_line, double n,double SigmaScale,double WeightScale);
};

