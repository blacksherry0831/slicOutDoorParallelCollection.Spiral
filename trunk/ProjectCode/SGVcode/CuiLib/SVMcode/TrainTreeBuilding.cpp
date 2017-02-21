#include "StdAfx.h"
#include "modules.h"
//#include "TrainTreeBuilding.h"

/*-------------------------------------------------------------------------------------------*/
/*--模块依赖*/
/*-------------------------------------------------------------------------------------------*/
#include "tinyxml2/tinyxml2.h"
using namespace tinyxml2;


//#include "FilePathName/FileNameSplit.h"
//#include <SGVcode/cui_GeneralImgProcess.h>
//#include <SGVcode/SpAnalyze.h>
/*-------------------------------------------------------------------------------------------*/
/**
*
*/
/*--------------------------------------------------------------------------------------------*/
int TrainTreeBuilding::CategoryFeature=0;
double TrainTreeBuilding:: winwidth=48;
double TrainTreeBuilding:: winheight=96;
//// Directory containing positive sample images
string TrainTreeBuilding::posSamplesDir = "pos/";
//// Directory containing negative sample images
string TrainTreeBuilding::negSamplesDir = "neg/";
//// Set the file to write the features to
string TrainTreeBuilding::featuresFile = "genfiles/features.dat";
//// Set the file to write the SVM model to
string TrainTreeBuilding::svmModelFile = "genfiles/svmlightmodel.dat";
//// Set the file to write the resulting detecting descriptor vector to
string TrainTreeBuilding::descriptorVectorFile = "genfiles/descriptorvector.dat";
string TrainTreeBuilding:: SVMDetectorxml="SVMDetector.xml";
const Size TrainTreeBuilding:: trainingPadding = Size(0, 0);
const Size TrainTreeBuilding::winStride = Size(8, 8);
#if USE_MFC
CWinThread* TrainTreeBuilding:: pThread=NULL;
#endif
ThreadInfo TrainTreeBuilding::ThreadData;
/***********************************************************/
vector<string>TrainTreeBuilding::positiveTrainingImages;
vector<string>TrainTreeBuilding::negativeTrainingImages;

