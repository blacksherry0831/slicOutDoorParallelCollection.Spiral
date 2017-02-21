#pragma once
#define   BuildTemplates  (1)
//#include "CuiHough3.h"
/**
*�����ټ��ʱ�õ��ķ�ֵ��ģ��\n
*|--------|	      |-|         |--------|\n
*|	      |	      | |		  |        |\n
*|	      |-------| |---------|	       |\n
* L[0] < L[1]<   M[0]<M[1]       R[0]<R[1]\n
*������ֵ�鹹����������������ֵ��������ģ��\n
*/
typedef struct{
 unsigned int Lpeak[2];/**<���ֵ�������*/
 unsigned int Mpeak[2];/**<�з�ֵ�����ļ��*/
 unsigned int Rpeak[2];/**<�ҷ�ֵ�����ļ��*/
}B3TemplatePeak;
/**
*�������������������������
*/
typedef struct{
	double Lpeak;/**<����ֵ������*/
	double Mpeak;/**<�м��ֵ������*/
	double Rpeak;/**<�Ҳ��ֵ������*/
	double Sumpeak;/**<�ܵķ�ֵ������*/
}Pos3Peak;
/**
*�������ٷ��������������
*/
typedef struct{
	double Lpeak;/**<��಻���������ٷ��������*/
	double Rpeak;/**<�Ҳ಻���������ٷ��������*/
	double Sumpeak;/**<Lpeak��Rpeak��������*/
}Neg3Peak;
/**
*��������ͼƬ�����ľ����ж�\n
*�������������¼���\n
*num BuildingType{BT_bigbuildingVP=0,BT_bigbuindingFront=1,BT_trees=2,BT_unknow=3};
*/
class SceneDetermine
{
public:
	SceneDetermine(ImageMemData* MemData_t);
	SceneDetermine(void);
	~SceneDetermine(void);
private:
	ImageMemData* pMD;/**<�����õĹ����������м��ڴ�*/
private:
	float ManhattanEnergy_ALL;//ok
	int IsMaxValueMiddle;//ok
	BuildingType   bt;//ok
public:
	void SaveBulidingTypeInfo(string path,string filename);
public:
	B3TemplatePeak B3Peaks[BuildTemplates];/**<������������ֵ��λ��*/
static	B3TemplatePeak B3PeaksIndoor[BuildTemplates];
	float B3Templates[BuildTemplates][AngLeDivided];/**<ֱ��ֱ��ͼ*/
static float B3TemplatesInDoor[BuildTemplates][AngLeDivided180];
	Pos3Peak pos[BuildTemplates];/**<��������ģ���ϵ������ֲ�*/
	Neg3Peak neg[BuildTemplates];/**<��������ģ��֮��������ֲ�*/
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

