#pragma once
#define  POS_Category_TREE (1)
#define  NEG_Category_Building (0)
/*-------------------------------------------------------------------------------------*/
/*
*
*/
/*-------------------------------------------------------------------------------------*/
#ifndef MFC_ThreadInfo
#define MFC_ThreadInfo (TRUE&&_MSC_VER)
struct ThreadInfo
{
#if TRUE
	UINT nMilliSecond;	
	CEdit*           pCtrlEdit;
#endif	  	
	CProgressCtrl* pctrlProgress;//进程指针
	CStatic*       pctrlStatic;	
	CButton*         pCtrlButton;
	UINT        ThreadinRun;
};//结构体
#else

#endif
/*-------------------------------------------------------------------------------------------*/
/**
*
*/
/*--------------------------------------------------------------------------------------------*/
class TrainTreeBuilding
{
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
	static const Size trainingPadding;// = Size(0, 0);
	static const Size winStride;// = Size(8, 8); 
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
		static UINT TrainWithXMLdata(LPVOID lpParam);
	TrainTreeBuilding(void);
	~TrainTreeBuilding(void);
public:
#if USE_MFC&&Use_CString
		static CWinThread* pThread;
		static string ConvertCS2string(CString cstring);
#endif
		static   ThreadInfo ThreadData;
public:

	static UINT GetFileNameFromPath(void);
	static void Read2MemFromXML(vector<float>& data,vector<float>& res,int& Dim,unsigned long& Num);
	static void Read2MemFromXML(vector<float>& data,vector<float>& res,int& Dim,unsigned long& Num,vector<string>& filenames);
	static UINT ThreadPickUpFeature(LPVOID lpParam);	
	static void SaveFeature2XML(string filename,vector<float> feature,int Category,float FileIndex,string SrcFileName);
	static void getFilesInDirectory(	const string& dirName, vector<string>& fileNames, const vector<string>& validExtensions);
	static string toLowerCase(const string& in);
	static int EndProcess(void);
	static int StartProcessFromXmlFile(void);
	static void Learning_samples_detection_rate(vector<float>& dataMatMem,vector<float>& resMatMem);
	static void Learning_samples_detection_rate(vector<float>& dataMatMem,vector<float>& resMatMem,vector<string>& filenames);
	static int  ClassifyTreeBuilding(float *data,unsigned int Dim);
};
/*-------------------------------------------------------------------------------------------*/
/**
*
*/
/*--------------------------------------------------------------------------------------------*/