CvMat*TrainTreeBuilding::data_mat;
CvMat*TrainTreeBuilding::res_mat;
/*-------------------------------------------------------------------------------------------*/
/**
*
*/
/*--------------------------------------------------------------------------------------------*/
TrainTreeBuilding::TrainTreeBuilding(void)
{

}
/*-------------------------------------------------------------------------------------------*/
/**
*
*/
/*--------------------------------------------------------------------------------------------*/
TrainTreeBuilding::~TrainTreeBuilding(void)
{

}
/*-------------------------------------------------------------------------------------------*/
/**
*
*/
/*--------------------------------------------------------------------------------------------*/
UINT TrainTreeBuilding::TrainWithXMLdata(LPVOID lpParam)
{
	ASSERT(sizeof(float)==4);
#if ProgressBar
	ThreadData.pCtrlButton->EnableWindow(FALSE);
	ThreadData.ThreadinRun=FALSE;
#endif 		
#if TRUE 
	int FeatureDim=0;
	unsigned long samplenum=0;
	vector<float> dataMatMem;
	vector<float> resMatMem;
	vector<string> filenames;
	static CvMat dataMatHeader;
	static CvMat resMatHeader;

	Read2MemFromXML(dataMatMem,resMatMem,FeatureDim,samplenum,filenames);
	ASSERT(samplenum!=0);
	res_mat=cvInitMatHeader(&resMatHeader,samplenum,1,CV_32FC1,resMatMem.data());
	data_mat=cvInitMatHeader(&dataMatHeader,samplenum,FeatureDim,CV_32FC1,dataMatMem.data());
#endif

	Learning_samples_detection_rate(dataMatMem,resMatMem,filenames);

#if ProgressBar&&USE_MFC  
	MessageBox(NULL,_T("训练完成"),_T("TrainSample"),MB_OK);
	ThreadData.pCtrlButton->EnableWindow(TRUE);
	ThreadData.ThreadinRun=TRUE;
#endif	
	return 0;
}
/*-------------------------------------------------------------------------------------*/
/*
*
*/
/*-------------------------------------------------------------------------------------*/
UINT TrainTreeBuilding::ThreadPickUpFeature(LPVOID lpParam)
{
#if ProgressBar&&USE_MFC
	ThreadData.pCtrlButton->EnableWindow(FALSE);
	ThreadData.ThreadinRun=FALSE;
#endif 	
	ASSERT(sizeof(float)==4);
	int FeatureDim=SpAnalyze::GetFeatureSize();
	UINT samplenum=GetFileNameFromPath();
	ASSERT(samplenum!=0);

	for (unsigned long pi=0;pi<positiveTrainingImages.size();pi++){
		const string currentImageFile=positiveTrainingImages.at(pi);
#if TRUE
		SpAnalyze Spa;
		vector<float> feature;
		ImageMemData MemData(currentImageFile,"",0,0,0.5);
		TRACE("%s\n",currentImageFile.c_str());
		Spa.IsSpFatorThin(&MemData);
		Spa.SpSA=SpSizeAttr::FAT;
		Spa.ExtractImageFeatureWithSrcCany(feature);
#endif		 

		SaveFeature2XML("Feature.xml",feature,CategoryFeature,pi,currentImageFile);

		if (pi%10==0){
			TRACE("%f\n",1.0*pi/positiveTrainingImages.size());
		}
	}

#if ProgressBar&&USE_MFC  
	MessageBox(NULL,_T("训练完成"),_T("TrainSample"),MB_OK);
	ThreadData.pCtrlButton->EnableWindow(TRUE);
	ThreadData.ThreadinRun=TRUE;
#endif	
	return 0;
}
/*-------------------------------------------------------------------------------------*/
/*
*
*/
/*-------------------------------------------------------------------------------------*/
void TrainTreeBuilding::Read2MemFromXML(vector<float>& data,vector<float>& res,int& Dim,unsigned long& Num)
{
	data.clear();
	res.clear();
	if (posSamplesDir[posSamplesDir.size()-1]=='\\'){
		posSamplesDir.resize(posSamplesDir.size()-1);
	}
	string filename=posSamplesDir;
#if 0
try{
			
			 FileStorage fs(filename, FileStorage::READ);  
			 FileNode n = fs["SvmData"];                         // Read string sequence - Get node  
			 if (n.type() != FileNode::SEQ)  
			 {  
				 ASSERT(FALSE);
				 cerr << "strings is not a sequence! FAIL" << endl;  
			 } 
			 FileNodeIterator it = n.begin(), it_end = n.end(); // Go through the node  
			 for (; it != it_end; ++it) {
				cout << (string)*it << endl; 
			 }
	}catch( cv::Exception& e ){
		const char* err_msg = e.what();
	
	}
#endif
#if FALSE
tinyxml2::XMLDocument doc;  
doc.LoadFile(filename.c_str());  


tinyxml2::XMLElement *scene=doc.RootElement();  
tinyxml2::XMLElement *surface=scene->FirstChildElement("SvmData");

while (surface){  
	XMLElement *surfaceChild=surface->FirstChildElement();  
	const char* content;  
	const XMLAttribute *attributeOfSurface = surface->FirstAttribute();  

	while(surfaceChild)  
	{  
		content=surfaceChild->GetText();  
		surfaceChild=surfaceChild->NextSiblingElement();  
		TRACE("%s\n ",content);
	}  
	surface=surface->NextSiblingElement();  
}  
#endif
		tinyxml2::XMLDocument doc;  
		doc.LoadFile(filename.c_str());  
		tinyxml2::XMLError xmlError;

		tinyxml2::XMLElement *scene=doc.RootElement();  
		tinyxml2::XMLElement *surface=scene->FirstChildElement("SvmData");

		while (surface){  
					////////////////////////////////////////////////////////////////////////////
					int filesize=0;
					tinyxml2::XMLElement *FileSize_svmData=surface->FirstChildElement("FSize");
					xmlError=FileSize_svmData->QueryIntText(&filesize);
                    Dim=filesize;
					ASSERT(xmlError==XMLError::XML_SUCCESS);
					////////////////////////////////////////////////////////////////////////////
					float categoryfeature=0;
					tinyxml2::XMLElement *Category_svmData=surface->FirstChildElement("Category");
					xmlError=Category_svmData->QueryFloatText(&categoryfeature);
					res.push_back(categoryfeature);
					ASSERT(xmlError==XMLError::XML_SUCCESS);
					/////////////////////////////////////////////////////////////////////////////
					tinyxml2::XMLElement *Feature_svmData=surface->FirstChildElement("Feature");
					ASSERT(Dim!=0);
					char buff[1024];
					for (int i=0;i<Dim;i++){
						sprintf_s(buff,sizeof(buff),"F%d",i);
						tinyxml2::XMLElement *F_One=Feature_svmData->FirstChildElement(buff);
						ASSERT(F_One!=NULL);
						float F_One_data;
						xmlError=F_One->QueryFloatText(&F_One_data);
						ASSERT(xmlError==XMLError::XML_SUCCESS);
						data.push_back(F_One_data);
					}
					/////////////////////////////////////////////////////////////////////////////
				/*	const char *featurestr=Feature_svmData->GetText();
					vector<string> list_str;
					splitstring(featurestr," ",list_str);*/
			surface=surface->NextSiblingElement();  
		}  

		Num=res.size();
}
/*-------------------------------------------------------------------------------------*/
/*
*
*/
/*-------------------------------------------------------------------------------------*/
void TrainTreeBuilding::Read2MemFromXML(vector<float>& data,vector<float>& res,int& Dim,unsigned long& Num,vector<string>& filenames)
{
	data.clear();
	res.clear();
	filenames.clear();
	if (posSamplesDir[posSamplesDir.size()-1]=='\\'){
		posSamplesDir.resize(posSamplesDir.size()-1);
	}
	string filename=posSamplesDir;
#if 0
try{
			
			 FileStorage fs(filename, FileStorage::READ);  
			 FileNode n = fs["SvmData"];                         // Read string sequence - Get node  
			 if (n.type() != FileNode::SEQ)  
			 {  
				 ASSERT(FALSE);
				 cerr << "strings is not a sequence! FAIL" << endl;  
			 } 
			 FileNodeIterator it = n.begin(), it_end = n.end(); // Go through the node  
			 for (; it != it_end; ++it) {
				cout << (string)*it << endl; 
			 }
	}catch( cv::Exception& e ){
		const char* err_msg = e.what();
	
	}
#endif
#if FALSE
tinyxml2::XMLDocument doc;  
doc.LoadFile(filename.c_str());  


tinyxml2::XMLElement *scene=doc.RootElement();  
tinyxml2::XMLElement *surface=scene->FirstChildElement("SvmData");

while (surface){  
	XMLElement *surfaceChild=surface->FirstChildElement();  
	const char* content;  
	const XMLAttribute *attributeOfSurface = surface->FirstAttribute();  

	while(surfaceChild)  
	{  
		content=surfaceChild->GetText();  
		surfaceChild=surfaceChild->NextSiblingElement();  
		TRACE("%s\n ",content);
	}  
	surface=surface->NextSiblingElement();  
}  
#endif
		tinyxml2::XMLDocument doc;  
		doc.LoadFile(filename.c_str());  
		tinyxml2::XMLError xmlError;

		tinyxml2::XMLElement *scene=doc.RootElement();  
		tinyxml2::XMLElement *surface=scene->FirstChildElement("SvmData");

		while (surface){  
					tinyxml2::XMLElement *FileName_svmData=surface->FirstChildElement("FIleName");
					string filename_t=FileName_svmData->GetText();
					filenames.push_back(filename_t);
					////////////////////////////////////////////////////////////////////////////
					int filesize=0;
					tinyxml2::XMLElement *FileSize_svmData=surface->FirstChildElement("FSize");
					xmlError=FileSize_svmData->QueryIntText(&filesize);
                    Dim=filesize;
					ASSERT(xmlError==XMLError::XML_SUCCESS);
					////////////////////////////////////////////////////////////////////////////
					float categoryfeature=0;
					tinyxml2::XMLElement *Category_svmData=surface->FirstChildElement("Category");
					xmlError=Category_svmData->QueryFloatText(&categoryfeature);
					res.push_back(categoryfeature);
					ASSERT(xmlError==XMLError::XML_SUCCESS);
					/////////////////////////////////////////////////////////////////////////////
					tinyxml2::XMLElement *Feature_svmData=surface->FirstChildElement("Feature");
					ASSERT(Dim!=0);
					char buff[1024];
					for (int i=0;i<Dim;i++){
						sprintf_s(buff,sizeof(buff),"F%d",i);
						tinyxml2::XMLElement *F_One=Feature_svmData->FirstChildElement(buff);
						ASSERT(F_One!=NULL);
						float F_One_data;
						xmlError=F_One->QueryFloatText(&F_One_data);
						ASSERT(xmlError==XMLError::XML_SUCCESS);
						data.push_back(F_One_data);
					}
					/////////////////////////////////////////////////////////////////////////////
				/*	const char *featurestr=Feature_svmData->GetText();
					vector<string> list_str;
					splitstring(featurestr," ",list_str);*/
			surface=surface->NextSiblingElement();  
		}  

		Num=res.size();
}
/*-------------------------------------------------------------------------------------*/
/*
*
*/
/*-------------------------------------------------------------------------------------*/
UINT TrainTreeBuilding::GetFileNameFromPath(void)
{

	static vector<string> validExtensions;
	validExtensions.push_back("jpg");
	validExtensions.push_back("png");
	validExtensions.push_back("ppm");
	validExtensions.push_back("bmp");
	// </editor-fold>

	// <editor-fold defaultstate="collapsed" desc="Read image files">
	getFilesInDirectory(posSamplesDir, positiveTrainingImages, validExtensions);
	getFilesInDirectory(negSamplesDir, negativeTrainingImages, validExtensions);
	/// Retrieve the descriptor vectors from the samples
	unsigned long overallSamples = positiveTrainingImages.size() + negativeTrainingImages.size();
	// </editor-fold>

	// <editor-fold defaultstate="collapsed" desc="Calculate HOG features and save to file">
	// Make sure there are actually samples to train
	if (overallSamples == 0) {
		printf("No training sample files found, nothing to do!\n");	        
		TRACE("No training sample files found, nothing to do!\n");
		return overallSamples;
	}else{
		TRACE("ReadFileDown!\n");
		return overallSamples;
	}
}
/*-------------------------------------------------------------------------------------*/
/*
*
*/
/*-------------------------------------------------------------------------------------*/
void TrainTreeBuilding::SaveFeature2XML(string filename,vector<float> feature,int Category,float FileIndex,string SrcFileName)
{
	string filepath=SVMDetectorxml+"\\"+filename;
	FileStorage fs(filepath, FileStorage::APPEND);
	if (fs.isOpened()){
		fs<<"SvmData";
		fs<<"{";
		fs<<"FileIndex"<<FileIndex;
		fs<<"FIleName"<<SrcFileName;
		fs<<"Category"<<Category;
		int featureSize=feature.size();
		fs<<"FSize"<<featureSize;
		fs<<"Feature";
		fs<<"{";
		char buff[1024];
		for(int i=0;i<feature.size();i++){
			sprintf_s(buff,sizeof(buff),"F%d",i);
			string Fstr=buff;
			float FeatureOne=feature[i];
			fs<<buff<<FeatureOne;
		}
		fs<<"}";


		fs<<"}";
	}else{
		ASSERT(0);
	}


}
/*-------------------------------------------------------------------------------------*/
/*
*
*/
/*-------------------------------------------------------------------------------------*/

