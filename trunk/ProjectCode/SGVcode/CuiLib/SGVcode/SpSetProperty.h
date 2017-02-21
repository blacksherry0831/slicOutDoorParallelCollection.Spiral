#pragma once
#include <platform_cui.h>
//#include <string>
//#include <iostream>
//using namespace std;
//typedef  unsigned int UINT32;
//#include "module_all_cui.h"
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/
#define spBorderCategoryTop        0x00001
#define spBorderCategoryBottom     0x00010
#define spBorderCategoryRight      0x00100
#define spBorderCategoryLeft       0x01000
#define spBorderCategoryCrossHorz  0x10000
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/
struct SpPoint{
	int x;
	int y;
#if  DEBUG_CUI
	int   Blue,G,R,Alpha;
	float	L,A,B;
	double  Light，M，Theta;//角度，亮度，模长
#endif
};
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/
class SpProperty
{
public:
	SpProperty()
	{
		this->SpIndex=-1;
		this->pointSetIdx.clear();
		this->BorderCategory=0;
		harr_energy.Energy_HH=0;
		harr_energy.Size_HH=0;
		harr_energy.Energy_HL=0;
		harr_energy.Size_HL=0;
		harr_energy.Energy_LH=0;
		harr_energy.Size_LH=0;
		harr_energy.Energy_LL=0;
		harr_energy.Size_LL=0;
		harr_energy.EnergyHighsigma=0;
		harr_energy.EnergyL_HH=0;
		harr_energy.EnergyL_HL=0;
		harr_energy.EnergyL_LH=0;
		harr_energy.EnergyL_LL=0;
		harr_energy.EnergyGrade=0;
		harr_energy.harrDepth=0;
		ComputeCategory=0;
		Sky_Candidate=false;
		Gnd_Candidate=false;
	}
	static inline SpPoint spPoint(int x,int y)
	{
		SpPoint point;
		point.x=x;
		point.y=y;
		return point;
	}
public:
	int SpIndex;
	int SpSize;
	vector<int> pointSetIdx;
	int BorderCategory;
	int fuzzyCategory;
	int ComputeCategory;
	bool Sky_Candidate;
	bool Gnd_Candidate;
	/*********/
	int min_x;/**<包围超像素的矩阵在X轴上的最小值*/
	int min_y;/**<包围超像素的矩阵在Y轴上的最小值*/
	int max_x;/**<包围超像素的矩阵在X轴上的最大值*/
	int max_y;/**<包围超像素的矩阵在Y轴上的最大值*/
	int width;
	int height;
	/*********/	
	Energy_Harr  harr_energy;/**<超像素小波能量*/
}; 
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/
class SpPointSet
{
public:
	SpPointSet()
	{
		this->SpIndex=-1;
		this->pointSet.clear();
		this->pointSet.reserve(8000);
	}
	static inline SpPoint spPoint(int x,int y)
	{
		SpPoint point;
		point.x=x;
		point.y=y;
		return point;
	}
public:
	int SpIndex;
	vector<SpPoint> pointSet;
}; 
/*----------------------------------------------------------------*/
/**
*本类包含了图像处理算法中一些通用的数据结构\n
*和运算用到的中间变量
*
*/
/*----------------------------------------------------------------*/
class SpSetProperty
{
public:
	SpSetProperty(void);
	~SpSetProperty(void);
public:
	vector<SpPointSet> PointSet;
	vector<SpProperty> SpPropertySet;
	int ImgWidth;
	int ImgHeight;
	int HorizontalLinePos;
public:
	void Parse(int*ImgLabels,int slicNum,int width,int height,int HorizontalLinePos);
inline	void ParsespBorderCategory(int spi,int width_x,int height_y);
	void Combine2in1(int spi,int spj,bool CombineHarr);
	void combinePointSet(int DesSp,int SrcSp);
	void CombineBorderProperty(int DesSp,int SrcSp);
	void CombineHarrProperty(int DesSp,int SrcSp,bool CombineHarr);
	/**/
	void RemoveEmptySet(void);
	void AdjustSpSet(void);
public:
	void cleardata();
};

