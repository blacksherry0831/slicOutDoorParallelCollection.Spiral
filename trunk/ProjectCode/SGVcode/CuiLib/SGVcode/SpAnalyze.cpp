#include "StdAfx.h"
#include "module_all_cui.h"
//#include "SpAnalyze.h"
/*------------------------------------------------*/
/**
*��ʼ������
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
*��ʼ������\n
*����ͼ�������жϱ߽�
*/
/*------------------------------------------------*/
SpAnalyze::SpAnalyze(void)
{
	this->fatorthin=2.9053096752613783E-1F;
	this->ImgMaskControus=nullptr;
}
/*------------------------------------------------*/
/**
*�ͷ��ڴ�
*/
/*------------------------------------------------*/
SpAnalyze::~SpAnalyze(void)
{
	this->DeInit();
	cvReleaseImage(&ImgMaskControus);
}
/*------------------------------------------------*/
/**
*�����ܳ�m�����
*/
/*------------------------------------------------*/
double SpAnalyze:: CalculateP_cs(void)
{
	float C=Calculate_C();
	float S=Calculate_S();

	IplImage* ImgContour=cvCreateImage(cvSize(pMD->ImgWidth,pMD->ImgHeight),IPL_DEPTH_8U,4);

	UINT32 *ImgData=(UINT32*)ImgContour->imageData;
	/*��ʼ��PNGͼ������*/
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
	 //��ɫ����
	 UINT32 *ColorPixel=(UINT32*)ImgMaskTemp->imageData;
	 uchar* data   = (uchar*)ImgMaskTemp->imageData;
	 // ��ɫͼ��
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
*�����ܳ�
*@deprecated ����
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
*������Ч�������
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
*����ͼƬ�����ʼ��
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
*ȥ��ʼ������������ͼ��
*/
/*------------------------------------------------*/
void SpAnalyze::DeInit(void)
{
	this->SpSA=SpSizeAttr::SA_UnKnow;
}
/*------------------------------------------------*/
/**
*�����ܳ������
*@param MemData_t ͼ�����ݣ�����ɫ��Ե��
*@param IsSave  �Ƿ񱣴����ݵ�Ӳ��
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
*������ͼƬ����ת����Labels
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
*�����������
*APPEND׷�ӷ�ʽ
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
*�����ܳ�����ȣ���Ӳ��
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
*���ݣ��ܳ�����ȵģ����ֵ����Сֵ���೬����ͼ��ĵȼ�
*
*@param		maxValue ���ܳ���������ֵ��
*@param		minValue ���ܳ��������Сֵ��
*@param		CateNum  ����ĵȼ���
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
*����ͼ�����������
*�����ܳ������
*@param    MemData_t  ͼ������
*@return   SpSizeAttr �ܳ�������жϵ�����
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
*return  Manhattanֱ��ͼ+Manhattan����ֱ��ͼ����������
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
*����������ͼ�����������
*@param [out] ��������
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
*����������ͼ�����������
*@param [out] ��������
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
*����������ͼ�����������
*@param [out] ��������
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
*��Manhattanֱ��ͼ����ѹ��feature
*@param feature [in][out] ����
*@param histdata[in]  ������ֱ��ͼ
*@param Dim Ҫѹ���ά��
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
*@deprecated ����
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
*������������ֱ��ͼ����ͣ���������Ϣ
*@return �������
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
*��ȡ��ɫ���������
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
*��ó�����ͼ������Ч���
*@return �Ǻ�ɫ���ֳ�����ͼ�����
*@note ����ͼ���к�ɫ��Ե
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
