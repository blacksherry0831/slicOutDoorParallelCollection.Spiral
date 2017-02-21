#include "StdAfx.h"
#include "modules.h"

/*-------------------------------------------------------------------------------------------*/
/*--模块依赖*/
/*-------------------------------------------------------------------------------------------*/
//#include "svmlight/svmlight.h"
//#include "dirent.h"
#include "TrainImgUseSvm.h"	
//#include <ML.H>   
//#include "FilePathName/FileNameSplit.h"
//#include <SGVcode/cui_GeneralImgProcess.h>
//#include <SGVcode/SpAnalyze.h>
/*-------------------------------------------------------------------------------------------*/
/**
*
*/
/*--------------------------------------------------------------------------------------------*/
int TrainImgUseSvm::CategoryFeature=0;
double TrainImgUseSvm:: winwidth=48;
double TrainImgUseSvm:: winheight=96;
//// Directory containing positive sample images
string TrainImgUseSvm::posSamplesDir = "pos/";
//// Directory containing negative sample images
string TrainImgUseSvm::negSamplesDir = "neg/";
//// Set the file to write the features to
string TrainImgUseSvm::featuresFile = "genfiles/features.dat";
//// Set the file to write the SVM model to
string TrainImgUseSvm::svmModelFile = "genfiles/svmlightmodel.dat";
//// Set the file to write the resulting detecting descriptor vector to
string TrainImgUseSvm::descriptorVectorFile = "genfiles/descriptorvector.dat";
string TrainImgUseSvm:: SVMDetectorxml="SVMDetector.xml";
Size TrainImgUseSvm:: trainingPadding = Size(0, 0);
Size TrainImgUseSvm::winStride = Size(8, 8);

#if USE_MFC
CWinThread*  TrainImgUseSvm:: pThread=NULL;
#endif

ThreadInfo TrainImgUseSvm::ThreadData;
/***********************************************************/
vector<string> TrainImgUseSvm::positiveTrainingImages;
vector<string> TrainImgUseSvm::negativeTrainingImages;
CvMat* TrainImgUseSvm::data_mat;
CvMat* TrainImgUseSvm::res_mat;

	
/*-------------------------------------------------------------------------------------*/
/**
*D:\ImageDataBase\svmlearnTreeBuilding\spb-vertical-testout\tree\img-math10-p-46t0_vertical__SPb_85.png_C_1_.jpg
D:\ImageDataBase\svmlearnTreeBuilding\spb-vertical-testout\tree\img-math10-p-46t0_vertical__SPb_86.png_C_1_.jpg
D:\ImageDataBase\svmlearnTreeBuilding\spb-vertical-testout\tree\img-math10-p-46t0_vertical__SPb_87.png_C_2_.jpg
D:\ImageDataBase\svmlearnTreeBuilding\spb-vertical-testout\tree\img-math10-p-46t0_vertical__SPb_89.png_C_1_.jpg
D:\ImageDataBase\svmlearnTreeBuilding\spb-vertical-testout\tree\img-math10-p-46t0_vertical__SPb_92.png_C_2_.jpg
D:\ImageDataBase\svmlearnTreeBuilding\spb-vertical-testout\tree\img-math10-p-46t0_vertical__SPb_94.png_C_1_.jpg
D:\ImageDataBase\svmlearnTreeBuilding\spb-vertical-testout\tree\img-math10-p-46t0_vertical__SPb_95.png_C_1_.jpg
D:\ImageDataBase\svmlearnTreeBuilding\spb-vertical-testout\tree\img-math10-p-46t0_vertical__SPb_96.png_C_2_.jpg
D:\ImageDataBase\svmlearnTreeBuilding\spb-vertical-testout\tree\img-math10-p-46t0_vertical__SPb_98.png_C_2_.jpg
D:\ImageDataBase\svmlearnTreeBuilding\spb-vertical-testout\tree\img-math11-p-108t0_vertical__SPb_0.png_C_1_.jpg
MFCtrainHOGsvmlight.exe 中的 0x757ec41f 处最可能的异常: Microsoft C++ 异常: 内存位置 0x04e4bbdc 处的 cv::Exception。
MFCtrainHOGsvmlight.exe 中的 0x757ec41f 处有未经处理的异常: Microsoft C++ 异常: 内存位置 0x04e4bbdc 处的 cv::Exception。
*/
/*-------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------*/
/**
*
*/
/*-------------------------------------------------------------------------------------*/
TrainImgUseSvm::TrainImgUseSvm(void)
{
#if 0
    int TrainImgUseSvm::CategoryFeature=0;
	double TrainImgUseSvm:: winwidth=48;
	double TrainImgUseSvm:: winheight=96;
	//// Directory containing positive sample images
	string TrainImgUseSvm::posSamplesDir = "pos/";
	//// Directory containing negative sample images
	string TrainImgUseSvm::negSamplesDir = "neg/";
	//// Set the file to write the features to
	string TrainImgUseSvm::featuresFile = "genfiles/features.dat";
	//// Set the file to write the SVM model to
	string TrainImgUseSvm::svmModelFile = "genfiles/svmlightmodel.dat";
	//// Set the file to write the resulting detecting descriptor vector to
	string TrainImgUseSvm::descriptorVectorFile = "genfiles/descriptorvector.dat";
	string TrainImgUseSvm:: SVMDetectorxml="SVMDetector.xml";
	const Size TrainImgUseSvm:: trainingPadding = Size(0, 0);
	const Size TrainImgUseSvm::winStride = Size(8, 8);
	CWinThread*  TrainImgUseSvm:: pThread=NULL;
	ThreadInfo TrainImgUseSvm::ThreadData;
	/***********************************************************/
	vector<string> TrainImgUseSvm::positiveTrainingImages;
	vector<string> TrainImgUseSvm::negativeTrainingImages;

	CvMat* TrainImgUseSvm::data_mat;
	CvMat* TrainImgUseSvm::res_mat;
#endif
	CategoryFeature=0;
	winwidth=48;
	winheight=96;
	//// Directory containing positive sample images
	posSamplesDir = "pos/";
	//// Directory containing negative sample images
	negSamplesDir = "neg/";
	//// Set the file to write the features to
	featuresFile = "genfiles/features.dat";
	//// Set the file to write the SVM model to
	svmModelFile = "genfiles/svmlightmodel.dat";
	//// Set the file to write the resulting detecting descriptor vector to
	descriptorVectorFile = "genfiles/descriptorvector.dat";
	SVMDetectorxml="SVMDetector.xml";
	trainingPadding = Size(0, 0);
	winStride = Size(8, 8);
#if USE_MFC
	pThread=NULL;
#endif

	
	/***********************************************************/
	//vector<string> TrainImgUseSvm::positiveTrainingImages;
	//vector<string> TrainImgUseSvm::negativeTrainingImages;

	//CvMat* TrainImgUseSvm::data_mat;
	//CvMat* TrainImgUseSvm::res_mat;
}
/*-------------------------------------------------------------------------------------*/
/**
*
*/
/*-------------------------------------------------------------------------------------*/
 Feature_VegaBuilding:: Feature_VegaBuilding(void)
{
	this->feature.clear();
}
/*-------------------------------------------------------------------------------------*/
/**
*
*/
/*-------------------------------------------------------------------------------------*/
Feature_VegaBuilding:: ~Feature_VegaBuilding(void)
{
	this->feature.clear();
}
/*-------------------------------------------------------------------------------------*/
/**
*导入lab的颜色特征
@param lc 颜色
*/
/*-------------------------------------------------------------------------------------*/
void Feature_VegaBuilding::SetLabColor(LabColor lc){
	lc.A_Color-=128;
	lc.B_color-=128;
	ASSERT(lc.A_Color>=-128&&lc.A_Color<=128);
	ASSERT(lc.B_color>=-128&&lc.B_color<=128);
	this->color=lc;
	this->ABangle=atan2(color.B_color,color.A_Color)*180/M_PI;
	ASSERT(ABangle>-180.1&&ABangle<=180.1);
	this->feature.push_back(ABangle);
}
/*-------------------------------------------------------------------------------------*/
/**
*导入小波能量特征
@param HarryImg
*/
/*-------------------------------------------------------------------------------------*/
void Feature_VegaBuilding::SetHarrEnergy(Energy_Harr HarrImg)
{
	this->HarrImg=HarrImg;
}
/*-------------------------------------------------------------------------------------*/
/**
*导入曼哈顿直方图特征
*/
/*-------------------------------------------------------------------------------------*/
void Feature_VegaBuilding::SetHist180(HistData180 histdata)
{
#if TRUE
	{
		float *histall=histdata.hist_all;
		float max_value=cui_GeneralImgProcess::GetMaxValue(histall,180);
		for (int i=0;i<180;i++){
			if (max_value!=0){
				histall[i]=histall[i]/max_value*100;
			}else{
				histall[i]=0;
			}

		}
	}
#endif
	this->ZeroCount_FeatureManhattanLength=cui_GeneralImgProcess::GetZeroCountContinueWeight(histdata.hist_all,180);
	this->Sum_FeatureManhattanLength=cui_GeneralImgProcess::GetSumValue(histdata.hist_all,180);
	this->LineNumDensity=100*100*histdata.hist_all_count_sum/(this->Width*this->Height);
#if TRUE
	//this->feature.clear();
	this->feature.push_back(this->ZeroCount_FeatureManhattanLength);
	this->feature.push_back(this->Sum_FeatureManhattanLength);
	this->feature.push_back(this->LineNumDensity);
#endif
#if TRUE
	this->FeatureLineNum.clear();
	this->FeatureManhattanLength.clear();
	for (int i=0;i<180;i++){
		this->FeatureManhattanLength.push_back(histdata.hist_all[i]);
		this->FeatureLineNum.push_back(histdata.hist_all_count[i]);
	}
#endif
	
	

}
/*-------------------------------------------------------------------------------------*/
/**
*
*/
/*-------------------------------------------------------------------------------------*/
TrainImgUseSvm::~TrainImgUseSvm(void)
{

}
/*-------------------------------------------------------------------------------------*/
/**
*将string变成小写
*@param [in][out] 输入输出字符串
*@return 小写字符串
*/
/*-------------------------------------------------------------------------------------*/
string TrainImgUseSvm::toLowerCase(const string& in)
{
	string t;
	for (string::const_iterator i = in.begin(); i != in.end(); ++i) {
		t += tolower(*i);
	}
	return t;
}
/*-------------------------------------------------------------------------------------*/
/**
*
*/
/*-------------------------------------------------------------------------------------*/
void TrainImgUseSvm::storeCursor(void)
{
	 printf("\033[s");
}
/*-------------------------------------------------------------------------------------*/
/**
*
*/
/*-------------------------------------------------------------------------------------*/

