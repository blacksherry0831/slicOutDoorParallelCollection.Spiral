#include "StdAfx.h"
#include "module_all_cui.h"
//#include "ImageMemData.h"
//#include "cui_GeneralImgProcess.h"
#ifndef cvCopyImage
#define cvCopyImage( src, dst ) cvCopy( src, dst, 0 )
#endif // _DEBUG

/*----------------------------------------------------------------*/
/**
*构造函数\n
*在做超像素运算之前，设置基本的参数
*
*@param  filename  图像的文件名
*@param  filesavepath   中间结果输出路径
*@param  spcount 期望的超像素个数
*@param  compactness 做超像素时的密度因子
*@param  horizontal_line_pos  视平线的位置
*/
/*----------------------------------------------------------------*/
ImageMemData::ImageMemData(
	string filename,
	string filesavepath,
	int spcount,
	double compactness,
	float horizontal_line_pos)
{
	//IplImage* a=cvCreateImage(cvSize(100,100),IPL_DEPTH_8U,3);
//	new int[10];
	//malloc(100);
	this->initParam();
	ASSERT(filename!="");
	SetImageData(filename,filesavepath);
	SetSlicParameter(spcount,compactness);	
	InitMemoryData(nullptr,filename,filesavepath,spcount,compactness);
	this->Seg_HorizontalLinePos=horizontal_line_pos*this->ImgHeight;
	this->Seg_HorizontalLinePosScale=horizontal_line_pos;
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
ImageMemData::ImageMemData(
	IplImage* img,
	string filesavepath,
	int spcount,
	double compactness,
	float horizontal_line_pos)
{
	string filename="MemoryIMG";
	this->initParam();
	SetImageData(filename,filesavepath);
	SetSlicParameter(spcount,compactness);	
	InitMemoryData(img,filename,filesavepath,spcount,compactness);
	this->Seg_HorizontalLinePos=horizontal_line_pos*this->ImgHeight;
	this->Seg_HorizontalLinePosScale=horizontal_line_pos;
}
/*----------------------------------------------------------------*/
/**
*初始化成员变量
*
*
*/
/*----------------------------------------------------------------*/
void ImageMemData::initParam(void)
{	
	this->is_allcoate_mem=false;
	this->slic_current_num=-1;
	/////////////////////////
	p_SpProperty=nullptr;
	Src_ImgData=nullptr;
	src_ImgLabels=nullptr;
	src_Img_bgra=nullptr;
	////////////////////////
	Matrix_E=nullptr;
	Matrix_D=nullptr;
	Matrix_W_Color=nullptr;
	Matrix_W_Vein=nullptr;
	Matrix_L=nullptr;
	p_ImgLables_SVG=nullptr;
	////////////////////////
	this->ImgLables=nullptr;
	this->LineContour=nullptr;
	this->LineSource=nullptr;
	 Matrix_E_InDoor=nullptr;
	 Matrix_D_InDoor=nullptr;
	 Matrix_W_InDoor=nullptr;
	 Matrix_L_InDoor=nullptr;
		this->PgOffset=0;
		this->PsOffset=0;
	///////////////////
	l_plane=NULL;
	a_plane=NULL;
	b_plane=NULL;
	h_plane=NULL;
	s_plane=NULL;
	v_plane=NULL;
	this->src_ImgMaskLables=nullptr;
	///////////////////
}
/*----------------------------------------------------------------*/
/**
*设置图像的输入、输出路径
*
* @param filename      文件名
* @param filesavepath  中间结果的输出路径
*
*/
/*----------------------------------------------------------------*/
void ImageMemData::SetImageData(string filename,string filesavepath){

	
	FileReadFullPath=filename;
	FileWritePath=filesavepath;
	
}
/*----------------------------------------------------------------*/
/**
*设置超像素运算时期望的超像素个数、密度因子
*
*@param spcount 期望的超像素个数
*@param compactness  密度因子
*
*/
/*----------------------------------------------------------------*/
 void ImageMemData::SetSlicParameter(int spcount,double compactness){
	 slic_spcount=spcount;
	 slic_compactness=compactness;
 }
 /*----------------------------------------------------------------*/
 /**
 *析构函数释放分配的内存
 *
 *
 */
 /*----------------------------------------------------------------*/
ImageMemData::~ImageMemData()
{
	this->ReleaseMemory();
}
/*----------------------------------------------------------------*/
/**
*清理中间过程分配的内存
*
*
*/
/*----------------------------------------------------------------*/

void ImageMemData::ReleaseMemory(void)
{
	if (p_SpProperty!=nullptr){
	  delete[]p_SpProperty;
	  p_SpProperty=nullptr;
	}
	if (Src_ImgData!=nullptr){
		delete[]Src_ImgData;
		Src_ImgData=nullptr;
	}
	if (src_Img_bgra!=nullptr){
		cvReleaseImage(&src_Img_bgra);
	}
	if (src_ImgLabels!=nullptr){
		delete[]src_ImgLabels;
		src_ImgLabels=nullptr;
	}
	if (Matrix_W_Vein!=nullptr){
		delete[]Matrix_W_Vein;
		Matrix_W_Vein=nullptr;
	}
	if (p_ImgLables_SVG!=nullptr){
		delete[]p_ImgLables_SVG;
		p_ImgLables_SVG=nullptr;
	}
	if (LineContour!=nullptr){
		delete[] LineContour;
		LineContour=nullptr;
	}
	if (l_plane!=nullptr){
		cvReleaseImage(&l_plane);
	}  
	if (a_plane!=nullptr){
		cvReleaseImage(&a_plane);
	}  
	if (b_plane!=nullptr){
		cvReleaseImage(&b_plane);
	}  
	if (src_ImgMaskLables!=nullptr){
		cvReleaseImage(&src_ImgMaskLables);
	}
	if (this->LineSource!=nullptr){
		delete[] this->LineSource;
		this->LineSourceLen=0;
	}
}
/*----------------------------------------------------------------*/
/**
*hough变换时，检测水平线的最小长度
*
*@return 水平线的最小长度
*/
/*----------------------------------------------------------------*/
float ImageMemData::GetHorizontalThresholdLength(void)
 {
	 ASSERT(ImgWidth==0);
	 ASSERT(ImgHeight==0);
	 ASSERT(slic_spcount==0);
 float line_len=sqrt(1.0*ImgWidth*ImgHeight/slic_spcount);
	 line_len=0.3*line_len;
	 return line_len;	
 }
/*----------------------------------------------------------------*/
/**
*hough变换时，检测水平线的最小长度
*
*@return 水平线的最小长度
*/
/*----------------------------------------------------------------*/
float ImageMemData::GetSuperPixelDefaultEdgeLength(void)
{

	ASSERT(ImgWidth!=0);
	ASSERT(ImgHeight!=0);
	ASSERT(slic_spcount!=0);
	float line_len=sqrt(1.0*ImgWidth*ImgHeight/slic_spcount);
	return line_len;


}
 /*----------------------------------------------------------------*/
 /**
 *初始化已分配的内存
 *
*@param filename  图像文件名
*@param filesavepath 图像保存路径
*@param spcount   超像素个数
*@param compactness 密度因子
 */
 /*----------------------------------------------------------------*/
 void ImageMemData::InitMemoryData(
	 IplImage* img,
	 string filename,
	 string filesavepath,
	 int spcount,
	 double compactness)
 {
	
	 ReleaseMemory();
	 /*************************************************************************************************/
	 IplImage *src_img_t;
	if (img==nullptr){
		 src_img_t=cvLoadImage(filename.c_str(),CV_LOAD_IMAGE_UNCHANGED); 
	}else{
		src_img_t=cvCreateImage(cvGetSize(img),img->depth,4);
		if (img->nChannels==4){
			 cvCopyImage(img,src_img_t);
		}else if (img->nChannels==3){
			cvCvtColor(img,src_img_t,CV_BGR2BGRA);
		}
		else if (img->nChannels==1){
			cvCvtColor(img,src_img_t,CV_GRAY2BGRA);
		}else{
			ASSERT(0);
		}
	   
	}	
	 cui_GeneralImgProcess::ConvertImg2Eighth4Ch(&src_img_t);
#if TRUE
	 src_Img_bgra=cvCloneImage(src_img_t);
	 cui_GeneralImgProcess::ConvertImg3ChTo4Ch(&src_Img_bgra);
#endif
	
	
	 cvReleaseImage(&src_img_t);
	 ImgWidth=src_Img_bgra->width;
	 ImgHeight=src_Img_bgra->height;
	
	 /*********************************************************************/
	   Src_ImgData=new UINT32[ImgWidth*ImgHeight];
	   memcpy(Src_ImgData,src_Img_bgra->imageData,sizeof(UINT32)*ImgWidth*ImgHeight);
	 /*********************************************************************/

	   src_ImgLabels=new int[ImgWidth*ImgHeight];
	   memset(src_ImgLabels,0,sizeof(int)*ImgWidth*ImgHeight);
	   this->ImgLables= std::shared_ptr<int>(new int[ImgWidth*ImgHeight]); 
	 /*********************************************************************/
	   p_ImgLables_SVG=new INT32[ImgWidth*ImgHeight];
	   memset(p_ImgLables_SVG,0,sizeof(int)*ImgWidth*ImgHeight);
	 /*********************************************************************/
	   ImgData_Contours=std::shared_ptr<UINT32>(new UINT32[ImgWidth*ImgHeight]);

 }

 /*----------------------------------------------------------------*/
 /**
 *释放与超像素个数相关的数据内存
 *
 *@param spnumber 超像素个数
 */
 /*----------------------------------------------------------------*/

  void ImageMemData::AllocateMemRelated2spnumber(int spnumber)
 {
	 this->slic_current_num=spnumber;
	 if (is_allcoate_mem==false){
		 is_allcoate_mem=true;
		 this->AllocaeMemoryByspNumber(spnumber);
	 }
 }
/*----------------------------------------------------------------*/
 /**
 *
 *
 *
 */
 /*----------------------------------------------------------------*/
 void ImageMemData::AllocateMemRelated2spnumber(void)
 {
	 this->AllocateMemRelated2spnumber(slic_current_num);
 }
 /*----------------------------------------------------------------*/
 /**
 *为与超像素个数相关的数据结构分配内存
 *
 *@param spnumber 超像素个数
 */
 /*----------------------------------------------------------------*/

 void ImageMemData::AllocaeMemoryByspNumber(int spnumber)
 {
	 /*********************************************************************/
	 p_SpProperty=new SP_PROPERTY[spnumber];
	 memset(p_SpProperty,0,sizeof(SP_PROPERTY)*spnumber); 
	 /*********************************************************************/
	 Matrix_W_Vein=new double[spnumber*spnumber];
	 memset(Matrix_W_Vein,0,sizeof(double)*spnumber*spnumber);
	 /*********************************************************************/
	 Histogram180=std::shared_ptr<HistData180>(new HistData180[spnumber]);
	 Matrix_Visit=std::shared_ptr<UINT32>(new UINT32[spnumber]);
	 LinkConnection=std::shared_ptr<UINT32>(new UINT32[spnumber]);
	 Matrix_Category_Lable=std::shared_ptr<INT32>(new INT32[spnumber]);
	 PositionCategoryForce=std::shared_ptr<INT32>(new INT32[spnumber]);
 }
/*------------------------------------------------------------------------------------------------------------*/
/**
*导入超像素结果
*@param  slic_num 超像素结果 
*@param  p_ImgLables  超像素的Lables 
*
*/
/*------------------------------------------------------------------------------------------------------------*/
  void ImageMemData::ImportMemData(int slic_num,INT32 *p_ImgLables)
 {
	 int ImageDims=this->ImgWidth*this->ImgHeight;
	 int MatrixDims=slic_num*slic_num;
	 this->slic_current_num=slic_num;
	 memcpy(this->ImgLables.get(),p_ImgLables,sizeof(int)*ImageDims);
	 //
	 Matrix_D= std::shared_ptr<double>(new double[MatrixDims]); 
	 Matrix_E= std::shared_ptr<UINT32>(new UINT32[MatrixDims]); 
	 Matrix_L= std::shared_ptr<double>(new double[MatrixDims]); 

	 delete[]Matrix_W_Color;
	 Matrix_W_Color=new double[MatrixDims];

	 Matrix_E_InDoor=std::shared_ptr<UINT32>(new UINT32[MatrixDims]);
	 Matrix_D_InDoor=std::shared_ptr<double>(new double[MatrixDims]);
	 Matrix_W_InDoor=std::shared_ptr<double>(new double[MatrixDims]);
	 Matrix_L_InDoor=std::shared_ptr<double>(new double[MatrixDims]);
	 MatrixEigenVector_L_InDoor=std::shared_ptr<float>(new float[MatrixDims]);
	 MatrixEigenValue_L_InDoor=std::shared_ptr<double>(new double[slic_num]);
	 Matrix_Category_Lable_InDoor=std::shared_ptr<INT32>(new INT32[slic_num]);
	 Matrix_Category_Simple_InDoor=std::shared_ptr<float>(new float[MatrixDims]);
	 this->DrawContours();
 }
/*------------------------------------------------------------------------------------------------------------*/
/**
*将轮廓绘制到轮廓图上并保存
*
*
*/
/*------------------------------------------------------------------------------------------------------------*/
void ImageMemData::DrawContours(void)
 {
	 cui_GeneralImgProcess::InitPNGData(
		ImgData_Contours.get(),
		 ImgWidth,
		 ImgHeight,
		BlackColorPNG);
	 cui_GeneralImgProcess::DrawContoursAroundSegments(
		ImgData_Contours.get(),
		ImgLables.get(),
		ImgWidth,
		ImgHeight,
		WhiteColorPNG);
	 /**/
	 cui_GeneralImgProcess::CuiSaveImageData(
		 ImgData_Contours.get(),
		 ImgWidth,ImgHeight,
		 this->FileReadFullPath,
		 this->FileWritePath,
		 1,
		 "Contours");
 }
/*------------------------------------------------------------------------------------------------------------*/
/**
*获取超像素块的平均大小
*@return 超像素块的大小  
*
*/
/*------------------------------------------------------------------------------------------------------------*/
unsigned long ImageMemData::GetSpSizeAvg(void)
{
	unsigned long SpSize=ImgHeight*ImgWidth/slic_spcount;
	return SpSize;
}
/*------------------------------------------------------------------------------------------------------------*/
/**
*初始化超像素分析的LAb特性
*
*
*/
/*------------------------------------------------------------------------------------------------------------*/
void ImageMemData::InitSPLabProperty(void)
{
	/**************************************************/
	this->SplitImgBGRALab();
	cui_GeneralImgProcess::CalculateAllSpPropertyRange(
		this->ImgLables.get(),
		this->ImgWidth,
		this->ImgHeight,
		this->p_SpProperty,
		this->slic_current_num);

/**************************************************/
	SP_PROPERTY *SpProperty=this->p_SpProperty;
	int* ImgLables=this->ImgLables.get();
	for (int spi=0;spi<this->slic_current_num;spi++){
		int SM=SpProperty[spi].IsInit_SquareMeter;
		SpProperty[spi].lab_color.L_Color=0;
		SpProperty[spi].lab_color.A_Color=0;
		SpProperty[spi].lab_color.B_color=0;
	}

	/**************************************************/
	for(register int hi=0;hi<this->ImgHeight;hi++){
		for (register int j=0;j<this->ImgWidth;j++){	

			int sp=ImgLables[hi*this->ImgWidth+j];//行号就是超像素标号

			SpProperty[sp].lab_color.L_Color+=cvGetReal2D(l_plane,hi,j);
			SpProperty[sp].lab_color.A_Color+=cvGetReal2D(a_plane,hi,j);
			SpProperty[sp].lab_color.B_color+=cvGetReal2D(b_plane,hi,j);

		}

	}

	/**************************************************/
	for (int spi=0;spi<this->slic_current_num;spi++){
		int SM=SpProperty[spi].IsInit_SquareMeter;
		SpProperty[spi].lab_color.L_Color=SpProperty[spi].lab_color.L_Color/SM-127;
		SpProperty[spi].lab_color.A_Color=SpProperty[spi].lab_color.A_Color/SM-127;
		SpProperty[spi].lab_color.B_color=SpProperty[spi].lab_color.B_color/SM-127;
	}
	/**************************************************/

	this->SaveLabProperty("","SpLabColor.data");
}
/*------------------------------------------------------------------------------------------------------------*/
/**
*@param path 文件路径 
*@param filename 文件名 
*
*
*/
/*------------------------------------------------------------------------------------------------------------*/
void ImageMemData::SaveLabProperty(string path,string filename)
{
	/********************************************************/
	SP_PROPERTY *SpProperty=this->p_SpProperty;
#ifdef WINDOWS
	char fname[256];
	char extn[256];
	_splitpath(filename.c_str(), NULL, NULL, fname, extn);
	string temp = fname;
	string finalpath = path + temp + string(".dat");
#else

	string nameandextension =filename;
	size_t pos = filename.find_last_of("/");
	if(pos != string::npos)//if a slash is found, then take the filename with extension
	{
		nameandextension = filename.substr(pos+1);
	}
	string newname = nameandextension.replace(nameandextension.rfind(".")+1, 3, "dat");//find the position of the dot and replace the 3 characters following it.
	string finalpath = path+newname;
#endif
	char data_t[1024];
	ofstream outfile;
	outfile.open(finalpath.c_str(),ios::out);
	
	if (outfile.is_open()){
		for(register int i = 0; i <slic_current_num; i++ ){
					////////////////////
					outfile<<FileReadFullPath<<"   ";
					outfile<<i<<"  ";					
					////////////////////
					double value_L=SpProperty[i].lab_color.L_Color;
					sprintf_s(data_t,1024," %4.6e ",value_L);
					outfile<<data_t;
					///////////////////
					double value_a=SpProperty[i].lab_color.A_Color;
					sprintf_s(data_t,1024," %4.6e",value_a);
					outfile<<data_t;
					///////////////////
					double value_b=SpProperty[i].lab_color.B_color;
					sprintf_s(data_t,1024," %4.6e",value_b);
					outfile<<data_t;
					///////////////////
				outfile<<endl;
			} 
	}else{
		ASSERT(0);
	}
	
	outfile.close();
	/********************************************************/
}
/*-----------------------------------------------------------------------------*/
/**
*分割图像的每一个通道\n
*BGRA分割成4个通道图像
*Lab分割成3个通道
*/
/*-----------------------------------------------------------------------------*/
void ImageMemData::SplitImgBGRALab(void)
{
	ASSERT(src_Img_bgra->nChannels==4);	
	if (l_plane!=nullptr){
		return;
	}  
	if (a_plane!=nullptr){
		return;
	}  
	if (b_plane!=nullptr){
		return;
	}  
	/*---------------------------------------------------------------------*/
	IplImage* Lab_Image=cvCreateImage(cvGetSize(src_Img_bgra),IPL_DEPTH_8U,3);
	IplImage* BGR_Image=cvCreateImage(cvGetSize(src_Img_bgra),IPL_DEPTH_8U,3);
	l_plane=cvCreateImage(cvGetSize(src_Img_bgra),IPL_DEPTH_8U,1);
	a_plane=cvCreateImage(cvGetSize(src_Img_bgra),IPL_DEPTH_8U,1);
	b_plane=cvCreateImage(cvGetSize(src_Img_bgra),IPL_DEPTH_8U,1);	

	cvCvtColor(src_Img_bgra,BGR_Image,CV_BGRA2BGR);
	cvCvtColor(BGR_Image,Lab_Image,CV_BGR2Lab);

	cvCvtPixToPlane(Lab_Image,l_plane,a_plane,b_plane,NULL);
	cvReleaseImage(&Lab_Image);
	cvReleaseImage(&BGR_Image);
	/*---------------------------------------------------------------------*/
}
/*-----------------------------------------------------------------------------*/
/**
*图像各种BGRA转换到Hsv 
*
*
*/
/*-----------------------------------------------------------------------------*/
void ImageMemData::SplitImgBGRAHsv(void)
{
	ASSERT(src_Img_bgra->nChannels==4);	
	if (h_plane!=nullptr){
		return;
	}  
	if (s_plane!=nullptr){
		return;
	}  
	if (v_plane!=nullptr){
		return;
	}  
	/*---------------------------------------------------------------------*/
	IplImage* Hsv_Image=cvCreateImage(cvGetSize(src_Img_bgra),IPL_DEPTH_8U,3);
	IplImage* BGR_Image=cvCreateImage(cvGetSize(src_Img_bgra),IPL_DEPTH_8U,3);
	h_plane=cvCreateImage(cvGetSize(src_Img_bgra),IPL_DEPTH_8U,1);
	s_plane=cvCreateImage(cvGetSize(src_Img_bgra),IPL_DEPTH_8U,1);
	v_plane=cvCreateImage(cvGetSize(src_Img_bgra),IPL_DEPTH_8U,1);	

	cvCvtColor(src_Img_bgra,BGR_Image,CV_BGRA2BGR);
	cvCvtColor(BGR_Image,Hsv_Image,CV_BGR2HSV);

	cvCvtPixToPlane(Hsv_Image,h_plane,s_plane,v_plane,NULL);
	cvReleaseImage(&Hsv_Image);
	cvReleaseImage(&BGR_Image);
	/*---------------------------------------------------------------------*/
}
/*-----------------------------------------------------------------------------*/
/**
*保存超像素的属性
*
*
*/
/*-----------------------------------------------------------------------------*/
void ImageMemData::SaveSpProperty(void)
{

#if Use_CString&&_MSC_VER
CString filepath;
	CString filename("SpProperty.xml");
	CString filefullpath=filepath+filename;

	string xml_save_path=FileNameSplit::ConvertCS2string(filefullpath);
	/*---------------------------------------*/
	CvFileStorage *fsW=cvOpenFileStorage(xml_save_path.c_str(),0,CV_STORAGE_APPEND);		
	cvStartWriteStruct(fsW,"Data",CV_NODE_MAP,NULL,cvAttrList(0,0));

	
	//cvWriteReal(fsW,"img_focus",img_focus);

	cvEndWriteStruct(fsW);
	cvReleaseFileStorage(&fsW);
#endif
	
}
/*-----------------------------------------------------------------------------*/
/**
*保存超像素的个数
*
*
*/
/*-----------------------------------------------------------------------------*/

void ImageMemData::SaveSuperPixelNum()
{
	string Property=__FUNCTION__;
    StrReplace(Property,"ImageMemData::","");

	string xml_cgf_path=Property+ ".xml";
	CvFileStorage *fsW=cvOpenFileStorage(xml_cgf_path.c_str(),0, CV_STORAGE_APPEND, "GB2312");	
	
	cvStartWriteStruct(fsW,"Data",CV_NODE_MAP,"TEST");
	cvWriteString(fsW,"FileName",this->FileReadFullPath.c_str());
	cvWriteInt(fsW,Property.c_str(),this->slic_current_num);    
	cvEndWriteStruct(fsW);  

	cvReleaseFileStorage(&fsW);
}
/*-----------------------------------------------------------------------------*/
/**
*保存颜色迭代时超像素的个数
*
*
*/
/*-----------------------------------------------------------------------------*/
void ImageMemData::SaveColorSpectralClusteringNum()
{
	string Property=__FUNCTION__;
	StrReplace(Property,"ImageMemData::","");

	string xml_cgf_path=Property+ ".xml";
	CvFileStorage *fsW=cvOpenFileStorage(xml_cgf_path.c_str(),0, CV_STORAGE_APPEND,"GB2312");	

	cvStartWriteStruct(fsW,"Data",CV_NODE_MAP,"TEST");
	cvWriteString(fsW,"FileName",this->FileReadFullPath.c_str());
	cvWriteInt(fsW,Property.c_str(),this->slic_current_num);    
	cvEndWriteStruct(fsW);  

	cvReleaseFileStorage(&fsW);
}
/*-----------------------------------------------------------------------------*/
/**
*保存颜色迭代时超像素的个数
*
*
*/
/*-----------------------------------------------------------------------------*/
void ImageMemData::SaveColorIterationNum()
{
	string Property=__FUNCTION__;
	StrReplace(Property,"ImageMemData::","");

	string xml_cgf_path=Property+ ".xml";
	CvFileStorage *fsW=cvOpenFileStorage(xml_cgf_path.c_str(),0, CV_STORAGE_APPEND,"GB2312");	

	cvStartWriteStruct(fsW,"Data",CV_NODE_MAP,"TEST");
	cvWriteString(fsW,"FileName",this->FileReadFullPath.c_str());
	cvWriteInt(fsW,Property.c_str(),this->slic_current_num);    
	cvEndWriteStruct(fsW);  

	cvReleaseFileStorage(&fsW);
}
/*-----------------------------------------------------------------------------*/
/**
*保存形态谱聚类个数
*
*
*/
/*-----------------------------------------------------------------------------*/
void ImageMemData::SaveShapeSpectralClusteringNum()
{
	string Property=__FUNCTION__;
	StrReplace(Property,"ImageMemData::","");

	string xml_cgf_path=Property+ ".xml";
	CvFileStorage *fsW=cvOpenFileStorage(xml_cgf_path.c_str(),0, CV_STORAGE_APPEND,"GB2312");	

	cvStartWriteStruct(fsW,"Data",CV_NODE_MAP,"TEST");
	cvWriteString(fsW,"FileName",this->FileReadFullPath.c_str());
	cvWriteInt(fsW,Property.c_str(),this->slic_current_num);    
	cvEndWriteStruct(fsW);  

	cvReleaseFileStorage(&fsW);
}
/*-----------------------------------------------------------------------------*/
/**
*保持模糊函数分割的垂直立面个数
*@param VerticalNum 立面个数 
*
*/
/*-----------------------------------------------------------------------------*/
void ImageMemData::SaveFuzzyVerticalNum(int VerticalNum)
{
	string Property=__FUNCTION__;
	StrReplace(Property,"ImageMemData::","");

	string xml_cgf_path=Property+ ".xml";
	CvFileStorage *fsW=cvOpenFileStorage(xml_cgf_path.c_str(),0, CV_STORAGE_APPEND,"GB2312");	

	cvStartWriteStruct(fsW,"Data",CV_NODE_MAP,"TEST");
	cvWriteString(fsW,"FileName",this->FileReadFullPath.c_str());
	cvWriteInt(fsW,Property.c_str(),VerticalNum);    
	cvEndWriteStruct(fsW);  

	cvReleaseFileStorage(&fsW);
}
/*-----------------------------------------------------------------------------*/
/**
*保存条状立面个数
*
*
*/
/*-----------------------------------------------------------------------------*/
void ImageMemData::SaveStripSPVerticalNum(int VerticalNum)
{
	string Property=__FUNCTION__;
	StrReplace(Property,"ImageMemData::","");

	string xml_cgf_path=Property+ ".xml";
	CvFileStorage *fsW=cvOpenFileStorage(xml_cgf_path.c_str(),0, CV_STORAGE_APPEND,"GB2312");	

	cvStartWriteStruct(fsW,"Data",CV_NODE_MAP,"TEST");
	cvWriteString(fsW,"FileName",this->FileReadFullPath.c_str());
	cvWriteInt(fsW,Property.c_str(),VerticalNum);    
	cvEndWriteStruct(fsW);  

	cvReleaseFileStorage(&fsW);
}
/*-----------------------------------------------------------------------------*/
/**
*
*@deprecated
*
*/
/*-----------------------------------------------------------------------------*/
void ImageMemData::StrReplace(string& src,string sub_old,string sub_new)
{
	
	int pos;
	pos =src.find(sub_old);////查找指定的串
	while(pos != -1){
		src.replace(pos,sub_old.length(),sub_new);////用新的串替换掉指定的串
		pos = src.find(sub_old);//////继续查找指定的串，直到所有的都找到为止
	}
}
/*-----------------------------------------------------------------------------*/
/**
*设置屏蔽Lables   
*@param  屏蔽图像 
*
*/
/*-----------------------------------------------------------------------------*/
void ImageMemData::SetImgMaskLables(IplImage *lables)
{
	ASSERT(lables->depth==IPL_DEPTH_8U);
	ASSERT(lables->nChannels=4);
	this->src_ImgMaskLables=cvCloneImage(lables);
}
/*-----------------------------------------------------------------------------*/
/**
*
*
*
*/
/*-----------------------------------------------------------------------------*/

