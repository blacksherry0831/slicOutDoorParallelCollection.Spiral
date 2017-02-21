#pragma once
using namespace std;
/*-------------------------------------------------------------------------------------*/
#include <platform_cui.h>


#include "SGVcode/CuiHough/HoughAnalyze.h"
#include "tinyxml2/tinyxml2.h"
using namespace tinyxml2;
/*-------------------------------------------------------------------------------------*/
#define ManhattanFeature
#define SvmCategoryPos (1)
#define SvmCategoryNeg (-1)
#define SvmCategoryBuilding    SvmCategoryPos
#define SvmCategoryTree        SvmCategoryNeg
#define  ProgressBar (TRUE&&_MSC_VER)
/////////////////////////////////////////////
#define Normal  0
#define Sum_Count_Density		1
#define Sum_Density_Color		2
#define Sum_Count_Color			3
#define Count_Density_Color		4
#define Sum_Count		5
#define Sum_Density		6
#define Sum_Color		7
#define Count_Density	8
#define Count_Color		9
#define Density_Color	10
#define Sum_Count_Density_Color		 11
/////////////////////////////////////////////
#define Dimensions_USE       Count_Density_Color	
/////////////////////////////////////////////
#define  SVM_AllocMemCV  FALSE
#define  SVM_AllocMemVector TRUE
#define  USE_SVM_2_4_X FALSE
/*-------------------------------------------------------------------------------------*/
/*
*
*/
/*-------------------------------------------------------------------------------------*/
#ifndef  MFC_ThreadInfo
#define  MFC_ThreadInfo TRUE
#define  USE_MFC  (FALSE&&_MSC_VER)
#ifdef ProgressBar
#undef ProgressBar
#define ProgressBar  USE_MFC
#endif

struct ThreadInfo
{
#if FALSE&&_MSC_VER
	UINT nMilliSecond;	
	CEdit*           pCtrlEdit;
#endif

#if USE_MFC
    CProgressCtrl* pctrlProgress;//进程指针
	CStatic*       pctrlStatic;	
	CButton*         pCtrlButton;
#endif
	
	UINT        ThreadinRun;
};//结构体
#else
#endif
/*-------------------------------------------------------------------------------------*/
/*
*特征 \n
*包含了植被、建筑的特征。
*/
/*-------------------------------------------------------------------------------------*/
class  Feature_VegaBuilding{
public:
	Feature_VegaBuilding(void);
	~Feature_VegaBuilding(void);
public:
	int Width;/**<图像宽度*/
	int Height;/**<图像高度*/
	string SrcFileName;/**<文件名*/
public:
	LabColor color;/**<颜色的LAB分量*/
	float ABangle;/**<颜色AB（与0°）夹角*/
public:
	vector<float> FeatureManhattanLength;/**<Manhattan长度直方图的宽度*/
	float Sum_FeatureManhattanLength;/**<Manhattan直线总长*/
	float ZeroCount_FeatureManhattanLength;/**<*对0Manhattan线计数*/
public:
	vector<float> FeatureLineNum;/**<*/
	float LineNumDensity;/**<直线个数的密度*/
public:
	vector<float> FeatureHarr;/**<小波特征（数组）*/
	Energy_Harr HarrImg;/**<小波特性*/
	void SetHarrEnergy(Energy_Harr HarrImg);
	void SetHist180(HistData180 histdata);
	void SetLabColor(LabColor lc);
public:
	vector<float> feature;/**<特征*/
};
/*-------------------------------------------------------------------------------------*/
/**
*机器学习数据
*    -SCDC代表求和、计数、密度、颜色
*       -#S-对曼哈顿线柱状图的求和Sun
*       -#C-对曼哈顿线直方图值为0的计Count
*       -#D-密度==线个数/图块面积Desity
*       -#C-颜色角度Color [0,2π)
*/
/*-------------------------------------------------------------------------------------*/
class Svm_SCDC{
public:
	Svm_SCDC (float cate,float sum,float zcount, float desity,float color)
	{
		this->Category=cate;
		this->SumManhattan=sum;
		this->ZeroCount=zcount;
		this->LineNumDensity=desity;
		this->Color=color;
	}
public:
	float Category;/**<针对四维特征的分类--建筑or树木*/
	float SumManhattan;/**<对曼哈顿线直方图的求和*/
	float ZeroCount;/**<对曼哈顿线直方图值为0的计数*/
	float LineNumDensity;/**<线密度*/
	float Color;/**<颜色角度Color [0,2π)*/
};
/*-------------------------------------------------------------------------------------*/
/*
*
*/
/*-------------------------------------------------------------------------------------*/
class TrainImgUseSvm
{
public:
	TrainImgUseSvm(void);
	~TrainImgUseSvm(void);
public:
	static string posSamplesDir;// = "pos/";
	// Directory containing negative sample images
	static string negSamplesDir;// = "neg/";
	// Set the file to write the features to
	static string featuresFile;// = "genfiles/features.dat";
	// Set the file to write the SVM model to
	static string svmModelFile;// = "genfiles/svmlightmodel.dat";
	// Set the file to write the resulting detecting descriptor vector to
	static string descriptorVectorFile;// = "genfiles/descriptorvector.dat";
	/**********************************/
	// HOG parameters for training that for some reason are not included in the HOG class
	static  Size trainingPadding;// = Size(0, 0);
	static  Size winStride;// = Size(8, 8); 
	static string SVMDetectorxml;
	static double winwidth;
	static double winheight;
	static int CategoryFeature;
public:
	static CvMat* data_mat;
	static CvMat* res_mat;
	static vector<string> positiveTrainingImages;
	static vector<string> negativeTrainingImages;
public:		 
	static string toLowerCase(const string& in);
	static   ThreadInfo ThreadData;
	static void storeCursor(void);
	static void resetCursor(void);
	static void saveDescriptorVectorToFile(vector<float>& descriptorVector, vector<unsigned int>& _vectorIndices, string fileName);
	static void getFilesInDirectory(const string& dirName, vector<string>& fileNames, const vector<string>& validExtensions);
	static void calculateFeaturesFromInput(const string& imageFilename, vector<float>& featureVector, HOGDescriptor& hog);
	static	void showDetections(const vector<Rect>& found, Mat& imageData);
	static	void detectTest(const HOGDescriptor& hog, Mat& imageData);
	
public:
	static int StartProcess(void);
	static int StartProcessPickUpFeature(void);
	static int StartProcessFromXmlFile(void);
	static int EndProcess(void);
#if USE_MFC
  static CWinThread* pThread;
#endif
	
public:
		static	UINT TRainTrainHog(LPVOID lpParam);
		static UINT Train(LPVOID lpParam);
		static UINT TrainWithXMLdata(LPVOID lpParam);
		static UINT ThreadPickUpFeature(LPVOID lpParam);	
		static void Read2MemFromXML(vector<float>& data,vector<INT32>& res,int& Dim,unsigned long& Num);
public:
#if USE_CSTRING
	static string ConvertCS2string(CString cstring);
#endif
	static UINT GetFileNameFromPath(void);
	static void PickUpFeature(const string& imageFilename,HistData180 *histdata,int *Dim);
	static void FillFeatureInMat(int Dim);
	static void FillHarrFeatureInMat(int Dim);
	/*static void (const string& imageFilename,int category);*/
	static void SaveFeature2XML(string filename,vector<float> feature,int Category,float FileIndex,string SrcFileName);
	static void SaveFeature2XML(string filename,Feature_VegaBuilding f_vb,int Category,float FileIndex);
	static void ReadFeatrueFromXML(string filename,vector<float>& features,vector<float>& category);
	static void FillFeatureInMatFromXML(int Dim);
	static void splitstring(const char * str,const char * deli, vector<string>& list);
	static void strRemoveSpace(char *str);
public:
	
public:
	