void TrainImgUseSvm::resetCursor(void)
{
	 printf("\033[u");
}
/*-------------------------------------------------------------------------------------*/
/**
*保存特征向量到文件
*@param
*@param
*@param
*/
/*-------------------------------------------------------------------------------------*/
void TrainImgUseSvm::saveDescriptorVectorToFile(
	vector<float>& descriptorVector, 
	vector<unsigned int>& _vectorIndices,
	string fileName)
{
	printf("Saving descriptor vector to file '%s'\n", fileName.c_str());
	string separator = " "; // Use blank as default separator between single features
	fstream File;
	float percent;
	File.open(fileName.c_str(), ios::out);
	if (File.good() && File.is_open()) {
		printf("Saving descriptor vector features:\t");
		storeCursor();
		for (int feature = 0; feature < descriptorVector.size(); ++feature) {
			if ((feature % 10 == 0) || (feature == (descriptorVector.size()-1)) ) {
				percent = ((1 + feature) * 100 / descriptorVector.size());
				printf("%4u (%3.0f%%)", feature, percent);
				fflush(stdout);
				resetCursor();
			}
			File << descriptorVector.at(feature) << separator;
		}
		printf("\n");
		File << endl;
		File.flush();
		File.close();
	}
}
/*-------------------------------------------------------------------------------------*/
/**
*获得路径中的所有文件
*/
/*-------------------------------------------------------------------------------------*/
void TrainImgUseSvm::getFilesInDirectory(
	const string& dirName,
	vector<string>& fileNames,
	const vector<string>& validExtensions)
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
/**
*
*/
/*-------------------------------------------------------------------------------------*/
void TrainImgUseSvm::calculateFeaturesFromInput(
	const string& imageFilename,
	vector<float>& featureVector,
	HOGDescriptor& hog)
{
	 /** for imread flags from openCV documentation, 
     * @see http://docs.opencv.org/modules/highgui/doc/reading_and_writing_images_and_video.html?highlight=imread#Mat imread(const string& filename, int flags)
     * @note If you get a compile-time error complaining about following line (esp. imread),
     * you either do not have a current openCV version (>2.0) 
     * or the linking order is incorrect, try g++ -o openCVHogTrainer main.cpp `pkg-config --cflags --libs opencv`
     */
    Mat imageData = imread(imageFilename, 0);
    if (imageData.empty()) {
        featureVector.clear();
        printf("Error: HOG image '%s' is empty, features calculation skipped!\n", imageFilename.c_str());
        return;
    }
    // Check for mismatching dimensions
    if (imageData.cols != hog.winSize.width || imageData.rows != hog.winSize.height) {
        featureVector.clear();
        printf("Error: Image '%s' dimensions (%u x %u) do not match HOG window size (%u x %u)!\n", imageFilename.c_str(), imageData.cols, imageData.rows, hog.winSize.width, hog.winSize.height);
       
#if ProgressBar&&_MSC_VER  
		// MessageBox(NULL,_T("窗口大小不匹配"),_T("TRAIN"),MB_OK);		
	/*	ThreadData.pCtrlButton->EnableWindow(TRUE);
		ThreadData.ThreadinRun=TRUE;*/
		/*AfxEndThread(0);*/
#if UNICODE
		ThreadData.pCtrlEdit->SetWindowText(L"窗口大小不匹配");
#else
		ThreadData.pCtrlEdit->SetWindowText("窗口大小不匹配");
#endif

#endif
		return;
    }
    vector<Point> locations;
    hog.compute(imageData, featureVector, winStride, trainingPadding, locations);
    imageData.release(); // Release the image again after features are extracted
}

/*-------------------------------------------------------------------------------------*/
/**
*
*/
/*-------------------------------------------------------------------------------------*/
void TrainImgUseSvm::showDetections(const vector<Rect>& found, Mat& imageData)
{
	vector<Rect> found_filtered;
	size_t i, j;
	for (i = 0; i < found.size(); ++i) {
		Rect r = found[i];
		for (j = 0; j < found.size(); ++j)
			if (j != i && (r & found[j]) == r)
				break;
		if (j == found.size())
			found_filtered.push_back(r);
	}
	for (i = 0; i < found_filtered.size(); i++) {
		Rect r = found_filtered[i];
		rectangle(imageData, r.tl(), r.br(), Scalar(64, 255, 64), 3);
	}
}

/*-------------------------------------------------------------------------------------*/
/**
*
*/
/*-------------------------------------------------------------------------------------*/
void TrainImgUseSvm::detectTest(const HOGDescriptor& hog, Mat& imageData)
{
	vector<Rect> found;
	int groupThreshold = 2;
	Size padding(Size(32, 32));
	Size winStride(Size(8, 8));
	double hitThreshold = 0.; // tolerance
	hog.detectMultiScale(imageData, found, hitThreshold, winStride, padding, 1.05, groupThreshold);
	showDetections(found, imageData);
}

/*-------------------------------------------------------------------------------------*/
/**
*
*/
/*-------------------------------------------------------------------------------------*/
UINT TrainImgUseSvm::TRainTrainHog(LPVOID lpParam)
{
#if ProgressBar
	ThreadData.pCtrlButton->EnableWindow(FALSE);
	ThreadData.ThreadinRun=FALSE;
#endif  
	
     // <editor-fold defaultstate="collapsed" desc="Init">
    HOGDescriptor hog(Size(winwidth,winheight),Size(16,16),Size(8,8),Size(8,8),9); // Use standard parameters here
	//hog.winSize = cv::Size(70,134);
    // Get the files to train from somewhere
    static vector<string> positiveTrainingImages;
    static vector<string> negativeTrainingImages;
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
 #if ProgressBar  
		MessageBox(NULL,_T("正负样本数==0"),_T("TrainSample"),MB_OK);
	   ThreadData.pCtrlButton->EnableWindow(TRUE);
	   ThreadData.ThreadinRun=TRUE;
#endif	 
	return EXIT_SUCCESS;
    }
  
    /// @WARNING: This is really important, some libraries (e.g. ROS) seems to set the system locale which takes decimal commata instead of points which causes the file input parsing to fail
    setlocale(LC_ALL, "C"); // Do not use the system locale
    setlocale(LC_NUMERIC,"C");
    setlocale(LC_ALL, "POSIX");
#if ProgressBar
	ThreadData.pCtrlEdit->SetWindowText(_T("Reading files  \r\ngenerating HOG features and save them to file"));
	ThreadData.pctrlProgress->SetRange32(0,overallSamples);
#endif
    printf("Reading files, generating HOG features and save them to file '%s':\n", featuresFile.c_str());
    float percent;
    /**
     * Save the calculated descriptor vectors to a file in a format that can be used by SVMlight for training
     * @NOTE: If you split these steps into separate steps: 
     * 1. calculating features into memory (e.g. into a cv::Mat or vector< vector<float> >), 
     * 2. saving features to file / directly inject from memory to machine learning algorithm,
     * the program may consume a considerable amount of main memory
     */ 
    fstream File;
    File.open(featuresFile.c_str(), ios::out);
    if (File.good() && File.is_open()) {
        File << "# Use this file to train, e.g. SVMlight by issuing $ svm_learn -i 1 -a weights.txt " << featuresFile.c_str() << endl; // Remove this line for libsvm which does not support comments
        // Iterate over sample images
        for (unsigned long currentFile = 0; currentFile < overallSamples; ++currentFile) {
            storeCursor();
            vector<float> featureVector;
            // Get positive or negative sample image file path
            const string currentImageFile = (currentFile < positiveTrainingImages.size() ? positiveTrainingImages.at(currentFile) : negativeTrainingImages.at(currentFile - positiveTrainingImages.size()));
            // Output progress
            if ( (currentFile+1) % 10 == 0 || (currentFile+1) == overallSamples ) {
                percent = ((currentFile+1) * 100 / overallSamples);
                printf("%5lu (%3.0f%%):\tFile '%s'", (currentFile+1), percent, currentImageFile.c_str());
                fflush(stdout);
                resetCursor();
            }
            // Calculate feature vector from current image file
            calculateFeaturesFromInput(currentImageFile, featureVector, hog);
#if ProgressBar
			   ThreadData.pctrlProgress->SetPos(currentFile);

#endif
            if (!featureVector.empty()) {
                /* Put positive or negative sample class to file, 
                 * true=positive, false=negative, 
                 * and convert positive class to +1 and negative class to -1 for SVMlight
                 */
                File << ((currentFile < positiveTrainingImages.size()) ? "+1" : "-1");
                // Save feature vector components
                for (unsigned int feature = 0; feature < featureVector.size(); ++feature) {
                    File << " " << (feature + 1) << ":" << featureVector.at(feature);
                }
                File << endl;
            }
        }
        printf("\n");
        File.flush();
        File.close();
    } else {
        printf("Error opening file '%s'!\n", featuresFile.c_str());
        return EXIT_FAILURE;
    }
    // </editor-fold>
#if ProgressBar
	ThreadData.pctrlProgress->SetPos(0);
	ThreadData.pCtrlEdit->SetWindowText(_T("Calling SVMlight\r\n Read Data to Memory"));

#endif
    // <editor-fold defaultstate="collapsed" desc="Pass features to machine learning algorithm">
    /// Read in and train the calculated feature vectors
    printf("Calling SVMlight\n");
    SVMlight::getInstance()->read_problem(const_cast<char*> (featuresFile.c_str()));
#if ProgressBar
	ThreadData.pctrlProgress->SetPos(0);
	ThreadData.pCtrlEdit->SetWindowText(_T("Calling SVMlight\r\n train"));

#endif
    SVMlight::getInstance()->train(); // Call the core libsvm training procedure
#if ProgressBar
	ThreadData.pctrlProgress->SetPos(0);
	ThreadData.pCtrlEdit->SetWindowText(_T("Calling SVMlight\r\n Training done, saving model file!"));

#endif
	printf("Training done, saving model file!\n");
    SVMlight::getInstance()->saveModelToFile(svmModelFile);
    // </editor-fold>

    // <editor-fold defaultstate="collapsed" desc="Generate single detecting feature vector from calculated SVM support vectors and SVM model">
    printf("Generating representative single HOG feature vector using svmlight!\n");
    vector<float> descriptorVector;
    vector<unsigned int> descriptorVectorIndices;
    // Generate a single detecting feature vector (v1 | b) from the trained support vectors, for use e.g. with the HOG algorithm
    SVMlight::getInstance()->getSingleDetectingVector(descriptorVector, descriptorVectorIndices);
    // And save the precious to file system
    saveDescriptorVectorToFile(descriptorVector, descriptorVectorIndices, descriptorVectorFile);
    // </editor-fold>

    // <editor-fold defaultstate="collapsed" desc="Test detecting vector">
    printf("Testing custom detection using camera\n");
   
	/*hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());*/
	
 FileStorage fsW(SVMDetectorxml, FileStorage::WRITE| FileStorage::FORMAT_XML);
 fsW<<"SVMDetector"<<descriptorVector;
 fsW.release();
#if 0	
 FileStorage fsR(SVMDetectorxml, FileStorage::READ| FileStorage::FORMAT_XML);
 fsR["SVMDetector"] >>descriptorVector;
  fsR.release();

 hog.setSVMDetector(descriptorVector); // Set our custom detecting vector

	 ////////////////////////////////////////////////////////////////////////
    VideoCapture cap(0); // open the default camera
    if(!cap.isOpened()) { // check if we succeeded
        printf("Error opening camera!\n");
        return EXIT_FAILURE;
    }
    Mat testImage;
    while ((cvWaitKey(10) & 255) != 27) {
        cap >> testImage; // get a new frame from camera
//        cvtColor(testImage, testImage, CV_BGR2GRAY); // If you want to work on grayscale images
        detectTest(hog, testImage);
		cvWaitKey(1000);
        imshow("HOG custom detection", testImage);
    }
    // </editor-fold>
#endif
#if ProgressBar  
	MessageBox(NULL,_T("训练完成"),_T("TrainSample"),MB_OK);
	ThreadData.pCtrlButton->EnableWindow(TRUE);
	ThreadData.ThreadinRun=TRUE;
#endif	
	return EXIT_SUCCESS;
}
/*-------------------------------------------------------------------------------------*/
/**
*
*/
/*-------------------------------------------------------------------------------------*/

