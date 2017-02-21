#include "StdAfx.h"
#include "module_all_cui.h"
//#include "CuiHough3.h"
//#include <assert.h>	
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif
//#include "../SceneDetermine.h"
/*------------------------------------------------------------------------------------------------------------*/
/**
param1
第一个方法相关的参数:
对传统 Hough 变换，不使用(0).
对概率 Hough 变换，它是最小线段长度.
对多尺度 Hough 变换，它是距离精度 rho 的分母 (大致的距离精度是 rho 而精确的应该是 rho / param1 ).
param2
第二个方法相关参数:
对传统 Hough 变换，不使用 (0).
对概率 Hough 变换，这个参数表示在同一条直线上进行碎线段连接的最大间隔值(gap), 即当同一条直线上的两
条碎线段之间的间隔小于param2时，将其合二为一。
对多尺度 Hough 变换，它是角度精度 theta 的分母 (大致的角度精度是 theta 而精确的角度应该是 theta / param2).

*/
/*------------------------------------------------------------------------------------------------------------*/
#define  SHOW_IMG  FALSE

#define  C_Used2FindVP	 22222
/***************************/
#define LINE_UP	(0)
#define LINE_HORIZONTAL  (1)
#define LINE_DOWN (2)
/*------------------------------------------------------------------------------------------------------------*/
/**
*构造函数初始化成员变量（已弃用）
*
*/
/*------------------------------------------------------------------------------------------------------------*/
CuiHough3::CuiHough3(void)
{
   this->pMD=NULL;
   this->InitParam();
}
/*------------------------------------------------------------------------------------------------------------*/
/**
*构造函数初始化成员变量
*导入图像计算用的数据结构
*@param MemData_t 计算用的中间变量
*/
/*------------------------------------------------------------------------------------------------------------*/
CuiHough3::CuiHough3(ImageMemData* MemData_t)
{	
  this->pMD=MemData_t;
  this->InitParam();
}
/*------------------------------------------------------------------------------------------------------------*/
/**
*释放计算过程中分配的内存
*
*/
/*------------------------------------------------------------------------------------------------------------*/
CuiHough3::~CuiHough3(void)
{
	
#if SHOW_IMG
  cvDestroyAllWindows();
#endif
  this->ClearImgMemory();
  cvReleaseMemStorage(&storage);
}
/*------------------------------------------------------------------------------------------------------------*/
/**
*初始化成员变量
*
*/
/*------------------------------------------------------------------------------------------------------------*/
 void CuiHough3::InitParam(void){
#if SHOW_IMG
	 cvNamedWindow( "Source",  CV_WINDOW_NORMAL );
	 cvNamedWindow( "Hough",  CV_WINDOW_NORMAL );
	 cvNamedWindow("src", CV_WINDOW_NORMAL);

#endif
	 gray_img=NULL;
	 gray_binary_img=NULL;
	 gray_Color_src_img=NULL;
	 gray_Color_contour_img=NULL;
	 src_img=NULL;
	 lines=NULL;
	 cui_line_srcimg=NULL;
	 cui_ImgData=NULL;
	 cui_ImgLables=NULL;
	 cui_ImgLables_SVG=NULL;
	 DistortionIMGLables=NULL;
	 cui_Matrix_Category_Lable=NULL;
	 cui_ImgData_Contour=NULL;
	 cui_ImgData_Contour_GV=NULL;
	 cui_line_VGcontour=NULL;
	 cui_line_contour=NULL;
	 memset(&GndSegment,0,sizeof(GroundSegment));
	 storage=cvCreateMemStorage(0);
 }
/*------------------------------------------------------------------------------------------------------------*/
/**
*
*分配计算用的内存
*/
/*------------------------------------------------------------------------------------------------------------*/
void CuiHough3::ClearImgMemory(void)
{
	if (gray_img){
		cvReleaseImage(&gray_img);
	}
	if (gray_binary_img){
		cvReleaseImage(&gray_binary_img);
	}
	if (gray_Color_src_img){
		cvReleaseImage(&gray_Color_src_img);
	}
	if (src_img){
		cvReleaseImage(&src_img);
	}
	if (gray_Color_contour_img){
		cvReleaseImage(&gray_Color_contour_img);
	}
	if (cui_line_srcimg){
		delete []cui_line_srcimg;
		cui_line_srcimg=NULL;
	}
	if (cui_ImgData){
		delete[]cui_ImgData;
		cui_ImgData=NULL;
	}
	if (cui_ImgLables){
		 delete[]cui_ImgLables;
		 cui_ImgLables=NULL;
	}
	if (cui_Matrix_Category_Lable){
	   delete[]cui_Matrix_Category_Lable;
	   cui_Matrix_Category_Lable=NULL;
	}
	if (cui_ImgLables_SVG){
		delete []cui_ImgLables_SVG;
		cui_ImgLables_SVG=NULL;
	}
	if (cui_ImgData_Contour){
		delete []cui_ImgData_Contour;
		cui_ImgData_Contour=NULL;
	}
	if (cui_line_contour){
		delete []cui_line_contour;
		cui_line_contour=NULL;
	}
	if (cui_line_VGcontour){
		delete []cui_line_VGcontour;
		cui_line_VGcontour=NULL;
	}
	if (cui_ImgData_Contour_GV){
		delete []cui_ImgData_Contour_GV;
		cui_ImgData_Contour_GV=NULL;
	}
	if(DistortionIMGLables){
		delete [] DistortionIMGLables;
		DistortionIMGLables=NULL;
	}

	
}
/*------------------------------------------------------------------------------------------------------------*/
/**
*通过文件名获取图片的数据
*@param img_filename 文件名
*/
/*------------------------------------------------------------------------------------------------------------*/
void CuiHough3::GetImageData(string img_filename){
	this->ClearImgMemory();
	this->src_img=cvLoadImage(img_filename.c_str());
}
/*------------------------------------------------------------------------------------------------------------*/
/**
*通过IplImage数据结构导入内存
*@param imgscr 图片对应的原始数据
*/
/*------------------------------------------------------------------------------------------------------------*/
void CuiHough3::GetImageData(IplImage* imgscr)
{
	this->ClearImgMemory();
	this->src_img=imgscr;
}
/*------------------------------------------------------------------------------------------------------------*/
/**
*通过内存流直接导入数据
*
*@param ImgData  图片数据
*@param Width    图片宽度
*@param Height   图片高度
*@param Lables   图片的超像素标定数据
*@param LablesSVG 图片的天空、立面、地面标定数据
*@param NumLabels 图片中超像素的个数
*@param Category  每个超像素块对应的分类情况
*/
/*------------------------------------------------------------------------------------------------------------*/
void CuiHough3::GetImageData(
	UINT32* ImgData,int Width,int Height,
	int*  Lables,int *LablesSVG,int NumLabels,
	INT32* Category)
{
	this->ClearImgMemory();
	this->cui_Width=Width;
	this->cui_Height=Height;
	cui_ImgData=new UINT32[Width*Height];
	memcpy(cui_ImgData,ImgData,sizeof(UINT32)*Width*Height);

	cui_ImgData_Contour=new UINT32[Width*Height];
	//memset(cui_ImgData_Contour,0,sizeof(UINT32)*Width*Height);
	//for (register int x=0;x<Width;x++){
	//	for (register int y=0;y<Height;y++){
	//		cui_ImgData_Contour[y*Width+x]=0xff000000;
	//	}
	//}
	cui_GeneralImgProcess::InitPNGData(cui_ImgData_Contour,Width,Height,BlackColorPNG);
	cui_ImgLables=new int[Width*Height];	
	memcpy(cui_ImgLables,Lables,sizeof(int)*Width*Height);

	cui_ImgData_Contour_GV=new UINT32[Width*Height];
	//memset(cui_ImgData_Contour_GV,0,sizeof(UINT32)*Width*Height);
	//for (register int x=0;x<Width;x++){
	//	for (register int y=0;y<Height;y++){
	//		cui_ImgData_Contour_GV[y*Width+x]=0xff000000;
	//	}
	//}
	cui_GeneralImgProcess::InitPNGData(cui_ImgData_Contour_GV,Width,Height,BlackColorPNG);
	cui_ImgLables_SVG=new int[Width*Height];	
	memcpy(cui_ImgLables_SVG,LablesSVG,sizeof(int)*Width*Height);

	cui_Matrix_Category_Lable=new INT32[NumLabels];
	memcpy(cui_Matrix_Category_Lable,Category,sizeof(INT32)*NumLabels);
	this->src_img=cvCreateImage(cvSize(Width,Height),IPL_DEPTH_8U,4);
	memcpy(this->src_img->imageData,this->cui_ImgData,this->src_img->imageSize);

   cui_GeneralImgProcess::DrawContoursAroundSegments(cui_ImgData_Contour,cui_ImgLables,
	   cui_Width,cui_Height,0xffffffff);
   cui_GeneralImgProcess::CuiSaveImageData(cui_ImgData_Contour,cui_Width,cui_Height,
	   pMD->FileReadFullPath,pMD->FileWritePath,1,"Contours");

   this->SetGVsegmentPoint();

}
/*------------------------------------------------------------------------------------------------------------*/
/**
*计算地面与立面的分界线，
*将分界线轮廓保存到cui_ImgData_Contour_GV中
*
*
*/
/*------------------------------------------------------------------------------------------------------------*/
void CuiHough3::SetGVsegmentPoint(void)
{
	 int *cui_ImgLables_SVG_t=new int[cui_Width*cui_Height];
   /***********************************/
   memcpy(cui_ImgLables_SVG_t,cui_ImgLables_SVG,sizeof(UINT32)*cui_Width*cui_Height);
		 for (register int x=0;x<cui_Width;x++){
			 for (register int y=0;y<cui_Height;y++){
				 if (cui_ImgLables_SVG_t[y*cui_Width+x]==Sky){
					 cui_ImgLables_SVG_t[y*cui_Width+x]=Vertical;
				 }
			 }
		 }
	
	cui_GeneralImgProcess::DrawContoursAroundSegments(cui_ImgData_Contour_GV,cui_ImgLables_SVG_t,cui_Width,cui_Height,0xffffffff);
    delete []cui_ImgLables_SVG_t;	 
	GV_Segment_point.clear();
	/*cui_ImgData_Contour_GV取分界线的点*/
	for (register int x=0;x<cui_Width;x++){
		for (register int y=0;y<cui_Height;y++){
			   if (0!=(cui_ImgData_Contour_GV[y*cui_Width+x]&0x00ffffff)){

				   GV_Segment_point.push_back(cvPoint(x,y));			   
			   		//break;
			   }
		}
	}
	//memset(cui_ImgData_Contour_GV,0,sizeof(UINT32)*cui_Width*cui_Height);
	//for (register int x=0;x<cui_Width;x++){
	//	for (register int y=0;y<cui_Height;y++){
	//		cui_ImgData_Contour_GV[y*cui_Width+x]=0xff000000;
	//	}
	//}
	cui_GeneralImgProcess::InitPNGData(cui_ImgData_Contour_GV,cui_Width,cui_Height,BlackColorPNG);
	/*cui_ImgData_Contour_GV重新绘制分界线*/
  	for (unsigned int pi=0;pi<GV_Segment_point.size();pi++){
		CvPoint ContourPoint=cvPoint(GV_Segment_point[pi].x,GV_Segment_point[pi].y);
		cui_ImgData_Contour_GV[ContourPoint.y*cui_Width+ContourPoint.x]=WhiteColorPNG;
  	}
   
   cui_GeneralImgProcess::CuiSaveImageData(cui_ImgData_Contour_GV,cui_Width,cui_Height,
	   pMD->FileReadFullPath,pMD->FileWritePath,1,"Contours_GV");
}
/*------------------------------------------------------------------------------------------------------------*/
/**
*在一张图像上寻找最长线
*@parma[in] src 要寻找的图像
*/
/*------------------------------------------------------------------------------------------------------------*/
int CuiHough3::findLongestLine(IplImage* src)
{
	IplImage* dst = cvCreateImage( cvGetSize(src), 8, 1 ); 
	IplImage* src1=cvCreateImage(cvGetSize(src),IPL_DEPTH_8U,1); //灰度
	
	IplImage* color_dst = cvCreateImage( cvGetSize(src), 8, 3 );
	CvMemStorage* storage = cvCreateMemStorage(0); 
	
	CvSeq* lines = 0;
	int i;
	

	cvCvtColor(src, src1, CV_BGR2GRAY);  
	//cvCopy(src,src1);
	cvCanny( src1, dst, 50, 200, 3 );

	cvCvtColor( dst, color_dst, CV_GRAY2BGR );

#if SHOW_IMG

	cvShowImage("src",dst); //灰度
#endif
	

	lines = cvHoughLines2( dst, storage, CV_HOUGH_PROBABILISTIC, 1, CV_PI/180, 10, 30, 20 );
	printf("%d\n",lines->total);
	int dis=0;
	int max=0;
	int j=0;
	CvPoint* line;
	CvPoint pointOne;
	CvPoint pointTwo;
	int *a=new int[lines->total*4];
	for( i = 0; i < lines->total; i++ )
	{
		line = (CvPoint*)cvGetSeqElem(lines,i);

		dis=(line[1].y-line[0].y)*(line[1].y-line[0].y)+(line[1].x-line[0].x)*(line[1].x-line[0].x);

		//  pointOne[i].x=line[0].x;
		//   pointOne[i].y=line[0].y;
		//    pointTwo[i].x=line[1].x;
		//pointTwo[i].y=line[1].y;
		a[4*i]=line[0].x;
		a[4*i+1]=line[0].y;
		a[4*i+2]=line[1].x;
		a[4*i+3]=line[1].y;
		if(dis>max){
			max=dis;
			j=i;
		}
		// cvLine( color_dst, line[0], line[1], CV_RGB(255,0,0), 3, 8 );
	}
	pointOne.x=a[4*j];
	pointOne.y=a[4*j+1];
	pointTwo.x=a[4*j+2];
	pointTwo.y=a[4*j+3];
	cvLine( color_dst, pointOne, pointTwo, CV_RGB(255,0,0), 3, 8 );    //画出最长的直线


	double Angle=0.0;

	Angle = atan2(fabsl(pointTwo.y-pointOne.y),fabsl(pointTwo.x-pointOne.x));   //得到最长直线与水平夹角

	if(pointTwo.x>pointOne.x && pointTwo.y>pointOne.y)
	{
		Angle=-Angle;
	}

	Angle=Angle*180/CV_PI;
	cout<<"角度 "<<Angle<<endl;
	
	cvShowImage( "Source", src ); 	
	cvShowImage( "Hough", color_dst );


   return 0;
}
/*------------------------------------------------------------------------------------------------------------*/
/**
*移除直线统计直方图中过于水平和垂直的直线
*以便观察和计算出寻找灭点所用的直线
*@param hist_data 直方图数据
*@param hist_size 直方图大小
/*------------------------------------------------------------------------------------------------------------*/
void   CuiHough3::RemoveHistData0and90(float *hist_data,int hist_size){
	//去掉0/180度和
	hist_data[0]=0;
	hist_data[1]=0;
	hist_data[hist_size-2]=0;
	hist_data[hist_size-1]=0;
	/****90度数据************/
	hist_data[hist_size/2-1]=0;
	hist_data[hist_size/2]=0;
	hist_data[hist_size/2+1]=0;
}
/*------------------------------------------------------------------------------------------------------------*/
/**
*运用cany算子，src_unknow二值化图像
*
*@param src_unknow 原图可以为任意图像
*
*/
/*------------------------------------------------------------------------------------------------------------*/
int CuiHough3::CuiUseCany2binaryzation(IplImage* src_unknow)
{
	IplImage* gray = cvCreateImage(cvGetSize(src_unknow), 8, 1 ); 
		IplImage* dst_binaryzation = cvCreateImage( cvGetSize(src_unknow), 8, 1 ); 		
		 cvShowImage( "Source", src_unknow ); 

		cvCvtColor(src_unknow, gray, CV_BGR2GRAY);  
	
	cvCanny( gray,dst_binaryzation, 50, 200, 3 );

	

	cvShowImage("src",dst_binaryzation); //灰度
	return 0;
}
/*------------------------------------------------------------------------------------------------------------*/
/**
*获取图像上所有的直线
*
*@param src_img_t[in]		原图内存数据
*@param gray_Color_img_t [out] 得到灰度图像
*@param plinedata [out] 输出的直线数据
*@param pline_len [out] 直线数据
*@param int method [in] 图像平滑方法
*/
/*------------------------------------------------------------------------------------------------------------*/
void CuiHough3::GetAllhoughLineData(
	IplImage **src_img_t,
	IplImage **gray_Color_img_t,
	LINEDATA **plinedata,
	int *pline_len,
	int method){	
	if (gray_binary_img){
		cvReleaseImage(&gray_binary_img);		
	}
	if (gray_img){
		cvReleaseImage(&gray_img);
	}
	if (*gray_Color_img_t){
		cvReleaseImage(gray_Color_img_t);		
	}
	gray_binary_img = cvCreateImage( cvGetSize(*src_img_t),IPL_DEPTH_8U, 1 ); 
	gray_img=cvCreateImage(cvGetSize(*src_img_t),IPL_DEPTH_8U,1); //灰度
	*gray_Color_img_t = cvCreateImage( cvGetSize(*src_img_t), 8, 3 );
	/*******************************/
	this->ImgPreprocess(src_img_t,method);
	/*******************************/
	cvCvtColor( gray_binary_img, *gray_Color_img_t, CV_GRAY2BGR );
#if SHOW_IMG
cvShowImage("src",gray_binary_img); //灰度
#else

#endif
	CvPoint* point_t;
	//2013-11-19lines = cvHoughLines2(gray_binary_img, storage, CV_HOUGH_PROBABILISTIC, 1, CV_PI/180,35,20,4);
	//2013-11-19lines = cvHoughLines2(gray_binary_img, storage, CV_HOUGH_PROBABILISTIC, 1, CV_PI/180,25,20,4);
	//
if (method==1){
	//轮廓---另算
	cvSaveImage("gray_binary_img.jpg",gray_binary_img);
	IplImage* color_img_t=cvLoadImage("gray_binary_img.jpg");
	cvConvertImage(color_img_t,gray_binary_img,CV_BGR2GRAY);
	cvReleaseImage(&color_img_t);
	//其中有个大BUG
	lines = cvHoughLines2(gray_binary_img, storage, CV_HOUGH_PROBABILISTIC, 1, CV_PI/180,20,20,2);
}else if (method==2){  
	//原图
	lines = cvHoughLines2(gray_binary_img, storage, CV_HOUGH_PROBABILISTIC, 1, CV_PI/180,35,20,4);
}else{
	ASSERT(0);
}
	
	printf("%d\n",lines->total);
/************************************************/	
	if (*plinedata){
		delete [](*plinedata);
	}
	*pline_len=lines->total;
	*plinedata=new LINEDATA[*pline_len];
	LINEDATA *linedata=*plinedata;
	memset(*plinedata,0,sizeof(LINEDATA)*(*pline_len));
/*************************************************/	
	for( int i= 0; i < lines->total; i++ )
	{
		point_t = (CvPoint*)cvGetSeqElem(lines,i);
		memcpy(&(linedata[i]).pointOne,&point_t[0],sizeof(CvPoint));
		memcpy(&(linedata[i]).pointTwo,&point_t[1],sizeof(CvPoint));
		linedata[i].LineLength=sqrtl(powl(point_t[1].y-point_t[0].y,2)+powl(point_t[1].x-point_t[0].x,2));
		/********************************************************************************************************/
		if (point_t[1].x-point_t[0].x){
			linedata[i].alpha_radian=atanl(1.0*(point_t[1].y-point_t[0].y)/(point_t[1].x-point_t[0].x));
			if (linedata[i].alpha_radian<0){
				linedata[i].alpha_radian=CV_PI+linedata[i].alpha_radian;
			}
		}else{
				linedata[i].alpha_radian=CV_PI/2;
		   //正负90度歧义
		}
				linedata[i].alpha_angle=linedata[i].alpha_radian*180/CV_PI;
		/********************************************************************************************************/
		
		double a,b;
		if (point_t[1].x-point_t[0].x){
			linedata[i].b_org=linedata[i].b=b=1.0*(point_t[1].y*point_t[0].x-point_t[0].y*point_t[1].x)/(point_t[0].x-point_t[1].x);
			linedata[i].a_org=linedata[i].a=a=1.0*(point_t[0].y-point_t[1].y)/(point_t[0].x-point_t[1].x);
			linedata[i].p=fabsl(b/sqrtl(powl(a,2)+powl(-1,2)));
		}else{
			linedata[i].p=fabsl(point_t[1].y-point_t[0].y);
		}
		/**************分类************************/
			linedata[i].Category=Unclassify;
		
	}
	 memcpy(cui_line_cpy,*plinedata,min(MAXLINEDATA,lines->total)*sizeof(LINEDATA));
	 this->AdjustLine(*plinedata,*pline_len);


}
/*------------------------------------------------------------------------------------------------------------*/
/**
*获得超像素轮廓的hough变换数据
*
*/
/*------------------------------------------------------------------------------------------------------------*/
void CuiHough3::GetAllHoughLineByContour(void)
{
	   IplImage *imgContour;
		
		 imgContour=cvCreateImage(cvSize(cui_Width,cui_Height),IPL_DEPTH_8U,4);
#if 0
	   for (register int x=0;x<cui_Width;x++){
			 for(register int y=0;y<cui_Height;y++){
				 if (cui_ImgLables_SVG[y*cui_Width+x]==Ground){
					 
				 }else{
					  cui_ImgData_Contour[y*cui_Width+x]=0;
				 }
					
			 }
		 }
#endif


	  memcpy(imgContour->imageData,cui_ImgData_Contour,imgContour->imageSize);

		
		 this->GetAllhoughLineData(&imgContour,&gray_Color_contour_img,&cui_line_contour,&cui_line_contour_len,1);

#ifdef TEST_CUI
		 /******显示直线*********************/
//		 this->cui_ShowImageLine("Classify_1_",0);
		 this->GetAllhoughLineData(&imgContour,&gray_Color_contour_img,&cui_line_contour,&cui_line_contour_len,1);
#else 	
		 /**********分类直线*******************/
		 this->ClassifyLine(cui_line_contour,cui_line_contour_len,gray_Color_contour_img,"Contour_");

#endif  
		 /*********计算有效直线************/
		 this->CalculateEffectiveLine(cui_line_contour,cui_line_contour_len,&hist_contours,gray_Color_contour_img,"Contour_");
		
		 cvSaveImage("contour.jpg",imgContour);
		 cvReleaseImage(&imgContour);

}
/*------------------------------------------------------------------------------------------------------------*/
/**
*获得立面和地面之间轮廓的hough变换数据
*
*/
/*------------------------------------------------------------------------------------------------------------*/
void CuiHough3::GetAllHoughLineByContourVG(void)
{
	IplImage *imgContourVG;

	imgContourVG=cvCreateImage(cvSize(cui_Width,cui_Height),IPL_DEPTH_8U,4);
	memcpy(imgContourVG->imageData,cui_ImgData_Contour_GV,imgContourVG->imageSize);
	this->GetAllhoughLineData(&imgContourVG,&gray_Color_contour_img,&cui_line_VGcontour,&cui_line_VGcontour_len,1);

	/**********分类直线*******************/
	this->ClassifyLine(cui_line_VGcontour,cui_line_VGcontour_len,gray_Color_contour_img,"Contour_VG_");


	/*********计算有效直线************/
	this->CalculateEffectiveLine(cui_line_VGcontour,cui_line_VGcontour_len,&hist_contours_VG,gray_Color_contour_img,"Contour_VG_");

	cvSaveImage("contourVG.jpg",imgContourVG);
	cvReleaseImage(&imgContourVG);

}
/*------------------------------------------------------------------------------------------------------------*/
/**
*获得立面和地面之间轮廓的hough变换数据
*
*/
/*------------------------------------------------------------------------------------------------------------*/
void CuiHough3:: GetAllHoughLineBySourceImg(void)
{
	  this->GetAllhoughLineData(&src_img,&gray_Color_src_img,
		  &cui_line_srcimg,&cui_line_src_len,2);
//#ifdef TEST_CUI
//	  /******显示直线*********************/
//	 
//	  this->GetAllhoughLineData(&src_img,&gray_Color_src_img,&cui_line_srcimg,&cui_line_src_len,2);
//#else 	
	  /**********分类直线*******************/
	  this->ClassifyLine(cui_line_srcimg,cui_line_src_len,gray_Color_src_img,"Classify1_src");

//#endif  
	  /*********计算有效直线************/
	  this->DetermineSceneCategory(&hist_scrimg,
		  cui_line_srcimg,cui_line_src_len);

	  this->CalculateEffectiveLine(cui_line_srcimg,cui_line_src_len,
		  &hist_scrimg,gray_Color_src_img,"SrcImg");
}
/*------------------------------------------------------------------------------------------------------------*/
/**
*在图像上显示数据，并保存到数据
*
*@param string filename 保存的文件名
*@param directline 显示直线的分类
*@param gray_Color_img 灰度图像
*@param cui_line_data [in]   直线数据
*@param line_len [in]        直线长度
*
*/
/*------------------------------------------------------------------------------------------------------------*/
void CuiHough3::cui_ShowImageLine(
	string filename,
	int directline,
	IplImage *gray_Color_img,
	LINEDATA *cui_line_data,
	int line_len)
{
  CvScalar color_use;
  //CvScalar color_tab[16];
  int thickness;
  IplImage *src_img_t,*gray_Color_src_img_t;
  src_img_t=cvCloneImage(src_img);
  gray_Color_src_img_t=cvCloneImage(gray_Color_img);
#if 0 
  CvRNG rng_state = cvRNG(0xffffffff);
  color_tab[0] = CV_RGB(255,0,0);
  color_tab[1] = CV_RGB(0,255,0);
  color_tab[2] = CV_RGB(100,100,255);
  color_tab[3] = CV_RGB(255,0,255);
  color_tab[4] = CV_RGB(255,255,0);
  for (int i=5;i<16;i++){
	  color_tab[i]=CV_RGB(cvRandInt(&rng_state)%255,cvRandInt(&rng_state)%255,cvRandInt(&rng_state)%255);
  }
#endif
   for( int i= 0; i <line_len; i++ ){
/*#if 0
	color_use=color_tab[((int)cui_line_srcimg[i].theta_angle+90)/30];
#endif*/	
	  thickness=Getthickness(cui_line_data[i].Category);
	  if (thickness<=0) continue;
	  color_use=GetGradientColor(cui_line_data[i].alpha_angle);
	  if (directline==C_Used2FindVP){
		  	if(cui_line_srcimg[i].Used2FindVP==C_Used2FindVP){
				cvLine(src_img_t, cui_line_data[i].AdjPoint1,cui_line_data[i].AdjPoint2,color_use,thickness, 8 );   
				cvLine(gray_Color_src_img_t,cui_line_data[i].AdjPoint1,cui_line_data[i].AdjPoint2,color_use,thickness, 8 );  
		  	}
	  }else{
	    cvLine(src_img_t, cui_line_data[i].pointOne,cui_line_data[i].pointTwo,color_use,thickness, 8 );   
	    cvLine(gray_Color_src_img_t,cui_line_data[i].pointOne,cui_line_data[i].pointTwo,color_use,thickness, 8 );   
	  }
     
   }
  
#if SHOW_IMG
	cvShowImage( "Source", src_img_t); 	
	cvShowImage( "Hough",gray_Color_src_img_t);
	cvSaveImage("C:\\Users\\Administrator\\Desktop\\src_img.jpg",src_img_t);
	cvSaveImage("C:\\Users\\Administrator\\Desktop\\gray_Color_src_img.jpg",gray_Color_src_img_t);
	cvSaveImage("C:\\Documents and Settings\\Administrator\\桌面\\src_img.jpg",src_img_t);
	cvSaveImage("C:\\Documents and Settings\\Administrator\\桌面\\gray_Color_src_img.jpg",gray_Color_src_img_t);
#endif 
	string src_filename,gray_Color_filename;
	src_filename=filename+"src_img.jpg";
	gray_Color_filename=filename+"gray_Color_src_img.jpg";
	cvSaveImage(src_filename.c_str(),src_img_t);
	cvSaveImage(gray_Color_filename.c_str(),gray_Color_src_img_t);
  /**************************************************************/
   cvReleaseImage(&src_img_t);
   cvReleaseImage(&gray_Color_src_img_t);
 
}
/*------------------------------------------------------------------------------------------------------------*/
/**
*根据角度从HSV颜色空间获取颜色
*
*@param angle_color HSV颜色空间中H的变化范围0-180
*
*/
/*------------------------------------------------------------------------------------------------------------*/

 CvScalar  CuiHough3::GetGradientColor(double angle_color)
 {
	   CvMat mat;
	   CvScalar color;	   
	   unsigned char  color_4[4];
#if 0
	   color_4[0]=180;
	   color_4[1]=80*cos(angle_color)+100;
	   color_4[2]=80*sin(angle_color)+100;
	   color_4[3]=0;
	   cvInitMatHeader(&mat,1,1,CV_8UC3,&color_4);
	   cvCvtColor(&mat,&mat,CV_Lab2BGR);
#endif
	   assert(angle_color<=180);
	   color_4[0]=(unsigned char)angle_color;
	   color_4[1]=255;
	   color_4[2]=255;
	   color_4[3]=0;
	   cvInitMatHeader(&mat,1,1,CV_8UC3,&color_4);
	   cvCvtColor(&mat,&mat,CV_HSV2BGR);
	  
	   // b g r a
	   // l a b 0
	   color=cvScalar(color_4[0],color_4[1],color_4[2],0);
	   
	   return color;
}
/*------------------------------------------------------------------------------------------------------------*/
/**
*获得绘图时，直线的宽度
*
*@param category_t 天空、立面、地面的分类
*/
/*------------------------------------------------------------------------------------------------------------*/
int  CuiHough3::Getthickness(int category_t){
#ifdef TEST_CUI
	return 1;
#endif
	if (category_t==Ground){
		return 1;
	 }else if (category_t==Vertical){
		 return 1;
	 }else if (category_t==Sky){
		 return -1;		 
	 }else if(category_t==Remove){
	 	 return -1;
	 }else if (category_t==Unclassify){
		 return 1;
	 }else{
		 return -1;
	 }
	
 }