void TrainTreeBuilding::getFilesInDirectory(const string& dirName, vector<string>& fileNames, const vector<string>& validExtensions)
{
	printf("Opening directory %s\n", dirName.c_str());
	struct dirent* ep;
	size_t extensionLocation;
	DIR* dp = opendir(dirName.c_str());
	if (dp != NULL) {
		while ((ep = readdir(dp))) {
			// Ignore (sub-)directories like . , .. , .svn, etc.
			if (ep->d_type & DT_DIR) {
				continue;
			}
			extensionLocation = string(ep->d_name).find_last_of("."); // Assume the last point marks beginning of extension like file.ext
			// Check if extension is matching the wanted ones
			string tempExt = toLowerCase(string(ep->d_name).substr(extensionLocation + 1));
			if (find(validExtensions.begin(), validExtensions.end(), tempExt) != validExtensions.end()) {
				printf("Found matching data file '%s'\n", ep->d_name);
				fileNames.push_back((string) dirName + ep->d_name);
			} else {
				printf("Found file does not match required file type, skipping: '%s'\n", ep->d_name);
			}
		}
		(void) closedir(dp);
	} else {
		printf("Error opening directory '%s'!\n", dirName.c_str());
	}
	return;
}
/*-------------------------------------------------------------------------------------*/
/*
*
*/
/*-------------------------------------------------------------------------------------*/
string TrainTreeBuilding::toLowerCase(const string& in)
{
	string t;
	for (string::const_iterator i = in.begin(); i != in.end(); ++i) {
		t += tolower(*i);
	}
	return t;
}
/*-------------------------------------------------------------------------------------*/
/*
*
*/
/*-------------------------------------------------------------------------------------*/
int TrainTreeBuilding::EndProcess(void)
{
#if _MSC_VER
	Sleep(500);
#endif


	return TRUE;
}
/*-------------------------------------------------------------------------------------*/
/*
*
*/
/*-------------------------------------------------------------------------------------*/
int TrainTreeBuilding::StartProcessFromXmlFile(void)
{
#if USE_MFC
Sleep(200);

	pThread=AfxBeginThread(TrainWithXMLdata,NULL);	

	if (pThread){ 	
		return TRUE;
	}else{
		return FALSE;
	}
#else
	ASSERT(0);
	return 0;
#endif
	

}
/*-------------------------------------------------------------------------------------*/
/*
*
*/
/*-------------------------------------------------------------------------------------*/
void TrainTreeBuilding::Learning_samples_detection_rate(vector<float>& dataMatMem,vector<float>& resMatMem)
{
	unsigned long Category_0_ALL=0;
	unsigned long Category_0_Success=0;
	unsigned long Category_1_ALL=0;
	unsigned long Category_1_Success=0;
	CvMat MatOne;	
	int Dim=dataMatMem.size()/resMatMem.size();
	CvMat *MatOneFeature=cvInitMatHeader(&MatOne,1,Dim,CV_32FC1,resMatMem.data());
	float *memData=dataMatMem.data();
	for (unsigned long si=0;si<resMatMem.size();si++){
		MatOneFeature->data.fl=&memData[si*Dim];
#if 0
int cate_si=svm.predict(MatOneFeature);
#else
int cate_si=ClassifyTreeBuilding(&memData[si*Dim],Dim);
#endif
		
		int cate_org=resMatMem[si];
		if (cate_si==0){
			Category_0_ALL++;
		}
		if (cate_si==1){
			Category_1_ALL++;
		}
		if (cate_si==0&&cate_org==0){
			Category_0_Success++;
		}
		if (cate_si==1&&cate_org==1){
			Category_1_Success++;
		}
	}
	float Rate_0=1.0*Category_0_Success/Category_0_ALL;
	float Rate_1=1.0*Category_1_Success/Category_1_ALL;
	string filename=SVMDetectorxml+"\\RATEdata.xml";
	FileStorage fsw(filename,FileStorage::WRITE);
	if (fsw.isOpened()){
		fsw<<"Rate_0"<<Rate_0;
		fsw<<"Rate_1"<<Rate_1;
	}else{
		ASSERT(FALSE);
	}
}
/*-------------------------------------------------------------------------------------*/
/*
*
*/
/*-------------------------------------------------------------------------------------*/
void TrainTreeBuilding::Learning_samples_detection_rate(vector<float>& dataMatMem,vector<float>& resMatMem,vector<string>& filenames)
{
	unsigned long Category_0_ALL=0;
	unsigned long Category_0_Success=0;
	unsigned long Category_1_ALL=0;
	unsigned long Category_1_Success=0;
	CvMat MatOne;	
	int Dim=dataMatMem.size()/resMatMem.size();
	CvMat *MatOneFeature=cvInitMatHeader(&MatOne,1,Dim,CV_32FC1,resMatMem.data());
	float *memData=dataMatMem.data();
	for (unsigned long si=0;si<resMatMem.size();si++){
		MatOneFeature->data.fl=&memData[si*Dim];
#if 0
		int cate_si=svm.predict(MatOneFeature);
#else
		int cate_si=ClassifyTreeBuilding(&memData[si*Dim],Dim);
#endif

		int cate_org=resMatMem[si];
		if (cate_si==0){
			Category_0_ALL++;
		}
		if (cate_si==1){
			Category_1_ALL++;
		}
		if (cate_si==0&&cate_org==0){
			Category_0_Success++;
		}
		if (cate_si==1&&cate_org==1){
			Category_1_Success++;
		}
		if (cate_org==1&&cate_si!=1){
			//正样本判断错误的
			IplImage *img=cvLoadImage(filenames[si].c_str());
			FileNameSplit fns;
			string  filesavepath="aschioahscvua.png";
#if Use_CString&&_MSC_VER
	        fns.Parse(FileNameSplit::Convertstring2CS(filenames[si]));
			filesavepath=SVMDetectorxml+"\\PosError\\"+FileNameSplit::ConvertCS2string(fns.filefullname);
#endif
			cvSaveImage(filesavepath.c_str(),img);
		}
		if (cate_org==0&&cate_si!=0){
			//负样本判断错误的
			IplImage *img=cvLoadImage(filenames[si].c_str());
			FileNameSplit fns;
			string  filesavepath="anaojfioasj.png";
#if Use_CString&&_MSC_VER
	fns.Parse(FileNameSplit::Convertstring2CS(filenames[si]));
	filesavepath=SVMDetectorxml+"\\NegError\\"+FileNameSplit::ConvertCS2string(fns.filefullname);
#endif		
		

			cvSaveImage(filesavepath.c_str(),img);
		}
	}
	float Rate_0=1.0*Category_0_Success/Category_0_ALL;
	float Rate_1=1.0*Category_1_Success/Category_1_ALL;
	float Rate_sum=1.0*(Category_0_Success+Category_1_Success)/(Category_0_ALL+Category_1_ALL);
	string filename=SVMDetectorxml+"\\RATEdata.xml";
	FileStorage fsw(filename,FileStorage::WRITE);
	if (fsw.isOpened()){
		fsw<<"Rate_0"<<Rate_0;
		fsw<<"Rate_1"<<Rate_1;
		fsw<<"Rate_Sum"<<Rate_sum;
	}else{
		ASSERT(FALSE);
	}
}
/*-------------------------------------------------------------------------------------*/
/*
*Sum  Neg    avg --1722.748842-dev--1162.205992
*Sum  Pos    avg   4781.534735-dev--1411.253575
*  ------         1722+1162     ---------     4781-1411     --------
*                 2884----------------------3370------------------
*/
/*-------------------------------------------------------------------------------------*/
int TrainTreeBuilding::ClassifyTreeBuilding(float *data,unsigned int Dim)
{
	float sum=cui_GeneralImgProcess::GetSumValue(data,Dim);
	float ZeroCount=cui_GeneralImgProcess::GetZeroCount(data,Dim);
	if (sum<2884){
		return NEG_Category_Building;
	}else if((sum>=2884)&&sum<=3370){
		
		if (ZeroCount<=120){
			return NEG_Category_Building;
		}else{
			return POS_Category_TREE;
		}
	}else if(sum>3370)	{
		return POS_Category_TREE;
	}else{
		ASSERT(0);
	}
}
/*-------------------------------------------------------------------------------------*/
/*
*
*/
/*-------------------------------------------------------------------------------------*/