int TrainImgUseSvm::StartProcess(void)
{
#if USE_MFC
	Sleep(200);
#if 0
	pThread=AfxBeginThread(TRainTrainHog,NULL);
#else
	pThread=AfxBeginThread(Train,NULL);
#endif	
	
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
/**
*
*/
/*-------------------------------------------------------------------------------------*/

int TrainImgUseSvm::StartProcessPickUpFeature(void)
{
#if USE_MFC
Sleep(200);

	pThread=AfxBeginThread(ThreadPickUpFeature,NULL);	

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
/**
*
*/
/*-------------------------------------------------------------------------------------*/

int TrainImgUseSvm::StartProcessFromXmlFile(void)
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
/**
*结束进程
*/
/*-------------------------------------------------------------------------------------*/
int TrainImgUseSvm::EndProcess(void)
{
#if _MSC_VER
	Sleep(500);
#endif

	return TRUE;
}
/*-------------------------------------------------------------------------------------*/
/**
*转换CString成string
*@param cstring CString字符串 
*@return  string 字符串 
*/
/*-------------------------------------------------------------------------------------*/
#if Use_CString
string TrainImgUseSvm::ConvertCS2string(CString cstring)
{
	CStringA stra(cstring.GetBuffer(0));
	cstring.ReleaseBuffer();	
	std::string cui_t=stra;		
	stra.ReleaseBuffer();
	return cui_t;
}
#endif
/*-------------------------------------------------------------------------------------*/
/**
*
*/
/*-------------------------------------------------------------------------------------*/
 UINT TrainImgUseSvm::Train(LPVOID lpParam)
 {
#if ProgressBar
	 ThreadData.pCtrlButton->EnableWindow(FALSE);
	 ThreadData.ThreadinRun=FALSE;
#endif 	
	 ASSERT(sizeof(float)==4);
	int FeatureDim=SpAnalyze::GetFeatureSize();
	UINT samplenum=GetFileNameFromPath();
	ASSERT(samplenum!=0);
	
#if AllocMemVector
	vector<float> dataMatMem;
	vector<float> resMatMem;
	static CvMat dataMatHeader;
	static CvMat resMatHeader;
    dataMatMem.resize(samplenum*FeatureDim,0);
	resMatMem.resize(samplenum,0);
	res_mat=cvInitMatHeader(&resMatHeader,samplenum,1,CV_32FC1,resMatMem.data());
	data_mat=cvInitMatHeader(&dataMatHeader,samplenum,FeatureDim,CV_32FC1,dataMatMem.data());
#endif
#if AllocMemCV
	data_mat=cvCreateMat(samplenum,FeatureDim,CV_32FC1);
	res_mat=cvCreateMat(samplenum,1,CV_32FC1);
#endif
	
#if USE_SVM_2_4_X&&_MSC_VER
	CvSVM svm; 
	CvSVMParams param;
	CvTermCriteria criteria=cvTermCriteria(CV_TERMCRIT_EPS, 1000, FLT_EPSILON); 
	//☆☆☆☆☆☆☆☆☆(5)SVM学习☆☆☆☆☆☆☆☆☆☆☆☆    
	//FillFeatureInMat(FeatureDim);
	 FillHarrFeatureInMat(FeatureDim);
	 svm.train(data_mat, res_mat, NULL, NULL, param);//☆    
	//☆☆利用训练数据和确定的学习参数,进行SVM学习☆☆☆☆  
	string svmsavepath=SVMDetectorxml+"\\SvmModule.xml";
	svm.save(svmsavepath.c_str(),0);
#else
	ASSERT(0);
#endif

#if AllocMemCV
	cvReleaseMat(&data_mat);
	cvReleaseMat(&res_mat);
#endif

#if ProgressBar  
	MessageBox(NULL,_T("训练完成"),_T("TrainSample"),MB_OK);
	ThreadData.pCtrlButton->EnableWindow(TRUE);
	ThreadData.ThreadinRun=TRUE;
#endif	
	return 0;
}
/*-------------------------------------------------------------------------------------*/
/**
*读取XML文件，并机器学习
*/
/*-------------------------------------------------------------------------------------*/
 UINT TrainImgUseSvm::TrainWithXMLdata(LPVOID lpParam)
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
	 vector<INT32> resMatMem;
	 static CvMat dataMatHeader;
	 static CvMat resMatHeader;
	 vector<string> filePathList;
	  ReadTrainDataFromXML(dataMatMem,resMatMem,FeatureDim,samplenum, filePathList);
#endif	
#if SVM_AllocMemVector
	 res_mat=cvInitMatHeader(&resMatHeader,samplenum,1, CV_32SC1,resMatMem.data());
	 data_mat=cvInitMatHeader(&dataMatHeader,samplenum,FeatureDim,CV_32FC1,dataMatMem.data());
#endif
#if SVM_AllocMemCV
	 res_mat=cvCreateMat(,,);
#endif
#if (CV_MAJOR_VERSION==2)&&(CV_MINOR_VERSION==4)&&USE_SVM_2_4_X
	 CvSVM svm; 
	 CvSVMParams param;
#ifdef SVM_USE_Linear
#if SVM_USE_Linear
	 param=GetLinearSVMParams();
#endif
#endif
#ifdef SVM_USE_Gaussian
#if SVM_USE_Gaussian
	 param=GetGaussianSVMParams();
#endif
#endif
#ifdef SVM_USE_Poly
#if SVM_USE_Poly
	 param=GetPolySVMParams();
#endif
#endif
	 //☆☆☆☆☆☆☆☆☆(5)SVM学习☆☆☆☆☆☆☆☆☆☆☆☆    
	 //FillFeatureInMat(FeatureDim);
	 svm.train(data_mat, res_mat, NULL, NULL, param);//☆    
	 //☆☆利用训练数据和确定的学习参数,进行SVM学习☆☆☆☆  
	 string svmsavepath=SVMDetectorxml+"\\SvmModule.xml";
	 svm.save(svmsavepath.c_str(),0);
	 Learning_samples_detection_rate(svm,dataMatMem,resMatMem,filePathList);
#endif

#if (CV_MAJOR_VERSION==2)&&(CV_MINOR_VERSION==4)&&USE_SVM_2_4_X
	if(Dimensions_USE==Normal){
	
	 }else if(Dimensions_USE==Sum_Count_Density){
		 GetLearnModule_SCountD(svm);
	 }else if(Dimensions_USE==Sum_Density_Color){
		 GetLearnModule_SDensityC(svm);
	 }else if(Dimensions_USE==Sum_Count_Color){
		 GetLearnModule_SCountColor(svm);
	 }else if(Dimensions_USE==Count_Density_Color){
		 GetLearnModule_CountDColor(svm);
	 }else if(Dimensions_USE==Sum_Count){
		 GetLearnModule_SCount(svm);
	 }else if(Dimensions_USE==Sum_Density){
		 GetLearnModule_SDensity(svm);
	 }else if(Dimensions_USE==Sum_Color){
		GetLearnModule_SColor(svm);
	 }else if(Dimensions_USE==Count_Density){
		GetLearnModule_Count_Density(svm);
	 }else if(Dimensions_USE==Count_Color){
		GetLearnModule_Count_Color(svm);
	 }else if(Dimensions_USE==Density_Color){
		GetLearnModule_Density_Color(svm);
	 }		
#endif
#if (CV_MAJOR_VERSION==3)  
	 ASSERT(0);
#endif	
	
#if ProgressBar  
	 MessageBox(NULL,_T("训练完成"),_T("TrainSample"),MB_OK);
	 ThreadData.pCtrlButton->EnableWindow(TRUE);
	 ThreadData.ThreadinRun=TRUE;
#endif	
	 return 0;
 }
/*-------------------------------------------------------------------------------------*/
/**
*提取植被，建筑机器学习
*/
/*-------------------------------------------------------------------------------------*/
 UINT TrainImgUseSvm::ThreadPickUpFeature(LPVOID lpParam)
 {
#if ProgressBar
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
		 Feature_VegaBuilding fvb=Spa.ExtractImage_Sum_Count_Feature();
#endif
#if 0
		SaveFeature2XML("Feature.xml",feature,CategoryFeature,pi,currentImageFile);
#endif
		 SaveFeature2XML("FeatureHarrManhattanColor.xml",fvb,CategoryFeature,pi);
			 if (pi%10==0){
				 TRACE("%f\n",1.0*pi/positiveTrainingImages.size());
			 }
	 }
	
#if ProgressBar  
	 MessageBox(NULL,_T("训练完成"),_T("TrainSample"),MB_OK);
	 ThreadData.pCtrlButton->EnableWindow(TRUE);
	 ThreadData.ThreadinRun=TRUE;
#endif	
	 return 0;
 }
/*-------------------------------------------------------------------------------------*/
/**
*
*/
/*-------------------------------------------------------------------------------------*/
UINT TrainImgUseSvm::GetFileNameFromPath(void)
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
/**
*
*/
/*-------------------------------------------------------------------------------------*/
 void TrainImgUseSvm::PickUpFeature(const string& imageFilename,HistData180 *histdata,int *Dim)
{
	ImageMemData memdata(imageFilename,"",0,0,0);
	HoughAnalyze ha(&memdata);
	*histdata=ha.GetSaveALlImgHistogram_180_0to100();
	*Dim=AngLeDivided180;

}
/*-------------------------------------------------------------------------------------*/
/**
*
*/
/*-------------------------------------------------------------------------------------*/
 void TrainImgUseSvm::FillFeatureInMat(int Dim)
 {
	  unsigned long overallSamples = positiveTrainingImages.size() + negativeTrainingImages.size();
	  float *data=data_mat->data.fl;
	  float *category=res_mat->data.fl;
		unsigned long  ProgressCount=0;
		HistData180 histdata;
		   for (unsigned long ni=0;ni<negativeTrainingImages.size();ni++,ProgressCount++){
			   const string currentImageFile=negativeTrainingImages.at(ni);
			   PickUpFeature(currentImageFile,&histdata,&Dim);
			   memcpy(&data[ProgressCount*Dim],histdata.hist_all,sizeof(float)*Dim);
			   category[ProgressCount]=SvmCategoryNeg;
			   if (ProgressCount%10==0)
			   {
				   TRACE("%5.2f\n",ProgressCount/overallSamples);
			   }
		   }
		   for (unsigned long pi=0;pi<positiveTrainingImages.size();pi++,ProgressCount++){
			    const string currentImageFile=positiveTrainingImages.at(pi);
				PickUpFeature(currentImageFile,&histdata,&Dim);
				memcpy(&data[ProgressCount*Dim],histdata.hist_all,sizeof(float)*Dim);
				category[ProgressCount]=SvmCategoryPos;
				if (ProgressCount%10==0)
				{
					TRACE("%5.2f\n",1.0*ProgressCount/overallSamples);
				}
		   }

 }
/*-------------------------------------------------------------------------------------*/
/**
*将特征填入矩阵
*/
/*-------------------------------------------------------------------------------------*/
 void TrainImgUseSvm::FillHarrFeatureInMat(int Dim)
 {
	 unsigned long overallSamples = positiveTrainingImages.size() + negativeTrainingImages.size();
	 float *data=data_mat->data.fl;
	 float *category=res_mat->data.fl;
	 unsigned long  ProgressCount=0;
	 HistData180 histdata;
	 for (unsigned long ni=0;ni<negativeTrainingImages.size();ni++,ProgressCount++){
		 const string currentImageFile=negativeTrainingImages.at(ni);
#if TRUE
			 SpAnalyze Spa;
			 vector<float> feature;
			 ImageMemData MemData(currentImageFile,"",0,0,0.5);
			 TRACE("%s\n",currentImageFile.c_str());
			 Spa.IsSpFatorThin(&MemData);
			 Spa.SpSA=SpSizeAttr::FAT;
			 Spa.ExtractImageFeatureWithSrcCany(feature);
			 ASSERT(feature.size()==Dim);
#endif
		 memcpy(&data[ProgressCount*Dim],feature.data(),sizeof(float)*Dim);
		 category[ProgressCount]=SvmCategoryNeg;
		 if (ProgressCount%10==0)
		 {
			 TRACE("%5.2f\n",1.0*ProgressCount/overallSamples);
		 }
	 }
	 for (unsigned long pi=0;pi<positiveTrainingImages.size();pi++,ProgressCount++){
		 const string currentImageFile=positiveTrainingImages.at(pi);
#if TRUE
		 SpAnalyze Spa;
		 vector<float> feature;
		 ImageMemData MemData(currentImageFile,"",0,0,0.5);
		 TRACE("%s\n",currentImageFile.c_str());
		 Spa.IsSpFatorThin(&MemData);
		 Spa.SpSA=SpSizeAttr::FAT;
		 Spa.ExtractImageFeatureWithSrcCany(feature);
		 ASSERT(feature.size()==Dim);
#endif
		 memcpy(&data[ProgressCount*Dim],feature.data(),sizeof(float)*Dim);
		 category[ProgressCount]=SvmCategoryPos;
		 if (ProgressCount%10==0)
		 {
			 TRACE("%5.2f\n",1.0*ProgressCount/overallSamples);
		 }
	 }

 }
