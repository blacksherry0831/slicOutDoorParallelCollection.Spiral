#include "StdAfx.h"
#include "module_all_cui.h"
//#include "SpAnalyze.h"
/*------------------------------------------------*/
/**
*初始化变量
*/
/*------------------------------------------------*/
SpSpAnalyzeData::SpSpAnalyzeData()
{
	 P_cs=0;
	 P_sRects=0;
	 Sp_C=0;
	 Sp_S=0;
	 Rect_S=0;
	 FileName="";
	 FileCategory=0;
     
	
}
/*------------------------------------------------*/
/**
*
*/
/*------------------------------------------------*/
SpSpAnalyzeData::~SpSpAnalyzeData()
{
	
}
/*------------------------------------------------*/
/**
*初始化变量\n
*输入图块胖瘦判断边界
*/
/*------------------------------------------------*/
SpAnalyze::SpAnalyze(void)
{
	this->fatorthin=2.9053096752613783E-1F;
	this->ImgMaskControus=nullptr;
}
/*------------------------------------------------*/
/**
*释放内存
*/
/*------------------------------------------------*/
SpAnalyze::~SpAnalyze(void)
{
	this->DeInit();
	cvReleaseImage(&ImgMaskControus);
}
/*------------------------------------------------*/
/**
*计算周长m面积比
*/
/*------------------------------------------------*/
double SpAnalyze:: CalculateP_cs(void)
{
	float C=Calculate_C();
	float S=Calculate_S();

	IplImage* ImgContour=cvCreateImage(cvSize(pMD->ImgWidth,pMD->ImgHeight),IPL_DEPTH_8U,4);

	UINT32 *ImgData=(UINT32*)ImgContour->imageData;
	/*初始化PNG图像数据*/
	cui_GeneralImgProcess::InitPNGData(ImgData,pMD->ImgWidth,pMD->ImgHeight,BlackColorPNG);
#if TRUE
	IplImage lables;
	cvInitImageHeader(&lables,cvSize(pMD->ImgWidth,pMD->ImgHeight),IPL_DEPTH_8U,4);
	ASSERT(sizeof(int)==4);
	lables.imageData=(char *)this->ImgLabels;
	 SpADataNow.P_cs=cui_GeneralImgProcess::GetContour2Draw(&lables,ImgContour,cvScalar(0xff,0xff,0xff,0xff),1,&SpADataNow.Sp_C,&SpADataNow.Sp_S);
	 SpADataNow.Rect_S=1.0*pMD->ImgWidth*pMD->ImgHeight;
	 cvSaveImage("C_1.png",ImgContour);
	 cvSaveImage("Lables.png",&lables);
#endif
#if TRUE

	 if (ImgMaskControus!=nullptr){
		 cvReleaseImage(&ImgMaskControus);
	 }

	 IplImage* ImgMaskTemp=cvCloneImage(&lables);
	 //反色处理
	 UINT32 *ColorPixel=(UINT32*)ImgMaskTemp->imageData;
	 uchar* data   = (uchar*)ImgMaskTemp->imageData;
	 // 反色图像
	 for (int x=0;x<ImgMaskTemp->width;x++){
		 for (int y=0;y<ImgMaskTemp->height;y++)
		 {
			 UINT32 RGBA=ColorPixel[x+y*ImgMaskTemp->width];
			 if (RGBA==BlackColorPNG){
				 ColorPixel[x+y*ImgMaskTemp->width]=WhiteColorPNG;
			 }else if (RGBA==WhiteColorPNG){
				 ColorPixel[x+y*ImgMaskTemp->width]=BlackColorPNG;
			 }else{
				 ASSERT(0);
			 }
		 }
	 }

	 cui_GeneralImgProcess::GetContour2Draw(&lables,ImgMaskTemp,cvScalar(0xff,0xff,0xff,0xff),4);
	 ImgMaskControus=cvCloneImage(ImgMaskTemp);
	 /////////////////////////////////////////////
	 pMD->SetImgMaskLables(this->ImgMaskControus);
	 /////////////////////////////////////////////
	  cvSaveImage("C_4.png",ImgMaskTemp);
	  cvReleaseImage(&ImgMaskTemp);
#endif
	
	cvReleaseImage(&ImgContour);

	SpADataNow.P_sRects=SpADataNow.Sp_S/SpADataNow.Rect_S;
	return  SpADataNow.P_cs;
}
/*------------------------------------------------*/
/**
*计算周长
*@deprecated 弃用
*/
/*------------------------------------------------*/
float SpAnalyze::Calculate_C(void)
{
    float C=0;
#if 0
IplImage* ImgContour=cvCreateImage(cvSize(pMD->ImgWidth,pMD->ImgHeight),IPL_DEPTH_8U,4);
	UINT32 *ImgData=(UINT32*)ImgContour->imageData;
	cui_GeneralImgProcess::InitPNGData(ImgData,pMD->ImgWidth,pMD->ImgHeight,BlackColorPNG);
#if 0
cui_GeneralImgProcess::DrawContoursAroundSegments(ImgData,ImgLabels,pMD->ImgWidth,pMD->ImgHeight,WhiteColorPNG);
#else
	IplImage lables;
	cvInitImageHeader(&lables,cvSize(pMD->ImgWidth,pMD->ImgHeight),IPL_DEPTH_8U,4);
	lables.imageData=(char *)this->ImgLabels;
	this->P_cs=cui_GeneralImgProcess::GetContour2Draw(&lables,ImgContour,cvScalar(0xff,0xff,0xff,0xff),1);
#endif
	
	cvSaveImage("C.png",ImgContour);


	cvReleaseImage(&ImgContour);
#endif
	
	return C;
}
/*------------------------------------------------*/
/**
*计算有效区域面积
*/
/*------------------------------------------------*/
float SpAnalyze::Calculate_S(void)
{

	float S=0;

	for (int x=0;x<pMD->ImgWidth;x++){
		for (int y=0;y<pMD->ImgHeight;y++){
			int SpIndex=y*pMD->ImgWidth+x;
			if (ImgData[SpIndex]!=BlackColorPNG){
					S++;
			}			
		}
	}
	ASSERT(S);
	return S;
}
/*------------------------------------------------*/
/**
*根据图片输入初始化
*
*/
/*------------------------------------------------*/
void SpAnalyze::Init(ImageMemData* MemData_t)
{
	this->pMD=MemData_t;
	this->SpSA=SpSizeAttr::SA_UnKnow;
	this->ImgData=pMD->Src_ImgData;
	this->ImgLabels=pMD->ImgLables.get();
	memset(ImgLabels,0,sizeof(int)*pMD->ImgWidth*pMD->ImgHeight);
	this->ChangeColor2Labels();
	FileNameSplit fns;
#if Use_CString&&_MSC_VER
   fns.Parse(FileNameSplit::Convertstring2CS(pMD->FileReadFullPath));
   SpADataNow.FileName=FileNameSplit::ConvertCS2string(fns.filefullname);
#endif

#if linux
    SpADataNow.FileName="linux";
#endif
	

	SpADataNow.File=pMD->FileReadFullPath;
	SpADataNow.FileWritePath=pMD->FileWritePath;
	ImgMaskControus=nullptr;
}
/*------------------------------------------------*/
/**
*去初始化，重设胖瘦图块
*/
/*------------------------------------------------*/
void SpAnalyze::DeInit(void)
{
	this->SpSA=SpSizeAttr::SA_UnKnow;
}
/*------------------------------------------------*/
/**
*计算周长面积比
*@param MemData_t 图像数据（带黑色边缘）
*@param IsSave  是否保存数据到硬盘
*/
/*------------------------------------------------*/
void SpAnalyze::Calcuate(ImageMemData* MemData_t,bool IsSave)
{
		this->Init(MemData_t);
		this->CalculateP_cs();
		if (IsSave){
			AnalyzeData.push_back(SpADataNow.P_cs);
			SpAnalyzeData.push_back(SpADataNow);
		}		
		this->SaveSpAnalyze();
		this->DeInit();
}
/*------------------------------------------------*/
/**
*将输入图片数据转换成Labels
*/
/*------------------------------------------------*/
void SpAnalyze::ChangeColor2Labels(void)
{
	for (int x=0;x<pMD->ImgWidth;x++){
		for (int y=0;y<pMD->ImgHeight;y++){
			int SpIndex=y*pMD->ImgWidth+x;
			ASSERT(SpIndex<pMD->ImgHeight*pMD->ImgWidth);
			if (ImgData[SpIndex]==BlackColorPNG){
				ImgLabels[SpIndex]=BlackColorPNG;
			}else{
				ImgLabels[SpIndex]=WhiteColorPNG;
			}			
		}
	}
}
/*------------------------------------------------*/
/**
*保存计算数据
*APPEND追加方式
*/
/*------------------------------------------------*/
void SpAnalyze::SaveSpAnalyze()
{
	string xml_save_path=pMD->FileWritePath+"SpAnalyze.xml";
	CvFileStorage *fsW=cvOpenFileStorage(xml_save_path.c_str(),0,CV_STORAGE_APPEND);
	CvFileNode* RootNode=cvGetRootFileNode(fsW);
	CvFileNode* DataNode=cvGetFileNodeByName(fsW,NULL,"Data");

	
		cvStartWriteStruct(fsW,"Data",CV_NODE_MAP,NULL,cvAttrList(0,0));
		
		cvWriteReal(fsW,"P_cs", SpADataNow.P_cs);
		cvWriteReal(fsW,"P_sRects",SpADataNow.P_sRects);
		ASSERT( SpADataNow.P_cs!=0);
		cvWriteString(fsW,"FileName",SpADataNow.FileName.c_str());
		cvWriteInt(fsW,"FileCategory",SpADataNow.FileCategory);
		cvEndWriteStruct(fsW);
	
	cvReleaseFileStorage(&fsW);
}
/*------------------------------------------------*/
/**
*
*保存周长面积比，到硬盘
*/
/*------------------------------------------------*/
void SpAnalyze::PickUp(void)
{
	int CateNum=10;
	float MaxValue=cui_GeneralImgProcess::GetMaxValue(AnalyzeData.data(),AnalyzeData.size());
	float MinValue=cui_GeneralImgProcess::GetMinValue(AnalyzeData.data(),AnalyzeData.size());
	float delta=(MaxValue-MinValue)/CateNum;
	ASSERT(delta>=0);
#if 1
	string xml_save_path=pMD->FileWritePath+"SpInterval.xml";
	CvFileStorage *fsW=cvOpenFileStorage(xml_save_path.c_str(),0,CV_STORAGE_WRITE);
	
	
	for (int i=0;i<CateNum;i++){
		cvStartWriteStruct(fsW,"Interval",CV_NODE_MAP,NULL,cvAttrList(0,0));
		cvWriteInt(fsW,"Index",i+1);
		cvWriteReal(fsW,"From",MinValue+i*delta);
		cvWriteReal(fsW,"To",MinValue+(i+1)*delta);
		cvEndWriteStruct(fsW);
	}
	
	
	cvReleaseFileStorage(&fsW);
#endif
	for(long i=0;i<SpAnalyzeData.size();i++){
		SpAnalyzeData[i].classify(MaxValue,MinValue,CateNum);
	}
}
/*------------------------------------------------*/
/**
*根据（周长面积比的）最大值，最小值分类超像素图块的等级
*
*@param		maxValue （周长面积比最大值）
*@param		minValue （周长面积比最小值）
*@param		CateNum  分类的等级数
*
*/
/*------------------------------------------------*/
void SpSpAnalyzeData::classify(float maxValue,float minValue,int CateNum)
{
	 ASSERT(maxValue>=minValue);	 
	 float delta=(maxValue-minValue)/CateNum;
	 int CateIndex=(P_cs-minValue)/delta+1;
	 IplImage *img=cvLoadImage(File.c_str());
	 char buff[1024];
	 sprintf(buff,"_C_%d_.jpg",CateIndex);
	 string FileSaveName=FileWritePath+FileName+buff;
	 cvSaveImage(FileSaveName.c_str(),img);
	 cvReleaseImage(&img);
}
/*------------------------------------------------*/
/**
*计算图像的胖瘦属性
*依据周长面积比
*@param    MemData_t  图像数据
*@return   SpSizeAttr 周长面积比判断的属性
*@see   ::Calcuate()
*/
/*------------------------------------------------*/
SpSizeAttr SpAnalyze::IsSpFatorThin(ImageMemData* MemData_t)
{

this->Calcuate(MemData_t,false);
#if TEST_Feature_M
	return SpSizeAttr::FAT;
#else
if (SpADataNow.P_cs<fatorthin){
	this->SpSA=SpSizeAttr::FAT;
}else{
	this->SpSA=SpSizeAttr::THIN;
}
return this->SpSA;
#endif
}
/*------------------------------------------------*/
/**
*
*return  Manhattan直方图+Manhattan数量直方图的特征计数
*/
/*------------------------------------------------*/
int SpAnalyze::GetFeatureSize(void)
{
#if 0
	return 1080;
#endif
	return 360;
}
/*------------------------------------------------*/
/**
*导出超像素图块的特征向量
*@param [out] 特征向量
*
*/
/*------------------------------------------------*/
void SpAnalyze::ExtractImageFeatureWithHarr(vector<float>& feature)
{
	ASSERT(this->SpSA!=SpSizeAttr::SA_UnKnow);
	if (this->SpSA==SpSizeAttr::FAT){
		CuiHarrTransformLet harrtran(pMD);
		harrtran.CuiWaveletTrans(HarrDepth);
		HarrWaveletTransImg ImgSplit(harrtran.img_scr_color,harrtran.img_dst_wavelet,pMD->ImgLables.get(),harrtran.Scale());
		ImgSplit.SaveImg();	
		if (HarrDepth>=1){
			feature.clear();

			ImageMemData memdata_HH_1(ImgSplit.imgHH_1);	 
			HoughAnalyze ha_HH_1(&memdata_HH_1,ImgMaskControus,&HoughAnalyzeParam::GetHarrHAParam());	
			HistData180 histdata_HH_1=ha_HH_1.GetSaveALlImgHistogram_180_0to100();
			PushBackFeature(feature,histdata_HH_1,AngLeDivided180);

			ImageMemData memdata_HL_1(ImgSplit.imgHL_1);	 
			HoughAnalyze ha_HL_1(&memdata_HL_1,ImgMaskControus,&HoughAnalyzeParam::GetHarrHAParam());	
			HistData180 histdata_HL_1=ha_HL_1.GetSaveALlImgHistogram_180_0to100();
			PushBackFeature(feature,histdata_HL_1,AngLeDivided180);

			ImageMemData memdata_LH_1(ImgSplit.imgLH_1);	 
			HoughAnalyze ha_LH_1(&memdata_LH_1,ImgMaskControus,&HoughAnalyzeParam::GetHarrHAParam());	
			HistData180 histdata_LH_1=ha_LH_1.GetSaveALlImgHistogram_180_0to100();
			PushBackFeature(feature,histdata_LH_1,AngLeDivided180);
		}
	}
	if (this->SpSA==SpSizeAttr::THIN)
	{

	}
}
/*------------------------------------------------*/
/**
*导出超像素图块的特征向量
*@param [out] 特征向量
*
*/
/*------------------------------------------------*/
HistData180 SpAnalyze::ExtractImageFeatureWithSrcCany(vector<float>& feature)
{
	HistData180 histdata;
#if TEST_Feature_M	
	for (int i=0;i<360;i++){
		feature.push_back(i);
	}
#else
	ASSERT(this->SpSA!=SpSizeAttr::SA_UnKnow);
	feature.clear();

	if (this->SpSA==SpSizeAttr::FAT){
			
			ImageMemData SrcImg(pMD->src_Img_bgra);	 
			HoughAnalyze SrcHough(&SrcImg,ImgMaskControus,&HoughAnalyzeParam::GetHarrHAParam());	
			histdata=SrcHough.GetSaveALlImgHistogram_180_0to100();
			PushBackFeature(feature,histdata,AngLeDivided180);
			
	}

	if (this->SpSA==SpSizeAttr::THIN){
		
	}
#endif
	ASSERT(feature.size()==this->GetFeatureSize());
	return histdata;
}
/*------------------------------------------------*/
/**
*导出超像素图块的特征向量
*@param [out] 特征向量
*
*/
/*------------------------------------------------*/
HistData180 SpAnalyze::ExtractImageFeatureWithSrcCany(Feature_VegaBuilding& fvb)
{
	HistData180 histdata;
	fvb.SrcFileName=pMD->FileReadFullPath;
	fvb.Width=pMD->ImgWidth;
	fvb.Height=pMD->ImgHeight;
	ASSERT(this->SpSA!=SpSizeAttr::SA_UnKnow);
	if (this->SpSA==SpSizeAttr::FAT){
		ImageMemData SrcImg(pMD->src_Img_bgra);	 
		HoughAnalyze SrcHough(&SrcImg,ImgMaskControus,&HoughAnalyzeParam::GetHarrHAParam());
		histdata=SrcHough.GetSaveALlImgHistogram_180();
	}
	if (this->SpSA==SpSizeAttr::THIN){
	
	}
	fvb.SetHist180(histdata);
	return histdata;
}
/*------------------------------------------------*/
/**
*将Manhattan直方图特征压入feature
*@param feature [in][out] 特征
*@param histdata[in]  曼哈顿直方图
*@param Dim 要压入的维度
*/
/*------------------------------------------------*/
void SpAnalyze::PushBackFeature(vector<float> &feature,HistData180& histdata,int Dim)
{
	ASSERT(Dim==180);
	for (int i=0;i<Dim;i++){		
		feature.push_back(histdata.hist_all[i]);
	}
	for (int i=0;i<Dim;i++){
		feature.push_back(histdata.hist_all_count[i]);
	}
	
}
/*-------------------------------------------------------------------------------*/
/**
*@deprecated 弃用
*/
/*-------------------------------------------------------------------------------*/
Feature_VegaBuilding SpAnalyze::ExtractImage_Harr_Manhattan_Feature()
{
	Feature_VegaBuilding fvb;
#if 0
	ExtractImageFeatureWithSrcCany(fvb.FeatureManhattan);
	fvb.Sum_FeatureManhattan=cui_GeneralImgProcess::GetSumValue(fvb.FeatureManhattan.data(),fvb.FeatureManhattan.size());
	fvb.ZeroCount_FeatureManhattan=cui_GeneralImgProcess::GetZeroCount(fvb.FeatureManhattan.data(),fvb.FeatureManhattan.size());
	fvb.SrcFileName=pMD->FileReadFullPath;
	Energy_Harr HarrImg=cui_GeneralImgProcess::CalculateImageHarrEnergy(ImgData,(UINT32*)(ImgMaskControus->imageData),pMD->ImgWidth,pMD->ImgHeight);
	fvb.SetHarrEnergy(HarrImg);
#endif


	return fvb;
}
/*-------------------------------------------------------------------------------*/
/**
*导出对曼哈顿直方图的求和，计数等信息
*@return 输出特征
*/
/*-------------------------------------------------------------------------------*/
Feature_VegaBuilding SpAnalyze::ExtractImage_Sum_Count_Feature()
{
	Feature_VegaBuilding fvb;
	fvb.SrcFileName=pMD->FileReadFullPath;
	fvb.Width=pMD->ImgWidth;
	fvb.Height=pMD->ImgHeight;
	ExtractImageFeatureWithSrcCany(fvb);
	this->ExtractImage_LabColorFeature(fvb);
	return fvb;
}
/*-------------------------------------------------------------------------------*/
/**
*提取颜色特征并输出
*@param [out] f_vb 
*
*/
/*-------------------------------------------------------------------------------*/
void SpAnalyze::ExtractImage_LabColorFeature(Feature_VegaBuilding& f_vb)
{
	ColorAnalyzeOnLab ColorAnaly(pMD);
	
	LabColor color=ColorAnaly.GetImageColorMostly();
	f_vb.SetLabColor(color);
	
}
/*----------------------------------------------------------------*/
/**
*获得超像素图块中有效面积
*@return 非黑色部分超像素图块面积
*@note 输入图像有黑色边缘
*/
/*----------------------------------------------------------------*/
unsigned long SpAnalyze::GetEffectiveColorNumber()
{
	unsigned  long  BlackCount=0;
	unsigned  long  WhiteCount=0;
	UINT32* ImgMaskData=(UINT32*)this->ImgMaskControus->imageData;
	for (int x=0;x<ImgMaskControus->width;x++){
		for (int y=0;y<ImgMaskControus->height;y++){
			unsigned long int i=x+y*ImgMaskControus->width;
			if (ImgMaskData[i]==BlackColorPNG||ImgMaskData[i]==BlackColorRGB){
				BlackCount++;
			}else if (ImgMaskData[i]==WhiteColorPNG||ImgMaskData[i]==WhiteColorRGB){
				WhiteCount++;
			}else{
				UINT32 Color=ImgMaskData[i];
				ASSERT(FALSE);
			}

		}
	}
	EffectivePixelNum=BlackCount;
	return BlackCount;
}
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/