/*------------------------------------------------------------------------------------------------------------*/
/**
*将直方图绘制到图片上，并保存到硬盘
*@param winname  文件名
*@param hist_data[in] 直方图数据
*
*/
/*------------------------------------------------------------------------------------------------------------*/
 void CuiHough3::DrawHistogramOnImg(string winname,HistData* hist_data){
	 CvHistogram *hist=NULL;
	 IplImage  *hist_matrix=NULL;
	 int	hist_size[1]={180};
	 float  angle_ranges[]={0,181};
	 float *ranges[]={angle_ranges};
	 /*********************************/
	 memset(hist_data->hist_all,0,sizeof(float)*AngLeDivided); 
	 hist=cvCreateHist(1,hist_size,CV_HIST_ARRAY,ranges);

	
	 for (register int i=0;i<cui_line_src_len;i++){
		   int angle_index;
		   angle_index=cvFloor(cui_line_srcimg[i].alpha_angle);
		   hist_data->hist_all[angle_index]+=cui_line_srcimg[i].LineLength;
	 } 

	 for (register int i=0;i<AngLeDivided;i++){
		float* p=cvGetHistValue_1D(hist,i);
			*p=hist_data->hist_all[i];
	 }
#if 1
	  cvNormalizeHist(hist,1.0);
#endif
 /********************************************************/


	  /** 获取直方图统计的最大值，用于动态显示直方图 */
	  float max_value;
	  int h_bins=180;
	  cvGetMinMaxHistValue( hist, 0, &max_value, 0, 0 );


	  /** 设置直方图显示图像 */
	  int height = 240;
	  int bin_w=6;
	  int width =h_bins*bin_w;
	  IplImage* hist_img = cvCreateImage( cvSize(width,height), 8, 4 );
#if SHOW_IMG
	  cvShowImage(winname.c_str(),hist_img);
#endif	 
	  cvZero( hist_img );
	  /** 用来进行HSV到RGB颜色转换的临时单位图像 */

	  for(int h = 0; h < h_bins; h++){
		  /** 获得直方图中的统计次数，计算显示在图像中的高度 */
#if 1
		  float bin_val = cvQueryHistValue_1D( hist, h);
#else
		  float bin_val =hist_value[h];
#endif

		  int intensity = cvRound(bin_val*height/max_value);

		  /** 获得当前直方图代表的颜色，转换成RGB用于绘制 */

		  CvScalar color=GetGradientColor(h);

		  cvRectangle( hist_img, cvPoint(h*bin_w,height),
			  cvPoint((h+1)*bin_w,height - intensity),
			  color, -1, 8, 0 );

	  }	 
#if SHOW_IMG
	 cvShowImage(winname.c_str(),hist_img);
#else
	 winname+=".jpg";
	 cvSaveImage(winname.c_str(),hist_img);
#endif
	
	cvReleaseHist(&hist);
	cvReleaseImage(&hist_img);
 }
/*------------------------------------------------------------------------------------------------------------*/
/**
*对直线进行分类，确定每条直线所属的分类
*直线分类包括 天空 立面 地面
*
*@param line_data [in]直线数据
*@param line_len  [in]直线数量
*@param gray_Color_img [in] 原始图像对应的灰度 
*@param add_t 要保存的文件名附加字符串
*/
/*------------------------------------------------------------------------------------------------------------*/
 void CuiHough3::ClassifyLine(
	 LINEDATA *line_data,
	 int line_len,
	 IplImage *gray_Color_img,
	 string add_t)
 {
	 for( int i= 0; i <line_len; i++ ){
		 line_data[i].Category=this->CheckLineArea(line_data[i].pointOne,line_data[i].pointTwo);
	 }
	 memcpy(cui_line_cpy,line_data,min(MAXLINEDATA,line_len)*sizeof(LINEDATA));
	 /******显示直线*********************/
	 this->cui_ShowImageLine(add_t,0,gray_Color_img,line_data,line_len);
	 //here IS a BUg
	// 2013-11-30 
 }
 /*------------------------------------------------------------------------------------------------------------*/
/**
*检测直线位于天空、立面还是地面
*
*@param point1 线段的其中一端点
*@param point2 线段的其中一端点
*@return 返回直线的位置
*- SKY 线段在天空区域
*- VERTICAL 线段在立面区域
*- GROUND 线段在地面区域
*- Remove 线段跨区域
*/
/*------------------------------------------------------------------------------------------------------------*/
 int CuiHough3::CheckLineArea(CvPoint point1,CvPoint point2)
 {
	 int position1,position2;
	 int lable_index1,lable_index2;
	 lable_index1=cui_ImgLables[point1.y*src_img->width+point1.x];
	 lable_index2=cui_ImgLables[point2.y*src_img->width+point2.x];
	 position1=cui_Matrix_Category_Lable[lable_index1];
	 position2=cui_Matrix_Category_Lable[lable_index2];
	 if (position1==position2){
		 return position1;
	 }else{

		 return Remove;
	 }
 }