/*-------------------------------------------------------------------------------------*/
/**
*保存特征到XML文件
*@param filename 文件名
*@param feature 特性向量
*@param Category 特征类别
*@param FileIndex 文件索引
*@param SrcFileName 原文件名
*/
/*-------------------------------------------------------------------------------------*/
 void TrainImgUseSvm::SaveFeature2XML(
	 string filename,
	 vector<float> feature,
	 int Category,
	 float FileIndex,
	 string SrcFileName)
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
/**
*保存特征到XML文件（FileStorage::APPEND）
*@param  filename 文件名
*@param  f_vb 特征类
*@param  Category 特征分类
*@param  FileIndex 文件索引
*/
/*-------------------------------------------------------------------------------------*/
 void TrainImgUseSvm::SaveFeature2XML(
	 string filename,
	 Feature_VegaBuilding f_vb,
	 int Category,
	 float FileIndex)
 {
#if TRUE
 string SrcFileName=f_vb.SrcFileName;
	 string filepath=SVMDetectorxml+"\\"+filename;
	 vector<float> feature=f_vb.feature;
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
#if TRUE
		fs<<"SumManhattan"<<f_vb.Sum_FeatureManhattanLength;
		fs<<"ZeroCount"<<f_vb.ZeroCount_FeatureManhattanLength;
		fs<<"LineNumDensity"<<f_vb.LineNumDensity;
		fs<<"Color"<<f_vb.ABangle;
		fs<<"Color_L"<<f_vb.color.L_Color;
		fs<<"Color_A"<<f_vb.color.A_Color;
		fs<<"Color_B"<<f_vb.color.B_color;
#endif
#if FALSE
		fs<<"LL"<<f_vb.HarrImg.Energy_LL;
		fs<<"LH"<<f_vb.HarrImg.Energy_LH;
		fs<<"HL"<<f_vb.HarrImg.Energy_HL;
		fs<<"HH"<<f_vb.HarrImg.Energy_HH;	
#endif
		 fs<<"}";
	 }else{
		 ASSERT(0);
	 }
#endif
 }
