#pragma once
#define   BuildTemplates  (1)
//#include "CuiHough3.h"
/**
*曼哈顿检测时用到的峰值组模型\n
*|--------|	      |-|         |--------|\n
*|	      |	      | |		  |        |\n
*|	      |-------| |---------|	       |\n
* L[0] < L[1]<   M[0]<M[1]       R[0]<R[1]\n
*三个峰值组构成了曼哈顿三个峰值能量检测的模型\n
*/
typedef struct{
 unsigned int Lpeak[2];/**<左峰值能量检测*/
 unsigned int Mpeak[2];/**<中峰值能量的检测*/
 unsigned int Rpeak[2];/**<右峰值能量的检测*/
}B3TemplatePeak;
/**
*曼哈顿三个方向的能量计算结果
*/
typedef struct{
	double Lpeak;/**<左侧峰值计算结果*/
	double Mpeak;/**<中间峰值计算结果*/
	double Rpeak;/**<右侧峰值计算结果*/
	double Sumpeak;/**<总的峰值计算结果*/
}Pos3Peak;
/**
*除曼哈顿方向线以外的能量
*/
typedef struct{
	double Lpeak;/**<左侧不属于曼哈顿方向的能量*/
	double Rpeak;/**<右侧不属于曼哈顿方向的能量*/
	double Sumpeak;/**<Lpeak和Rpeak的能量和*/
}Neg3Peak;
/**
*本类用于图片场景的决策判断\n
*场景大致有如下几种\n
*num BuildingType{BT_bigbuildingVP=0,BT_bigbuindingFront=1,BT_trees=2,BT_unknow=3};
*/
class SceneDetermine
{
public:
	SceneDetermine(ImageMemData* MemData_t);
	SceneDetermine(void);
	~SceneDetermine(void);
private:
	ImageMemData* pMD;/**<计算用的公共变量和中间内存*/
private:
	float ManhattanEnergy_ALL;//ok
	int IsMaxValueMiddle;//ok
	BuildingType   bt;//ok
public:
	void SaveBulidingTypeInfo(string path,string filename);
public:
	B3TemplatePeak B3Peaks[BuildTemplates];/**<曼哈顿三个峰值的位置*/
static	B3TemplatePeak B3PeaksIndoor[BuildTemplates];
	float B3Templates[BuildTemplates][AngLeDivided];/**<直线直方图*/
static float B3TemplatesInDoor[BuildTemplates][AngLeDivided180];
	Pos3Peak pos[BuildTemplates];/**<在曼哈顿模型上的能量分布*/
	Neg3Peak neg[BuildTemplates];/**<在曼哈顿模型之外的能量分布*/
public:
		BuildingType DetermineScene(HistData hist_data);
		void Init3PeakTemplate(void);
		double CalculatePos(float *hist);
		double CalculateNeg(float *hist);
		bool IsPeakValueInMiddle(float *hist);
		bool IsPeakValueInHorizontal(float *hist);
		bool IsManhattanPeak(double pos_weight,HistData hist_data);
		bool IsGndUp2HorizontalLine(INT32* cui_ImgLables_SVG);
	
		void SetSVGLabel(INT32* cui_ImgLables_SVG);
		bool IsBigBuildingVP(HistData hist_data);
		bool IsBigBuildingFront(HistData hist_data);
		bool IsPeakValueInMiddleArea(HistData histdata);
public:
static	bool IsHistRegularShapes(HistData180 data);

static  double  CalculateNeg(
	float *B3Templates,
	Neg3Peak* neg,
	float *hist,
	int Dim);

static  double  CalculatePos(
	const B3TemplatePeak* B3Peaks,
	Pos3Peak* pos,
	float *hist,
	int Dim);

static  void Init3PeakTemplateInDoor(
	const B3TemplatePeak* B3Peaks,
	float*B3Templates,
	int Dim);

static bool IsBlockTree(HistData180 histdata,int Dim);
static bool IsBlockBuilding(HistData180 histdata,int Dim);
static bool IsVerticalEnergyFocus(HistData180 hist,int Dim);
static bool IsSpRectangle(HistData180 hist,int Dim);
static bool IsManhattan(ManhattanHistogram hist);
};