/*------------------------------------------------------------------------------------------------------------*/
/**
*根据图像的直线集获取直线对应的直方图，并保存到硬盘
*
*@param filename  直方图文件名
*@parma [in] linedata  直线数据
*@param [in] linelen   直线数量
*@param [out] hist_data 直方图数据结构
*/
/*------------------------------------------------------------------------------------------------------------*/
 void CuiHough3::GetHistogramSVG2(
	 string filename,
	 LINEDATA* linedata,
	 int linelen,
	 HistData *hist_data){
  
	 CvHistogram *hist[HistNUM]={NULL};
	 IplImage  *hist_matrix=NULL;
	 int	hist_size[1]={AngLeDivided};
	 float  angle_ranges[]={0,AngLeDivided+1};
	 float *ranges[]={angle_ranges};
	 /*********************************/
	 memset(hist_data,0,sizeof(HistData));
	
	  for (int i=0;i<HistNUM;i++){
		 hist[i]=cvCreateHist(1,hist_size,CV_HIST_ARRAY,ranges);
	  }
	  /****统计直方图*****************************/
#if 0
	  for (register int i=0;i<linelen;i++){
		  int angle_index=cvFloor(linedata[i].alpha_angle*AngLeDivided/180.0);	

		  if(linedata[i].Category==Ground){
			  hist_data->hist_ground[angle_index]+=linedata[i].LineLength;
		  }else if (linedata[i].Category==Sky){
			  hist_data->hist_sky[angle_index]+=linedata[i].LineLength;
		  }else if (linedata[i].Category==Vertical){
			  hist_data->hist_vertical[angle_index]+=linedata[i].LineLength;
		  } 
		  if (linedata[i].Category!=Remove){
			  hist_data->hist_all[angle_index]+=linedata[i].LineLength;
		  }

	  } 
	  /***************************************************************/
	  for (register int ai=0;ai<AngLeDivided;ai++){
		  hist_data->hist_Multiply[ai]=hist_data->hist_ground[ai]*hist_data->hist_vertical[ai];
	  }
#else
	  HoughAnalyze::StatisticsHistogramByLine(filename,linedata,linelen,hist_data,AngLeDivided);
#endif
	
	 /****************************************************************/
	 for (int ci=0;ci<HistNUM;ci++){
		 for (register int i=0;i<AngLeDivided;i++){
			 if (ci==0){
				 *(cvGetHistValue_1D(hist[ci],i))=hist_data->hist_sky[i];
			 }else if (ci==1){
				 *(cvGetHistValue_1D(hist[ci],i))=hist_data->hist_ground[i];
			 }else if (ci==2){
				 *(cvGetHistValue_1D(hist[ci],i))=hist_data->hist_vertical[i];
			 }else if (ci==3){
				 *(cvGetHistValue_1D(hist[ci],i))=hist_data->hist_all[i];
			 }else if(ci==4){
			 	 *(cvGetHistValue_1D(hist[ci],i))=hist_data->hist_Multiply[i];
			 }
		 }
	 }
	
/*************************************************************************************************************************/
/** 设置直方图显示图像 */
	 int height = 200;
	 char  text_buff_t[1024];
	 int bin_w=1800/AngLeDivided;
	 int h_bins=AngLeDivided;
	 int width =h_bins*bin_w;
	 IplImage* hist_img = cvCreateImage( cvSize(width,height*HistNUM), 8, 4 ); 
	 cvZero( hist_img );
	   for (int ci=0;ci<HistNUM;ci++){
#if 1
						cvNormalizeHist(hist[ci],1.0);
						if (ci==0){
						    memcpy(hist_data->hist_sky,cvGetHistValue_1D(hist[ci],0),sizeof(float)*AngLeDivided);
						}else if (ci==1){
						    memcpy(hist_data->hist_ground,cvGetHistValue_1D(hist[ci],0),sizeof(float)*AngLeDivided);
						}else if (ci==2){
							memcpy(hist_data->hist_vertical,cvGetHistValue_1D(hist[ci],0),sizeof(float)*AngLeDivided);
						}else if (ci==3){
							memcpy(hist_data->hist_all,cvGetHistValue_1D(hist[ci],0),sizeof(float)*AngLeDivided);							
						}else if (ci==4){
							memcpy(hist_data->hist_Multiply,cvGetHistValue_1D(hist[ci],0),sizeof(float)*AngLeDivided);
						}
						
#endif
						/** 获取直方图统计的最大值，用于动态显示直方图 */
						float max_value;
						
						cvGetMinMaxHistValue( hist[ci], 0, &max_value, 0, 0 );


						
						
						/** 用来进行HSV到RGB颜色转换的临时单位图像 */
						cvSetImageROI(hist_img,cvRect(0,hist_img->height/HistNUM*ci,hist_img->width,hist_img->height));
						for(int h = 0; h < h_bins; h++){
							/** 获得直方图中的统计次数，计算显示在图像中的高度 */
#if 1
							float bin_val = cvQueryHistValue_1D( hist[ci], h);
#else
							float bin_val; 
							if (ci==0){
								bin_val=hist_sky[h];
							}else if (ci==1){
								bin_val=hist_ground[h];
							}else if (ci==2){
								bin_val=hist_vertical[h];
							}else if (ci==3){
							    bin_val=hist_all[h];
							}
#endif

							int intensity =cvRound(bin_val*height/max_value);
							intensity=intensity<0?0:intensity;
							/** 获得当前直方图代表的颜色，转换成RGB用于绘制 */

							CvScalar color=GetGradientColor(1.0*h/AngLeDivided*180);
							
							CvFont font;
							cvInitFont(&font,CV_FONT_VECTOR0,0.3,0.3,0,1,8);
							//在图像中显示文本字符串
							sprintf(text_buff_t,"%0.2f",bin_val*100/max_value);
							cvPutText(hist_img,text_buff_t,cvPoint(h*bin_w,height - intensity-10),&font,CV_RGB(255,255,255));
							
							cvRectangle( hist_img, cvPoint(h*bin_w,height),cvPoint((h+1)*bin_w,height - intensity),color,-1, 8, 0 );
							sprintf(text_buff_t,"%d",h*180/AngLeDivided);
							cvPutText(hist_img,text_buff_t,cvPoint(h*bin_w,height),&font,CV_RGB(255,255,255));
						}	
						cvResetImageROI(hist_img);
#if SHOW_IMG
			/*			if (ci==0){
							cvShowImage("sky_h.jpg",hist_img);	
						}else if (ci==1){
							cvShowImage("ground_h.jpg",hist_img);	
						}else if (ci==2){
							cvShowImage("vertical_h.jpg",hist_img);	
						}
			*/			   
						cvShowImage("sgvall.jpg",hist_img); 
			
						cvSaveImage("sgvall.jpg",hist_img);
			
							
#endif
						
						
 

}
/*************************************************************************************************************************/
	   filename+="SGVAM.jpg";
	   cvSaveImage(filename.c_str(),hist_img);
	   cvReleaseImage(&hist_img); 
     for (int i=0;i<HistNUM;i++){
		   cvReleaseHist(&hist[i]);
	 }	
 }

/*------------------------------------------------------------------------------------------------------------*/
/**
*
*删除不必要的直线组（找灭点用）
*@param [in]  linedata  直线数据
*@param [in] linelen    直线数量
*@param [out] hist_data 直线对应的直方图数据
*@param [in] gray_color_img 原图对应的灰度图像
*@param file_add 附加文件名
*
*/
/*------------------------------------------------------------------------------------------------------------*/
 void CuiHough3::CalculateEffectiveLine(
	 LINEDATA* linedata,
	 int linelen,
	 HistData *hist_data,
	 IplImage* gray_color_img,
	 string file_add){
	 this->GetHistogramSVG2(file_add+"ORG_Histogram2",linedata,linelen,hist_data);
	 if (file_add!= "Contour_VG_") {
		 /***********************************/
		this->RemoveLineVertical90(linedata,linelen);
		/***********************************/
		this->GetHistogramSVG2(file_add+"NO90_Histogram2",linedata,linelen,hist_data);
		/******显示直线*********************/

	 }	 
	 this->cui_ShowImageLine(file_add+"Classify_NO90_",1,gray_color_img,linedata,linelen);
 }
/*------------------------------------------------------------------------------------------------------------*/
/**
*图像的预处理，对原始图像进行二值化处理
*
*@param src_img_t 原始图像三通道
*@param  method  
*- 1 原图本身为黑白图像，直接拷贝
*- 2 对其运用小波降噪对原图进行滤波
*/
/*------------------------------------------------------------------------------------------------------------*/
 void CuiHough3::ImgPreprocess(
	 IplImage **src_img_t,
	 int method){

	 cvCvtColor(*src_img_t, gray_img, CV_BGR2GRAY);  
	 /*******************************/
	//cvSmooth(gray_img,gray_img); 
/*
	 //中值滤波 
	 cvSmooth(gray_img,gray_img,CV_MEDIAN,3,3,0,0);
	   //邻域平均滤波
	 cvSmooth(gray_img,gray_img,CV_BLUR,3,3,0,0);           //3x3
*/
#if 0
	 cvErode(gray_img,gray_img);
	 cvDilate(gray_img,gray_img);
#endif


  if (method==1){
	  /*cvErode(gray_img,gray_img);
	  cvDilate(gray_img,gray_img);*/
	  cvCopyImage(gray_img,gray_binary_img);

  }else if (method==2){


	if (cui_Width*cui_Height>1000000){
		CuiHarrTransformLet harr_smooth;
		harr_smooth.CuiLoadImgBuffer(cui_ImgData,cui_Width,cui_Height);
		IplImage *img_harr_smooth;
		img_harr_smooth=harr_smooth.HarrSmooth(1);
		cvResize(img_harr_smooth,gray_img);	
	}else{
		 cvErode(gray_img,gray_img);
		cvDilate(gray_img,gray_img);
	}
	  cvCanny(gray_img,gray_binary_img, 40, 200, 3 );	
  }
	 /**********************
	
	 cvSmooth(pImg,pImg,CV_BLUR,5,5,0,0);          //5x5
	 
	 cvSmooth(pImg,pImg,CV_MEDIAN,3,3,0,0);    //3x3
	 cvSmooth(pImg,pImg,CV_MEDIAN,5,5,0,0);    //5x5
	 //高斯滤波
	 cvSmooth(pImg,pImg,CV_GAUSSIAN,3,3,0,0);//3x3
	 cvSmooth(pImg,pImg,CV_GAUSSIAN,5,5,0,0);//5x5
	 *******************/
	 /*******************************/

	 cvSaveImage("gray_img.jpg",gray_img);
	 cvSaveImage("gray_binary_img.jpg",gray_binary_img);
	/* cvSaveImage("gray_Color_src_img.jpg",gray_Color_src_img);
	 cvSaveImage("gray_Color_contour_img.jpg",gray_Color_contour_img);*/
 }
/*------------------------------------------------------------------------------------------------------------*/
/**
*去除直线组中90度的直线，方便观察和计算
*@param line_data 直线数据
*@param line_len  直线长度
*/
/*------------------------------------------------------------------------------------------------------------*/
 void CuiHough3::RemoveLineVertical90(
	 LINEDATA* line_data,
	 int line_len)
 {
	 for (register int li=0;li<line_len;li++){
		 if (line_data[li].alpha_angle==90){
				 line_data[li].Category=Remove;
		 }
	 }
 }
/*------------------------------------------------------------------------------------------------------------*/
/**
*决定图片的场景
*
*@param [in] hist_data  直方图数据
*@param [in] linedata 直线数据
*@param  linelen  直线数量
*/
/*------------------------------------------------------------------------------------------------------------*/
 void CuiHough3::DetermineSceneCategory(
	 HistData *hist_data,
	 LINEDATA* linedata,
	 int linelen)
 {
	 this->GetHistogramSVG2("SenceDetermine_",linedata,linelen,hist_data);
	 SceneDetermine Sence_t(pMD);
	 Sence_t.SetSVGLabel(this->cui_ImgLables_SVG);
	 this->ImageEenvironment=Sence_t.DetermineScene(*hist_data);
	 Sence_t.SaveBulidingTypeInfo("","Manhattan.data");
 }

/*------------------------------------------------------------------------------------------------------------*/
/**
*调整直线，计算直线与X=0，X=img_width的交点
*便于显示
*
*@param [in] cui_line_data 直线数据
*@param cui_line_len  直线长度
*
*/
/*------------------------------------------------------------------------------------------------------------*/
void CuiHough3::AdjustLine(
	 LINEDATA *cui_line_data,
	 int cui_line_len)
 {
	  CvPoint point_t[4];
	 for (register int li=0;li<cui_line_len;li++){

			 point_t[0].x=cui_line_data[li].AdjPoint1.x=0;
			 point_t[0].y=cui_line_data[li].AdjPoint1.y=cui_line_data[li].a*point_t[0].x+cui_line_data[li].b;
			 point_t[1].x=cui_line_data[li].AdjPoint2.x=src_img->width;
			 point_t[1].y=cui_line_data[li].AdjPoint2.y=cui_line_data[li].a*point_t[1].x+cui_line_data[li].b;
		 }	 
		 
}
/*------------------------------------------------------------------------------------------------------------*/
/**
*将灭点绘制到图像上，图像保存到硬盘
*
*@param GNDSegline  非零，绘制切割地面用的分界线
*
*/
/*------------------------------------------------------------------------------------------------------------*/
void CuiHough3::DrawVPonImg(int GNDSegline)
 {
	 CvScalar color_use;
	 int thickness=1;
	 IplImage *src_img_t,*gray_Color_src_img_t;
	 string src_filename,gray_Color_filename;
	 string filename="VP";
	 
#if 0
	 for( int i= 0; i <this->cui_line_src_len; i++ ){
		 thickness=Getthickness(cui_line_srcimg[i].Category);
		 if (thickness<=0) continue;
		 color_use=GetGradientColor(cui_line_srcimg[i].alpha_angle);
		 cvLine(src_img_t, cui_line_srcimg[i].pointOne,cui_line_srcimg[i].pointTwo,color_use,thickness, 8 );   
		 cvLine(gray_Color_src_img_t,cui_line_srcimg[i].pointOne,cui_line_srcimg[i].pointTwo,color_use,thickness, 8 );   
	 }
#endif
#if 1
   /******绘制N跳线************************************************************/
	
	 for (register int lefti=0;lefti<2;lefti++){
		 const char * f_tab[2]={"al","bl"}; 
		// src_img_t=cvCloneImage(src_img);
	     gray_Color_src_img_t=cvCloneImage(gray_Color_src_img);
		// src_filename=filename+f_tab[lefti]+"src_img_VP.jpg";
		 gray_Color_filename=filename+f_tab[lefti]+"gray_Color_src_img_VP.jpg";
		 for(register unsigned int pi=0;pi<VP_V_L_UP[lefti].PreparePoint.size();pi++){
				
			 color_use=GetGradientColor((VP_V_L_UP[lefti].Vp_angle_down+VP_V_L_UP[lefti].Vp_angle_up)/2);
			// cvLine(src_img_t,cvPoint(0,VP_V_L_UP[lefti].PreparePoint[pi].y),cvPoint(src_img->width,VP_V_L_UP[lefti].PreparePoint[pi].y),color_use,thickness, 8 );   
			 cvLine(gray_Color_src_img_t,cvPoint(0,VP_V_L_UP[lefti].PreparePoint[pi].y),cvPoint(src_img->width,VP_V_L_UP[lefti].PreparePoint[pi].y),color_use,thickness, 8 ); 
		 }		
		// cvSaveImage(src_filename.c_str(),src_img_t);
		 cvSaveImage(gray_Color_filename.c_str(),gray_Color_src_img_t);
		// cvReleaseImage(&src_img_t);
		 cvReleaseImage(&gray_Color_src_img_t);


	 }
	 for (register unsigned int lefti=0;lefti<2;lefti++){
		 const char * f_tab[2]={"cr","dr"}; 
		// src_img_t=cvCloneImage(src_img);
		 gray_Color_src_img_t=cvCloneImage(gray_Color_src_img);
		 //src_filename=filename+f_tab[lefti]+"src_img_VP.jpg";
		 gray_Color_filename=filename+f_tab[lefti]+"gray_Color_src_img_VP.jpg";
		 for(register unsigned int pi=0;pi<VP_V_R_DOWN[lefti].PreparePoint.size();pi++){
			 color_use=GetGradientColor((VP_V_R_DOWN[lefti].Vp_angle_down+VP_V_R_DOWN[lefti].Vp_angle_up)/2);
			// cvLine(src_img_t,cvPoint(0,VP_V_R_DOWN[lefti].PreparePoint[pi].y),cvPoint(src_img->width,VP_V_R_DOWN[lefti].PreparePoint[pi].y),color_use,thickness, 8 );   
			 cvLine(gray_Color_src_img_t,cvPoint(0,VP_V_R_DOWN[lefti].PreparePoint[pi].y),cvPoint(src_img->width,VP_V_R_DOWN[lefti].PreparePoint[pi].y),color_use,thickness, 8 );
		 }
		// cvSaveImage(src_filename.c_str(),src_img_t);
		 cvSaveImage(gray_Color_filename.c_str(),gray_Color_src_img_t);
		// cvReleaseImage(&src_img_t);
		 cvReleaseImage(&gray_Color_src_img_t);
	 }
	
		 // src_img_t=cvCloneImage(src_img);
		 gray_Color_src_img_t=cvCloneImage(gray_Color_src_img);
		 //src_filename=filename+f_tab[lefti]+"src_img_VP.jpg";
		 gray_Color_filename=filename+"4Line_img_VP.jpg";
	 /**绘制4==1条-条线****************************************************************/
	 for (register int lefti=0;lefti<2;lefti++){
		
		 color_use=GetGradientColor((VP_V_L_UP[lefti].Vp_angle_down+VP_V_L_UP[lefti].Vp_angle_up)/2);
		// cvLine(src_img_t,cvPoint(0,VP_V_L_UP[lefti].Vpoint.y),cvPoint(src_img->width,VP_V_L_UP[lefti].Vpoint.y),color_use,thickness, 8 );   
		 cvLine(gray_Color_src_img_t,cvPoint(0,VP_V_L_UP[lefti].Vpoint.y),cvPoint(src_img->width,VP_V_L_UP[lefti].Vpoint.y),color_use,thickness, 8 ); 
		color_use=GetGradientColor((VP_V_R_DOWN[lefti].Vp_angle_down+VP_V_R_DOWN[lefti].Vp_angle_up)/2);
		//cvLine(src_img_t,cvPoint(0,VP_V_R_DOWN[lefti].Vpoint.y),cvPoint(src_img->width,VP_V_R_DOWN[lefti].Vpoint.y),color_use,thickness, 8 );   
		cvLine(gray_Color_src_img_t,cvPoint(0,VP_V_R_DOWN[lefti].Vpoint.y),cvPoint(src_img->width,VP_V_R_DOWN[lefti].Vpoint.y),color_use,thickness, 8 );
		
	 } 
	 if (GNDSegline){ 
		 /*****水平线***********/
		 cvLine(gray_Color_src_img_t,cvPoint(0,GndSegment.VanishingPoint.y),cvPoint(gray_Color_src_img->width,GndSegment.VanishingPoint.y),cvScalar(255,255,255),2);
		 for (register int i=0;i<3;i++){
			  color_use=GetGradientColor(GndSegment.GndSegLine[i].alpha_angle);
			  cvLine(gray_Color_src_img_t,GndSegment.GndSegLine[i].pointOne,GndSegment.GndSegLine[i].pointTwo,color_use,3, 8 );
			   cvLine(gray_Color_src_img_t,GndSegment.GndSegLine[i].AdjPoint1,GndSegment.GndSegLine[i].AdjPoint2,color_use,1, 8 );
		  }
		
	 }
	 
	 // cvSaveImage(src_filename.c_str(),src_img_t);
		cvSaveImage(gray_Color_filename.c_str(),gray_Color_src_img_t);
		// cvReleaseImage(&src_img_t);
		cvReleaseImage(&gray_Color_src_img_t);
#endif
	
	
	
 }
/*------------------------------------------------------------------------------------------------------------*/
/**
*计算寻找灭点时左右两边的方向线，哪一边更强
*@retval -1 左边，单调增的方向线更强
*@retval 1  右边，单调减的方向线更强
*/
/*------------------------------------------------------------------------------------------------------------*/
int CuiHough3::SelectVPLineLeftorRight(void)
 {
	int LorR=0,LlineNum=0,RightLineNum=0;

	for (register int ci=0;ci<2;ci++){
	  LlineNum+=VP_V_L_UP[ci].PrepareLine.size();
	}
	for (register int ci=0;ci<2;ci++){
		RightLineNum+=VP_V_R_DOWN[ci].PrepareLine.size();
	}
	if (LlineNum>RightLineNum){
		//left用于计算
	  return -1;
	}else{
		//right用于计算
	  return 1;
	}
	
 }