	static void ReadFileName2PathList(vector<string>& pathList);
	static void ReadTrainDataFromXML(vector<float>& dataMatMem,vector<INT32>& resMatMem,int& FeatureDim,unsigned long& samplenumvector,vector<string>& pathList);
public:
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static void Read2MemFromXML_SCountD(vector<float>& data,vector<INT32>& res,int& Dim,unsigned long& Num);
	static void Read2MemFromXML_SDensityC(vector<float>& data,vector<INT32>& res,int& Dim,unsigned long& Num);
	static void Read2MemFromXML_SCountColor(vector<float>& data,vector<INT32>& res,int& Dim,unsigned long& Num);
	static void Read2MemFromXML_CountDColor(vector<float>& data,vector<INT32>& res,int& Dim,unsigned long& Num);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static void Read2MemFromXML_SCount(vector<float>& data,vector<INT32>& res,int& Dim,unsigned long& Num);
	static void Read2MemFromXML_SDensity(vector<float>& data,vector<INT32>& res,int& Dim,unsigned long& Num);
	static void Read2MemFromXML_SColor(vector<float>& data,vector<INT32>& res,int& Dim,unsigned long& Num);
	static void Read2MemFromXML_Count_Density(vector<float>& data,vector<INT32>& res,int& Dim,unsigned long& Num);
	static void Read2MemFromXML_Count_Color(vector<float>& data,vector<INT32>& res,int& Dim,unsigned long& Num);
	static void Read2MemFromXML_Density_Color(vector<float>& data,vector<INT32>& res,int& Dim,unsigned long& Num);
	static void Read2MemFromXML_Sum_Count_Density_Color(vector<float>& data,vector<INT32>& res,int& Dim,unsigned long& Num);
	
#if (CV_MAJOR_VERSION==2)&&(CV_MINOR_VERSION==4)&&USE_SVM_2_4_X
	static void Learning_samples_detection_rate(CvSVM& svm ,vector<float>& dataMatMem,vector<INT32>& resMatMem,vector<string>& pathList);
	static CvSVMParams GetLinearSVMParams(void);
	static CvSVMParams GetGaussianSVMParams(void);
	static CvSVMParams GetPolySVMParams(void);
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	static void GetLearnModule_SCountD(CvSVM& svm);
	static void GetLearnModule_SDensityC(CvSVM& svm);
	static void GetLearnModule_SCountColor(CvSVM& svm);
	static void GetLearnModule_CountDColor(CvSVM& svm);
	//////////////////////////////////////////////////////
	static void GetLearnModule_SCount(CvSVM& svm);
	static void GetLearnModule_SColor(CvSVM& svm);
	static void GetLearnModule_SDensity(CvSVM& svm);
	static void GetLearnModule_Count_Density(CvSVM& svm);
	static void GetLearnModule_Count_Color(CvSVM& svm);
	static void GetLearnModule_Density_Color(CvSVM& svm);
	//////////////////////////////////////////////////////
#endif
#if (CV_MAJOR_VERSION==3)  
	
#endif	
#if (CV_MAJOR_VERSION==2)&&(CV_MINOR_VERSION==4)
	
#endif
#if (CV_MAJOR_VERSION==3)  

#endif	
	
};
/*-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------*/
