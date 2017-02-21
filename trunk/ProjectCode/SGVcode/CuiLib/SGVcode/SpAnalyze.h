#pragma once
#define  TEST_Feature_M FALSE
#include <SVMcode/TrainImgUseSvm.h>
/*------------------------------------------------*/
/**
*������ͼ��������ݽṹ
*-��������
*-#�ܳ�
*-#���
*-#�ȵ�
*/
/*------------------------------------------------*/
class SpSpAnalyzeData{
public:
	SpSpAnalyzeData();
	~SpSpAnalyzeData();
public:
	string File;/**<����*/
	string FileWritePath;/**<�������·��*/
	void classify(float maxValue,float minValue,int CateNum);
public:
	double P_cs;/**<==�ܳ�/���*/
	double P_sRects;/**<==ͼ�����/��Ӿ������/*/
	float Sp_C;/**<������ͼ����ܳ�*/
	float Sp_S;/**<������ͼ������*/
	float Rect_S;/**<ͼ����Ӿ��ε����*/
	string FileName;/**<������ͼ��������ͼ���ļ���*/
	int FileCategory;/**<*/

};
/*------------------------------------------------*/
/**
*
*/
/*------------------------------------------------*/
#define  HarrDepth   (1)/**<������deprecated*/
enum SpSizeAttr {
SA_UnKnow,/*ͼ������δ֪*<*/
FAT,/**<��ͼ��*/
THIN/**<��ͼ��*/
};
/*------------------------------------------------*/
/**
*����������ͼ�������жϳ�����ͼ������building����tree
*/
/*------------------------------------------------*/
class SpAnalyze
{	
private:
	vector<double> AnalyzeData;/**<�ܳ������*/
	vector<SpSpAnalyzeData>  SpAnalyzeData;/**<��ʷ���������*/
	SpSpAnalyzeData        SpADataNow;/**<��ǰ����������*/
	ImageMemData* pMD;/**<�����õĹ����������м��ڴ�*/
	UINT32 *ImgData;/**<ͼ������*/
	int *ImgLabels;/**<�����ر궨labels*/
	IplImage *ImgMaskControus;/**<���α߽�����أ������к��*/
	float fatorthin;/**<�ж�ͼ�����ݵ�*/
public:
	float EffectivePixelNum;/**<��Ӿ�������Ч�����ظ���*/
	unsigned long GetEffectiveColorNumber();
public:
	SpSizeAttr SpSA;/**<ͼ�����������*/
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