/*------------------------------------------------------------------------------------------------------------*/
/**
*根据直方图数据 估算出灭点的个数
*
*@param[in] hist_data 直方图数据
*@param scale 直方图两侧方向线强度比值的临界值，
*
*@return 大于零表示单灭点或多灭点中，单调递增线组中寻找灭点更为可靠
*大于零表示单灭点或多灭点中，单调递减线组中寻找灭点更为可靠
*
*/
/*------------------------------------------------------------------------------------------------------------*/
int CuiHough3::CheckVPNumberbyHist(
	HistData* hist_data,
	double scale)
{
	float hist_vertical_t[AngLeDivided],all_sum=0,left_up_sum=0,right_down_sum=0,left_right_scale=0;
	memcpy(hist_vertical_t,hist_data->hist_vertical,sizeof(hist_vertical_t));
	//去掉0/180度和
	hist_vertical_t[0]=0;
	hist_vertical_t[AngLeDivided-1]=0;
	/****90度数据************/
	hist_vertical_t[AngLeDivided/2-1]=0;
	hist_vertical_t[AngLeDivided/2]=0;
	hist_vertical_t[AngLeDivided/2+1]=0;
	for(register int i=0;i<AngLeDivided;i++){
		if (i<AngLeDivided/2){
			left_up_sum+=hist_vertical_t[i];
		}else if (i>AngLeDivided/2){
			right_down_sum+=hist_vertical_t[i];
		}
		all_sum+=hist_vertical_t[i];
	}
	left_up_sum/=all_sum;
	right_down_sum/=all_sum;

	if (left_up_sum>scale){
		//单灭点 ，up组寻找
		return -1;
	}else if (right_down_sum>scale){
		//单灭点，down组寻找
		return 1;
	}else if (max(left_up_sum,right_down_sum)<scale){
		
		if (left_up_sum>right_down_sum){
			//双灭点 up组更有效
             return -2;
		}else{
			//双灭点。down组更有效
			 return 2;
		}
	}else{
		ASSERT(0);
		return 0;
	}
}
/*------------------------------------------------------------------------------------------------------------*/
/**
*将灭点设置到gnddata数据中
*
*@param [out] gnddata 切割地面用数据结构，包括直线组合灭点组
*@param  p1st 灭点一
*@param  p2nd 灭点二
*@param  point_num 灭点个数
*@param  Img_width 图像宽度（用于计算灭点位置）
*
*/
/*------------------------------------------------------------------------------------------------------------*/
void    CuiHough3::SetPoint2GndData(
	GroundSegment * gnddata,
	VpPoint p1st,
	VpPoint p2nd,
	int point_num,
	int Img_width){
	
	double P1_third,P2_third;
	P1_third=1.0/3*src_img->width;
	P2_third=2.0/3*src_img->width;
	////Left_【0】左 Right 在【1】////////////////////////////
	if (abs(point_num)==1){
		//right--(+ 1
		gnddata->VanishingPoint=p1st.point;
		gnddata->VanishPoint[0]=p1st.point;
		gnddata->vpPoint[0]=p1st;
		gnddata->VPnumber=abs(point_num);
	}else if (abs(point_num)==2){		//
		gnddata->VanishingPoint=p1st.point;
		gnddata->VanishPoint[0]=p1st.point;
		gnddata->VanishPoint[1]=p2nd.point;
		gnddata->vpPoint[0]=p1st;
		gnddata->vpPoint[1]=p2nd;
		gnddata->VPnumber=abs(point_num);
	}
	/************************************************************/
	for (register int i=0;i<gnddata->VPnumber;i++){
		if (gnddata->VanishPoint[i].x<P1_third){
			   gnddata->VPposition[i]=VanishPosition::LEFT;

		}else if (gnddata->VanishPoint[i].x>=P1_third&&gnddata->VanishPoint[i].x<=P2_third){
			  gnddata->VPposition[i]=VanishPosition::MIDDLE;
		}else if (gnddata->VanishPoint[i].x>=P2_third){

			  gnddata->VPposition[i]=VanishPosition::RIGHT;
			  
		}
	}
	/*************************************************************/
	this->CalculateEffetiveVPoint(gnddata->VPnumber);
	/*************************************************************/
if (gnddata->VPnumber==1){

	if (GndSegment.VanishingPoint.x<P1_third){
		//left point
		GndSegment.VPointPositionIndex[LINE_UP]=1;
		GndSegment.VPointPositionIndex[LINE_HORIZONTAL]=1;
		GndSegment.VPointPositionIndex[LINE_DOWN]=0;

	}else if (GndSegment.VanishingPoint.x>=P1_third&&GndSegment.VanishingPoint.x<=P2_third){
		//mid
		GndSegment.VPointPositionIndex[LINE_UP]=1;
		GndSegment.VPointPositionIndex[LINE_HORIZONTAL]=1;
		GndSegment.VPointPositionIndex[LINE_DOWN]=1;

	}else if(GndSegment.VanishingPoint.x>P2_third){
		//right
		GndSegment.VPointPositionIndex[LINE_UP]=0;
		GndSegment.VPointPositionIndex[LINE_HORIZONTAL]=1;
		GndSegment.VPointPositionIndex[LINE_DOWN]=1;
	}
	//point  (-1,373)
	//VPPInedx 1 0 0
	//VPP      1   {1,-1}
}else if (gnddata->VPnumber==2){
	GndSegment.VPointPositionIndex[LINE_UP]=1;
	GndSegment.VPointPositionIndex[LINE_HORIZONTAL]=1;
	GndSegment.VPointPositionIndex[LINE_DOWN]=1;
}



}
/*------------------------------------------------------------------------------------------------------------*/
/**
*如果找到两个灭点，比较两灭点的位置，舍弃其中一个灭点
*
*@param line_in_use 未使用的形参
*
*/
/*------------------------------------------------------------------------------------------------------------*/
void CuiHough3::CalculateEffetiveVPoint(int line_in_use)
{
	if (2==GndSegment.VPnumber){
		///////////////////////////////////////////////////////////////
		if ((GndSegment.VPposition[0]==VanishPosition::RIGHT)){
				if ((GndSegment.VPposition[1]==VanishPosition::LEFT)){
					//判断距离 移除second
					if (abs(GndSegment.vpPoint[0].point.y-GndSegment.vpPoint[1].point.y)>100){
						 GndSegment.VPnumber=1;
					}

				}else{
					//移出second 灭点
				   GndSegment.VPnumber=1;
				}
		}
		///////////////////////////////////////////////////////////////
			if ((GndSegment.VPposition[0]==VanishPosition::LEFT)){
				if ((GndSegment.VPposition[1]==VanishPosition::RIGHT)){
					//判断距离 移除second
					if (abs(GndSegment.vpPoint[0].point.y-GndSegment.vpPoint[1].point.y)>100){
						 GndSegment.VPnumber=1;
					}

				}else{
					//移出second 灭点
					GndSegment.VPnumber=1;
				}
		}
		/////////////////////////////////////////////////////////////////
	 }
	

}
/*------------------------------------------------------------------------------------------------------------*/
/**
*寻找灭点，同一个方向不同角度的直线相交获取直线
*
*/
/*------------------------------------------------------------------------------------------------------------*/
void CuiHough3::FindVPbyLeftorRightCross(void)
{  		
	/**********************************/
	vector<CvPoint> point_dbg;
	VpPoint cross_t[2]; 
#if 1

 	int   line_in_use=this->CheckVPNumberbyHist(&hist_scrimg);
	
	switch(line_in_use){
	case -1:{
			//left组线，未必是left点			
			cross_t[0]=this->FindVPby2ClusterLine(&VP_V_L_UP[0],&VP_V_L_UP[1]);
			SetPoint2GndData(&this->GndSegment,cross_t[0],cross_t[0],line_in_use,cui_Width);
		    break;
			}
		
	case 1:{
			//right组
		   cross_t[0]=this->FindVPby2ClusterLine(&VP_V_R_DOWN[0],&VP_V_R_DOWN[1]);
		   SetPoint2GndData(&this->GndSegment,cross_t[0],cross_t[0],line_in_use,cui_Width);
		   break;
		}
		
	case 2:{
			//right组 更强
			cross_t[0]=this->FindVPby2ClusterLine(&VP_V_R_DOWN[0],&VP_V_R_DOWN[1]);
			cross_t[1]=this->FindVPby2ClusterLine(&VP_V_L_UP[0],&VP_V_L_UP[1]);			
			SetPoint2GndData(&this->GndSegment,cross_t[0],cross_t[1],line_in_use,cui_Width);
			break;
		   }
	case -2:{
			 //left组 更强
			cross_t[0]=this->FindVPby2ClusterLine(&VP_V_L_UP[0],&VP_V_L_UP[1]);
			cross_t[1]=this->FindVPby2ClusterLine(&VP_V_R_DOWN[0],&VP_V_R_DOWN[1]);
			SetPoint2GndData(&this->GndSegment,cross_t[0],cross_t[1],line_in_use,cui_Width);
			break;
	}		
	default:
		break;
	}
#endif
	/************************************************/	
	this->DrawVPonImg();
}
/*------------------------------------------------------------------------------------------------------------*/
/**
*通过两簇直线寻找灭点（方法已弃用）
*
*/
/*------------------------------------------------------------------------------------------------------------*/
VpPoint CuiHough3::FindVPby2ClusterLine(
	VanishingPointData * VP_dat0,
	VanishingPointData* VP_dat1)
{
	/**********************************/
	//vector<CvPoint> point_dbg;
	CvPoint cross_t; 
	/***********计算候选点************************************/
	for(register unsigned int li=0;li<VP_dat0->PrepareLine.size();li++){
		for (register unsigned int lj=0;lj<VP_dat1->PrepareLine.size();lj++){

			if (VP_dat0->PrepareLine[li].a-VP_dat1->PrepareLine[lj].a){
				//相交
				cross_t.x=1.0*(VP_dat0->PrepareLine[li].b-VP_dat1->PrepareLine[lj].b)/(VP_dat1->PrepareLine[lj].a-VP_dat0->PrepareLine[li].a);
				cross_t.y=VP_dat0->PrepareLine[li].b+cross_t.x*VP_dat0->PrepareLine[li].a;
				if (cross_t.y>=0&&cross_t.y<src_img->height){
				/*	point_dbg.push_back(cross_t);*/
					VP_dat0->PreparePoint.push_back(cross_t);
					VP_dat1->PreparePoint.push_back(cross_t);
				}

			}else{
				//除数为0	  //两直线平行 
			}
		}
	} 
	/*********中位数筛选候选点***************************************/		
	return this->SelectVPbyMedianIteration(VP_dat0->PreparePoint,1);
}
/*------------------------------------------------------------------------------------------------------------*/
/**
*在灭点的从候选点集中找到正确的灭点
*
*/
/*------------------------------------------------------------------------------------------------------------*/
void CuiHough3::MatchingVP(void)
{
	vector<CvPoint> point_dbg;
	CvPoint cross_t;
	/***********计算候选点************************************/
	for (register int lefti=0;lefti<2;lefti++){
		for(register unsigned int li=0;li<VP_V_L_UP[lefti].PrepareLine.size();li++){
			for (register unsigned int lj=li+1;lj<VP_V_L_UP[lefti].PrepareLine.size();lj++){

				if (VP_V_L_UP[lefti].PrepareLine[li].a-VP_V_L_UP[lefti].PrepareLine[lj].a){
					//相交
					cross_t.x=1.0*(VP_V_L_UP[lefti].PrepareLine[lj].b-VP_V_L_UP[lefti].PrepareLine[li].b)/(VP_V_L_UP[lefti].PrepareLine[li].a-VP_V_L_UP[lefti].PrepareLine[lj].a);
					cross_t.y=VP_V_L_UP[lefti].PrepareLine[li].b+cross_t.x*VP_V_L_UP[lefti].PrepareLine[li].a;
					if (cross_t.y>=0&&cross_t.y<src_img->height){
						point_dbg.push_back(cross_t);
						VP_V_L_UP[lefti].PreparePoint.push_back(cross_t);
					}

				}else{
					//除数为0	  //两直线平行 
				}
			}
		}
	}
	/***********************************************/
	for (register  int lefti=0;lefti<2;lefti++){
		for(register unsigned int li=0;li<VP_V_R_DOWN[lefti].PrepareLine.size();li++){
			for (register unsigned int lj=li+1;lj<VP_V_R_DOWN[lefti].PrepareLine.size();lj++){

				if (VP_V_R_DOWN[lefti].PrepareLine[li].a-VP_V_R_DOWN[lefti].PrepareLine[lj].a){
					//香蕉
					cross_t.x=1.0*(VP_V_R_DOWN[lefti].PrepareLine[lj].b-VP_V_R_DOWN[lefti].PrepareLine[li].b)/(VP_V_R_DOWN[lefti].PrepareLine[li].a-VP_V_R_DOWN[lefti].PrepareLine[lj].a);
					cross_t.y=VP_V_R_DOWN[lefti].PrepareLine[li].b+cross_t.x*VP_V_R_DOWN[lefti].PrepareLine[li].a;
					if (cross_t.y>=0&&cross_t.y<src_img->height){
						//关于灭点的位置（0<y<height）
						point_dbg.push_back(cross_t);
						VP_V_R_DOWN[lefti].PreparePoint.push_back(cross_t);
					}

				}else{
					//除数为0	  //两直线平行 
				}
			}
		}
	}
	/*********计算候选点***************************************/
	vector <int> x_t,y_t;
	for (register int lefti=0;lefti<2;lefti++){
		x_t.clear();  y_t.clear();
		for(register unsigned int pi=0;pi<VP_V_R_DOWN[lefti].PreparePoint.size();pi++){
			x_t.push_back(VP_V_R_DOWN[lefti].PreparePoint[pi].x);
			y_t.push_back(VP_V_R_DOWN[lefti].PreparePoint[pi].y);
		}
		std::sort(x_t.begin(),x_t.end(),greater<int>());
		std::sort(y_t.begin(),y_t.end(),greater<int>());
		VP_V_R_DOWN[lefti].Vpoint.x=x_t.at(x_t.size()/2);
		VP_V_R_DOWN[lefti].Vpoint.y=y_t.at(y_t.size()/2);
	}
	for (register int lefti=0;lefti<2;lefti++){
		x_t.clear();  y_t.clear();
		for(register unsigned int pi=0;pi<VP_V_L_UP[lefti].PreparePoint.size();pi++){
			x_t.push_back(VP_V_L_UP[lefti].PreparePoint[pi].x);
			y_t.push_back(VP_V_L_UP[lefti].PreparePoint[pi].y);
		}
		std::sort(x_t.begin(),x_t.end(),greater<int>());
		std::sort(y_t.begin(),y_t.end(),greater<int>());
		VP_V_L_UP[lefti].Vpoint.x=x_t.at(x_t.size()/2);
		VP_V_L_UP[lefti].Vpoint.y=y_t.at(y_t.size()/2);
	}
	/************************************************/
	this->DrawVPonImg();

}
/*------------------------------------------------------------------------------------------------------------*/
/**
*在直方图中寻找峰值组（“山”字形），确定用于寻找灭点的直线组
*
*@param [in] hist_data 直方图数据
*@param hist_size 直方图的长度
*@param [out]group 符合“山“这个形状的峰值组在直方图中的位置 
*@param group_num 峰值组的数目
*/
/*------------------------------------------------------------------------------------------------------------*/
void    CuiHough3::GetPeakValueGroup(
	float* hist_data,
	int hist_size,
	int* group,
	int group_num){

	float  peak_value[AngLeDivided][3];
	memset(peak_value,0,sizeof(peak_value));
	for (register int i=1;i<hist_size-2;i++){
		memcpy(&peak_value[i],&hist_data[i-1],sizeof(float)*3);
	}
	/********江不合适的峰值组去掉********************************************/
	for (register int i=0;i<AngLeDivided;i++){
		if ((peak_value[i][1]>0)&&
			(peak_value[i][1]>peak_value[i][0])&&
			(peak_value[i][1]>peak_value[i][2])&&
			((peak_value[i][0]/peak_value[i][1]>=0.25)||(peak_value[i][2]/peak_value[i][1]>0.25))){
				//是一个有效的峰值组
		}else{
			memset(&peak_value[i],0,sizeof(float)*3);
		}
	}
	/*****寻找最值***************************************************************/
	for (int j=0;j<group_num;j++){
		float max_value=peak_value[0][1];
		int max_value_i=0;
		for (register int i=0;i<AngLeDivided;i++){
			if (peak_value[i][1]>=max_value){
				max_value=peak_value[i][1];
				max_value_i=i;
			}
		}
		/******************/
		group[j]=max_value_i;
		memset(&peak_value[max_value_i],0,sizeof(float)*3);

	}


}
/*------------------------------------------------------------------------------------------------------------*/
/**
*获得地面分割线组（已弃用）
*
*@param [in] hist_data 直方图数据结构
*/
/*------------------------------------------------------------------------------------------------------------*/
void CuiHough3::GetGroundSegmentLine(HistData *hist_data)
{
	this->GetHist3effectiveDir(hist_data->hist_ground,VP_GV_L_UP,VP_GV_R_DOWN);//水平候选方向，4个强方向，
	
/**************************************************************************************************/
 	for (register int i=0;i<3;i++){
	  GndSegment.Line2VPdst[i]=src_img->width*src_img->height;
 	}
/**************************************************************************************************/
	for (int li=0;li<cui_line_src_len;li++){
			if ((cui_line_srcimg[li].pointOne.y>GndSegment.VanishingPoint.y)&&
			(cui_line_srcimg[li].pointTwo.y>GndSegment.VanishingPoint.y)){
			//直线在灭点下方
#if 0
	 			if ((cui_line_srcimg[li].alpha_angle<(180/AngLeDivided)*(0))||
					(cui_line_srcimg[li].alpha_angle>(180/AngLeDivided)*(AngLeDivided-1))){
						//水平最短
							double dst_t=DistancePoint2Line(GndSegment.VanishingPoint,cui_line_srcimg[li]);
							if (dst_t<GndSegment.Line2VPdst[0]){
								GndSegment.Line2VPdst[0]=dst_t;
								GndSegment.GndSegLine[0]=cui_line_srcimg[li];
							}

				}
				if ((cui_line_srcimg[li].alpha_angle>(180/AngLeDivided)*(1))&&
					(cui_line_srcimg[li].alpha_angle<(180/AngLeDivided)*(AngLeDivided/2-1))){
					//Left最短
						double dst_t=DistancePoint2Line(GndSegment.VanishingPoint,cui_line_srcimg[li]);
						if (dst_t<GndSegment.Line2VPdst[1]){
							GndSegment.Line2VPdst[1]=dst_t;
							GndSegment.GndSegLine[1]=cui_line_srcimg[li];
						}

				}
				if ((cui_line_srcimg[li].alpha_angle>(180/AngLeDivided)*(AngLeDivided/2+1))&&
					(cui_line_srcimg[li].alpha_angle<(180/AngLeDivided)*(AngLeDivided))){
					//Right最短
						double dst_t=DistancePoint2Line(GndSegment.VanishingPoint,cui_line_srcimg[li]);
						if (dst_t<GndSegment.Line2VPdst[2]){
							GndSegment.Line2VPdst[2]=dst_t;
							GndSegment.GndSegLine[2]=cui_line_srcimg[li];
						}

				}
#else
				if ((cui_line_srcimg[li].alpha_angle<(180/AngLeDivided)*(1))||
					(cui_line_srcimg[li].alpha_angle>(180/AngLeDivided)*(AngLeDivided-1))){
						//水平最短
						double dst_t=DistancePoint2Line(GndSegment.VanishingPoint,cui_line_srcimg[li]);
						if (dst_t<GndSegment.Line2VPdst[LINE_HORIZONTAL]){
							GndSegment.Line2VPdst[LINE_HORIZONTAL]=dst_t;
							GndSegment.GndSegLine[LINE_HORIZONTAL]=cui_line_srcimg[li];
						}

				}
				if ((cui_line_srcimg[li].alpha_angle>(VP_GV_L_UP[0].Vp_angle_down))&&
					(cui_line_srcimg[li].alpha_angle<(VP_GV_L_UP[0].Vp_angle_up))){
						//Left最短
						double dst_t=DistancePoint2Line(GndSegment.VanishingPoint,cui_line_srcimg[li]);
						if (dst_t<GndSegment.Line2VPdst[LINE_UP]){
							GndSegment.Line2VPdst[LINE_UP]=dst_t;
							GndSegment.GndSegLine[LINE_UP]=cui_line_srcimg[li];
						}

				}
				if ( (cui_line_srcimg[li].alpha_angle>(VP_GV_R_DOWN[0].Vp_angle_down))&&
					 (cui_line_srcimg[li].alpha_angle<(VP_GV_R_DOWN[0].Vp_angle_up)) ){
						//Right最短
						double dst_t=DistancePoint2Line(GndSegment.VanishingPoint,cui_line_srcimg[li]);
						if (dst_t<GndSegment.Line2VPdst[LINE_DOWN]){
							GndSegment.Line2VPdst[LINE_DOWN]=dst_t;
							GndSegment.GndSegLine[LINE_DOWN]=cui_line_srcimg[li];
						}

				}
#endif
	
			}
	}
	this->DrawVPonImg(1);
/**************************************************************************************************/
}
/*------------------------------------------------------------------------------------------------------------*/
/**
*获取斜向切割地面的两条直线，可能只有一条
*
*@param [in] hist_data 直方图数据 
*@param [in] line_data 直线数据
*@param line_len  直线的个数
*
*/
/*------------------------------------------------------------------------------------------------------------*/
void CuiHough3::Get2GroundSegmentLine(
	HistData *hist_data,
	LINEDATA *line_data,
	int line_len)
{
	  if (GndSegment.VPnumber==1){
		  if (GndSegment.VPposition[0]==VanishPosition::LEFT){
			   this->GetGroundSegmentLineUp(hist_data,line_data,line_len); 
		  }else if (GndSegment.VPposition[0]==VanishPosition::RIGHT){
			   this->GetGroundSegmentLineDown(hist_data,line_data,line_len);
		  }else if (GndSegment.VPposition[0]==VanishPosition::MIDDLE){
				 this->GetGroundSegmentLineUp(hist_data,line_data,line_len);
				 this->GetGroundSegmentLineDown(hist_data,line_data,line_len);
		  }

	  }else if (GndSegment.VPnumber==2){
		  if ((GndSegment.VPposition[0]==VanishPosition::LEFT)&&(GndSegment.VPposition[1]==VanishPosition::RIGHT)||
			  (GndSegment.VPposition[0]==VanishPosition::RIGHT)&&(GndSegment.VPposition[1]==VanishPosition::LEFT)){
				  this->GetGroundSegmentLineUp(hist_data,line_data,line_len);
				  this->GetGroundSegmentLineDown(hist_data,line_data,line_len);
		  }

	  }
	 /******************************************************************/
	 this->DrawVPonImg(1);
}
/*------------------------------------------------------------------------------------------------------------*/
/**
*获取水平方向切割地面的直线
*
*@param [in] hist_data 直方图数据 
*@param [in] line_data 直线数据
*@param line_len  直线的个数
*/
/*------------------------------------------------------------------------------------------------------------*/
void CuiHough3::Get1GroundSegmentLine(
	HistData *hist_data,
	LINEDATA *line_data,
	int line_len)
{
	
	this->GetGroundSegmentLineHorizontal(hist_data,line_data,line_len);
	/******************************************************************/
	this->DrawVPonImg(1);
}
/*------------------------------------------------------------------------------------------------------------*/
/**
*在地面分割数据结构中提取左侧的灭点
*
*@return 左侧灭点
*/
/*------------------------------------------------------------------------------------------------------------*/
VpPoint CuiHough3::GetLeftVPoint(void)
{  
	VpPoint VpPoint_t;
	double P1_third,P2_third;
	P1_third=1.0/3*src_img->width;
	P2_third=2.0/3*src_img->width;
	for (register int i=0;i<GndSegment.VPnumber;i++){
		if (GndSegment.vpPoint[i].point.x<P1_third){
			if (GndSegment.vpPoint[i].sigema>0){
			 return GndSegment.vpPoint[i];
			}			
		}
	}
	VpPoint_t.sigema=-1;
	return VpPoint_t;
}
/*------------------------------------------------------------------------------------------------------------*/
/**
*在地面分割数据结构中提取左侧的灭点
*
*@return 右侧灭点
*/
/*------------------------------------------------------------------------------------------------------------*/
VpPoint CuiHough3::GetRightVPoint(void)
{  
	VpPoint VpPoint_t;
	double P1_third,P2_third;
	P1_third=1.0/3*src_img->width;
	P2_third=2.0/3*src_img->width;
	for (register int i=0;i<GndSegment.VPnumber;i++){
		if (GndSegment.vpPoint[i].point.x>P2_third){
			if (GndSegment.vpPoint[i].sigema>0){
			   return GndSegment.vpPoint[i];
			}
			
		}
	}
	VpPoint_t.sigema=-1;
	return VpPoint_t;
}
/*------------------------------------------------------------------------------------------------------------*/
/**
*获得0-90度方向分割地面的直线
*
*@param [in] hist_data 直方图数据
*@param [in] line_data 直线数据 
*@param line_len  直线数量
*
*/
/*------------------------------------------------------------------------------------------------------------*/
void CuiHough3::GetGroundSegmentLineUp(
	HistData *hist_data,
	LINEDATA *line_data,
	int line_len)
{  
   VpPoint VpPoint_t=GetLeftVPoint();
   if (VpPoint_t.sigema>0){
	   GndSegment.GndSegLine[LINE_UP]=this->GetGroundSegmentLineUporDown(hist_data,line_data,line_len,1,VpPoint_t);
   }else{
	   ;//不分割线
   }
 
}
/*------------------------------------------------------------------------------------------------------------*/
/**
*
*获得90-180度方向分割地面的直线
*
*@param [in] hist_data 直方图数据
*@param [in] line_data 直线数据 
*@param line_len  直线数量
*/
/*------------------------------------------------------------------------------------------------------------*/
void CuiHough3::GetGroundSegmentLineDown(
	HistData *hist_data,
	LINEDATA *line_data,
	int line_len)
{
  VpPoint VpPoint_t=GetRightVPoint();
  if (VpPoint_t.sigema>0){
	  GndSegment.GndSegLine[LINE_DOWN]=this->GetGroundSegmentLineUporDown(hist_data,line_data,line_len,-1,VpPoint_t);
  }else{
	  ;//不分割线
  }
 
}
/*------------------------------------------------------------------------------------------------------------*/
/**
*
*获得0-90,90-180度方向分割地面的直线
*
*@param [in] hist_data 直方图数据
*@param [in] line_data 直线数据 
*@param line_len  直线数量
*@param UporDown  
*- 1 0-90度
*- -1 90-180度
*@param vppoint   灭点
*/
/*------------------------------------------------------------------------------------------------------------*/
LINEDATA  CuiHough3::GetGroundSegmentLineUporDown(
	HistData *hist_data,
	LINEDATA *line_data,
	int line_len ,
	int UporDown,
	VpPoint vppoint){

	float  hist_ground[AngLeDivided];
	int   group[AngLeDivided/4];	
	LINEDATA line_best;
	double distance_min=-1;
	memset(&line_best,0,sizeof(LINEDATA));
	memcpy(hist_ground,hist_data->hist_ground,sizeof(hist_ground));
	if (UporDown==1){
		for (int i=AngLeDivided/2;i<AngLeDivided;i++){
			hist_ground[i]=0;
		}
	}else if(UporDown==-1){
		for (int i=0;i<AngLeDivided/2;i++){
			hist_ground[i]=0;
		}
	}
	
	this->RemoveHistData0and90(hist_ground,AngLeDivided);
	cui_GeneralImgProcess::GetMaxValueIndex(hist_ground,AngLeDivided,group,sizeof(group)/sizeof(int));
	for (int ai=0;ai<3;ai++){
		for (int aj=ai+1;aj<3;aj++){
			if (abs(group[ai]-group[aj])==1){
				//i ,j相邻
				if(hist_ground[group[aj]]/hist_ground[group[ai]]<=0.8){
					//相邻同时比例过低,q=去掉啊aj
					group[aj]=group[ai];
				}
			}
		}
	}
	for (int ai=0;ai<3;ai++){	
		LINEDATA line_t;
		if (hist_ground[group[ai]]/hist_ground[group[0]]>=0.6){
			if (UporDown==1){
				//UP
				VP_GV_L_UP[0].hist_pos=group[ai];
				VP_GV_L_UP[0].Vp_angle_up=(group[ai]+1)*180/AngLeDivided;
				VP_GV_L_UP[0].Vp_angle_down=group[ai]*180/AngLeDivided;
				line_t=this->GetGroundSegmentLineByDirect(line_data,line_len,&VP_GV_L_UP[0],vppoint);
			}else if(UporDown==-1){
				//DOWN
				VP_GV_R_DOWN[0].hist_pos=group[ai];
				VP_GV_R_DOWN[0].Vp_angle_up=(group[ai]+1)*180/AngLeDivided;
				VP_GV_R_DOWN[0].Vp_angle_down=group[ai]*180/AngLeDivided;
				line_t=this->GetGroundSegmentLineByDirect(line_data,line_len,&VP_GV_R_DOWN[0],vppoint);
			}
			
			double distance_sum=this->Line2VGContourDistance(line_t);	 
			if (distance_min==-1){
				distance_min=distance_sum;
				line_best=line_t;
			}
			if (distance_sum<distance_min){
				distance_min=distance_sum;
				line_best=line_t;
			}
		}

	}
#if 1
	/************测试代码***************/
	for (int ai=0;ai<3;ai++){	
		LINEDATA line_t;
		if (hist_ground[group[ai]]>0){
			if (UporDown==1){
				//UP
				VP_GV_L_UP[0].hist_pos=group[ai];
				VP_GV_L_UP[0].Vp_angle_up=(group[ai]+1)*180/AngLeDivided;
				VP_GV_L_UP[0].Vp_angle_down=group[ai]*180/AngLeDivided;
				line_t=this->GetGroundSegmentLineByDirect(line_data,line_len,&VP_GV_L_UP[0],vppoint);
			}else if(UporDown==-1){
				//DOWN
				VP_GV_R_DOWN[0].hist_pos=group[ai];
				VP_GV_R_DOWN[0].Vp_angle_up=(group[ai]+1)*180/AngLeDivided;
				VP_GV_R_DOWN[0].Vp_angle_down=group[ai]*180/AngLeDivided;
				line_t=this->GetGroundSegmentLineByDirect(line_data,line_len,&VP_GV_R_DOWN[0],vppoint);
			}

			double distance_sum=this->Line2VGContourDistance(line_t);	 
			if (distance_sum==distance_min){
				break;
			}
	}
 }
#endif
	return line_best;

}
/*------------------------------------------------------------------------------------------------------------*/
/**
*获得分割地面的水平线
*
*@param [in] hist_data 直方图数据
*@param [in] line_data 直线数据 
*@param line_len  直线数量
*/
/*------------------------------------------------------------------------------------------------------------*/
void CuiHough3::GetGroundSegmentLineHorizontal(
	HistData *hist_data,
	LINEDATA *line_data,
	int line_len)
{
	int group[2]={0,AngLeDivided-1};
	float  hist_ground[AngLeDivided];
	LINEDATA line_best;
	double distance_min=-1;
	//立面上找水平线
	 memset(&line_best,0,sizeof(LINEDATA));
	 memcpy(hist_ground,hist_data->hist_all,sizeof(hist_ground)); 	
	 VP_GV_H_H[0].PrepareLine.clear();
	for (int ai=0;ai<2;ai++){	
		LINEDATA line_t;
		if (hist_ground[group[ai]]>0){
			
				VP_GV_H_H[0].hist_pos=group[ai];
				VP_GV_H_H[0].Vp_angle_up=(group[ai]+1)*180/AngLeDivided;
				VP_GV_H_H[0].Vp_angle_down=group[ai]*180/AngLeDivided;
			
				for (int li=0;li<line_len;li++){	
#if 1
				   if (line_data[li].Category>Ground)
#endif					
					
					{
					   /*********直线在灭点之下*******************/
#if 1
					  if ((line_data[li].pointOne.y>GndSegment.VanishingPoint.y)&&
							(line_data[li].pointTwo.y>GndSegment.VanishingPoint.y))
#endif
						
						 {
									/*****直线在候选角度之下********/
							 //2013年11月18日放到157度--hough线提取不精确
#if 1
							 if ((line_data[li].alpha_angle>=(VP_GV_H_H[0].Vp_angle_down))&&
								 (line_data[li].alpha_angle<=(VP_GV_H_H[0].Vp_angle_up)))
#endif	
									
									{

#if 1
											//0.5 sigema ,以内
										if (this->DistancePoint2Line(GndSegment.vpPoint[0].point,line_data[li])<0.9*GndSegment.vpPoint[0].sigema)	
#endif								
										{
												VP_GV_H_H[0].PrepareLine.push_back(line_data[li]);
										}
										


											
									}


						}


					 }
					
				}
				/****************************************************************/
			}

		/****************************************************************/
		cui_GeneralImgProcess::DrawLine2BlackImg(VP_GV_H_H[0].PrepareLine,cui_Width,cui_Height,"Horizontal_Line");
#if 1
		/**************距离垂面最近**************************************************/
			for (register unsigned int li=0;li<VP_GV_H_H[0].PrepareLine.size();li++){
				line_t=VP_GV_H_H[0].PrepareLine[li];
#if 0
            double distance_sum=this->Line2VPContourDistance(line_t);
#else
			double distance_sum=this->Line2VGContourDistance(line_t);
#endif
				

				if (distance_min==-1){
					distance_min=distance_sum;
					line_best=line_t;
				}
				if (distance_sum<distance_min){
					distance_min=distance_sum;
					line_best=line_t;
				}
			}
			/****************************************************************/
#else
		double distance_max=-1;
		/*****************最长的***********************************************/
		for (register int li=0;li<VP_GV_H_H[0].PrepareLine.size();li++){
			line_t=VP_GV_H_H[0].PrepareLine[li];
			double distance_sum=line_t.LineLength;	 
			if (distance_max==-1){
				distance_max=distance_sum;
				line_best=line_t;
			}
			if (distance_sum>distance_max){
				distance_max=distance_sum;
				line_best=line_t;
			}
		}
		/****************************************************************/
#endif
		


		}
	float Line_threshold=pMD->GetHorizontalThresholdLength();
	if (line_best.LineLength<Line_threshold){
		memset(&line_best,0,sizeof(LINEDATA));
       GndSegment.GndSegLine[LINE_HORIZONTAL]=line_best;
	}else{
       GndSegment.GndSegLine[LINE_HORIZONTAL]=line_best;
	}
   
}
/*------------------------------------------------------------------------------------------------------------*/
/**
*通过已确定的分割线方向获得地面的分割线（已弃用）
*
*/
/*------------------------------------------------------------------------------------------------------------*/
LINEDATA CuiHough3::GetGroundSegmentLineByDirect(
	LINEDATA *line_data,
	int line_len,
	VanishingPointData *VP_LINE,
	VpPoint vppoint)
{
	LINEDATA line_cut;
	memset(&line_cut,0,sizeof(line_cut));
	VP_LINE->PrepareLine.clear();
	/****************************************************************/
   for (int li=0;li<line_len;li++){	
			 /*********直线在灭点之下*******************/
		if ((line_data[li].pointOne.y>GndSegment.VanishingPoint.y)&&
			(line_data[li].pointTwo.y>GndSegment.VanishingPoint.y)){
			/*****直线在候选角度之下********/
				if ((line_data[li].alpha_angle>(VP_LINE->Vp_angle_down))&&
					(line_data[li].alpha_angle<(VP_LINE->Vp_angle_up))){						 
						if (this->DistancePoint2Line(vppoint.point,line_data[li])<vppoint.sigema){
						   VP_LINE->PrepareLine.push_back(line_data[li]);
						}				      
				}

				
		}
	}
   /****************************************************************/
    for (register unsigned int li=0;li<VP_LINE->PrepareLine.size();li++){
		LINEDATA line_t=VP_LINE->PrepareLine[li];
		line_cut.alpha_angle+=line_t.alpha_angle;
    }

	line_cut.alpha_angle/=VP_LINE->PrepareLine.size();

	for (register unsigned int li=0;li<VP_LINE->PrepareLine.size();li++){
		CvPoint	CenterPoint;
		double a,b;
		CenterPoint.x=(VP_LINE->PrepareLine[li].pointOne.x+VP_LINE->PrepareLine[li].pointTwo.x)/2;
		CenterPoint.y=(VP_LINE->PrepareLine[li].pointOne.y+VP_LINE->PrepareLine[li].pointTwo.y)/2;
		/**************************************************************************************/
		VP_LINE->PrepareLine[li].alpha_angle=line_cut.alpha_angle; //矫正到一个方向
		VP_LINE->PrepareLine[li].alpha_radian=line_cut.alpha_angle*CV_PI/180;
		VP_LINE->PrepareLine[li].a=a=tan(VP_LINE->PrepareLine[li].alpha_radian);
		VP_LINE->PrepareLine[li].b=b=CenterPoint.y-a*CenterPoint.x;		
		VP_LINE->PrepareLine[li].AdjPoint1.y=a*(VP_LINE->PrepareLine[li].AdjPoint1.x)+b;
		VP_LINE->PrepareLine[li].AdjPoint2.y=a*(VP_LINE->PrepareLine[li].AdjPoint2.x)+b;
		VP_LINE->PrepareLine[li].pointOne.y=a*(VP_LINE->PrepareLine[li].pointOne.x)+b;	
		VP_LINE->PrepareLine[li].pointTwo.y=a*(VP_LINE->PrepareLine[li].pointTwo.x)+b;
		VP_LINE->PrepareLine[li].LineLength=cui_GeneralImgProcess::P2PointDiatance(VP_LINE->PrepareLine[li].pointOne,VP_LINE->PrepareLine[li].pointTwo);
		
	}
	 cui_GeneralImgProcess::DrawLine2BlackImg(VP_LINE->PrepareLine,cui_Width,cui_Height,"adjAngule");
	VP_LINE->PreparePoint.clear();
  /****************************************************************/
	for (register unsigned int li=0;li<VP_LINE->PrepareLine.size();li++){
		LINEDATA line_t=VP_LINE->PrepareLine[li];
		/*line_cut.alpha_angle+=line_t.alpha_angle;*/

	   		CvPoint  point_t;
#if   1
			int max_xi=max(line_t.pointOne.x,line_t.pointTwo.x);
			int min_xi=min(line_t.pointOne.x,line_t.pointTwo.x); 			
#else
		    int max_xi=cui_Width;
			int min_xi=0; 
#endif	   
			
			for (register int xi=min_xi;xi<max_xi;xi++){
				point_t.x=xi;
				point_t.y=line_t.a*xi+line_t.b;
				VP_LINE->PreparePoint.push_back(point_t);
			}


	}
	if (VP_LINE->PrepareLine.size()==0){
	     memset(&line_cut,0,sizeof(line_cut));
	}else{
		line_cut=this->SelectCutLinebyIntercept2(VP_LINE->PrepareLine,VP_LINE->PreparePoint);
	}
	
  /***************************************************************/
   return line_cut;
}
/*------------------------------------------------------------------------------------------------------------*/
/**
*计算点到直线的距离
*@param point 点
*@param line 线
*/
/*------------------------------------------------------------------------------------------------------------*/
double CuiHough3::DistancePoint2Line(CvPoint point, LINEDATA line)
{
	
	//计算点到直线距离
	double distance_t=0;	
	distance_t=fabsl(line.a*point.x-point.y+line.b)/sqrtl(powl(line.a,2)+powl(1,2));
	return	distance_t;
}
/*------------------------------------------------------------------------------------------------------------*/
/**
*在直方图中提取有效的方向线
*
*@param hist
*@param  VP_V_L_UPt
*@param  VP_V_R_DOWNt
*@deprecated 已弃用
*/
/*------------------------------------------------------------------------------------------------------------*/