/*-------------------------------------------------------------------------------------*/
/**
*
*@param [out] data 特征矩阵
*@param [out] res 特征类别
*@param [out] Dim 特征维度
*@param [out] Num 特征数量
*/
/*-------------------------------------------------------------------------------------*/
void TrainImgUseSvm::Read2MemFromXML(
	vector<float>& data,
	vector<INT32>& res,
	int& Dim,
	unsigned long& Num)
{
	data.clear();
	res.clear();
	if (posSamplesDir[posSamplesDir.size()-1]=='\\'){
		posSamplesDir.resize(posSamplesDir.size()-1);
	}
	string filename=posSamplesDir;
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
/**
*分隔字符串
*/
/*-------------------------------------------------------------------------------------*/
void TrainImgUseSvm::splitstring(const char * str,const char * deli, vector<string>&list)
{
#if TRUE
		vector<char> buff_t(1024*4000);
		char *buff=buff_t.data();
		//_snprintf_s(buff,buff_t.size()*sizeof(char),buff_t.size()*sizeof(char)-1,str);
		sprintf_s(buff,buff_t.size()*sizeof(char),str);
#else
		char* buff=str;
#endif
		 char* token;
		 token = strtok(buff, deli);
		
		list.clear();
		while(token !=NULL)
		{
			list.push_back(token);
			 token = strtok(NULL, ",");
		};
	
}
/*-------------------------------------------------------------------------------------*/
/**
*移除字符串中的空格
*/
/*-------------------------------------------------------------------------------------*/
void TrainImgUseSvm::strRemoveSpace(char *str){
	if(str==NULL)   
		return;  
	bool flag = true;
	while(  *(str) != '\0'){
		if(*(str) == ' ' && flag)   {
			 flag =false;     
		}   

		while (*(str) != ' '){ 
			flag = true;   break;  
		}   str++;   
	}    
}
#if (CV_MAJOR_VERSION==2)&&(CV_MINOR_VERSION==4)&&USE_SVM_2_4_X
/*-------------------------------------------------------------------------------------*/
/**
*
*/
/*-------------------------------------------------------------------------------------*/
CvSVMParams TrainImgUseSvm::GetLinearSVMParams()
{
	 CvSVMParams params;    
	 params.svm_type = CvSVM::C_SVC;    
	 params.kernel_type = 0;    
	 params.term_crit = cvTermCriteria(CV_TERMCRIT_ITER, 1000, FLT_EPSILON);  
	 params.C = 0.01; 
	return params;
}
/*-------------------------------------------------------------------------------------*/
/**
*机器学习高斯内核
*@return 高斯内核
*@note 注意尺度缩放，特征在[-1，1] [0,1]
*/
/*-------------------------------------------------------------------------------------*/
CvSVMParams TrainImgUseSvm::GetGaussianSVMParams()
{
	CvSVMParams param;
	param.svm_type= CvSVM::C_SVC;
	param.kernel_type= RBF;
	param.term_crit=cvTermCriteria(CV_TERMCRIT_EPS, 1000, FLT_EPSILON);
	param.gamma=8;
	param.C=10;
	return param;
}
/*-------------------------------------------------------------------------------------*/
/**
*机器学习多项式内核
*@return 多项式内核
*@note 注意尺度缩放,特征在[-1，1] [0,1]
*/
/*-------------------------------------------------------------------------------------*/
CvSVMParams TrainImgUseSvm::GetPolySVMParams()
{
	CvSVMParams param;
	 param.svm_type=CvSVM::C_SVC;
	 param.kernel_type=POLY;
	 param.term_crit=cvTermCriteria(CV_TERMCRIT_EPS, 1000, FLT_EPSILON);
	 param.degree=4;//最高相次
	 param.gamma=0.5;//1/k
	 param.coef0=0;
	 param.C=1;//0
	return param;
}
/*-------------------------------------------------------------------------------------*/
/*
*检测学习结果的查全率、查准率
*@param [in] svm 支持向量 
*@param [in] dataMatMem 特征矩阵 
*@param [in] resMatMem 特征类别
*@param [in] pathList 特征数量
*/
/*-------------------------------------------------------------------------------------*/
void TrainImgUseSvm::Learning_samples_detection_rate(
	CvSVM& svm ,
	vector<float>& dataMatMem,
	vector<INT32>& resMatMem,
	vector<string>& pathList)
{
	unsigned long Category_0_ALL_N=0;	
	unsigned long Category_1_ALL_N=0;
	unsigned long Category_0_ALL_H=0;	
	unsigned long Category_1_ALL_H=0;
	unsigned long Category_0_Success_HN=0;
	unsigned long Category_1_Success_HN=0;
	CvMat MatOne;	
	int Dim=dataMatMem.size()/resMatMem.size();
	CvMat *MatOneFeature=cvInitMatHeader(&MatOne,1,Dim,CV_32FC1,NULL);
	float *memData=dataMatMem.data();
	for (unsigned long si=0;si<resMatMem.size();si++){
		MatOneFeature->data.fl=&memData[si*Dim];
		int cate_si=svm.predict(MatOneFeature);
		int cate_org=resMatMem[si];
		if (cate_org==0){
			Category_0_ALL_N++;
		}
		if (cate_org==1){
			Category_1_ALL_N++;
		}
		if (cate_si==0){
			Category_0_ALL_H++;
		}
		if (cate_si==1){
			Category_1_ALL_H++;
		}
		if ((cate_si==0)&&(cate_org==0)){
			Category_0_Success_HN++;
		}
		if ((cate_si==1)&&(cate_org==1)){
			Category_1_Success_HN++;
		}
		if ((cate_si==0)&&(cate_org==1)){
#if TRUE
			string fileNameold=pathList[si];
			FileNameSplit fnsp;
			IplImage *img=cvLoadImage(fileNameold.c_str());
			string fileNameneNew="2143124.png";
#if Use_CString&&_MSC_VER
			fnsp.Parse(FileNameSplit::Convertstring2CS(fileNameold));
			fileNameneNew=SVMDetectorxml+"\\POS2NEG\\"+FileNameSplit::ConvertCS2string(fnsp.filefullname);
#endif
		
			
			cvSaveImage(fileNameneNew.c_str(),img);
			cvReleaseImage(&img);
#endif
		}
		if ((cate_si==1)&&(cate_org==0)){
#if TRUE
			string fileNameold=pathList[si];
			FileNameSplit fnsp;
			IplImage *img=cvLoadImage(fileNameold.c_str());
		    string fileNameneNew="agifvauhvau.png";
#if Use_CString&&_MSC_VER
			fnsp.Parse(FileNameSplit::Convertstring2CS(fileNameold));
			fileNameneNew=SVMDetectorxml+"\\NEG2POS\\"+FileNameSplit::ConvertCS2string(fnsp.filefullname);
#endif
		
		
			cvSaveImage(fileNameneNew.c_str(),img);
			cvReleaseImage(&img);
#endif
		}
	}
	float Rate_0_R=1.0*Category_0_Success_HN/Category_0_ALL_N;
	float Rate_1_R=1.0*Category_1_Success_HN/Category_1_ALL_N;
	float Rate_0_P=1.0*Category_0_Success_HN/Category_0_ALL_H;
	float Rate_1_P=1.0*Category_1_Success_HN/Category_1_ALL_H;
	string filename=SVMDetectorxml+"\\RATEdata.xml";
	FileStorage fsw(filename,FileStorage::WRITE);
    if (fsw.isOpened()){
		fsw<<"Rate_0_ChaQuan_R"<<Rate_0_R;
		fsw<<"Rate_1_ChaQuan_R"<<Rate_1_R;
		fsw<<"Rate_0_ChaZhun_P"<<Rate_0_P;
		fsw<<"Rate_1_ChaZhun_P"<<Rate_1_P;
    }else{
		ASSERT(FALSE);
	}





}
#endif
/*-------------------------------------------------------------------------------------*/
/**
*@param [out] data 特征矩阵
*@param [out] res 特征类别
*@param [out] Dim 特征维度
*@param [out] Num 特征数量
*/
/*-------------------------------------------------------------------------------------*/
void TrainImgUseSvm::Read2MemFromXML_SCountD(
	vector<float>& data,
	vector<INT32>& res,
	int& Dim,
	unsigned long& Num)
{
	data.clear();
	res.clear();
	if (posSamplesDir[posSamplesDir.size()-1]=='\\'){
		posSamplesDir.resize(posSamplesDir.size()-1);
	}
	string filename=posSamplesDir;
		tinyxml2::XMLDocument doc;  
		doc.LoadFile(filename.c_str());  
		tinyxml2::XMLError xmlError;

		tinyxml2::XMLElement *scene=doc.RootElement();  
		tinyxml2::XMLElement *surface=scene->FirstChildElement("SvmData");

		while (surface){  
					////////////////////////////////////////////////////////////////////////////
					int filesize=3;
                    Dim=filesize;
					////////////////////////////////////////////////////////////////////////////
					float categoryfeature=0;
					tinyxml2::XMLElement *Category_svmData=surface->FirstChildElement("Category");
					xmlError=Category_svmData->QueryFloatText(&categoryfeature);
					res.push_back(categoryfeature);
					ASSERT(xmlError==XMLError::XML_SUCCESS);
					/////////////////////////////////////////////////////////////////////////////
					ASSERT(Dim!=0);
					tinyxml2::XMLElement *Feature_SumData=surface->FirstChildElement("SumManhattan");
					ASSERT(Feature_SumData!=NULL);
					float F_One_data=0;
					xmlError=Feature_SumData->QueryFloatText(&F_One_data);
					ASSERT(xmlError==XMLError::XML_SUCCESS);
					data.push_back(F_One_data/(180*100));
					/////////////////////////////////////////////////////////////////////////////
					tinyxml2::XMLElement *Feature_CountData=surface->FirstChildElement("ZeroCount");
					ASSERT(Feature_CountData!=NULL);
					xmlError=Feature_CountData->QueryFloatText(&F_One_data);
					ASSERT(xmlError==XMLError::XML_SUCCESS);
					data.push_back(F_One_data/180);
					/////////////////////////////////////////////////////////////////////////////
					tinyxml2::XMLElement *Feature_DensityData=surface->FirstChildElement("LineNumDensity");
					ASSERT(Feature_DensityData!=NULL);
					xmlError=Feature_DensityData->QueryFloatText(&F_One_data);
					ASSERT(xmlError==XMLError::XML_SUCCESS);
					data.push_back(F_One_data/300);
					/////////////////////////////////////////////////////////////////////////////
					surface=surface->NextSiblingElement();  
			}
					Num=res.size();
}
/*-------------------------------------------------------------------------------------*/
/**
*@param [out] data 特征矩阵
*@param [out] res 特征类别
*@param [out] Dim 特征维度
*@param [out] Num 特征数量
*/
/*-------------------------------------------------------------------------------------*/
void TrainImgUseSvm::Read2MemFromXML_SCount(
	vector<float>& data,
	vector<INT32>& res,
	int& Dim,
	unsigned long& Num)
{
	data.clear();
	res.clear();
	if (posSamplesDir[posSamplesDir.size()-1]=='\\'){
		posSamplesDir.resize(posSamplesDir.size()-1);
	}
	string filename=posSamplesDir;
		tinyxml2::XMLDocument doc;  
		doc.LoadFile(filename.c_str());  
		tinyxml2::XMLError xmlError;

		tinyxml2::XMLElement *scene=doc.RootElement();  
		tinyxml2::XMLElement *surface=scene->FirstChildElement("SvmData");

		while (surface){  
					////////////////////////////////////////////////////////////////////////////
					int filesize=2;
                    Dim=filesize;
					////////////////////////////////////////////////////////////////////////////
					float categoryfeature=0;
					tinyxml2::XMLElement *Category_svmData=surface->FirstChildElement("Category");
					xmlError=Category_svmData->QueryFloatText(&categoryfeature);
					res.push_back(categoryfeature);
					ASSERT(xmlError==XMLError::XML_SUCCESS);
					/////////////////////////////////////////////////////////////////////////////
					ASSERT(Dim!=0);
					tinyxml2::XMLElement *Feature_SumData=surface->FirstChildElement("SumManhattan");
					ASSERT(Feature_SumData!=NULL);
					float F_One_data=0;
					xmlError=Feature_SumData->QueryFloatText(&F_One_data);
					ASSERT(xmlError==XMLError::XML_SUCCESS);
					data.push_back(F_One_data/(180*100));
					/////////////////////////////////////////////////////////////////////////////
					tinyxml2::XMLElement *Feature_CountData=surface->FirstChildElement("ZeroCount");
					ASSERT(Feature_CountData!=NULL);
					xmlError=Feature_CountData->QueryFloatText(&F_One_data);
					ASSERT(xmlError==XMLError::XML_SUCCESS);
					data.push_back(F_One_data/180);
					/////////////////////////////////////////////////////////////////////////////
					surface=surface->NextSiblingElement();
			}
					Num=res.size();
}
/*-------------------------------------------------------------------------------------*/
/**
*@param [out] data 特征矩阵
*@param [out] res 特征类别
*@param [out] Dim 特征维度
*@param [out] Num 特征数量
*/
/*-------------------------------------------------------------------------------------*/
void TrainImgUseSvm::Read2MemFromXML_SDensityC(
	vector<float>& data,
	vector<INT32>& res,
	int& Dim,
	unsigned long& Num)
{
	data.clear();
	res.clear();
	if (posSamplesDir[posSamplesDir.size()-1]=='\\'){
		posSamplesDir.resize(posSamplesDir.size()-1);
	}
	string filename=posSamplesDir;
		tinyxml2::XMLDocument doc;  
		doc.LoadFile(filename.c_str());  
		tinyxml2::XMLError xmlError;

		tinyxml2::XMLElement *scene=doc.RootElement();  
		tinyxml2::XMLElement *surface=scene->FirstChildElement("SvmData");

		while (surface){  
					////////////////////////////////////////////////////////////////////////////
					int filesize=3;
                    Dim=filesize;
					////////////////////////////////////////////////////////////////////////////
					float categoryfeature=0;
					tinyxml2::XMLElement *Category_svmData=surface->FirstChildElement("Category");
					xmlError=Category_svmData->QueryFloatText(&categoryfeature);
					res.push_back(categoryfeature);
					ASSERT(xmlError==XMLError::XML_SUCCESS);
					/////////////////////////////////////////////////////////////////////////////
					ASSERT(Dim!=0);
					tinyxml2::XMLElement *Feature_SumData=surface->FirstChildElement("SumManhattan");
					ASSERT(Feature_SumData!=NULL);
					float F_One_data=0;
					xmlError=Feature_SumData->QueryFloatText(&F_One_data);
					ASSERT(xmlError==XMLError::XML_SUCCESS);
					data.push_back(F_One_data/(180*100));
					/////////////////////////////////////////////////////////////////////////////
					tinyxml2::XMLElement *Feature_CountData=surface->FirstChildElement("LineNumDensity");
					ASSERT(Feature_CountData!=NULL);
					xmlError=Feature_CountData->QueryFloatText(&F_One_data);
					ASSERT(xmlError==XMLError::XML_SUCCESS);
					data.push_back(F_One_data/300);
					/////////////////////////////////////////////////////////////////////////////
					tinyxml2::XMLElement *Feature_DensityData=surface->FirstChildElement("Color");
					ASSERT(Feature_DensityData!=NULL);
					xmlError=Feature_DensityData->QueryFloatText(&F_One_data);
					ASSERT(xmlError==XMLError::XML_SUCCESS);
					data.push_back((F_One_data+180.0)/360);
					/////////////////////////////////////////////////////////////////////////////
					surface=surface->NextSiblingElement();  
			}
					Num=res.size();
}
/*-------------------------------------------------------------------------------------*/
/**
*@param [out] data 特征矩阵
*@param [out] res 特征类别
*@param [out] Dim 特征维度
*@param [out] Num 特征数量
*/
/*-------------------------------------------------------------------------------------*/
void TrainImgUseSvm::Read2MemFromXML_SDensity(
	vector<float>& data,
	vector<INT32>& res,
	int& Dim,
	unsigned long& Num)
{
	data.clear();
	res.clear();
	if (posSamplesDir[posSamplesDir.size()-1]=='\\'){
		posSamplesDir.resize(posSamplesDir.size()-1);
	}
	string filename=posSamplesDir;
		tinyxml2::XMLDocument doc;  
		doc.LoadFile(filename.c_str());  
		tinyxml2::XMLError xmlError;

		tinyxml2::XMLElement *scene=doc.RootElement();  
		tinyxml2::XMLElement *surface=scene->FirstChildElement("SvmData");

		while (surface){  
					////////////////////////////////////////////////////////////////////////////
					int filesize=2;
                    Dim=filesize;
					////////////////////////////////////////////////////////////////////////////
					float categoryfeature=0;
					tinyxml2::XMLElement *Category_svmData=surface->FirstChildElement("Category");
					xmlError=Category_svmData->QueryFloatText(&categoryfeature);
					res.push_back(categoryfeature);
					ASSERT(xmlError==XMLError::XML_SUCCESS);
					/////////////////////////////////////////////////////////////////////////////
					ASSERT(Dim!=0);
					tinyxml2::XMLElement *Feature_SumData=surface->FirstChildElement("SumManhattan");
					ASSERT(Feature_SumData!=NULL);
					float F_One_data=0;
					xmlError=Feature_SumData->QueryFloatText(&F_One_data);
					ASSERT(xmlError==XMLError::XML_SUCCESS);
					data.push_back(F_One_data/(180*100));
					/////////////////////////////////////////////////////////////////////////////
					tinyxml2::XMLElement *Feature_CountData=surface->FirstChildElement("LineNumDensity");
					ASSERT(Feature_CountData!=NULL);
					xmlError=Feature_CountData->QueryFloatText(&F_One_data);
					ASSERT(xmlError==XMLError::XML_SUCCESS);
					data.push_back(F_One_data/300);
					/////////////////////////////////////////////////////////////////////////////
					surface=surface->NextSiblingElement();  
			}
					Num=res.size();
}
/*-------------------------------------------------------------------------------------*/
/**
*@param [out] data 特征矩阵
*@param [out] res 特征类别
*@param [out] Dim 特征维度
*@param [out] Num 特征数量
*/
/*-------------------------------------------------------------------------------------*/
void TrainImgUseSvm::Read2MemFromXML_SCountColor(
	vector<float>& data,
	vector<INT32>& res,
	int& Dim,
	unsigned long& Num)
{
	data.clear();
	res.clear();
	if (posSamplesDir[posSamplesDir.size()-1]=='\\'){
		posSamplesDir.resize(posSamplesDir.size()-1);
	}
	string filename=posSamplesDir;
		tinyxml2::XMLDocument doc;  
		doc.LoadFile(filename.c_str());  
		tinyxml2::XMLError xmlError;

		tinyxml2::XMLElement *scene=doc.RootElement();  
		tinyxml2::XMLElement *surface=scene->FirstChildElement("SvmData");

		while (surface){  
					////////////////////////////////////////////////////////////////////////////
					int filesize=3;
                    Dim=filesize;
					////////////////////////////////////////////////////////////////////////////
					float categoryfeature=0;
					tinyxml2::XMLElement *Category_svmData=surface->FirstChildElement("Category");
					xmlError=Category_svmData->QueryFloatText(&categoryfeature);
					res.push_back(categoryfeature);
					ASSERT(xmlError==XMLError::XML_SUCCESS);
					/////////////////////////////////////////////////////////////////////////////
					ASSERT(Dim!=0);
					tinyxml2::XMLElement *Feature_SumData=surface->FirstChildElement("SumManhattan");
					ASSERT(Feature_SumData!=NULL);
					float F_One_data=0;
					xmlError=Feature_SumData->QueryFloatText(&F_One_data);
					ASSERT(xmlError==XMLError::XML_SUCCESS);
					data.push_back(F_One_data/(180*100));
					/////////////////////////////////////////////////////////////////////////////
					tinyxml2::XMLElement *Feature_CountData=surface->FirstChildElement("ZeroCount");
					ASSERT(Feature_CountData!=NULL);
					xmlError=Feature_CountData->QueryFloatText(&F_One_data);
					ASSERT(xmlError==XMLError::XML_SUCCESS);
					data.push_back(F_One_data/180);
					/////////////////////////////////////////////////////////////////////////////
					tinyxml2::XMLElement *Feature_DensityData=surface->FirstChildElement("Color");
					ASSERT(Feature_DensityData!=NULL);
					xmlError=Feature_DensityData->QueryFloatText(&F_One_data);
					ASSERT(xmlError==XMLError::XML_SUCCESS);
					data.push_back((F_One_data+180.0)/360.0);
					/////////////////////////////////////////////////////////////////////////////
					surface=surface->NextSiblingElement();  
			}
					Num=res.size();
}
/*-------------------------------------------------------------------------------------*/
/**
*@param [out] data 特征矩阵
*@param [out] res 特征类别
*@param [out] Dim 特征维度
*@param [out] Num 特征数量
*/
/*-------------------------------------------------------------------------------------*/
void TrainImgUseSvm::Read2MemFromXML_CountDColor(
	vector<float>& data,
	vector<INT32>& res,
	int& Dim,
	unsigned long& Num)
{
	data.clear();
	res.clear();
	if (posSamplesDir[posSamplesDir.size()-1]=='\\'){
		posSamplesDir.resize(posSamplesDir.size()-1);
	}
	string filename=posSamplesDir;
		tinyxml2::XMLDocument doc;  
		doc.LoadFile(filename.c_str());  
		tinyxml2::XMLError xmlError;

		tinyxml2::XMLElement *scene=doc.RootElement();  
		tinyxml2::XMLElement *surface=scene->FirstChildElement("SvmData");

		while (surface){  
					////////////////////////////////////////////////////////////////////////////
					int filesize=3;
                    Dim=filesize;
					////////////////////////////////////////////////////////////////////////////
					float categoryfeature=0;
					tinyxml2::XMLElement *Category_svmData=surface->FirstChildElement("Category");
					xmlError=Category_svmData->QueryFloatText(&categoryfeature);
					res.push_back(categoryfeature);
					ASSERT(xmlError==XMLError::XML_SUCCESS);
					/////////////////////////////////////////////////////////////////////////////
					ASSERT(Dim!=0);
					tinyxml2::XMLElement *Feature_SumData=surface->FirstChildElement("ZeroCount");
					ASSERT(Feature_SumData!=NULL);
					float F_One_data=0;
					xmlError=Feature_SumData->QueryFloatText(&F_One_data);
					ASSERT(xmlError==XMLError::XML_SUCCESS);
					data.push_back(F_One_data/180);
					/////////////////////////////////////////////////////////////////////////////
					tinyxml2::XMLElement *Feature_CountData=surface->FirstChildElement("LineNumDensity");
					ASSERT(Feature_CountData!=NULL);
					xmlError=Feature_CountData->QueryFloatText(&F_One_data);
					ASSERT(xmlError==XMLError::XML_SUCCESS);
					data.push_back(F_One_data/300);
					/////////////////////////////////////////////////////////////////////////////
					tinyxml2::XMLElement *Feature_DensityData=surface->FirstChildElement("Color");
					ASSERT(Feature_DensityData!=NULL);
					xmlError=Feature_DensityData->QueryFloatText(&F_One_data);
					ASSERT(xmlError==XMLError::XML_SUCCESS);
					data.push_back((F_One_data+180.0)/360);
					/////////////////////////////////////////////////////////////////////////////
					surface=surface->NextSiblingElement();  
			}
					Num=res.size();
}
/*--------------------------------------------------------------------------------------*/
/**
*
*@param [out] data 特征矩阵
*@param [out] res 特征类别
*@param [out] Dim 特征维度
*@param [out] Num 特征数量
*/
/*--------------------------------------------------------------------------------------*/
void TrainImgUseSvm::Read2MemFromXML_Sum_Count_Density_Color(
	vector<float>& data,
	vector<INT32>& res,
	int& Dim,
	unsigned long& Num)
{
	data.clear();
	res.clear();
	if (posSamplesDir[posSamplesDir.size()-1]=='\\'){
		posSamplesDir.resize(posSamplesDir.size()-1);
	}
	string filename=posSamplesDir;
	tinyxml2::XMLDocument doc;  
	doc.LoadFile(filename.c_str());  
	tinyxml2::XMLError xmlError;

	tinyxml2::XMLElement *scene=doc.RootElement();  
	tinyxml2::XMLElement *surface=scene->FirstChildElement("SvmData");

	while (surface){  
		////////////////////////////////////////////////////////////////////////////
		int filesize=4;
		Dim=filesize;
		////////////////////////////////////////////////////////////////////////////
		float categoryfeature=0;
		tinyxml2::XMLElement *Category_svmData=surface->FirstChildElement("Category");
		xmlError=Category_svmData->QueryFloatText(&categoryfeature);
		res.push_back(categoryfeature);
		ASSERT(xmlError==XMLError::XML_SUCCESS);
		/////////////////////////////////////////////////////////////////////////////
		ASSERT(Dim!=0);
		tinyxml2::XMLElement *Feature_SumData=surface->FirstChildElement("SumManhattan");
		ASSERT(Feature_SumData!=NULL);
		float F_One_data=0;
		xmlError=Feature_SumData->QueryFloatText(&F_One_data);
		ASSERT(xmlError==XMLError::XML_SUCCESS);
		data.push_back(F_One_data/(180*100));
		/////////////////////////////////////////////////////////////////////////////
		tinyxml2::XMLElement *Feature_CountData=surface->FirstChildElement("ZeroCount");
		ASSERT(Feature_CountData!=NULL);
		xmlError=Feature_CountData->QueryFloatText(&F_One_data);
		ASSERT(xmlError==XMLError::XML_SUCCESS);
		data.push_back(F_One_data/180);
		/////////////////////////////////////////////////////////////////////////////
		tinyxml2::XMLElement *Feature_DensityData=surface->FirstChildElement("LineNumDensity");
		ASSERT(Feature_DensityData!=NULL);
		xmlError=Feature_DensityData->QueryFloatText(&F_One_data);
		ASSERT(xmlError==XMLError::XML_SUCCESS);
		data.push_back(F_One_data/300);
		/////////////////////////////////////////////////////////////////////////////
		tinyxml2::XMLElement *Feature_Color=surface->FirstChildElement("Color");
		ASSERT(Feature_Color!=NULL);
		xmlError=Feature_Color->QueryFloatText(&F_One_data);
		ASSERT(xmlError==XMLError::XML_SUCCESS);
		data.push_back((F_One_data+180.0)/360);
		/////////////////////////////////////////////////////////////////////////////
		surface=surface->NextSiblingElement();  
	}
	Num=res.size();
}
/*-------------------------------------------------------------------------------------*/
/**
*求和 颜色
*@param [out] data 特征矩阵
*@param [out] res 特征类别
*@param [out] Dim 特征维度
*@param [out] Num 特征数量
*/
/*-------------------------------------------------------------------------------------*/
void TrainImgUseSvm::Read2MemFromXML_SColor(
	vector<float>& data,
	vector<INT32>& res,
	int& Dim,
	unsigned long& Num)
{
		data.clear();
	res.clear();
	if (posSamplesDir[posSamplesDir.size()-1]=='\\'){
		posSamplesDir.resize(posSamplesDir.size()-1);
	}
	string filename=posSamplesDir;
		tinyxml2::XMLDocument doc;  
		doc.LoadFile(filename.c_str());  
		tinyxml2::XMLError xmlError;

		tinyxml2::XMLElement *scene=doc.RootElement();  
		tinyxml2::XMLElement *surface=scene->FirstChildElement("SvmData");

		while (surface){  
					////////////////////////////////////////////////////////////////////////////
					int filesize=2;
                    Dim=filesize;
					////////////////////////////////////////////////////////////////////////////
					float categoryfeature=0;
					tinyxml2::XMLElement *Category_svmData=surface->FirstChildElement("Category");
					xmlError=Category_svmData->QueryFloatText(&categoryfeature);
					res.push_back(categoryfeature);
					ASSERT(xmlError==XMLError::XML_SUCCESS);
					/////////////////////////////////////////////////////////////////////////////
					ASSERT(Dim!=0);
					tinyxml2::XMLElement *Feature_SumData=surface->FirstChildElement("SumManhattan");
					ASSERT(Feature_SumData!=NULL);
					float F_One_data=0;
					xmlError=Feature_SumData->QueryFloatText(&F_One_data);
					ASSERT(xmlError==XMLError::XML_SUCCESS);
					data.push_back(F_One_data/(180*100));
					/////////////////////////////////////////////////////////////////////////////
					tinyxml2::XMLElement *Feature_CountData=surface->FirstChildElement("Color");
					ASSERT(Feature_CountData!=NULL);
					xmlError=Feature_CountData->QueryFloatText(&F_One_data);
					ASSERT(xmlError==XMLError::XML_SUCCESS);
					data.push_back((F_One_data+180.0)/360.0);
					/////////////////////////////////////////////////////////////////////////////
					surface=surface->NextSiblingElement();
			}
					Num=res.size();
}

/*-------------------------------------------------------------------------------------*/
/**
*@param [out] data 特征矩阵
*@param [out] res 特征类别
*@param [out] Dim 特征维度
*@param [out] Num 特征数量
*/
/*-------------------------------------------------------------------------------------*/
void TrainImgUseSvm::Read2MemFromXML_Count_Density(
	vector<float>& data,
	vector<INT32>& res,
	int& Dim,
	unsigned long& Num)
{
	data.clear();
	res.clear();
	if (posSamplesDir[posSamplesDir.size()-1]=='\\'){
		posSamplesDir.resize(posSamplesDir.size()-1);
	}
	string filename=posSamplesDir;
		tinyxml2::XMLDocument doc;  
		doc.LoadFile(filename.c_str());  
		tinyxml2::XMLError xmlError;

		tinyxml2::XMLElement *scene=doc.RootElement();  
		tinyxml2::XMLElement *surface=scene->FirstChildElement("SvmData");

		while (surface){  
					////////////////////////////////////////////////////////////////////////////
					int filesize=2;
                    Dim=filesize;
					float F_One_data=0;
					////////////////////////////////////////////////////////////////////////////
					float categoryfeature=0;
					tinyxml2::XMLElement *Category_svmData=surface->FirstChildElement("Category");
					xmlError=Category_svmData->QueryFloatText(&categoryfeature);
					res.push_back(categoryfeature);
					ASSERT(xmlError==XMLError::XML_SUCCESS);
					/////////////////////////////////////////////////////////////////////////////
					tinyxml2::XMLElement *Feature_CountData=surface->FirstChildElement("ZeroCount");
					ASSERT(Feature_CountData!=NULL);
					xmlError=Feature_CountData->QueryFloatText(&F_One_data);
					ASSERT(xmlError==XMLError::XML_SUCCESS);
					data.push_back(F_One_data/180);
					/////////////////////////////////////////////////////////////////////////////
					Feature_CountData=surface->FirstChildElement("LineNumDensity");
					ASSERT(Feature_CountData!=NULL);
					xmlError=Feature_CountData->QueryFloatText(&F_One_data);
					ASSERT(xmlError==XMLError::XML_SUCCESS);
					data.push_back(F_One_data/300);
					/////////////////////////////////////////////////////////////////////////////
					surface=surface->NextSiblingElement();  
			}
					Num=res.size();
}

/*-------------------------------------------------------------------------------------*/
/**
*从XML文件读取 计数 颜色
*@param [out] data 特征矩阵
*@param [out] res 特征类别
*@param [out] Dim 特征维度
*@param [out] Num 特征数量
*/
/*-------------------------------------------------------------------------------------*/
void TrainImgUseSvm::Read2MemFromXML_Count_Color(
	vector<float>& data,
	vector<INT32>& res,
	int& Dim,
	unsigned long& Num)
{
	data.clear();
	res.clear();
	if (posSamplesDir[posSamplesDir.size()-1]=='\\'){
		posSamplesDir.resize(posSamplesDir.size()-1);
	}
	string filename=posSamplesDir;
	tinyxml2::XMLDocument doc;  
	doc.LoadFile(filename.c_str());  
	tinyxml2::XMLError xmlError;

	tinyxml2::XMLElement *scene=doc.RootElement();  
	tinyxml2::XMLElement *surface=scene->FirstChildElement("SvmData");

	while (surface){
		////////////////////////////////////////////////////////////////////////////
		int filesize=2;
		Dim=filesize;
		float F_One_data=0;
		////////////////////////////////////////////////////////////////////////////
		float categoryfeature=0;
		tinyxml2::XMLElement *Category_svmData=surface->FirstChildElement("Category");
		xmlError=Category_svmData->QueryFloatText(&categoryfeature);
		res.push_back(categoryfeature);
		ASSERT(xmlError==XMLError::XML_SUCCESS);
		/////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////
		tinyxml2::XMLElement *Feature_CountData=surface->FirstChildElement("ZeroCount");
		ASSERT(Feature_CountData!=NULL);
		xmlError=Feature_CountData->QueryFloatText(&F_One_data);
		ASSERT(xmlError==XMLError::XML_SUCCESS);
		data.push_back(F_One_data/180);
		/////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////
		tinyxml2::XMLElement *Feature_DensityData=surface->FirstChildElement("Color");
		ASSERT(Feature_DensityData!=NULL);
		xmlError=Feature_DensityData->QueryFloatText(&F_One_data);
		ASSERT(xmlError==XMLError::XML_SUCCESS);
		data.push_back((F_One_data+180.0)/360);
		//////////////////////////////////////////////////////////////////////////////
		surface=surface->NextSiblingElement();  
	}
	Num=res.size();
}
/*-------------------------------------------------------------------------------------*/
/**
*从XML文件读取 密度 颜色
*@param [out] data 特征矩阵
*@param [out] res 特征类别
*@param [out] Dim 特征维度
*@param [out] Num 特征数量
*/
/*-------------------------------------------------------------------------------------*/
void TrainImgUseSvm::Read2MemFromXML_Density_Color(
	vector<float>& data,
	vector<INT32>& res,
	int& Dim,
	unsigned long& Num)
{
	data.clear();
	res.clear();
	if (posSamplesDir[posSamplesDir.size()-1]=='\\'){
		posSamplesDir.resize(posSamplesDir.size()-1);
	}
	string filename=posSamplesDir;
	tinyxml2::XMLDocument doc;  
	doc.LoadFile(filename.c_str());  
	tinyxml2::XMLError xmlError;

	tinyxml2::XMLElement *scene=doc.RootElement();  
	tinyxml2::XMLElement *surface=scene->FirstChildElement("SvmData");

	while (surface){  
		////////////////////////////////////////////////////////////////////////////
		int filesize=2;
		Dim=filesize;
		float F_One_data=0;
		////////////////////////////////////////////////////////////////////////////
		float categoryfeature=0;
		tinyxml2::XMLElement *Category_svmData=surface->FirstChildElement("Category");
		xmlError=Category_svmData->QueryFloatText(&categoryfeature);
		res.push_back(categoryfeature);
		ASSERT(xmlError==XMLError::XML_SUCCESS);
		/////////////////////////////////////////////////////////////////////////////
		tinyxml2::XMLElement *Feature_CountData=surface->FirstChildElement("LineNumDensity");
		ASSERT(Feature_CountData!=NULL);
		xmlError=Feature_CountData->QueryFloatText(&F_One_data);
		ASSERT(xmlError==XMLError::XML_SUCCESS);
		data.push_back(F_One_data/300);
		/////////////////////////////////////////////////////////////////////////////
		tinyxml2::XMLElement *Feature_DensityData=surface->FirstChildElement("Color");
		ASSERT(Feature_DensityData!=NULL);
		xmlError=Feature_DensityData->QueryFloatText(&F_One_data);
		ASSERT(xmlError==XMLError::XML_SUCCESS);
		data.push_back((F_One_data+180.0)/360);
		//////////////////////////////////////////////////////////////////////////////
		surface=surface->NextSiblingElement();  
	}
	Num=res.size();
}
/*-------------------------------------------------------------------------------------*/
/**
*从XML文件读取数据
*
*@param [out] dataMatMem 机器学习内存数据
*@param [out] resMatMem 每一个特征向量的分类
*@param [out] FeatureDim 特征的维度
*@param [out] samplenum 文件数目
*@param [out] pathList 文件列表
*
*/
/*-------------------------------------------------------------------------------------*/
void  TrainImgUseSvm::ReadTrainDataFromXML(
	vector<float>& dataMatMem,
	vector<INT32>& resMatMem,
	int& FeatureDim,
	unsigned long& samplenum,
	vector<string>& pathList)
{
	if(Dimensions_USE==Normal){
		Read2MemFromXML(dataMatMem,resMatMem,FeatureDim,samplenum);
	}else if(Dimensions_USE==Sum_Count_Density){
		Read2MemFromXML_SCountD(dataMatMem,resMatMem,FeatureDim,samplenum);
	}else if(Dimensions_USE==Sum_Density_Color){
		Read2MemFromXML_SDensityC(dataMatMem,resMatMem,FeatureDim,samplenum);
	}else if(Dimensions_USE==Sum_Count_Color){
		Read2MemFromXML_SCountColor(dataMatMem,resMatMem,FeatureDim,samplenum);
	}else if(Dimensions_USE==Count_Density_Color){
		Read2MemFromXML_CountDColor(dataMatMem,resMatMem,FeatureDim,samplenum);
	}else if(Dimensions_USE==Sum_Count){
		Read2MemFromXML_SCount(dataMatMem,resMatMem,FeatureDim,samplenum);
	}else if(Dimensions_USE==Sum_Density){
		Read2MemFromXML_SDensity(dataMatMem,resMatMem,FeatureDim,samplenum);
	}else if(Dimensions_USE==Sum_Color){
		Read2MemFromXML_SColor(dataMatMem,resMatMem,FeatureDim,samplenum);
	}else if(Dimensions_USE==Count_Density){
		Read2MemFromXML_Count_Density(dataMatMem,resMatMem,FeatureDim,samplenum);
	}else if(Dimensions_USE==Count_Color){
		Read2MemFromXML_Count_Color(dataMatMem,resMatMem,FeatureDim,samplenum);
	}else if(Dimensions_USE==Density_Color){
		Read2MemFromXML_Density_Color(dataMatMem,resMatMem,FeatureDim,samplenum);
	}else if (Dimensions_USE==Sum_Count_Density_Color){
		Read2MemFromXML_Sum_Count_Density_Color(dataMatMem,resMatMem,FeatureDim,samplenum);
	}
	ReadFileName2PathList(pathList);
	ASSERT(samplenum!=0);
}
/*-------------------------------------------------------------------------------------*/
/**
*将文件读入PathList\n
*从XML文件读入
*@param[in] pathlist  
*
*/
/*-------------------------------------------------------------------------------------*/
void TrainImgUseSvm::ReadFileName2PathList(vector<string>& pathList)
{
	pathList.clear();
	/////////////////////////////////////////////////////////////////////
	if (posSamplesDir[posSamplesDir.size()-1]=='\\'){
		posSamplesDir.resize(posSamplesDir.size()-1);
	}
	string filename=posSamplesDir;
	tinyxml2::XMLDocument doc;  
	doc.LoadFile(filename.c_str());  
	tinyxml2::XMLError xmlError;

	tinyxml2::XMLElement *scene=doc.RootElement();  
	tinyxml2::XMLElement *surface=scene->FirstChildElement("SvmData");

	while (surface){
		////////////////////////////////////////////////////////////////////////////
		float categoryfeature=0;
		tinyxml2::XMLElement *Category_svmData=surface->FirstChildElement("FIleName");
		
		string filename=Category_svmData->GetText();
		pathList.push_back(filename);
		surface=surface->NextSiblingElement();  
	}

}
/*-------------------------------------------------------------------------------------*/
/**
*
*/
/*-------------------------------------------------------------------------------------*/

#if (CV_MAJOR_VERSION==2)&&(CV_MINOR_VERSION==4)&&USE_SVM_2_4_X
/*-------------------------------------------------------------------------------------*/
/**
*将学习模型保存到三维坐标系
*/
/*-------------------------------------------------------------------------------------*/
void TrainImgUseSvm::GetLearnModule_Count_Density(CvSVM& svm)
{
	CvMat MatOne;
	int Dim=2;
	vector<float> SCountD;
	vector<Svm_SCDC> data_all;

	for(float counti=0;counti<1;counti+=0.02){
		for(float di=0;di<1;di+=0.02){
			{
				SCountD.clear();
				SCountD.push_back(counti);
				SCountD.push_back(di);
				CvMat *MatOneFeature=cvInitMatHeader(&MatOne,1,Dim,CV_32FC1, SCountD.data());	
				int cate_si=svm.predict(MatOneFeature);
				data_all.push_back(Svm_SCDC(cate_si,0,counti,di,0));

			}
		}
	}

#if TRUE 
	string filepath=SVMDetectorxml+"\\"+"SvmDataModule.xml";
	FileStorage fs(filepath, FileStorage::APPEND);
	for(int i=0;i<data_all.size();i++){
		if (fs.isOpened()){
			fs<<"SvmData";
			fs<<"{";
			fs<<"Category"<<data_all[i].Category;
			fs<<"SumManhattan"<<data_all[i].SumManhattan;
			fs<<"ZeroCount"<<data_all[i].ZeroCount;
			fs<<"LineNumDensity"<<data_all[i].LineNumDensity;
			fs<<"Color"<<data_all[i].Color;
			fs<<"}";
		}else{
			ASSERT(0);
		}
	}
#endif
}
/*-------------------------------------------------------------------------------------*/
/**
*将学习模型保存到三维坐标系
*/
/*-------------------------------------------------------------------------------------*/
void TrainImgUseSvm::GetLearnModule_Count_Color(CvSVM& svm)
{
	CvMat MatOne;
	int Dim=2;
	vector<float> SCountD;
	vector<Svm_SCDC> data_all;

	for(float counti=0;counti<1;counti+=0.02){
		for(float colori=0;colori<1;colori+=0.02){
			{
				SCountD.clear();
				SCountD.push_back(counti);
				SCountD.push_back(colori);
				CvMat *MatOneFeature=cvInitMatHeader(&MatOne,1,Dim,CV_32FC1, SCountD.data());	
				int cate_si=svm.predict(MatOneFeature);
				data_all.push_back(Svm_SCDC(cate_si,0,counti,0,colori));

			}
		}
	}

#if TRUE 
	string filepath=SVMDetectorxml+"\\"+"SvmDataModule.xml";
	FileStorage fs(filepath, FileStorage::APPEND);
	for(int i=0;i<data_all.size();i++){
		if (fs.isOpened()){
			fs<<"SvmData";
			fs<<"{";
			fs<<"Category"<<data_all[i].Category;
			fs<<"SumManhattan"<<data_all[i].SumManhattan;
			fs<<"ZeroCount"<<data_all[i].ZeroCount;
			fs<<"LineNumDensity"<<data_all[i].LineNumDensity;
			fs<<"Color"<<data_all[i].Color;
			fs<<"}";
		}else{
			ASSERT(0);
		}
	}
#endif
}
/*-------------------------------------------------------------------------------------*/
/**
*将学习模型保存到三维坐标系
*/
/*-------------------------------------------------------------------------------------*/
void TrainImgUseSvm::GetLearnModule_Density_Color(CvSVM& svm)
{
	CvMat MatOne;
	int Dim=2;
	vector<float> SCountD;
	vector<Svm_SCDC> data_all;

	for(float di=0;di<1;di+=0.02){
		for(float ci=0;ci<1;ci+=0.02){
			{
				SCountD.clear();
				SCountD.push_back(di);
				SCountD.push_back(ci);
				CvMat *MatOneFeature=cvInitMatHeader(&MatOne,1,Dim,CV_32FC1, SCountD.data());	
				int cate_si=svm.predict(MatOneFeature);
				data_all.push_back(Svm_SCDC(cate_si,0,0,di,ci));
			}
		}
	}

#if TRUE 
	string filepath=SVMDetectorxml+"\\"+"SvmDataModule.xml";
	FileStorage fs(filepath, FileStorage::APPEND);
	for(int i=0;i<data_all.size();i++){
		if (fs.isOpened()){
			fs<<"SvmData";
			fs<<"{";
			fs<<"Category"<<data_all[i].Category;
			fs<<"SumManhattan"<<data_all[i].SumManhattan;
			fs<<"ZeroCount"<<data_all[i].ZeroCount;
			fs<<"LineNumDensity"<<data_all[i].LineNumDensity;
			fs<<"Color"<<data_all[i].Color;
			fs<<"}";
		}else{
			ASSERT(0);
		}
	}
#endif
}
/*-------------------------------------------------------------------------------------*/
/**
*将学习模型保存到三维坐标系
*/
/*-------------------------------------------------------------------------------------*/
void TrainImgUseSvm::GetLearnModule_SCountD(CvSVM& svm)
{
	CvMat MatOne;
	int Dim=3;
	vector<float> SCountD;
	vector<Svm_SCDC> data_all;
	for(float si=0;si<1;si+=0.04){
		for(float counti=0;counti<1;counti+=0.04){
			for(float di=0;di<1;di+=0.04){
				SCountD.clear();
				SCountD.push_back(si);
				SCountD.push_back(counti);
				SCountD.push_back(di);
				CvMat *MatOneFeature=cvInitMatHeader(&MatOne,1,Dim,CV_32FC1, SCountD.data());	
				int cate_si=svm.predict(MatOneFeature);
				data_all.push_back(Svm_SCDC(cate_si,si,counti,di,0));

			}
		}
	}

#if TRUE 
	string filepath=SVMDetectorxml+"\\"+"SvmDataModule.xml";
	FileStorage fs(filepath, FileStorage::APPEND);
	for(int i=0;i<data_all.size();i++){	
		if (fs.isOpened()){
			fs<<"SvmData";
			fs<<"{";
			fs<<"Category"<<data_all[i].Category;
			fs<<"SumManhattan"<<data_all[i].SumManhattan;
			fs<<"ZeroCount"<<data_all[i].ZeroCount;
			fs<<"LineNumDensity"<<data_all[i].LineNumDensity;
			fs<<"Color"<<data_all[i].Color;
			fs<<"}";
		}else{
			ASSERT(0);
		}
	}
#endif
}
/*-------------------------------------------------------------------------------------*/
/**
*将学习模型保存到三维坐标系
*/
/*-------------------------------------------------------------------------------------*/
void TrainImgUseSvm::GetLearnModule_SDensityC(CvSVM& svm)
{
	CvMat MatOne;
	int Dim=3;
	vector<float> SCountD;
	vector<Svm_SCDC> data_all;
	for(float si=0;si<1;si+=0.04){
		for(float di=0;di<1;di+=0.04){
			for(float ci=0;ci<1;ci+=0.04){
				SCountD.clear();
				SCountD.push_back(si);
				SCountD.push_back(di);
				SCountD.push_back(ci);
				CvMat *MatOneFeature=cvInitMatHeader(&MatOne,1,Dim,CV_32FC1, SCountD.data());	
				int cate_si=svm.predict(MatOneFeature);
				data_all.push_back(Svm_SCDC(cate_si,si,0,di,ci));

			}
		}
	}

#if TRUE 
	string filepath=SVMDetectorxml+"\\"+"SvmDataModule.xml";
	FileStorage fs(filepath, FileStorage::APPEND);
	for(int i=0;i<data_all.size();i++){	
		if (fs.isOpened()){
			fs<<"SvmData";
			fs<<"{";
			fs<<"Category"<<data_all[i].Category;
			fs<<"SumManhattan"<<data_all[i].SumManhattan;
			fs<<"ZeroCount"<<data_all[i].ZeroCount;
			fs<<"LineNumDensity"<<data_all[i].LineNumDensity;
			fs<<"Color"<<data_all[i].Color;
			fs<<"}";
		}else{
			ASSERT(0);
		}
	}
#endif
}
/*-------------------------------------------------------------------------------------*/
/**
*将学习模型保存到三维坐标系
*/
/*-------------------------------------------------------------------------------------*/
void TrainImgUseSvm::GetLearnModule_SCountColor(CvSVM& svm)
{
	CvMat MatOne;
	int Dim=3;
	vector<float> SCountD;
	vector<Svm_SCDC> data_all;
	for(float si=0;si<1;si+=0.04){
		for(float counti=0;counti<1;counti+=0.04){
			for(float ci=0;ci<1;ci+=0.04){
				SCountD.clear();
				SCountD.push_back(si);
				SCountD.push_back(counti);
				SCountD.push_back(ci);
				CvMat *MatOneFeature=cvInitMatHeader(&MatOne,1,Dim,CV_32FC1, SCountD.data());	
				int cate_si=svm.predict(MatOneFeature);
				data_all.push_back(Svm_SCDC(cate_si,si,counti,0,ci));

			}
		}
	}

#if TRUE 
	string filepath=SVMDetectorxml+"\\"+"SvmDataModule.xml";
	FileStorage fs(filepath, FileStorage::APPEND);
	for(int i=0;i<data_all.size();i++){	
		if (fs.isOpened()){
			fs<<"SvmData";
			fs<<"{";
			fs<<"Category"<<data_all[i].Category;
			fs<<"SumManhattan"<<data_all[i].SumManhattan;
			fs<<"ZeroCount"<<data_all[i].ZeroCount;
			fs<<"LineNumDensity"<<data_all[i].LineNumDensity;
			fs<<"Color"<<data_all[i].Color;
			fs<<"}";
		}else{
			ASSERT(0);
		}
	}
#endif
}
/*-------------------------------------------------------------------------------------*/
/**
*将学习模型保存到三维坐标系
*/
/*-------------------------------------------------------------------------------------*/
void TrainImgUseSvm::GetLearnModule_CountDColor(CvSVM& svm)
{
	CvMat MatOne;
	int Dim=3;
	vector<float> SCountD;
	vector<Svm_SCDC> data_all;
	for(float counti=0;counti<1;counti+=0.04){
		for(float di=0;di<1;di+=0.04){
			for(float ci=0;ci<1;ci+=0.04){
				SCountD.clear();
				SCountD.push_back(counti);
				SCountD.push_back(di);
				SCountD.push_back(ci);
				CvMat *MatOneFeature=cvInitMatHeader(&MatOne,1,Dim,CV_32FC1, SCountD.data());	
				int cate_si=svm.predict(MatOneFeature);
				data_all.push_back(Svm_SCDC(cate_si,-1,counti,di,ci));

			}
		}
	}

#if TRUE 
	string filepath=SVMDetectorxml+"\\"+"SvmDataModule.xml";
	FileStorage fs(filepath, FileStorage::APPEND);
	for(int i=0;i<data_all.size();i++){	
		if (fs.isOpened()){
			fs<<"SvmData";
			fs<<"{";
			fs<<"Category"<<data_all[i].Category;
			fs<<"SumManhattan"<<data_all[i].SumManhattan;
			fs<<"ZeroCount"<<data_all[i].ZeroCount;
			fs<<"LineNumDensity"<<data_all[i].LineNumDensity;
			fs<<"Color"<<data_all[i].Color;
			fs<<"}";
		}else{
			ASSERT(0);
		}
	}
#endif
}
/*-------------------------------------------------------------------------------------*/
/**
*将学习模型保存到三维坐标系
*/
/*-------------------------------------------------------------------------------------*/
void TrainImgUseSvm::GetLearnModule_SCount(CvSVM& svm)
{

	CvMat MatOne;
	int Dim=2;
	vector<float> SCountD;
	vector<Svm_SCDC> data_all;
#if 0
	for(int si=0;si<6000;si+=240)
#else
	for(float si=0;si<1;si+=0.02)
#endif

	{
#if 0
		for(int counti=0;counti<180;counti+=6)
#else
		for(float counti=0;counti<1;counti+=0.02)
#endif

		{
			{
				SCountD.clear();
				SCountD.push_back(si);
				SCountD.push_back(counti);
				CvMat *MatOneFeature=cvInitMatHeader(&MatOne,1,Dim,CV_32FC1, SCountD.data());	
				int cate_si=svm.predict(MatOneFeature);
				data_all.push_back(Svm_SCDC(cate_si,si,counti,0,0));

			}
		}
	}

#if TRUE 
	string filepath=SVMDetectorxml+"\\"+"SvmDataModule.xml";
	FileStorage fs(filepath, FileStorage::APPEND);
	for(int i=0;i<data_all.size();i++){	
		if (fs.isOpened()){
			fs<<"SvmData";
			fs<<"{";
			fs<<"Category"<<data_all[i].Category;
			fs<<"SumManhattan"<<data_all[i].SumManhattan;
			fs<<"ZeroCount"<<data_all[i].ZeroCount;
			fs<<"LineNumDensity"<<data_all[i].LineNumDensity;
			fs<<"Color"<<data_all[i].Color;
			fs<<"}";
		}else{
			ASSERT(0);
		}
	}
#endif
}
/*-------------------------------------------------------------------------------------*/
/**
*将学习模型保存到三维坐标系
*/
/*-------------------------------------------------------------------------------------*/
void TrainImgUseSvm::GetLearnModule_SDensity(CvSVM& svm)
{
	CvMat MatOne;
	int Dim=2;
	vector<float> SCountD;
	vector<Svm_SCDC> data_all;
#if 0
	for(int si=0;si<6000;si+=240)
#else
	for(float si=0;si<1;si+=0.02)
#endif

	{
#if 0
		for(int counti=0;counti<180;counti+=6)
#else
		for(float di=0;di<1;di+=0.02)
#endif
		{
			{
				SCountD.clear();
				SCountD.push_back(si);
				SCountD.push_back(di);
				CvMat *MatOneFeature=cvInitMatHeader(&MatOne,1,Dim,CV_32FC1, SCountD.data());	
				int cate_si=svm.predict(MatOneFeature);
				data_all.push_back(Svm_SCDC(cate_si,si,0,di,0));

			}
		}
	}

#if TRUE 
	string filepath=SVMDetectorxml+"\\"+"SvmDataModule.xml";
	FileStorage fs(filepath, FileStorage::APPEND);
	for(int i=0;i<data_all.size();i++){
		if (fs.isOpened()){
			fs<<"SvmData";
			fs<<"{";
			fs<<"Category"<<data_all[i].Category;
			fs<<"SumManhattan"<<data_all[i].SumManhattan;
			fs<<"ZeroCount"<<data_all[i].ZeroCount;
			fs<<"LineNumDensity"<<data_all[i].LineNumDensity;
			fs<<"Color"<<data_all[i].Color;
			fs<<"}";
		}else{
			ASSERT(0);
		}
	}
#endif
}
/*-------------------------------------------------------------------------------------*/
/**
*将学习模型保存到三维坐标系
*/
/*-------------------------------------------------------------------------------------*/
void TrainImgUseSvm::GetLearnModule_SColor(CvSVM& svm)
{
	CvMat MatOne;
	int Dim=2;
	vector<float> SCountD;
	vector<Svm_SCDC> data_all;
#if 0
	for(int si=0;si<6000;si+=240)
#else
	for(float si=0;si<1;si+=0.02)
#endif

	{
#if 0
		for(int counti=0;counti<180;counti+=6)
#else
		for(float ci=0;ci<1;ci+=0.02)
#endif
		{
			{
				SCountD.clear();
				SCountD.push_back(si);
				SCountD.push_back(ci);
				CvMat *MatOneFeature=cvInitMatHeader(&MatOne,1,Dim,CV_32FC1, SCountD.data());	
				int cate_si=svm.predict(MatOneFeature);
				data_all.push_back(Svm_SCDC(cate_si,si,0,0,ci));

			}
		}
	}

#if TRUE 
	string filepath=SVMDetectorxml+"\\"+"SvmDataModule.xml";
	FileStorage fs(filepath, FileStorage::APPEND);
	for(int i=0;i<data_all.size();i++){
		if (fs.isOpened()){
			fs<<"SvmData";
			fs<<"{";
			fs<<"Category"<<data_all[i].Category;
			fs<<"SumManhattan"<<data_all[i].SumManhattan;
			fs<<"ZeroCount"<<data_all[i].ZeroCount;
			fs<<"LineNumDensity"<<data_all[i].LineNumDensity;
			fs<<"Color"<<data_all[i].Color;
			fs<<"}";
		}else{
			ASSERT(0);
		}
	}
#endif
}
#endif
#if (CV_MAJOR_VERSION==3)  

#endif	