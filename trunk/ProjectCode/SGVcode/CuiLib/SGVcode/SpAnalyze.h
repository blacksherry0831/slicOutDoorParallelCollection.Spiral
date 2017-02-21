#pragma once
#define  TEST_Feature_M FALSE
#include <SVMcode/TrainImgUseSvm.h>
/*------------------------------------------------*/
/**
*超像素图块分析数据结构
*-分析内容
*-#周长
*-#面积
*-#等等
*/
/*------------------------------------------------*/
class SpSpAnalyzeData{
public:
	SpSpAnalyzeData();
	~SpSpAnalyzeData();
public:
	string File;/**<？？*/
	string FileWritePath;/**<数据输出路径*/
	void classify(float maxValue,float minValue,int CateNum);
public:
	double P_cs;/**<==周长/面积*/
	double P_sRects;/**<==图块面积/外接矩形面积/*/
	float Sp_C;/**<不规则图块的周长*/
	float Sp_S;/**<不规则图块的面积*/
	float Rect_S;/**<图块外接矩形的面积*/
	string FileName;/**<超像素图块所属的图像文件名*/
	int FileCategory;/**<*/

};
/*------------------------------------------------*/
/**
*
*/
/*------------------------------------------------*/
#define  HarrDepth   (1)/**<无意义deprecated*/
enum SpSizeAttr {
SA_UnKnow,/*图块属性未知*<*/
FAT,/**<胖图块*/
THIN/**<瘦图块*/
};
/*------------------------------------------------*/
/**
*分析超像素图块用于判断超像素图块属于building还是tree
*/
/*------------------------------------------------*/
class SpAnalyze
{	
private:
	vector<double> AnalyzeData;/**<周长面积比*/
	vector<SpSpAnalyzeData>  SpAnalyzeData;/**<历史计算结果输出*/
	SpSpAnalyzeData        SpADataNow;/**<当前计数结果输出*/
	ImageMemData* pMD;/**<计算用的公共变量和中间内存*/
	UINT32 *ImgData;/**<图像数据*/
	int *ImgLabels;/**<超像素标定labels*/
	IplImage *ImgMaskControus;/**<屏蔽边界的像素，轮廓有厚度*/
	float fatorthin;/**<判断图块胖瘦的*/
public:
	float EffectivePixelNum;/**<外接矩形中有效的像素个数*/
	unsigned long GetEffectiveColorNumber();
public:
	SpSizeAttr SpSA;/**<图块的胖瘦属性*/
public:
	SpAnalyze(void);
	~SpAnalyze(void);
private:
	void Init(ImageMemData* MemData_t);
	void DeInit(void);
	float Calculate_C(void);
	float Calculate_S(void);
	void ChangeColor2Labels(void);
	double CalculateP_cs(void);
public:	
	void Calcuate(ImageMemData* MemData_t,bool IsSave=true);
	SpSizeAttr IsSpFatorThin(ImageMemData* MemData_t);
	void PickUp(void);
	void SaveSpAnalyze(void);
	void ExtractImageFeatureWithHarr(vector<float>& feature);
	HistData180 ExtractImageFeatureWithSrcCany(vector<float>& feature);
	HistData180 ExtractImageFeatureWithSrcCany(Feature_VegaBuilding& fvb);
	static	int GetFeatureSize(void);
	void PushBackFeature(vector<float> &feature,HistData180& histdata,int Dim);
	Feature_VegaBuilding ExtractImage_Harr_Manhattan_Feature();
	Feature_VegaBuilding ExtractImage_Sum_Count_Feature();
	void ExtractImage_LabColorFeature(Feature_VegaBuilding& f_vb);
};