void CuiHough3::GetHist3effectiveDir(
	float *hist,
	VanishingPointData* VP_V_L_UPt,
	VanishingPointData* VP_V_R_DOWNt)
{
	float hist_vertical_t[AngLeDivided];
	float *hist_vertical_l,*hist_vertical_r;
	memcpy(hist_vertical_t,hist,sizeof(float)*AngLeDivided);
	hist_vertical_l=hist_vertical_t;
	hist_vertical_r=&hist_vertical_t[AngLeDivided/2+1];
	/************************************************/
	//去掉0/180度和
	hist_vertical_t[0]=0;
	hist_vertical_t[AngLeDivided-1]=0;
	/****90度数据************/
	hist_vertical_t[AngLeDivided/2-1]=0;
	hist_vertical_t[AngLeDivided/2]=0;
	hist_vertical_t[AngLeDivided/2+1]=0;
	/*****************/	  
	std::sort( hist_vertical_l, hist_vertical_l+AngLeDivided/2,greater<float>()); 
	std::sort( hist_vertical_r, hist_vertical_r+AngLeDivided/2,greater<float>()); 
	/********找到最大值********************************/
	VP_V_L_UPt[0].Vp_height=hist_vertical_l[0];
	VP_V_L_UPt[1].Vp_height=hist_vertical_l[1];
	VP_V_R_DOWNt[0].Vp_height=hist_vertical_r[0];
	VP_V_R_DOWNt[1].Vp_height=hist_vertical_r[1];
	/**************************************************/
	for (register int ai=0;ai<AngLeDivided;ai++){
		if (ai<AngLeDivided/2){
			//left
			for(register int mi=0;mi<2;mi++){
				if (VP_V_L_UPt[mi].Vp_height==hist[ai]){
					VP_V_L_UPt[mi].Vp_angle_up=(ai+1)*180/AngLeDivided;
					VP_V_L_UPt[mi].Vp_angle_down=ai*180/AngLeDivided;
					VP_V_L_UPt[mi].hist_pos=ai;
				}
			}

		}else{
			//right
			for(register int mi=0;mi<2;mi++){
				if (VP_V_R_DOWNt[mi].Vp_height==hist[ai]){
					VP_V_R_DOWNt[mi].Vp_angle_up=(ai+1)*180/AngLeDivided;
					VP_V_R_DOWNt[mi].Vp_angle_down=ai*180/AngLeDivided;
					VP_V_R_DOWNt[mi].hist_pos=ai;
				}
			}
		}
	}
	/*********************************************************************************/
}
/*------------------------------------------------------------------------------------------------------------*/
/**
*
*在直方图中提取有效的方向线
*
*@param hist
*@param  VP_V_L_UPt
*@param  VP_V_R_DOWNt
*@deprecated 已弃用
*/
/*------------------------------------------------------------------------------------------------------------*/
void CuiHough3::GetHist4EffectiveLinecluster(
	float *hist,
	VanishingPointData* VP_V_L_UPt,
	VanishingPointData* VP_V_R_DOWNt)
{
	float hist_vertical_t[AngLeDivided];
	float *hist_vertical_l,*hist_vertical_r;
	memcpy(hist_vertical_t,hist,sizeof(float)*AngLeDivided);
	hist_vertical_l=hist_vertical_t;
	hist_vertical_r=&hist_vertical_t[AngLeDivided/2+1];
	/************************************************/
	//去掉0/180度和
	hist_vertical_t[0]=0;
	hist_vertical_t[AngLeDivided-1]=0;
	/****90度数据************/
	hist_vertical_t[AngLeDivided/2-1]=0;
	hist_vertical_t[AngLeDivided/2]=0;
	hist_vertical_t[AngLeDivided/2+1]=0;
	/*****************/	  
	std::sort( hist_vertical_l, hist_vertical_l+AngLeDivided/2,greater<float>()); 
	std::sort( hist_vertical_r, hist_vertical_r+AngLeDivided/2,greater<float>()); 
	/********找到最大值********************************/
	VP_V_L_UPt[0].Vp_height=hist_vertical_l[0]; //最大
	VP_V_L_UPt[1].Vp_height=hist_vertical_l[1]; //次大
	VP_V_R_DOWNt[0].Vp_height=hist_vertical_r[0];
	VP_V_R_DOWNt[1].Vp_height=hist_vertical_r[1];
	/*********寻找最值对应的参数*****************************************/
	for (register int ai=0;ai<AngLeDivided;ai++){
		if (ai<AngLeDivided/2){
			//left
			for(register int mi=0;mi<2;mi++){
				if (VP_V_L_UPt[mi].Vp_height==hist[ai]){
					VP_V_L_UPt[mi].Vp_angle_up=(ai+1)*180/AngLeDivided;
					VP_V_L_UPt[mi].Vp_angle_down=ai*180/AngLeDivided;
					VP_V_L_UPt[mi].hist_pos=ai;
					VP_V_L_UPt[mi].Effective_cluster=true;
				}
			}

		}else{
			//right
			for(register int mi=0;mi<2;mi++){
				if (VP_V_R_DOWNt[mi].Vp_height==hist[ai]){
					VP_V_R_DOWNt[mi].Vp_angle_up=(ai+1)*180/AngLeDivided;
					VP_V_R_DOWNt[mi].Vp_angle_down=ai*180/AngLeDivided;
					VP_V_R_DOWNt[mi].hist_pos=ai;
					VP_V_R_DOWNt[mi].Effective_cluster=true;
				}
			}
		}
	}
	/**********矫正参数*****************************************************************/
			double max_value,neighbour_value;
			int    max_pos,neibor_pos;
#if 1
			int   Search_l_pos[4]={1,-1,2,-2};
			int   Search_r_pos[4]={-1,1,-2,2};
#else
			int   Search_l_pos[2]={2,-2};
			int   Search_r_pos[2]={-2,2};
#endif
		

	        //left
			 max_pos=VP_V_L_UPt[0].hist_pos;
			 max_value=hist_vertical_t[max_pos];
											   
			 int pi;
			 for (pi=0;pi<sizeof(Search_l_pos)/sizeof(int);pi++){
					int ai=max_pos+Search_l_pos[pi];
					if (ai>0&&ai<AngLeDivided){
						neighbour_value=hist_vertical_t[ai];
								if (neighbour_value/max_value<0.25){
									//无效的
								  }else{
									 //有效的邻边
                    
									  VP_V_L_UPt[1].Vp_angle_up=(ai+1)*180/AngLeDivided;
									  VP_V_L_UPt[1].Vp_angle_down=ai*180/AngLeDivided;
									  VP_V_L_UPt[1].hist_pos=ai;
									  VP_V_L_UPt[1].Effective_cluster=true;
									  //找到一个有效的
									  break;
								 }
					}
			 }
			 if(pi==sizeof(Search_l_pos)/sizeof(int)){
				 VP_V_L_UPt[0].Effective_cluster=false;
			 }else{
				  goto NEXT_RIGHT;
			 }
			 /***********************************************************************************/
			 //left			 goto
			 max_pos=VP_V_L_UPt[1].hist_pos;
			 max_value=hist_vertical_t[max_pos];

			 for (register int pi=0;pi<sizeof(Search_l_pos)/sizeof(int);pi++){
				 int ai=max_pos+Search_l_pos[pi];
				 if (ai>0&&ai<AngLeDivided){
					 neighbour_value=hist_vertical_t[ai];
					 if (neighbour_value/max_value<0.25){
						 //无效的
					 }else{
						 //有效的邻边
						 VP_V_L_UPt[1].Vp_angle_up=(ai+1)*180/AngLeDivided;
						 VP_V_L_UPt[1].Vp_angle_down=ai*180/AngLeDivided;
						 VP_V_L_UPt[1].hist_pos=ai;
						 VP_V_L_UPt[1].Effective_cluster=true;
						 //找到一个有效的
						 break;
					 }
				 }
			 }
			 if(pi==sizeof(Search_l_pos)/sizeof(int)){
				 VP_V_L_UPt[1].Effective_cluster=false;
			 }
			 /***********************************************************************************/	 
NEXT_RIGHT:
			 //right
			max_pos=VP_V_R_DOWNt[0].hist_pos;
			max_value=hist_vertical_t[max_pos];
			for (pi=0;pi<sizeof(Search_l_pos)/sizeof(int);pi++){
				int ai=max_pos+Search_r_pos[pi];
				if (ai>0&&ai<AngLeDivided){
					neighbour_value=hist_vertical_t[ai];
					if (neighbour_value/max_value<0.25){
						//无效的
					}else{
						//有效的邻边

						VP_V_R_DOWNt[1].Vp_angle_up=(ai+1)*180/AngLeDivided;
						VP_V_R_DOWNt[1].Vp_angle_down=ai*180/AngLeDivided;
						VP_V_R_DOWNt[1].hist_pos=ai;
						VP_V_R_DOWNt[1].Effective_cluster=true;
						//找到一个有效的
						break;
					}
				}
			}
			if(pi==sizeof(Search_l_pos)/sizeof(int)){
				VP_V_L_UPt[0].Effective_cluster=false;
			}else{
				goto NEXT_COMPLETE;		
			}
			/***********************************************************************************/
			//right
			max_pos=VP_V_R_DOWNt[1].hist_pos;
			max_value=hist_vertical_t[max_pos];

			for (register int pi=0;pi<sizeof(Search_l_pos)/sizeof(int);pi++){
				int ai=max_pos+Search_l_pos[pi];
				if (ai>0&&ai<AngLeDivided){
					neighbour_value=hist_vertical_t[ai];
					if (neighbour_value/max_value<0.25){
						//无效的

					}else{
						//有效的邻边
						VP_V_R_DOWNt[1].Vp_angle_up=(ai+1)*180/AngLeDivided;
						VP_V_R_DOWNt[1].Vp_angle_down=ai*180/AngLeDivided;
						VP_V_R_DOWNt[1].hist_pos=ai;
						VP_V_R_DOWNt[1].Effective_cluster=true;
						//找到一个有效的
						break;
					}
				}
			}
			if(pi==sizeof(Search_l_pos)/sizeof(int)){
				VP_V_R_DOWNt[1].Effective_cluster=false;
			}
			/***********************************************************************************/
NEXT_COMPLETE:
			 ;
}
/*------------------------------------------------------------------------------------------------------------*/
/**
*
*在直方图中提取有效的4组方向线，对应的两组交叉，用已确定灭点的位置
*
*@param hist
*@param  VP_V_L_UPt
*@param  VP_V_R_DOWNt
*/
/*------------------------------------------------------------------------------------------------------------*/
void CuiHough3::GetLine4cluster2determineVP(
	float *hist,
	VanishingPointData* VP_V_L_UPt,
	VanishingPointData* VP_V_R_DOWNt)
{
	this->GetLine2cluster2determineVP(hist,VP_V_L_UPt,1);
	this->GetLine2cluster2determineVP(hist,VP_V_R_DOWNt,-1);

}
/*------------------------------------------------------------------------------------------------------------*/
/**
*
*在直方图中提取有效的2组方向线，对应的两组交叉，用已确定灭点的位置
*
*@param[in] hist 直方图
*@param[in] VP_V_L_t
*@param  UporDown UP组为0-90的直线或 Down组为90-180的直线
*/
/*------------------------------------------------------------------------------------------------------------*/
void CuiHough3::GetLine2cluster2determineVP(
	float *hist,
	VanishingPointData* VP_V_L_t,
	int UporDown){
	 float  hist_t[AngLeDivided];
//	int   group[AngLeDivided/4];	
	int  group_max[2];
	int  group_max2[2];
//	LINEDATA line_best;
	double distance_min=-1;
	memcpy(hist_t,hist,sizeof(hist_t));
	if (UporDown==1){
		//删除DOWN线
		for (int i=AngLeDivided/2;i<AngLeDivided;i++){
			hist_t[i]=0;
		}
	}else if(UporDown==-1){
		//删除UP线
		for (int i=0;i<AngLeDivided/2;i++){
			hist_t[i]=0;
		}
	}
	/**************************************/
	this->RemoveHistData0and90(hist_t,AngLeDivided);
	//获得两个峰值组
	this->GetPeakValueGroup(hist_t,AngLeDivided,group_max,sizeof(group_max)/sizeof(int));
	/*****寻找各自峰值组中较大的肩*********************************/ 	
	   for (int i=0;i<2;i++){
			   group_max2[i]=(hist_t[group_max[i]-1]>hist_t[group_max[i]+1])?(group_max[i]-1):(group_max[i]+1);
		}
	/************************************************************/
	  
	   if (fabs(hist_t[group_max[0]-1]-hist_t[group_max[0]+1])/std::max(hist_t[group_max[0]-1],hist_t[group_max[0]+1])<0.2){
			//两肩差距很小-取两肩cross
		   VP_V_L_t[0].Vp_height=hist_t[group_max[0]-1];
		   VP_V_L_t[0].Vp_angle_up=(group_max[0])*180/AngLeDivided;
		   VP_V_L_t[0].Vp_angle_down=(group_max[0]-1)*180/AngLeDivided;
		   VP_V_L_t[0].hist_pos=group_max[0]-1;

		   VP_V_L_t[1].Vp_height=hist_t[group_max[0]+1];
		   VP_V_L_t[1].Vp_angle_up=(group_max2[0]+2)*180/AngLeDivided;
		   VP_V_L_t[1].Vp_angle_down=(group_max2[0]+1)*180/AngLeDivided;
		   VP_V_L_t[1].hist_pos=group_max[0]+1;

	   }else{
		   //两肩差距较大--取其中一头一肩
		   VP_V_L_t[0].Vp_height=hist_t[group_max[0]];
		   VP_V_L_t[0].Vp_angle_up=(group_max[0]+1)*180/AngLeDivided;
		   VP_V_L_t[0].Vp_angle_down=group_max[0]*180/AngLeDivided;
		   VP_V_L_t[0].hist_pos=group_max[0];

		   VP_V_L_t[1].Vp_height=group_max2[0];
		   VP_V_L_t[1].Vp_angle_up=(group_max2[0]+1)*180/AngLeDivided;
		   VP_V_L_t[1].Vp_angle_down=group_max2[0]*180/AngLeDivided;
		   VP_V_L_t[1].hist_pos=group_max2[0];
	   }
	
	/************************************************************/

}
/*------------------------------------------------------------------------------------------------------------*/
/**
*根据灭点的位置和个数，确定切割地面的方式
*主要有三种情况
*- 一根水平线，一根向上倾斜的直线
*- 一根水平线，一根向下倾斜的直线     
*- 一根水平线（可能没有），一根向下倾斜的直线 ，一根向上倾斜的直线           
*/
/*------------------------------------------------------------------------------------------------------------*/
void CuiHough3::EnsureVPLineCluster(void)
{
   double P1_third,P2_third;
   P1_third=1.0/3*src_img->width;
   P2_third=2.0/3*src_img->width;
   for(int i=0;i<3;i++){
	   GndSegment.VPointPositionIndex[i]=0;
   }
   if (GndSegment.VanishingPoint.x<P1_third){
	   //left point
	   GndSegment.VPointPositionIndex[LINE_UP]=1;
	   GndSegment.VPointPositionIndex[LINE_HORIZONTAL]=1;
	   GndSegment.VPointPositionIndex[LINE_DOWN]=0;

   }else if (GndSegment.VanishingPoint.x>=P1_third&&GndSegment.VanishingPoint.x<=P2_third){
	   //mid
	   GndSegment.VPointPositionIndex[LINE_UP]=1;
	   GndSegment.VPointPositionIndex[LINE_HORIZONTAL]=1;
	   GndSegment.VPointPositionIndex[LINE_DOWN]=1;

   }else if(GndSegment.VanishingPoint.x>P2_third){
	   //right
	   GndSegment.VPointPositionIndex[LINE_UP]=0;
	   GndSegment.VPointPositionIndex[LINE_HORIZONTAL]=1;
	   GndSegment.VPointPositionIndex[LINE_DOWN]=1;
   }
   //point  (-1,373)
   //VPPInedx 1 0 0
   //VPP      1   {1,-1}

}
/*------------------------------------------------------------------------------------------------------------*/
/**
*对自然环境做的判断
*超过视频线的地面，算作立面
@deprecated 已弃用
*/
/*------------------------------------------------------------------------------------------------------------*/

void CuiHough3::NatureClassifyGV(void)
{
#if 1
	UseEyeLevel2AdjustSVG(cui_ImgLables_SVG,cui_Width,cui_Height,GndSegment.GndSegLine[LINE_HORIZONTAL],GndSegment.vpPoint,pMD->Seg_HorizontalLinePosScale*cui_Height);
#endif	
}
/*------------------------------------------------------------------------------------------------------------*/
/**
*单灭点时，结合找到的直线，分割地面
*
*/
/*------------------------------------------------------------------------------------------------------------*/
void CuiHough3::HoughLineClassifyGV(void)
{
	unsigned int Y_SegPosition;
	for (register unsigned int x=0;x<(unsigned int)cui_Width;x++){
		Y_SegPosition=GetSegmentPoint(x);
	   for (register  int y=cui_Height-1;y>=0;y--){
			 if (y>=Y_SegPosition){
				 if (Vertical==cui_Matrix_Category_Lable[cui_ImgLables[y*cui_Width+x]]){
						 this->cui_ImgLables_SVG[y*cui_Width+x]=Ground;
				 }
				
			 
			 }else{
				  
				 if (Ground==cui_Matrix_Category_Lable[cui_ImgLables[y*cui_Width+x]]){
						 this->cui_ImgLables_SVG[y*cui_Width+x]=Vertical;
				   }
				
			 
			 }

	   }
	}
	
}
/*------------------------------------------------------------------------------------------------------------*/
/**
*双灭点点时，结合找到的直线，分割地面
*
*/
/*------------------------------------------------------------------------------------------------------------*/
void CuiHough3::HoughLineClassifyGV2(void)
{
	unsigned int Y_SegPosition;
	///分割线一
	for (register unsigned int x=0;x<(unsigned int)cui_Width;x++){
		Y_SegPosition=GndSegment.GndSegLine[0].a*x+GndSegment.GndSegLine[0].b;;
		for (register  int y=cui_Height-1;y>=0;y--){
	           if (y>Y_SegPosition){
				   if (this->cui_ImgLables_SVG[y*cui_Width+x]>0){
						this->cui_ImgLables_SVG[y*cui_Width+x]=Ground11;
				   }					
	           }
		}
	}
	//分割线二
	for (register unsigned int x=0;x<(unsigned int)cui_Width;x++){
		Y_SegPosition=GndSegment.GndSegLine[2].a*x+GndSegment.GndSegLine[2].b;
		for (register  int y=cui_Height-1;y>=0;y--){
			if (y>Y_SegPosition){
				if (this->cui_ImgLables_SVG[y*cui_Width+x]!=Ground11){
					this->cui_ImgLables_SVG[y*cui_Width+x]=Ground12;
				}else if (this->cui_ImgLables_SVG[y*cui_Width+x]==Ground11){
					this->cui_ImgLables_SVG[y*cui_Width+x]=Ground13;

				}
					
			}
		}
	}
	//修正---4分割
	for (register unsigned int x=0;x<(unsigned int)cui_Width;x++){
		double max_vpy=std::max(GndSegment.vpPoint[0].point.y,GndSegment.vpPoint[1].point.y);
		Y_SegPosition=std::max((GndSegment.GndSegLine[1].a*x+GndSegment.GndSegLine[1].b),max_vpy);
		for (register  int y=cui_Height-1;y>=0;y--){
			if (this->cui_ImgLables_SVG[y*cui_Width+x]==Ground){
				this->cui_ImgLables_SVG[y*cui_Width+x]=Vertical;
			}
			if (y<Y_SegPosition){
				if (this->cui_ImgLables_SVG[y*cui_Width+x]<0){
					this->cui_ImgLables_SVG[y*cui_Width+x]=Vertical;
				}
			}
		}
	}
	///////////////////////////////////////////////////////
	int gnd_sp[3]={Ground11,Ground12,Ground13};
	for (int i=0;i<3;i++){
		int category_t=ComputeSVG::ClassifyOneSPlCategory(cui_ImgLables_SVG,cui_Width,cui_Height,gnd_sp[i]);
		for (register int x=0;x<cui_Width;x++){
			for (register int y=cui_Height-1;y>=0;y--){
				if (gnd_sp[i]==this->cui_ImgLables_SVG[y*cui_Width+x]){
					this->cui_ImgLables_SVG[y*cui_Width+x]=category_t;
				}
			}
		}
	}
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
}
/*------------------------------------------------------------------------------------------------------------*/
/**
*根据不同的环境，分割地面和立面
*@see 单灭点分割函数::HoughLineClassifyGV2
*@see 双灭点分割函数::HoughLineClassifyGV
*/
/*------------------------------------------------------------------------------------------------------------*/
void CuiHough3::ClassifyGV(void)
{
	string file_add;
	if (this->ImageEenvironment==BT_bigbuildingVP){
	

		if(1==abs(GndSegment.VPnumber)){
			//建筑环境
			this->HoughLineClassifyGV();
		}else if(2==abs(GndSegment.VPnumber)){
			this->HoughLineClassifyGV2();
		}

		
	}else if (this->ImageEenvironment==BT_bigbuindingFront){
#if 0
	  //一线切割
		this->Use1HorizontalLine2CutGV(this->Get1HorizontalLine(this->hist_scrimg));
#endif
		

	}if (this->ImageEenvironment==BT_trees){
		//这个不存在
		ASSERT(0);
	}if (this->ImageEenvironment==BT_unknow){
		//
		
	}
	/*******************************************/
#if 0
		this->NatureClassifyGV();
#endif
	
	/************************************************************************************************************************************************************/
	switch(this->ImageEenvironment){
	case BT_bigbuildingVP:{
						file_add="_BT_bigbuildingVP";
						break;
						}
			
	case BT_bigbuindingFront:{
						file_add="_BT_bigbuindingFront";
						break;
						}
	case BT_trees:{
						file_add="_BT_trees";
						break;
				  }

	case BT_unknow:{
						  file_add="_BT_unknow";
						  break;
		           } 
	default:	file_add="_Error";

	}
    /************************************************************************************************************************************************************/
	//cui_GeneralImgProcess::Cui_CombinationImgSVG(cui_ImgData,cui_ImgLables,cui_ImgLables_SVG,pMatrix_Category_Lable_SVG,cui_Width,cui_Height,"","","");
	cui_GeneralImgProcess::CuiSaveImgWithPoints(
		cui_ImgData,
		cui_ImgLables_SVG,
		nullptr,
		cui_Width,
		cui_Height,
		(bool)OUT_NOGROUND_IMG,
		pMD,
		file_add);
	/*************************************************/
}
/*------------------------------------------------------------------------------------------------------------*/
/**
*单灭点用，输入图像X坐标，获得分割图像的Y坐标
*
*@param X_coordinate 图像的X坐标
*/
/*------------------------------------------------------------------------------------------------------------*/
unsigned int CuiHough3::GetSegmentPoint(int X_coordinate)
{
	unsigned int Y_coordinate=0;

	double YData[5];
	for(register int i=0;i<3;i++){

		if (GndSegment.VPointPositionIndex[i]){
			YData[i]=GndSegment.GndSegLine[i].a*X_coordinate+GndSegment.GndSegLine[i].b;
		}else{
			YData[i]=0;
		}

	}
		 YData[3]=GndSegment.VanishingPoint.y;
		 YData[4]=pMD->Seg_HorizontalLinePos;
	Y_coordinate=cui_GeneralImgProcess::GetMaxValue(YData,5);

	return Y_coordinate;
}
/*------------------------------------------------------------------------------------------------------------*/
/**
*单灭点用，输入图像X坐标，获得分割图像的Y坐标
*
*@param X_coordinate 图像的X坐标
*
*/
/*------------------------------------------------------------------------------------------------------------*/
unsigned int CuiHough3::GetSegmentPointCompare2VGcontrous(int X_coordinate){
	unsigned int Y_coordinate=0;
	/***********************************************/
	int max_value[3];
	 cui_GeneralImgProcess::GetMaxValueIndex(hist_contours_VG.hist_all,AngLeDivided,max_value,3);  
	 if ((max_value[0]>=AngLeDivided/2-1)&&
		 (max_value[0]<=AngLeDivided/2+1)){
	 }
	 /***********************************************/
	return Y_coordinate;
}
/*------------------------------------------------------------------------------------------------------------*/
/**
*确定用于寻找灭点的直线集合
*
*@param hist_data 直方图数据
*/
/*------------------------------------------------------------------------------------------------------------*/
void CuiHough3::GetPrepareLine(HistData* hist_data)
{
#if 1
	   this->GetLine4cluster2determineVP(hist_data->hist_vertical,VP_V_L_UP,VP_V_R_DOWN);
#else  
	/*********得到左右两个方向最强的方向线************************/
	this->GetHist4EffectiveLinecluster(hist_data->hist_vertical,VP_V_L_UP,VP_V_R_DOWN); //垂直候选方向
	/*********************************************************************************/

  // this->GetHist3effectiveDir(hist_data->hist_vertical,VP_V_L_UP,VP_V_R_DOWN);
#endif	 
	
	for(register int mi=0;mi<2;mi++){
		VP_V_L_UP[mi].PrepareLine.clear(); 
		VP_V_L_UP[mi].PreparePoint.clear();
		VP_V_R_DOWN[mi].PrepareLine.clear();
		VP_V_R_DOWN[mi].PreparePoint.clear();
	}
	/*********计算候选直线****垂直方向两个方向，寻找灭点****************************************************/
	int line_num=cui_line_src_len;
	for (register int li=0;li<cui_line_src_len;li++){
		//地面以上的所有直线
		if (cui_line_srcimg[li].Category>Ground) {
							for(register int mi=0;mi<2;mi++){
								if ((cui_line_srcimg[li].alpha_angle>=VP_V_L_UP[mi].Vp_angle_down)&&
									(cui_line_srcimg[li].alpha_angle<VP_V_L_UP[mi].Vp_angle_up)){
										cui_line_srcimg[li].Used2FindVP=C_Used2FindVP;
										VP_V_L_UP[mi].PrepareLine.push_back(cui_line_srcimg[li]);

								}
							}
							////////////////////
							for(register int mi=0;mi<2;mi++){
								if ((cui_line_srcimg[li].alpha_angle>=VP_V_R_DOWN[mi].Vp_angle_down)&&
									(cui_line_srcimg[li].alpha_angle<VP_V_R_DOWN[mi].Vp_angle_up)){
										cui_line_srcimg[li].Used2FindVP=C_Used2FindVP;
										VP_V_R_DOWN[mi].PrepareLine.push_back(cui_line_srcimg[li]);
								}

							}
		}
	}
	/***********************************************************************************/
	//显示用于计算的直线
	this->cui_ShowImageLine("Used2findVP",C_Used2FindVP,gray_Color_src_img,cui_line_srcimg,cui_line_src_len);
}
/*------------------------------------------------------------------------------------------------------------*/
/**
*确定用于寻找灭点的直线集合
*
*@param hist_data 直方图数据
*@deprecated 已弃用
*/
/*------------------------------------------------------------------------------------------------------------*/
void CuiHough3::Get4ClusterPrepareLine(HistData* hist_data)
{
	/*********得到左右两个方向最强的方向线************************/
	this->GetHist3effectiveDir(hist_data->hist_vertical,VP_V_L_UP,VP_V_R_DOWN); //垂直候选方向
	/*********************************************************************************/
	for(register int mi=0;mi<2;mi++){
		VP_V_L_UP[mi].PrepareLine.clear(); 
		VP_V_L_UP[mi].PreparePoint.clear();
		VP_V_R_DOWN[mi].PrepareLine.clear();
		VP_V_R_DOWN[mi].PreparePoint.clear();
	}
	/*********计算候选直线****垂直方向两个方向，寻找灭点****************************************************/
	int line_num=cui_line_src_len;
	for (register int li=0;li<cui_line_src_len;li++){
		//地面以上的所有直线
		if (cui_line_srcimg[li].Category>Ground) {
			for(register int mi=0;mi<2;mi++){
				if ((cui_line_srcimg[li].alpha_angle>=VP_V_L_UP[mi].Vp_angle_down)&&
					(cui_line_srcimg[li].alpha_angle<VP_V_L_UP[mi].Vp_angle_up)){
						cui_line_srcimg[li].Used2FindVP=C_Used2FindVP;
						VP_V_L_UP[mi].PrepareLine.push_back(cui_line_srcimg[li]);

				}
			}
			////////////////////
			for(register int mi=0;mi<2;mi++){
				if ((cui_line_srcimg[li].alpha_angle>=VP_V_R_DOWN[mi].Vp_angle_down)&&
					(cui_line_srcimg[li].alpha_angle<VP_V_R_DOWN[mi].Vp_angle_up)){
						cui_line_srcimg[li].Used2FindVP=C_Used2FindVP;
						VP_V_R_DOWN[mi].PrepareLine.push_back(cui_line_srcimg[li]);
				}

			}
		}
	}
	/***********************************************************************************/
	//显示用于计算的直线
	this->cui_ShowImageLine("Used2findVP",C_Used2FindVP,gray_Color_src_img,cui_line_srcimg,cui_line_src_len);
}
/*------------------------------------------------------------------------------------------------------------*/
/**
*获取灭点
*
*/
/*------------------------------------------------------------------------------------------------------------*/
void CuiHough3::GetVPoint(void)
{
	this->GetPrepareLine(&hist_scrimg);
	/***********************************************************************************/
	this->FindVPbyLeftorRightCross();
	/***********************************************************************************/
}
/*------------------------------------------------------------------------------------------------------------*/
/**
*将用于寻找灭点的点云绘制到图像上
*
*@param CloudPointL 右灭点对应的点云
*@param CloudPointR 左灭点对应的点云
*@param CenterPoint 点云的中心点
*@param type  中心点位置
*- 1  中心点是左灭点
*- 2  中心点是右灭点
*/
/*------------------------------------------------------------------------------------------------------------*/
void CuiHough3::DrawCloudVPointOnIMG(
	vector<CvPoint> CloudPointL,
	vector<CvPoint> CloudPointR,
	VpPoint CenterPoint,
	int type)
{
	IplImage *img;
	unsigned int Width_Muti=5;
	CvPoint  new_point;
	img=cvCreateImage(cvSize(cui_Width*Width_Muti,cui_Height),IPL_DEPTH_8U,4); 
	cvZero(img);
	cvSetImageROI(img,cvRect(cui_Width*(Width_Muti-1)/2,0,cui_Width,cui_Height));
	cvCopyImage(this->src_img,img);
	cvResetImageROI(img);

	for (unsigned int i=0;i<CloudPointL.size();i++){
		new_point=cvPoint(CloudPointL[i].x+cui_Width*(Width_Muti-1)/2 ,CloudPointL[i].y);
		cvCircle( img, new_point ,1, CV_RGB(255,0,0),-1, 8, 3 );
	}
	for (unsigned int i=0;i<CloudPointR.size();i++){
		new_point=cvPoint(CloudPointR[i].x+cui_Width*(Width_Muti-1)/2 ,CloudPointR[i].y);
		cvCircle( img,new_point ,1 , CV_RGB(0,0,255),-1, 8, 3 );
	}
	new_point=cvPoint(CenterPoint.point.x+cui_Width*(Width_Muti-1)/2 ,CenterPoint.point.y);
	if (CenterPoint.sigema<0){		
	  cvCircle(img, new_point,1,CV_RGB(0xff,0x0,0x0),2);
	}else{
	  cvCircle(img, new_point,CenterPoint.sigema,CV_RGB(0xff,0xff,0xff),2);
	}
	
	if (type==1){
		cvSaveImage("VPointCloudPoint.jpg",img);
	}else if (type==2){
		cvSaveImage("VLineCloudPoint.jpg",img);
	}

	cvReleaseImage(&img);


}
/*------------------------------------------------------------------------------------------------------------*/
/**
*获得点集的中位点
*
*@param   point_t 点集合
*@return 点集合的中心点
*/
/*------------------------------------------------------------------------------------------------------------*/
CvPoint CuiHough3::GetCloudMediaPoint(vector<CvPoint> point_t)
{
	CvPoint Mpoint;
	vector <double> x_t,y_t;

		x_t.clear();  y_t.clear();

		for(register unsigned int pi=0;pi<point_t.size();pi++){
			x_t.push_back(point_t[pi].x);
			y_t.push_back(point_t[pi].y);
		}

		std::sort(x_t.begin(),x_t.end(),greater<double>());
		std::sort(y_t.begin(),y_t.end(),greater<double>());
		if (point_t.size()>0){
			Mpoint.x=x_t.at(x_t.size()/2);
			Mpoint.y=y_t.at(y_t.size()/2);
		}

	return Mpoint;
}
/*------------------------------------------------------------------------------------------------------------*/
/**
*获得点集合的均值点
*
*@param point_t 点集合
*@return 点集合的均值点
*/
/*------------------------------------------------------------------------------------------------------------*/
CvPoint CuiHough3::GetCloudAvgPoint(vector<CvPoint> point_t)
{
	CvPoint Avgpoint;
	vector <double> x_t,y_t;

	x_t.clear();  y_t.clear();

	for(register unsigned int pi=0;pi<point_t.size();pi++){
		x_t.push_back(point_t[pi].x);
		y_t.push_back(point_t[pi].y);
	}

	Avgpoint.x=cui_GeneralImgProcess::GetAverageValue(x_t.data(),x_t.size());
	Avgpoint.y=cui_GeneralImgProcess::GetAverageValue(y_t.data(),y_t.size());

return Avgpoint;
}
/*------------------------------------------------------------------------------------------------------------*/
/**
*通过中值迭代获取灭点
*
*@param  point_t 灭点的点集合
*@param  type  灭点的位置
*- 1 左灭点
*- 2 右灭点
*@return 灭点
*/
/*------------------------------------------------------------------------------------------------------------*/
VpPoint CuiHough3::SelectVPbyMedianIteration(vector<CvPoint> point_t,int type)
{
	CvPoint Vpoint,Avgpoint,Avgpoint_last;
	VpPoint vpPoint_t; 
#if 0
	CvPoint Mpoint,Mpoint_last;
#endif
	vector<double> Diatance;
	double AvgDst,VarianceDst;
	vector<CvPoint>	IterationPoint=point_t;
	vector<CvPoint>	IterationPoint_t;
	/***********************************/
	if (point_t.size()==0){
		vpPoint_t.point=cvPoint(-1,-1);
		vpPoint_t.sigema=-1;
		return vpPoint_t;
	}
	for (double sigema=3;sigema>1;sigema-=0.2){
		do{  
#if 0
			Mpoint=this->GetCloudMediaPoint(IterationPoint);
#endif

			Avgpoint=this->GetCloudAvgPoint(IterationPoint);
			for(register unsigned int i=0;i<IterationPoint.size();i++){
				Diatance.push_back(cui_GeneralImgProcess::P2PointDiatance(Avgpoint,IterationPoint[i]));
			}
			cui_GeneralImgProcess::GetVarianceValue(Diatance.data(),Diatance.size(),&AvgDst,&VarianceDst);

			IterationPoint_t.clear();
			for(register unsigned int i=0;i<	IterationPoint.size();i++){
				double dst_t=cui_GeneralImgProcess::P2PointDiatance(Avgpoint,IterationPoint[i]);
				if ((dst_t>AvgDst-sigema*VarianceDst)&&
					(dst_t<AvgDst+sigema*VarianceDst)){
						IterationPoint_t.push_back(IterationPoint[i]);
				}
			}
			IterationPoint.clear();
			IterationPoint=IterationPoint_t;
#if 0
			Mpoint_last=this->GetCloudMediaPoint(IterationPoint);
#endif

			Avgpoint_last=this->GetCloudAvgPoint(IterationPoint);
			vpPoint_t.point=Avgpoint_last;
			vpPoint_t.sigema=VarianceDst;
			if (type==1){
			    this->DrawCloudVPointOnIMG(VP_V_L_UP[0].PreparePoint,VP_V_R_DOWN[0].PreparePoint,vpPoint_t,type);
			}else if(type==2){
				this->DrawCloudVPointOnIMG(VP_GV_L_UP[0].PreparePoint,VP_GV_R_DOWN[0].PreparePoint,vpPoint_t,type);
			}else if (type==3){

			}
			
		}while(cui_GeneralImgProcess::P2PointDiatance(Avgpoint,Avgpoint_last)>2);	
	}
 
	/************************************************/
	
	vpPoint_t.point=Avgpoint_last;
	vpPoint_t.sigema=VarianceDst;
	/************************************************/
	return vpPoint_t;
}
/*------------------------------------------------------------------------------------------------------------*/
/**
*
*获取分割地面的直线，采用截距（直线间的距离来聚类）
* 
*@param [in]  PrepareLine   要聚类的直线集合
*@param [out] preparepoint  直线集上面所用的点集合
*@return 拟合的直线 
*/
/*------------------------------------------------------------------------------------------------------------*/
LINEDATA CuiHough3::SelectCutLinebyIntercept2(vector<LINEDATA> PrepareLine,vector<CvPoint> preparepoint)
{	 
	LINEDATA line_cut;
	int  min_line_i=-1;
	double  line_distance=0;
	double  min_line_dst=-1;      	

	const int radius=3;
	const int y_pos_num=PrepareLine.size();
	double *y_pos=new double[y_pos_num];
	double *y_pos_len=new double[y_pos_num];
	double y_pos_len_dbg[200];
	double y_pos_dbg[200];
	memset(y_pos_len,0,sizeof(double)*y_pos_num);
	/****************************************/
#if 1
	for (int i=0;i<y_pos_num;i++){
		y_pos[i]=PrepareLine[i].b;
	}
		memcpy(y_pos_dbg,y_pos,sizeof(double)*min(100,y_pos_num));
		//cui_GeneralImgProcess::DrawLine2BlackImg(PrepareLine,cui_Width,,"old");
#endif
	
	/****************************************/
	for (int ri=radius-1;ri<radius;ri++){
		/***************调整//直线************************/
		vector<LINEDATA> PrepareLineAdj=PrepareLine;
#if 1
	  for (int i=0;i<y_pos_num;i++){
			y_pos[i]=PrepareLineAdj[i].b;
		}
		memcpy(y_pos_dbg,y_pos,sizeof(double)*min(100,y_pos_num));
		cui_GeneralImgProcess::DrawLine2BlackImg(PrepareLineAdj,cui_Width,cui_Height,"ADJPrepare");
#endif
		
		this->AdjustCutLineLength(&PrepareLineAdj,radius); 
		const int y_pos_num_adj=PrepareLineAdj.size();
		int   *max_sort=new int[y_pos_num_adj/2];
#if 1
		for (int i=0;i<y_pos_num_adj;i++){
			y_pos[i]=PrepareLineAdj[i].b;
		}
		memcpy(y_pos_dbg,y_pos,sizeof(double)*min(100,y_pos_num));
		cui_GeneralImgProcess::DrawLine2BlackImg(PrepareLineAdj,cui_Width,cui_Height,"adjOK");
#endif
		
		/***************计算范围和**********************/
		memset(y_pos_len,0,sizeof(double)*y_pos_num);
		memset(y_pos_len_dbg,0,sizeof(double)*y_pos_num);
		for (int li=0;li<y_pos_num_adj;li++){
			//自身的长度
			   y_pos_len_dbg[li]=y_pos_len[li]+=PrepareLineAdj[li].LineLength;
			for (int lj=0;lj<y_pos_num_adj;lj++){
				if (li!=lj){
					double dst_t=DistancePoint2Line(PrepareLineAdj.at(lj).pointOne,PrepareLineAdj.at(li));
					if (dst_t<ri){
						//与其临近的长度
						y_pos_len_dbg[li]=y_pos_len[li]+=PrepareLineAdj[lj].LineLength;
				    }
				}
				
			}
		}
		
#if 0  
		/********************统计范围和最大值***********/	
		memcpy(y_pos_len_dbg,y_pos_len,sizeof(double)*min(100,y_pos_num));
		double max_value=y_pos_len[0];
		int max_i=0;
		for (int pi=0;pi<y_pos_num_adj;pi++){
			if (y_pos_len[pi]>=max_value){
				max_value=y_pos_len[pi];
				max_i=pi;

			}
		}
		memcpy(y_pos_len_dbg,y_pos_len,sizeof(double)*min(100,y_pos_num));
		memcpy(y_pos_dbg,y_pos,sizeof(double)*min(100,y_pos_num));
		/*********判断有效************************/
		if (y_pos_len[max_i]>0){
			line_cut= PrepareLineAdj[max_i];
			line_cut.Used2CutImg=true;
		}else{
			line_cut.Used2CutImg=false;
		}
		/*****************************************/
#else  	  	
		int *sort_t=new int[y_pos_num_adj];
		cui_GeneralImgProcess::GetMaxValueIndexdouble(y_pos_len,y_pos_num_adj,sort_t,y_pos_num_adj);
		double line_30_pecsent=y_pos_len[sort_t[y_pos_num_adj/3]];//区最长值代表整体
		delete []sort_t;
		double line_len_avg=cui_GeneralImgProcess::GetAverageValue(y_pos_len,y_pos_num_adj);
		///////////////////////////////////////////
		////////////////////////////////////////////
		double line_2_VGmin=0;
		for (int pi=0;pi<y_pos_num_adj;pi++){
			LINEDATA line_t=PrepareLineAdj[pi];
			if (y_pos_len[pi]>=line_30_pecsent){
				  line_2_VGmin=this->Line2VGContourDistance(line_cut);
				  break;
			}
		}

		for (int pi=0;pi<y_pos_num_adj;pi++){
			 LINEDATA line_t=PrepareLineAdj[pi];
			 if (y_pos_len[pi]>=line_30_pecsent){
				 double line2_VGdst=this->Line2VGContourDistance(line_t);
					if (line2_VGdst<=line_2_VGmin){
						line_2_VGmin=line2_VGdst;
						  line_cut=PrepareLineAdj[pi];
						  line_cut.a=line_cut.a_org;
						  line_cut.b=line_cut.b_org;
						  line_cut.Used2CutImg=true;
							line_cut.pointOne.y=line_cut.a*line_cut.pointOne.x+line_cut.b;
						    line_cut.pointTwo.y=line_cut.a*line_cut.pointTwo.x+line_cut.b;
						    line_cut.AdjPoint1.y=line_cut.a*line_cut.AdjPoint1.x+line_cut.b;
							line_cut.AdjPoint2.y=line_cut.a*line_cut.AdjPoint2.x+line_cut.b;

					}else{
					   line_cut.Used2CutImg=false;
					}
			 }
		}
#endif
	
		delete[] max_sort;
	}
	/****************************************/
	VpPoint vppoint_t;
	vppoint_t.point=line_cut.pointOne;
	vppoint_t.sigema=2;

	this->DrawCloudVPointOnIMG(VP_GV_L_UP[0].PreparePoint,VP_GV_R_DOWN[0].PreparePoint,vppoint_t,2);
	/*********************************************/
	delete[] y_pos;
	delete[] y_pos_len;
	return line_cut;
}
/*------------------------------------------------------------------------------------------------------------*/
/**
*调整直线集的长度,对直线集中相似的部分进行拼接、延长、删除等操作
*
*@param [out] PrepareLine  直线集合
*@param radius 直线拟合时直线间的间距
*/
/*------------------------------------------------------------------------------------------------------------*/
void CuiHough3::AdjustCutLineLength(
	vector<LINEDATA> *PrepareLine,
	int radius)
{	 
	//const int y_pos_num=PrepareLine->size();	
	double dst_t;
	/***************计算范围和*******************/
	for (unsigned int li=0;li<PrepareLine->size();li++){					 
		for (unsigned int lj=0;lj<PrepareLine->size();lj++){
			if (li!=lj){
				dst_t=this->DistancePoint2Line(PrepareLine->at(lj).pointOne,PrepareLine->at(li));
				if (dst_t<radius){
					if (this->Adjust2LinePosition(PrepareLine,li,lj)==false){ 
						li=lj=0;						
					}
				}
			}

		}
	}
	 /****************************************/
	cui_GeneralImgProcess::DrawLine2BlackImg(*PrepareLine,cui_Width,cui_Height,"adjOK");
	/*****************************************/
}
/*------------------------------------------------------------------------------------------------------------*/
/**
*调整两条直线的位置\n
*对重合直线合并,交叉直线合并，错开直线延长等
*
*@param PrepareLine 直线集合
*@param li 直线i
*@param lj 直线j
*@retval  true  直线调整过
*@retval  false 直线为经调整
*/
/*------------------------------------------------------------------------------------------------------------*/
bool CuiHough3::Adjust2LinePosition(
	vector<LINEDATA> *PrepareLine,
	int li,
	int lj){
	 
	LINEDATA* Line_i=&(PrepareLine->at(li));
	LINEDATA* Line_j=&(PrepareLine->at(lj));
    standardLineStructure(Line_i);
	standardLineStructure(Line_j);
	
	if ((Line_i->pointTwo.x<Line_j->pointOne.x)){
	   //i----j
#if 1
		float length_all=cui_GeneralImgProcess::P2PointDiatance(Line_i->pointOne,Line_j->pointTwo);
		float length_part=cui_GeneralImgProcess::P2PointDiatance(Line_i->pointTwo,Line_j->pointOne);
		if (length_part/length_all<=0.3){
			PrepareLine->at(lj).pointOne=PrepareLine->at(li).pointOne;			
			PrepareLine->at(lj).LineLength=cui_GeneralImgProcess::P2PointDiatance(PrepareLine->at(lj).pointOne,PrepareLine->at(lj).pointTwo);
			PrepareLine->erase(PrepareLine->begin()+li);
			return false;
		}else{
			 return true;
		}
		
#else
         return true;
#endif
		  
	}else if((Line_j->pointTwo.x<Line_i->pointOne.x)){
		 //j---i
#if 1
		float length_all=cui_GeneralImgProcess::P2PointDiatance(Line_j->pointOne,Line_i->pointTwo);
		float length_part=cui_GeneralImgProcess::P2PointDiatance(Line_j->pointTwo,Line_i->pointOne);
		if (length_part/length_all<=0.3){
		PrepareLine->at(lj).pointTwo=PrepareLine->at(li).pointTwo;				
		PrepareLine->at(lj).LineLength=cui_GeneralImgProcess::P2PointDiatance(PrepareLine->at(lj).pointOne,PrepareLine->at(lj).pointTwo); 
		PrepareLine->erase(PrepareLine->begin()+li);
		    return false;
		}else{
			return true;
		}
#else
        return  true;
#endif
		   
	}else if ((Line_i->pointOne.x<=Line_j->pointOne.x)&&
		      (Line_i->pointTwo.x>=Line_j->pointTwo.x)){
			 // i包J
			  PrepareLine->erase(PrepareLine->begin()+lj);
			return false;	
		
	}else if((Line_j->pointOne.x<=Line_i->pointOne.x)&&
		     (Line_j->pointTwo.x>=Line_i->pointTwo.x)){
		  //j包I包含
			PrepareLine->erase(PrepareLine->begin()+li);
			return false;

	}else if ((Line_i->pointOne.x<Line_j->pointOne.x)&&
			  (Line_i->pointTwo.x>=Line_j->pointOne.x))	{
		 //相交 i--j
			  	   PrepareLine->at(lj).pointOne=PrepareLine->at(li).pointOne;
				   PrepareLine->at(lj).LineLength=cui_GeneralImgProcess::P2PointDiatance(PrepareLine->at(lj).pointOne,PrepareLine->at(lj).pointTwo);
				  PrepareLine->erase(PrepareLine->begin()+li);
			  	  return false;	
	
	}else if((Line_j->pointOne.x<Line_i->pointOne.x)&&
		     (Line_j->pointTwo.x>=Line_i->pointOne.x))	{
			//相交 j---i
				 PrepareLine->at(lj).pointTwo=PrepareLine->at(li).pointTwo;
				 PrepareLine->at(lj).LineLength=cui_GeneralImgProcess::P2PointDiatance(PrepareLine->at(lj).pointOne,PrepareLine->at(lj).pointTwo);
				 PrepareLine->erase(PrepareLine->begin()+li);
				  return false;
	}else{
		;
		ASSERT(FALSE);
		return true;
	}

}
/*------------------------------------------------------------------------------------------------------------*/
/**
*调整直线两端点存放的位置\n
*X坐标较小的放右边，X坐标较小的放左边
*
*@param [in out] 直线集合 
*/
/*------------------------------------------------------------------------------------------------------------*/
void CuiHough3::standardLineStructure(LINEDATA  *line){
	
	LINEDATA line_t=*line;
	
	if (line_t.AdjPoint1.x>line_t.AdjPoint2.x){
		
		line->AdjPoint1=line_t.AdjPoint2;
		line->AdjPoint2=line_t.AdjPoint1;
	}
}
/*------------------------------------------------------------------------------------------------------------*/
/**
*线段到立面与地面分界线的距离
*
*@param line 线段
*@return 平均距离
*/
/*------------------------------------------------------------------------------------------------------------*/
double CuiHough3::Line2VGContourDistance(LINEDATA line)
{
	double distance_t=0;
	for (register unsigned int pi=0;pi<GV_Segment_point.size();pi++){
		distance_t+=this->DistancePoint2Line(GV_Segment_point[pi],line);

	}
	distance_t/=GV_Segment_point.size();
	return distance_t;
}
/*------------------------------------------------------------------------------------------------------------*/
/**
*线段到灭点所在直线的距离
*
*@param line 线段
*@return 平均距离
*/
/*------------------------------------------------------------------------------------------------------------*/
double CuiHough3::Line2VPContourDistance(LINEDATA line)
{
	double distance_t=0;
	vector<CvPoint>	VP_Contour_point;

	for (register int px=0;px<cui_Width;px++){

	   VP_Contour_point.push_back(cvPoint(px,GndSegment.VanishingPoint.y));
	
	}
	for (register unsigned int pi=0;pi<VP_Contour_point.size();pi++){
		
		distance_t+=this->DistancePoint2Line(VP_Contour_point[pi],line);

	}
	distance_t/=GV_Segment_point.size();
	return distance_t;
}
/*------------------------------------------------------------------------------------------------------------*/
/**
*用水平线所在位置来调整，天空，立面，地面
*@param[in] cui_ImgLables_SVG 天空、立面、地面的标定图
*@param Width  图像宽度
*@param Height 图像长度
*@param horLine 水平线的直线方程（可能没有）
*@param[in] vpPoint 灭点位置
*@param EyeLevel  视平线位置（像素坐标）
*
*/
/*------------------------------------------------------------------------------------------------------------*/
void CuiHough3::UseEyeLevel2AdjustSVG(
	int* cui_ImgLables_SVG,
	int Width,
	int Height,
	LINEDATA horLine,
	VpPoint *vpPoint,
	int EyeLevel)
{
	//修正
	//1.双灭点位置--
	//2.视平线
	//3.水平分割线
	//以最下面的那根做分割线
	//cui_ImgLables_SVG--其内容为 321
	float max_vpy=std::max(vpPoint[0].point.y,vpPoint[1].point.y);
	
	for (register unsigned int x=0;x<(unsigned int)Width;x++){
		float  max_line=std::max((double)(horLine.a*x+horLine.b),(double)(EyeLevel));
		unsigned int Y_SegPosition=std::max(max_line,max_vpy);
		for (register  int y=Height-1;y>=0;y--){
			

			if (y<Y_SegPosition){
				//视平线之上的地为立面
				if (cui_ImgLables_SVG[y*Width+x]==Ground){
					cui_ImgLables_SVG[y*Width+x]=Vertical;
				}
			}
#if 0
			if (y>=Y_SegPosition){
				//视频线之下的天空为地面
				if (cui_ImgLables_SVG[y*Width+x]==Sky){
					cui_ImgLables_SVG[y*Width+x]=Ground;
				}
			}
#endif


		}
	}
	//质疑
}
/*------------------------------------------------------------------------------------------------------------*/
/**
*利用适当的方法，获得分割图像的方法\n
*比如，需要找灭点时，获得灭点和对应的分割线
*
*/
/*------------------------------------------------------------------------------------------------------------*/
void CuiHough3::GetClassifyMethod(void){
	if (this->ImageEenvironment==BT_bigbuildingVP){
		//人造环境
		this->GetVPoint();

#if 0
		//原图地面上的线
		//hough.GetGroundSegmentLine(&hough.hist_scrimg);
		hough.Get3GroundSegmentLine(&hough.hist_scrimg,hough.cui_line_srcimg,hough.cui_line_src_len);
#else
		//VG地面上的线
		//hough.Get1GroundSegmentLine(&hough.hist_scrimg,hough.cui_line_srcimg,hough.cui_line_src_len);
		//原图轮廓上的线
		this->Get1GroundSegmentLine(&this->hist_contours,this->cui_line_contour,this->cui_line_contour_len);

		//hough.Get1GroundSegmentLine(&hough.hist_contours_VG,hough.cui_line_VGcontour,hough.cui_line_VGcontour_len);

		this->Get2GroundSegmentLine(&this->hist_contours,this->cui_line_contour,this->cui_line_contour_len);

		//hough.GetGroundSegmentLine(&hough.hist_contours);
#endif				
	}else if (this->ImageEenvironment==BT_bigbuindingFront){
		//
	}if (this->ImageEenvironment==BT_trees){
		//
	}if (this->ImageEenvironment==BT_unknow){
	   //
	}
}
/*------------------------------------------------------------------------------------------------------------*/
/**
*在地面上找一根用于分割图像的水平线
*
*@param histdata 直方图数据
*@return 直线数据  
*/
/*-----------------------------------------------------------------------------------------------------------*/
LINEDATA CuiHough3::Get1HorizontalLine(HistData histdata){
	LINEDATA Line_t;
	memset(&Line_t,0,sizeof(LINEDATA));
	////////////////////////////////////////////////////////////////////////////////////////////////////
	int max_sort[3];
	float *hist=histdata.hist_all;
	LINEDATA *linedata=this->cui_line_VGcontour;
	int linelen=this->cui_line_VGcontour_len;
	cui_GeneralImgProcess::GetMaxValueIndex(hist,AngLeDivided,max_sort,sizeof(max_sort)/sizeof(int));
	//max_sort[0]---jiaodu
	for(long li=0;li<linelen;li++){
		if (linedata[li].Category==Ground){
			if (
#if 0
                (linedata[li].alpha_angle>=max_sort[0]*180/AngLeDivided)
				&&(linedata[li].alpha_angle<=(max_sort[0]+1)*180/AngLeDivided)
#else
				(linedata[li].alpha_angle>=180-(180/AngLeDivided)*1)    
				||(linedata[li].alpha_angle<=(180/AngLeDivided)*1)
#endif
				
				
				)
			{
				//地面之上，候选角度之内
				if (linedata[li].LineLength>Line_t.LineLength){
					Line_t=linedata[li];
				}
			}
		}
	}
    //直线未找到如何分割//////////////////////////////////////////////////////////////////////////////////
	return Line_t;
}
/*-----------------------------------------------------------------------------------------------------------*/
/**
*利用一条水平线来切割地面，立面
*@param line 切割用的水平线
*@deprecated 已弃用
*/
/*-----------------------------------------------------------------------------------------------------------*/
void CuiHough3::Use1HorizontalLine2CutGV(LINEDATA line){
	if (line.LineLength==0){
		return ;
	}
	/**********************************************************************************/
	for (register unsigned int x=0;x<(unsigned int)cui_Width;x++){
		unsigned int Y_SegPosition=(line.a*x+line.b);
		for (register  int y=cui_Height-1;y>=0;y--){
#if 1
		if (y<Y_SegPosition){
				//视平线之上的地为立面
				if (cui_ImgLables_SVG[y*cui_Width+x]==Ground){
					cui_ImgLables_SVG[y*cui_Width+x]=Vertical;
				}
			}
#endif	
#if 1
			if (y>=Y_SegPosition){
				//视频线之下的天空为地面
				if (cui_ImgLables_SVG[y*cui_Width+x]!=Ground){
					cui_ImgLables_SVG[y*cui_Width+x]=Ground;
				}
			}
#endif


		}
	}
	/**********************************************************************************/
}
/*-----------------------------------------------------------------------------------------------------------*/

