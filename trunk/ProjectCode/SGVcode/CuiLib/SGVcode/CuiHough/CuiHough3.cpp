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
��һ��������صĲ���:
�Դ�ͳ Hough �任����ʹ��(0).
�Ը��� Hough �任��������С�߶γ���.
�Զ�߶� Hough �任�����Ǿ��뾫�� rho �ķ�ĸ (���µľ��뾫���� rho ����ȷ��Ӧ���� rho / param1 ).
param2
�ڶ���������ز���:
�Դ�ͳ Hough �任����ʹ�� (0).
�Ը��� Hough �任�����������ʾ��ͬһ��ֱ���Ͻ������߶����ӵ������ֵ(gap), ����ͬһ��ֱ���ϵ���
�����߶�֮��ļ��С��param2ʱ������϶�Ϊһ��
�Զ�߶� Hough �任�����ǽǶȾ��� theta �ķ�ĸ (���µĽǶȾ����� theta ����ȷ�ĽǶ�Ӧ���� theta / param2).

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
*���캯����ʼ����Ա�����������ã�
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
*���캯����ʼ����Ա����
*����ͼ������õ����ݽṹ
*@param MemData_t �����õ��м����
*/
/*------------------------------------------------------------------------------------------------------------*/
CuiHough3::CuiHough3(ImageMemData* MemData_t)
{	
  this->pMD=MemData_t;
  this->InitParam();
}
/*------------------------------------------------------------------------------------------------------------*/
/**
*�ͷż�������з�����ڴ�
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
*��ʼ����Ա����
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
*��������õ��ڴ�
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
*ͨ���ļ�����ȡͼƬ������
*@param img_filename �ļ���
*/
/*------------------------------------------------------------------------------------------------------------*/
void CuiHough3::GetImageData(string img_filename){
	this->ClearImgMemory();
	this->src_img=cvLoadImage(img_filename.c_str());
}
/*------------------------------------------------------------------------------------------------------------*/
/**
*ͨ��IplImage���ݽṹ�����ڴ�
*@param imgscr ͼƬ��Ӧ��ԭʼ����
*/
/*------------------------------------------------------------------------------------------------------------*/
void CuiHough3::GetImageData(IplImage* imgscr)
{
	this->ClearImgMemory();
	this->src_img=imgscr;
}
/*------------------------------------------------------------------------------------------------------------*/
/**
*ͨ���ڴ���ֱ�ӵ�������
*
*@param ImgData  ͼƬ����
*@param Width    ͼƬ���
*@param Height   ͼƬ�߶�
*@param Lables   ͼƬ�ĳ����ر궨����
*@param LablesSVG ͼƬ����ա����桢����궨����
*@param NumLabels ͼƬ�г����صĸ���
*@param Category  ÿ�������ؿ��Ӧ�ķ������
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
*�������������ķֽ��ߣ�
*���ֽ����������浽cui_ImgData_Contour_GV��
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
	/*cui_ImgData_Contour_GVȡ�ֽ��ߵĵ�*/
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
	/*cui_ImgData_Contour_GV���»��Ʒֽ���*/
  	for (unsigned int pi=0;pi<GV_Segment_point.size();pi++){
		CvPoint ContourPoint=cvPoint(GV_Segment_point[pi].x,GV_Segment_point[pi].y);
		cui_ImgData_Contour_GV[ContourPoint.y*cui_Width+ContourPoint.x]=WhiteColorPNG;
  	}
   
   cui_GeneralImgProcess::CuiSaveImageData(cui_ImgData_Contour_GV,cui_Width,cui_Height,
	   pMD->FileReadFullPath,pMD->FileWritePath,1,"Contours_GV");
}
/*------------------------------------------------------------------------------------------------------------*/
/**
*��һ��ͼ����Ѱ�����
*@parma[in] src ҪѰ�ҵ�ͼ��
*/
/*------------------------------------------------------------------------------------------------------------*/
int CuiHough3::findLongestLine(IplImage* src)
{
	IplImage* dst = cvCreateImage( cvGetSize(src), 8, 1 ); 
	IplImage* src1=cvCreateImage(cvGetSize(src),IPL_DEPTH_8U,1); //�Ҷ�
	
	IplImage* color_dst = cvCreateImage( cvGetSize(src), 8, 3 );
	CvMemStorage* storage = cvCreateMemStorage(0); 
	
	CvSeq* lines = 0;
	int i;
	

	cvCvtColor(src, src1, CV_BGR2GRAY);  
	//cvCopy(src,src1);
	cvCanny( src1, dst, 50, 200, 3 );

	cvCvtColor( dst, color_dst, CV_GRAY2BGR );

#if SHOW_IMG

	cvShowImage("src",dst); //�Ҷ�
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
	cvLine( color_dst, pointOne, pointTwo, CV_RGB(255,0,0), 3, 8 );    //�������ֱ��


	double Angle=0.0;

	Angle = atan2(fabsl(pointTwo.y-pointOne.y),fabsl(pointTwo.x-pointOne.x));   //�õ��ֱ����ˮƽ�н�

	if(pointTwo.x>pointOne.x && pointTwo.y>pointOne.y)
	{
		Angle=-Angle;
	}

	Angle=Angle*180/CV_PI;
	cout<<"�Ƕ� "<<Angle<<endl;
	
	cvShowImage( "Source", src ); 	
	cvShowImage( "Hough", color_dst );


   return 0;
}
/*------------------------------------------------------------------------------------------------------------*/
/**
*�Ƴ�ֱ��ͳ��ֱ��ͼ�й���ˮƽ�ʹ�ֱ��ֱ��
*�Ա�۲�ͼ����Ѱ��������õ�ֱ��
*@param hist_data ֱ��ͼ����
*@param hist_size ֱ��ͼ��С
/*------------------------------------------------------------------------------------------------------------*/
void   CuiHough3::RemoveHistData0and90(float *hist_data,int hist_size){
	//ȥ��0/180�Ⱥ�
	hist_data[0]=0;
	hist_data[1]=0;
	hist_data[hist_size-2]=0;
	hist_data[hist_size-1]=0;
	/****90������************/
	hist_data[hist_size/2-1]=0;
	hist_data[hist_size/2]=0;
	hist_data[hist_size/2+1]=0;
}
/*------------------------------------------------------------------------------------------------------------*/
/**
*����cany���ӣ�src_unknow��ֵ��ͼ��
*
*@param src_unknow ԭͼ����Ϊ����ͼ��
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

	

	cvShowImage("src",dst_binaryzation); //�Ҷ�
	return 0;
}
/*------------------------------------------------------------------------------------------------------------*/
/**
*��ȡͼ�������е�ֱ��
*
*@param src_img_t[in]		ԭͼ�ڴ�����
*@param gray_Color_img_t [out] �õ��Ҷ�ͼ��
*@param plinedata [out] �����ֱ������
*@param pline_len [out] ֱ������
*@param int method [in] ͼ��ƽ������
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
	gray_img=cvCreateImage(cvGetSize(*src_img_t),IPL_DEPTH_8U,1); //�Ҷ�
	*gray_Color_img_t = cvCreateImage( cvGetSize(*src_img_t), 8, 3 );
	/*******************************/
	this->ImgPreprocess(src_img_t,method);
	/*******************************/
	cvCvtColor( gray_binary_img, *gray_Color_img_t, CV_GRAY2BGR );
#if SHOW_IMG
cvShowImage("src",gray_binary_img); //�Ҷ�
#else

#endif
	CvPoint* point_t;
	//2013-11-19lines = cvHoughLines2(gray_binary_img, storage, CV_HOUGH_PROBABILISTIC, 1, CV_PI/180,35,20,4);
	//2013-11-19lines = cvHoughLines2(gray_binary_img, storage, CV_HOUGH_PROBABILISTIC, 1, CV_PI/180,25,20,4);
	//
if (method==1){
	//����---����
	cvSaveImage("gray_binary_img.jpg",gray_binary_img);
	IplImage* color_img_t=cvLoadImage("gray_binary_img.jpg");
	cvConvertImage(color_img_t,gray_binary_img,CV_BGR2GRAY);
	cvReleaseImage(&color_img_t);
	//�����и���BUG
	lines = cvHoughLines2(gray_binary_img, storage, CV_HOUGH_PROBABILISTIC, 1, CV_PI/180,20,20,2);
}else if (method==2){  
	//ԭͼ
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
		   //����90������
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
		/**************����************************/
			linedata[i].Category=Unclassify;
		
	}
	 memcpy(cui_line_cpy,*plinedata,min(MAXLINEDATA,lines->total)*sizeof(LINEDATA));
	 this->AdjustLine(*plinedata,*pline_len);


}
/*------------------------------------------------------------------------------------------------------------*/
/**
*��ó�����������hough�任����
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
		 /******��ʾֱ��*********************/
//		 this->cui_ShowImageLine("Classify_1_",0);
		 this->GetAllhoughLineData(&imgContour,&gray_Color_contour_img,&cui_line_contour,&cui_line_contour_len,1);
#else 	
		 /**********����ֱ��*******************/
		 this->ClassifyLine(cui_line_contour,cui_line_contour_len,gray_Color_contour_img,"Contour_");

#endif  
		 /*********������Чֱ��************/
		 this->CalculateEffectiveLine(cui_line_contour,cui_line_contour_len,&hist_contours,gray_Color_contour_img,"Contour_");
		
		 cvSaveImage("contour.jpg",imgContour);
		 cvReleaseImage(&imgContour);

}
/*------------------------------------------------------------------------------------------------------------*/
/**
*�������͵���֮��������hough�任����
*
*/
/*------------------------------------------------------------------------------------------------------------*/
void CuiHough3::GetAllHoughLineByContourVG(void)
{
	IplImage *imgContourVG;

	imgContourVG=cvCreateImage(cvSize(cui_Width,cui_Height),IPL_DEPTH_8U,4);
	memcpy(imgContourVG->imageData,cui_ImgData_Contour_GV,imgContourVG->imageSize);
	this->GetAllhoughLineData(&imgContourVG,&gray_Color_contour_img,&cui_line_VGcontour,&cui_line_VGcontour_len,1);

	/**********����ֱ��*******************/
	this->ClassifyLine(cui_line_VGcontour,cui_line_VGcontour_len,gray_Color_contour_img,"Contour_VG_");


	/*********������Чֱ��************/
	this->CalculateEffectiveLine(cui_line_VGcontour,cui_line_VGcontour_len,&hist_contours_VG,gray_Color_contour_img,"Contour_VG_");

	cvSaveImage("contourVG.jpg",imgContourVG);
	cvReleaseImage(&imgContourVG);

}
/*------------------------------------------------------------------------------------------------------------*/
/**
*�������͵���֮��������hough�任����
*
*/
/*------------------------------------------------------------------------------------------------------------*/
void CuiHough3:: GetAllHoughLineBySourceImg(void)
{
	  this->GetAllhoughLineData(&src_img,&gray_Color_src_img,
		  &cui_line_srcimg,&cui_line_src_len,2);
//#ifdef TEST_CUI
//	  /******��ʾֱ��*********************/
//	 
//	  this->GetAllhoughLineData(&src_img,&gray_Color_src_img,&cui_line_srcimg,&cui_line_src_len,2);
//#else 	
	  /**********����ֱ��*******************/
	  this->ClassifyLine(cui_line_srcimg,cui_line_src_len,gray_Color_src_img,"Classify1_src");

//#endif  
	  /*********������Чֱ��************/
	  this->DetermineSceneCategory(&hist_scrimg,
		  cui_line_srcimg,cui_line_src_len);

	  this->CalculateEffectiveLine(cui_line_srcimg,cui_line_src_len,
		  &hist_scrimg,gray_Color_src_img,"SrcImg");
}
/*------------------------------------------------------------------------------------------------------------*/
/**
*��ͼ������ʾ���ݣ������浽����
*
*@param string filename ������ļ���
*@param directline ��ʾֱ�ߵķ���
*@param gray_Color_img �Ҷ�ͼ��
*@param cui_line_data [in]   ֱ������
*@param line_len [in]        ֱ�߳���
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
	cvSaveImage("C:\\Documents and Settings\\Administrator\\����\\src_img.jpg",src_img_t);
	cvSaveImage("C:\\Documents and Settings\\Administrator\\����\\gray_Color_src_img.jpg",gray_Color_src_img_t);
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
*���ݽǶȴ�HSV��ɫ�ռ��ȡ��ɫ
*
*@param angle_color HSV��ɫ�ռ���H�ı仯��Χ0-180
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
*��û�ͼʱ��ֱ�ߵĿ��
*
*@param category_t ��ա����桢����ķ���
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
*��ֱ��ͼ���Ƶ�ͼƬ�ϣ������浽Ӳ��
*@param winname  �ļ���
*@param hist_data[in] ֱ��ͼ����
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


	  /** ��ȡֱ��ͼͳ�Ƶ����ֵ�����ڶ�̬��ʾֱ��ͼ */
	  float max_value;
	  int h_bins=180;
	  cvGetMinMaxHistValue( hist, 0, &max_value, 0, 0 );


	  /** ����ֱ��ͼ��ʾͼ�� */
	  int height = 240;
	  int bin_w=6;
	  int width =h_bins*bin_w;
	  IplImage* hist_img = cvCreateImage( cvSize(width,height), 8, 4 );
#if SHOW_IMG
	  cvShowImage(winname.c_str(),hist_img);
#endif	 
	  cvZero( hist_img );
	  /** ��������HSV��RGB��ɫת������ʱ��λͼ�� */

	  for(int h = 0; h < h_bins; h++){
		  /** ���ֱ��ͼ�е�ͳ�ƴ�����������ʾ��ͼ���еĸ߶� */
#if 1
		  float bin_val = cvQueryHistValue_1D( hist, h);
#else
		  float bin_val =hist_value[h];
#endif

		  int intensity = cvRound(bin_val*height/max_value);

		  /** ��õ�ǰֱ��ͼ�������ɫ��ת����RGB���ڻ��� */

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
*��ֱ�߽��з��࣬ȷ��ÿ��ֱ�������ķ���
*ֱ�߷������ ��� ���� ����
*
*@param line_data [in]ֱ������
*@param line_len  [in]ֱ������
*@param gray_Color_img [in] ԭʼͼ���Ӧ�ĻҶ� 
*@param add_t Ҫ������ļ��������ַ���
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
	 /******��ʾֱ��*********************/
	 this->cui_ShowImageLine(add_t,0,gray_Color_img,line_data,line_len);
	 //here IS a BUg
	// 2013-11-30 
 }
 /*------------------------------------------------------------------------------------------------------------*/
/**
*���ֱ��λ����ա����滹�ǵ���
*
*@param point1 �߶ε�����һ�˵�
*@param point2 �߶ε�����һ�˵�
*@return ����ֱ�ߵ�λ��
*- SKY �߶����������
*- VERTICAL �߶�����������
*- GROUND �߶��ڵ�������
*- Remove �߶ο�����
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
*����ͼ���ֱ�߼���ȡֱ�߶�Ӧ��ֱ��ͼ�������浽Ӳ��
*
*@param filename  ֱ��ͼ�ļ���
*@parma [in] linedata  ֱ������
*@param [in] linelen   ֱ������
*@param [out] hist_data ֱ��ͼ���ݽṹ
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
	  /****ͳ��ֱ��ͼ*****************************/
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
/** ����ֱ��ͼ��ʾͼ�� */
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
						/** ��ȡֱ��ͼͳ�Ƶ����ֵ�����ڶ�̬��ʾֱ��ͼ */
						float max_value;
						
						cvGetMinMaxHistValue( hist[ci], 0, &max_value, 0, 0 );


						
						
						/** ��������HSV��RGB��ɫת������ʱ��λͼ�� */
						cvSetImageROI(hist_img,cvRect(0,hist_img->height/HistNUM*ci,hist_img->width,hist_img->height));
						for(int h = 0; h < h_bins; h++){
							/** ���ֱ��ͼ�е�ͳ�ƴ�����������ʾ��ͼ���еĸ߶� */
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
							/** ��õ�ǰֱ��ͼ�������ɫ��ת����RGB���ڻ��� */

							CvScalar color=GetGradientColor(1.0*h/AngLeDivided*180);
							
							CvFont font;
							cvInitFont(&font,CV_FONT_VECTOR0,0.3,0.3,0,1,8);
							//��ͼ������ʾ�ı��ַ���
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
*ɾ������Ҫ��ֱ���飨������ã�
*@param [in]  linedata  ֱ������
*@param [in] linelen    ֱ������
*@param [out] hist_data ֱ�߶�Ӧ��ֱ��ͼ����
*@param [in] gray_color_img ԭͼ��Ӧ�ĻҶ�ͼ��
*@param file_add �����ļ���
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
		/******��ʾֱ��*********************/

	 }	 
	 this->cui_ShowImageLine(file_add+"Classify_NO90_",1,gray_color_img,linedata,linelen);
 }
/*------------------------------------------------------------------------------------------------------------*/
/**
*ͼ���Ԥ������ԭʼͼ����ж�ֵ������
*
*@param src_img_t ԭʼͼ����ͨ��
*@param  method  
*- 1 ԭͼ����Ϊ�ڰ�ͼ��ֱ�ӿ���
*- 2 ��������С�������ԭͼ�����˲�
*/
/*------------------------------------------------------------------------------------------------------------*/
 void CuiHough3::ImgPreprocess(
	 IplImage **src_img_t,
	 int method){

	 cvCvtColor(*src_img_t, gray_img, CV_BGR2GRAY);  
	 /*******************************/
	//cvSmooth(gray_img,gray_img); 
/*
	 //��ֵ�˲� 
	 cvSmooth(gray_img,gray_img,CV_MEDIAN,3,3,0,0);
	   //����ƽ���˲�
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
	 //��˹�˲�
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
*ȥ��ֱ������90�ȵ�ֱ�ߣ�����۲�ͼ���
*@param line_data ֱ������
*@param line_len  ֱ�߳���
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
*����ͼƬ�ĳ���
*
*@param [in] hist_data  ֱ��ͼ����
*@param [in] linedata ֱ������
*@param  linelen  ֱ������
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
*����ֱ�ߣ�����ֱ����X=0��X=img_width�Ľ���
*������ʾ
*
*@param [in] cui_line_data ֱ������
*@param cui_line_len  ֱ�߳���
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
*�������Ƶ�ͼ���ϣ�ͼ�񱣴浽Ӳ��
*
*@param GNDSegline  ���㣬�����и�����õķֽ���
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
   /******����N����************************************************************/
	
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
	 /**����4==1��-����****************************************************************/
	 for (register int lefti=0;lefti<2;lefti++){
		
		 color_use=GetGradientColor((VP_V_L_UP[lefti].Vp_angle_down+VP_V_L_UP[lefti].Vp_angle_up)/2);
		// cvLine(src_img_t,cvPoint(0,VP_V_L_UP[lefti].Vpoint.y),cvPoint(src_img->width,VP_V_L_UP[lefti].Vpoint.y),color_use,thickness, 8 );   
		 cvLine(gray_Color_src_img_t,cvPoint(0,VP_V_L_UP[lefti].Vpoint.y),cvPoint(src_img->width,VP_V_L_UP[lefti].Vpoint.y),color_use,thickness, 8 ); 
		color_use=GetGradientColor((VP_V_R_DOWN[lefti].Vp_angle_down+VP_V_R_DOWN[lefti].Vp_angle_up)/2);
		//cvLine(src_img_t,cvPoint(0,VP_V_R_DOWN[lefti].Vpoint.y),cvPoint(src_img->width,VP_V_R_DOWN[lefti].Vpoint.y),color_use,thickness, 8 );   
		cvLine(gray_Color_src_img_t,cvPoint(0,VP_V_R_DOWN[lefti].Vpoint.y),cvPoint(src_img->width,VP_V_R_DOWN[lefti].Vpoint.y),color_use,thickness, 8 );
		
	 } 
	 if (GNDSegline){ 
		 /*****ˮƽ��***********/
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
*����Ѱ�����ʱ�������ߵķ����ߣ���һ�߸�ǿ
*@retval -1 ��ߣ��������ķ����߸�ǿ
*@retval 1  �ұߣ��������ķ����߸�ǿ
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
		//left���ڼ���
	  return -1;
	}else{
		//right���ڼ���
	  return 1;
	}
	
 }
/*------------------------------------------------------------------------------------------------------------*/
/**
*����ֱ��ͼ���� ��������ĸ���
*
*@param[in] hist_data ֱ��ͼ����
*@param scale ֱ��ͼ���෽����ǿ�ȱ�ֵ���ٽ�ֵ��
*
*@return �������ʾ�����������У���������������Ѱ������Ϊ�ɿ�
*�������ʾ�����������У������ݼ�������Ѱ������Ϊ�ɿ�
*
*/
/*------------------------------------------------------------------------------------------------------------*/
int CuiHough3::CheckVPNumberbyHist(
	HistData* hist_data,
	double scale)
{
	float hist_vertical_t[AngLeDivided],all_sum=0,left_up_sum=0,right_down_sum=0,left_right_scale=0;
	memcpy(hist_vertical_t,hist_data->hist_vertical,sizeof(hist_vertical_t));
	//ȥ��0/180�Ⱥ�
	hist_vertical_t[0]=0;
	hist_vertical_t[AngLeDivided-1]=0;
	/****90������************/
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
		//����� ��up��Ѱ��
		return -1;
	}else if (right_down_sum>scale){
		//����㣬down��Ѱ��
		return 1;
	}else if (max(left_up_sum,right_down_sum)<scale){
		
		if (left_up_sum>right_down_sum){
			//˫��� up�����Ч
             return -2;
		}else{
			//˫��㡣down�����Ч
			 return 2;
		}
	}else{
		ASSERT(0);
		return 0;
	}
}
/*------------------------------------------------------------------------------------------------------------*/
/**
*��������õ�gnddata������
*
*@param [out] gnddata �и���������ݽṹ������ֱ����������
*@param  p1st ���һ
*@param  p2nd ����
*@param  point_num ������
*@param  Img_width ͼ���ȣ����ڼ������λ�ã�
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
	////Left_��0���� Right �ڡ�1��////////////////////////////
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
*����ҵ�������㣬�Ƚ�������λ�ã���������һ�����
*
*@param line_in_use δʹ�õ��β�
*
*/
/*------------------------------------------------------------------------------------------------------------*/
void CuiHough3::CalculateEffetiveVPoint(int line_in_use)
{
	if (2==GndSegment.VPnumber){
		///////////////////////////////////////////////////////////////
		if ((GndSegment.VPposition[0]==VanishPosition::RIGHT)){
				if ((GndSegment.VPposition[1]==VanishPosition::LEFT)){
					//�жϾ��� �Ƴ�second
					if (abs(GndSegment.vpPoint[0].point.y-GndSegment.vpPoint[1].point.y)>100){
						 GndSegment.VPnumber=1;
					}

				}else{
					//�Ƴ�second ���
				   GndSegment.VPnumber=1;
				}
		}
		///////////////////////////////////////////////////////////////
			if ((GndSegment.VPposition[0]==VanishPosition::LEFT)){
				if ((GndSegment.VPposition[1]==VanishPosition::RIGHT)){
					//�жϾ��� �Ƴ�second
					if (abs(GndSegment.vpPoint[0].point.y-GndSegment.vpPoint[1].point.y)>100){
						 GndSegment.VPnumber=1;
					}

				}else{
					//�Ƴ�second ���
					GndSegment.VPnumber=1;
				}
		}
		/////////////////////////////////////////////////////////////////
	 }
	

}
/*------------------------------------------------------------------------------------------------------------*/
/**
*Ѱ����㣬ͬһ������ͬ�Ƕȵ�ֱ���ཻ��ȡֱ��
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
			//left���ߣ�δ����left��			
			cross_t[0]=this->FindVPby2ClusterLine(&VP_V_L_UP[0],&VP_V_L_UP[1]);
			SetPoint2GndData(&this->GndSegment,cross_t[0],cross_t[0],line_in_use,cui_Width);
		    break;
			}
		
	case 1:{
			//right��
		   cross_t[0]=this->FindVPby2ClusterLine(&VP_V_R_DOWN[0],&VP_V_R_DOWN[1]);
		   SetPoint2GndData(&this->GndSegment,cross_t[0],cross_t[0],line_in_use,cui_Width);
		   break;
		}
		
	case 2:{
			//right�� ��ǿ
			cross_t[0]=this->FindVPby2ClusterLine(&VP_V_R_DOWN[0],&VP_V_R_DOWN[1]);
			cross_t[1]=this->FindVPby2ClusterLine(&VP_V_L_UP[0],&VP_V_L_UP[1]);			
			SetPoint2GndData(&this->GndSegment,cross_t[0],cross_t[1],line_in_use,cui_Width);
			break;
		   }
	case -2:{
			 //left�� ��ǿ
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
*ͨ������ֱ��Ѱ����㣨���������ã�
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
	/***********�����ѡ��************************************/
	for(register unsigned int li=0;li<VP_dat0->PrepareLine.size();li++){
		for (register unsigned int lj=0;lj<VP_dat1->PrepareLine.size();lj++){

			if (VP_dat0->PrepareLine[li].a-VP_dat1->PrepareLine[lj].a){
				//�ཻ
				cross_t.x=1.0*(VP_dat0->PrepareLine[li].b-VP_dat1->PrepareLine[lj].b)/(VP_dat1->PrepareLine[lj].a-VP_dat0->PrepareLine[li].a);
				cross_t.y=VP_dat0->PrepareLine[li].b+cross_t.x*VP_dat0->PrepareLine[li].a;
				if (cross_t.y>=0&&cross_t.y<src_img->height){
				/*	point_dbg.push_back(cross_t);*/
					VP_dat0->PreparePoint.push_back(cross_t);
					VP_dat1->PreparePoint.push_back(cross_t);
				}

			}else{
				//����Ϊ0	  //��ֱ��ƽ�� 
			}
		}
	} 
	/*********��λ��ɸѡ��ѡ��***************************************/		
	return this->SelectVPbyMedianIteration(VP_dat0->PreparePoint,1);
}
/*------------------------------------------------------------------------------------------------------------*/
/**
*�����ĴӺ�ѡ�㼯���ҵ���ȷ�����
*
*/
/*------------------------------------------------------------------------------------------------------------*/
void CuiHough3::MatchingVP(void)
{
	vector<CvPoint> point_dbg;
	CvPoint cross_t;
	/***********�����ѡ��************************************/
	for (register int lefti=0;lefti<2;lefti++){
		for(register unsigned int li=0;li<VP_V_L_UP[lefti].PrepareLine.size();li++){
			for (register unsigned int lj=li+1;lj<VP_V_L_UP[lefti].PrepareLine.size();lj++){

				if (VP_V_L_UP[lefti].PrepareLine[li].a-VP_V_L_UP[lefti].PrepareLine[lj].a){
					//�ཻ
					cross_t.x=1.0*(VP_V_L_UP[lefti].PrepareLine[lj].b-VP_V_L_UP[lefti].PrepareLine[li].b)/(VP_V_L_UP[lefti].PrepareLine[li].a-VP_V_L_UP[lefti].PrepareLine[lj].a);
					cross_t.y=VP_V_L_UP[lefti].PrepareLine[li].b+cross_t.x*VP_V_L_UP[lefti].PrepareLine[li].a;
					if (cross_t.y>=0&&cross_t.y<src_img->height){
						point_dbg.push_back(cross_t);
						VP_V_L_UP[lefti].PreparePoint.push_back(cross_t);
					}

				}else{
					//����Ϊ0	  //��ֱ��ƽ�� 
				}
			}
		}
	}
	/***********************************************/
	for (register  int lefti=0;lefti<2;lefti++){
		for(register unsigned int li=0;li<VP_V_R_DOWN[lefti].PrepareLine.size();li++){
			for (register unsigned int lj=li+1;lj<VP_V_R_DOWN[lefti].PrepareLine.size();lj++){

				if (VP_V_R_DOWN[lefti].PrepareLine[li].a-VP_V_R_DOWN[lefti].PrepareLine[lj].a){
					//�㽶
					cross_t.x=1.0*(VP_V_R_DOWN[lefti].PrepareLine[lj].b-VP_V_R_DOWN[lefti].PrepareLine[li].b)/(VP_V_R_DOWN[lefti].PrepareLine[li].a-VP_V_R_DOWN[lefti].PrepareLine[lj].a);
					cross_t.y=VP_V_R_DOWN[lefti].PrepareLine[li].b+cross_t.x*VP_V_R_DOWN[lefti].PrepareLine[li].a;
					if (cross_t.y>=0&&cross_t.y<src_img->height){
						//��������λ�ã�0<y<height��
						point_dbg.push_back(cross_t);
						VP_V_R_DOWN[lefti].PreparePoint.push_back(cross_t);
					}

				}else{
					//����Ϊ0	  //��ֱ��ƽ�� 
				}
			}
		}
	}
	/*********�����ѡ��***************************************/
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
*��ֱ��ͼ��Ѱ�ҷ�ֵ�飨��ɽ�����Σ���ȷ������Ѱ������ֱ����
*
*@param [in] hist_data ֱ��ͼ����
*@param hist_size ֱ��ͼ�ĳ���
*@param [out]group ���ϡ�ɽ�������״�ķ�ֵ����ֱ��ͼ�е�λ�� 
*@param group_num ��ֵ�����Ŀ
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
	/********�������ʵķ�ֵ��ȥ��********************************************/
	for (register int i=0;i<AngLeDivided;i++){
		if ((peak_value[i][1]>0)&&
			(peak_value[i][1]>peak_value[i][0])&&
			(peak_value[i][1]>peak_value[i][2])&&
			((peak_value[i][0]/peak_value[i][1]>=0.25)||(peak_value[i][2]/peak_value[i][1]>0.25))){
				//��һ����Ч�ķ�ֵ��
		}else{
			memset(&peak_value[i],0,sizeof(float)*3);
		}
	}
	/*****Ѱ����ֵ***************************************************************/
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
*��õ���ָ����飨�����ã�
*
*@param [in] hist_data ֱ��ͼ���ݽṹ
*/
/*------------------------------------------------------------------------------------------------------------*/
void CuiHough3::GetGroundSegmentLine(HistData *hist_data)
{
	this->GetHist3effectiveDir(hist_data->hist_ground,VP_GV_L_UP,VP_GV_R_DOWN);//ˮƽ��ѡ����4��ǿ����
	
/**************************************************************************************************/
 	for (register int i=0;i<3;i++){
	  GndSegment.Line2VPdst[i]=src_img->width*src_img->height;
 	}
/**************************************************************************************************/
	for (int li=0;li<cui_line_src_len;li++){
			if ((cui_line_srcimg[li].pointOne.y>GndSegment.VanishingPoint.y)&&
			(cui_line_srcimg[li].pointTwo.y>GndSegment.VanishingPoint.y)){
			//ֱ��������·�
#if 0
	 			if ((cui_line_srcimg[li].alpha_angle<(180/AngLeDivided)*(0))||
					(cui_line_srcimg[li].alpha_angle>(180/AngLeDivided)*(AngLeDivided-1))){
						//ˮƽ���
							double dst_t=DistancePoint2Line(GndSegment.VanishingPoint,cui_line_srcimg[li]);
							if (dst_t<GndSegment.Line2VPdst[0]){
								GndSegment.Line2VPdst[0]=dst_t;
								GndSegment.GndSegLine[0]=cui_line_srcimg[li];
							}

				}
				if ((cui_line_srcimg[li].alpha_angle>(180/AngLeDivided)*(1))&&
					(cui_line_srcimg[li].alpha_angle<(180/AngLeDivided)*(AngLeDivided/2-1))){
					//Left���
						double dst_t=DistancePoint2Line(GndSegment.VanishingPoint,cui_line_srcimg[li]);
						if (dst_t<GndSegment.Line2VPdst[1]){
							GndSegment.Line2VPdst[1]=dst_t;
							GndSegment.GndSegLine[1]=cui_line_srcimg[li];
						}

				}
				if ((cui_line_srcimg[li].alpha_angle>(180/AngLeDivided)*(AngLeDivided/2+1))&&
					(cui_line_srcimg[li].alpha_angle<(180/AngLeDivided)*(AngLeDivided))){
					//Right���
						double dst_t=DistancePoint2Line(GndSegment.VanishingPoint,cui_line_srcimg[li]);
						if (dst_t<GndSegment.Line2VPdst[2]){
							GndSegment.Line2VPdst[2]=dst_t;
							GndSegment.GndSegLine[2]=cui_line_srcimg[li];
						}

				}
#else
				if ((cui_line_srcimg[li].alpha_angle<(180/AngLeDivided)*(1))||
					(cui_line_srcimg[li].alpha_angle>(180/AngLeDivided)*(AngLeDivided-1))){
						//ˮƽ���
						double dst_t=DistancePoint2Line(GndSegment.VanishingPoint,cui_line_srcimg[li]);
						if (dst_t<GndSegment.Line2VPdst[LINE_HORIZONTAL]){
							GndSegment.Line2VPdst[LINE_HORIZONTAL]=dst_t;
							GndSegment.GndSegLine[LINE_HORIZONTAL]=cui_line_srcimg[li];
						}

				}
				if ((cui_line_srcimg[li].alpha_angle>(VP_GV_L_UP[0].Vp_angle_down))&&
					(cui_line_srcimg[li].alpha_angle<(VP_GV_L_UP[0].Vp_angle_up))){
						//Left���
						double dst_t=DistancePoint2Line(GndSegment.VanishingPoint,cui_line_srcimg[li]);
						if (dst_t<GndSegment.Line2VPdst[LINE_UP]){
							GndSegment.Line2VPdst[LINE_UP]=dst_t;
							GndSegment.GndSegLine[LINE_UP]=cui_line_srcimg[li];
						}

				}
				if ( (cui_line_srcimg[li].alpha_angle>(VP_GV_R_DOWN[0].Vp_angle_down))&&
					 (cui_line_srcimg[li].alpha_angle<(VP_GV_R_DOWN[0].Vp_angle_up)) ){
						//Right���
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
*��ȡб���и���������ֱ�ߣ�����ֻ��һ��
*
*@param [in] hist_data ֱ��ͼ���� 
*@param [in] line_data ֱ������
*@param line_len  ֱ�ߵĸ���
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
*��ȡˮƽ�����и�����ֱ��
*
*@param [in] hist_data ֱ��ͼ���� 
*@param [in] line_data ֱ������
*@param line_len  ֱ�ߵĸ���
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
*�ڵ���ָ����ݽṹ����ȡ�������
*
*@return ������
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
*�ڵ���ָ����ݽṹ����ȡ�������
*
*@return �Ҳ����
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
*���0-90�ȷ���ָ�����ֱ��
*
*@param [in] hist_data ֱ��ͼ����
*@param [in] line_data ֱ������ 
*@param line_len  ֱ������
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
	   ;//���ָ���
   }
 
}
/*------------------------------------------------------------------------------------------------------------*/
/**
*
*���90-180�ȷ���ָ�����ֱ��
*
*@param [in] hist_data ֱ��ͼ����
*@param [in] line_data ֱ������ 
*@param line_len  ֱ������
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
	  ;//���ָ���
  }
 
}
/*------------------------------------------------------------------------------------------------------------*/
/**
*
*���0-90,90-180�ȷ���ָ�����ֱ��
*
*@param [in] hist_data ֱ��ͼ����
*@param [in] line_data ֱ������ 
*@param line_len  ֱ������
*@param UporDown  
*- 1 0-90��
*- -1 90-180��
*@param vppoint   ���
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
				//i ,j����
				if(hist_ground[group[aj]]/hist_ground[group[ai]]<=0.8){
					//����ͬʱ��������,q=ȥ����aj
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
	/************���Դ���***************/
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
*��÷ָ�����ˮƽ��
*
*@param [in] hist_data ֱ��ͼ����
*@param [in] line_data ֱ������ 
*@param line_len  ֱ������
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
	//��������ˮƽ��
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
					   /*********ֱ�������֮��*******************/
#if 1
					  if ((line_data[li].pointOne.y>GndSegment.VanishingPoint.y)&&
							(line_data[li].pointTwo.y>GndSegment.VanishingPoint.y))
#endif
						
						 {
									/*****ֱ���ں�ѡ�Ƕ�֮��********/
							 //2013��11��18�շŵ�157��--hough����ȡ����ȷ
#if 1
							 if ((line_data[li].alpha_angle>=(VP_GV_H_H[0].Vp_angle_down))&&
								 (line_data[li].alpha_angle<=(VP_GV_H_H[0].Vp_angle_up)))
#endif	
									
									{

#if 1
											//0.5 sigema ,����
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
		/**************���봹�����**************************************************/
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
		/*****************���***********************************************/
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
*ͨ����ȷ���ķָ��߷����õ���ķָ��ߣ������ã�
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
			 /*********ֱ�������֮��*******************/
		if ((line_data[li].pointOne.y>GndSegment.VanishingPoint.y)&&
			(line_data[li].pointTwo.y>GndSegment.VanishingPoint.y)){
			/*****ֱ���ں�ѡ�Ƕ�֮��********/
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
		VP_LINE->PrepareLine[li].alpha_angle=line_cut.alpha_angle; //������һ������
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
*����㵽ֱ�ߵľ���
*@param point ��
*@param line ��
*/
/*------------------------------------------------------------------------------------------------------------*/
double CuiHough3::DistancePoint2Line(CvPoint point, LINEDATA line)
{
	
	//����㵽ֱ�߾���
	double distance_t=0;	
	distance_t=fabsl(line.a*point.x-point.y+line.b)/sqrtl(powl(line.a,2)+powl(1,2));
	return	distance_t;
}
/*------------------------------------------------------------------------------------------------------------*/
/**
*��ֱ��ͼ����ȡ��Ч�ķ�����
*
*@param hist
*@param  VP_V_L_UPt
*@param  VP_V_R_DOWNt
*@deprecated ������
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
	//ȥ��0/180�Ⱥ�
	hist_vertical_t[0]=0;
	hist_vertical_t[AngLeDivided-1]=0;
	/****90������************/
	hist_vertical_t[AngLeDivided/2-1]=0;
	hist_vertical_t[AngLeDivided/2]=0;
	hist_vertical_t[AngLeDivided/2+1]=0;
	/*****************/	  
	std::sort( hist_vertical_l, hist_vertical_l+AngLeDivided/2,greater<float>()); 
	std::sort( hist_vertical_r, hist_vertical_r+AngLeDivided/2,greater<float>()); 
	/********�ҵ����ֵ********************************/
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
*��ֱ��ͼ����ȡ��Ч�ķ�����
*
*@param hist
*@param  VP_V_L_UPt
*@param  VP_V_R_DOWNt
*@deprecated ������
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
	//ȥ��0/180�Ⱥ�
	hist_vertical_t[0]=0;
	hist_vertical_t[AngLeDivided-1]=0;
	/****90������************/
	hist_vertical_t[AngLeDivided/2-1]=0;
	hist_vertical_t[AngLeDivided/2]=0;
	hist_vertical_t[AngLeDivided/2+1]=0;
	/*****************/	  
	std::sort( hist_vertical_l, hist_vertical_l+AngLeDivided/2,greater<float>()); 
	std::sort( hist_vertical_r, hist_vertical_r+AngLeDivided/2,greater<float>()); 
	/********�ҵ����ֵ********************************/
	VP_V_L_UPt[0].Vp_height=hist_vertical_l[0]; //���
	VP_V_L_UPt[1].Vp_height=hist_vertical_l[1]; //�δ�
	VP_V_R_DOWNt[0].Vp_height=hist_vertical_r[0];
	VP_V_R_DOWNt[1].Vp_height=hist_vertical_r[1];
	/*********Ѱ����ֵ��Ӧ�Ĳ���*****************************************/
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
	/**********��������*****************************************************************/
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
									//��Ч��
								  }else{
									 //��Ч���ڱ�
                    
									  VP_V_L_UPt[1].Vp_angle_up=(ai+1)*180/AngLeDivided;
									  VP_V_L_UPt[1].Vp_angle_down=ai*180/AngLeDivided;
									  VP_V_L_UPt[1].hist_pos=ai;
									  VP_V_L_UPt[1].Effective_cluster=true;
									  //�ҵ�һ����Ч��
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
						 //��Ч��
					 }else{
						 //��Ч���ڱ�
						 VP_V_L_UPt[1].Vp_angle_up=(ai+1)*180/AngLeDivided;
						 VP_V_L_UPt[1].Vp_angle_down=ai*180/AngLeDivided;
						 VP_V_L_UPt[1].hist_pos=ai;
						 VP_V_L_UPt[1].Effective_cluster=true;
						 //�ҵ�һ����Ч��
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
						//��Ч��
					}else{
						//��Ч���ڱ�

						VP_V_R_DOWNt[1].Vp_angle_up=(ai+1)*180/AngLeDivided;
						VP_V_R_DOWNt[1].Vp_angle_down=ai*180/AngLeDivided;
						VP_V_R_DOWNt[1].hist_pos=ai;
						VP_V_R_DOWNt[1].Effective_cluster=true;
						//�ҵ�һ����Ч��
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
						//��Ч��

					}else{
						//��Ч���ڱ�
						VP_V_R_DOWNt[1].Vp_angle_up=(ai+1)*180/AngLeDivided;
						VP_V_R_DOWNt[1].Vp_angle_down=ai*180/AngLeDivided;
						VP_V_R_DOWNt[1].hist_pos=ai;
						VP_V_R_DOWNt[1].Effective_cluster=true;
						//�ҵ�һ����Ч��
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
*��ֱ��ͼ����ȡ��Ч��4�鷽���ߣ���Ӧ�����齻�棬����ȷ������λ��
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
*��ֱ��ͼ����ȡ��Ч��2�鷽���ߣ���Ӧ�����齻�棬����ȷ������λ��
*
*@param[in] hist ֱ��ͼ
*@param[in] VP_V_L_t
*@param  UporDown UP��Ϊ0-90��ֱ�߻� Down��Ϊ90-180��ֱ��
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
		//ɾ��DOWN��
		for (int i=AngLeDivided/2;i<AngLeDivided;i++){
			hist_t[i]=0;
		}
	}else if(UporDown==-1){
		//ɾ��UP��
		for (int i=0;i<AngLeDivided/2;i++){
			hist_t[i]=0;
		}
	}
	/**************************************/
	this->RemoveHistData0and90(hist_t,AngLeDivided);
	//���������ֵ��
	this->GetPeakValueGroup(hist_t,AngLeDivided,group_max,sizeof(group_max)/sizeof(int));
	/*****Ѱ�Ҹ��Է�ֵ���нϴ�ļ�*********************************/ 	
	   for (int i=0;i<2;i++){
			   group_max2[i]=(hist_t[group_max[i]-1]>hist_t[group_max[i]+1])?(group_max[i]-1):(group_max[i]+1);
		}
	/************************************************************/
	  
	   if (fabs(hist_t[group_max[0]-1]-hist_t[group_max[0]+1])/std::max(hist_t[group_max[0]-1],hist_t[group_max[0]+1])<0.2){
			//�������С-ȡ����cross
		   VP_V_L_t[0].Vp_height=hist_t[group_max[0]-1];
		   VP_V_L_t[0].Vp_angle_up=(group_max[0])*180/AngLeDivided;
		   VP_V_L_t[0].Vp_angle_down=(group_max[0]-1)*180/AngLeDivided;
		   VP_V_L_t[0].hist_pos=group_max[0]-1;

		   VP_V_L_t[1].Vp_height=hist_t[group_max[0]+1];
		   VP_V_L_t[1].Vp_angle_up=(group_max2[0]+2)*180/AngLeDivided;
		   VP_V_L_t[1].Vp_angle_down=(group_max2[0]+1)*180/AngLeDivided;
		   VP_V_L_t[1].hist_pos=group_max[0]+1;

	   }else{
		   //������ϴ�--ȡ����һͷһ��
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
*��������λ�ú͸�����ȷ���и����ķ�ʽ
*��Ҫ���������
*- һ��ˮƽ�ߣ�һ��������б��ֱ��
*- һ��ˮƽ�ߣ�һ��������б��ֱ��     
*- һ��ˮƽ�ߣ�����û�У���һ��������б��ֱ�� ��һ��������б��ֱ��           
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
*����Ȼ���������ж�
*������Ƶ�ߵĵ��棬��������
@deprecated ������
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
*�����ʱ������ҵ���ֱ�ߣ��ָ����
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
*˫����ʱ������ҵ���ֱ�ߣ��ָ����
*
*/
/*------------------------------------------------------------------------------------------------------------*/
void CuiHough3::HoughLineClassifyGV2(void)
{
	unsigned int Y_SegPosition;
	///�ָ���һ
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
	//�ָ��߶�
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
	//����---4�ָ�
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
*���ݲ�ͬ�Ļ������ָ���������
*@see �����ָ��::HoughLineClassifyGV2
*@see ˫���ָ��::HoughLineClassifyGV
*/
/*------------------------------------------------------------------------------------------------------------*/
void CuiHough3::ClassifyGV(void)
{
	string file_add;
	if (this->ImageEenvironment==BT_bigbuildingVP){
	

		if(1==abs(GndSegment.VPnumber)){
			//��������
			this->HoughLineClassifyGV();
		}else if(2==abs(GndSegment.VPnumber)){
			this->HoughLineClassifyGV2();
		}

		
	}else if (this->ImageEenvironment==BT_bigbuindingFront){
#if 0
	  //һ���и�
		this->Use1HorizontalLine2CutGV(this->Get1HorizontalLine(this->hist_scrimg));
#endif
		

	}if (this->ImageEenvironment==BT_trees){
		//���������
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
*������ã�����ͼ��X���꣬��÷ָ�ͼ���Y����
*
*@param X_coordinate ͼ���X����
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
*������ã�����ͼ��X���꣬��÷ָ�ͼ���Y����
*
*@param X_coordinate ͼ���X����
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
*ȷ������Ѱ������ֱ�߼���
*
*@param hist_data ֱ��ͼ����
*/
/*------------------------------------------------------------------------------------------------------------*/
void CuiHough3::GetPrepareLine(HistData* hist_data)
{
#if 1
	   this->GetLine4cluster2determineVP(hist_data->hist_vertical,VP_V_L_UP,VP_V_R_DOWN);
#else  
	/*********�õ���������������ǿ�ķ�����************************/
	this->GetHist4EffectiveLinecluster(hist_data->hist_vertical,VP_V_L_UP,VP_V_R_DOWN); //��ֱ��ѡ����
	/*********************************************************************************/

  // this->GetHist3effectiveDir(hist_data->hist_vertical,VP_V_L_UP,VP_V_R_DOWN);
#endif	 
	
	for(register int mi=0;mi<2;mi++){
		VP_V_L_UP[mi].PrepareLine.clear(); 
		VP_V_L_UP[mi].PreparePoint.clear();
		VP_V_R_DOWN[mi].PrepareLine.clear();
		VP_V_R_DOWN[mi].PreparePoint.clear();
	}
	/*********�����ѡֱ��****��ֱ������������Ѱ�����****************************************************/
	int line_num=cui_line_src_len;
	for (register int li=0;li<cui_line_src_len;li++){
		//�������ϵ�����ֱ��
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
	//��ʾ���ڼ����ֱ��
	this->cui_ShowImageLine("Used2findVP",C_Used2FindVP,gray_Color_src_img,cui_line_srcimg,cui_line_src_len);
}
/*------------------------------------------------------------------------------------------------------------*/
/**
*ȷ������Ѱ������ֱ�߼���
*
*@param hist_data ֱ��ͼ����
*@deprecated ������
*/
/*------------------------------------------------------------------------------------------------------------*/
void CuiHough3::Get4ClusterPrepareLine(HistData* hist_data)
{
	/*********�õ���������������ǿ�ķ�����************************/
	this->GetHist3effectiveDir(hist_data->hist_vertical,VP_V_L_UP,VP_V_R_DOWN); //��ֱ��ѡ����
	/*********************************************************************************/
	for(register int mi=0;mi<2;mi++){
		VP_V_L_UP[mi].PrepareLine.clear(); 
		VP_V_L_UP[mi].PreparePoint.clear();
		VP_V_R_DOWN[mi].PrepareLine.clear();
		VP_V_R_DOWN[mi].PreparePoint.clear();
	}
	/*********�����ѡֱ��****��ֱ������������Ѱ�����****************************************************/
	int line_num=cui_line_src_len;
	for (register int li=0;li<cui_line_src_len;li++){
		//�������ϵ�����ֱ��
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
	//��ʾ���ڼ����ֱ��
	this->cui_ShowImageLine("Used2findVP",C_Used2FindVP,gray_Color_src_img,cui_line_srcimg,cui_line_src_len);
}
/*------------------------------------------------------------------------------------------------------------*/
/**
*��ȡ���
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
*������Ѱ�����ĵ��ƻ��Ƶ�ͼ����
*
*@param CloudPointL ������Ӧ�ĵ���
*@param CloudPointR ������Ӧ�ĵ���
*@param CenterPoint ���Ƶ����ĵ�
*@param type  ���ĵ�λ��
*- 1  ���ĵ��������
*- 2  ���ĵ��������
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
*��õ㼯����λ��
*
*@param   point_t �㼯��
*@return �㼯�ϵ����ĵ�
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
*��õ㼯�ϵľ�ֵ��
*
*@param point_t �㼯��
*@return �㼯�ϵľ�ֵ��
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
*ͨ����ֵ������ȡ���
*
*@param  point_t ���ĵ㼯��
*@param  type  ����λ��
*- 1 �����
*- 2 �����
*@return ���
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
*��ȡ�ָ�����ֱ�ߣ����ýؾֱࣨ�߼�ľ��������ࣩ
* 
*@param [in]  PrepareLine   Ҫ�����ֱ�߼���
*@param [out] preparepoint  ֱ�߼��������õĵ㼯��
*@return ��ϵ�ֱ�� 
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
		/***************����//ֱ��************************/
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
		
		/***************���㷶Χ��**********************/
		memset(y_pos_len,0,sizeof(double)*y_pos_num);
		memset(y_pos_len_dbg,0,sizeof(double)*y_pos_num);
		for (int li=0;li<y_pos_num_adj;li++){
			//����ĳ���
			   y_pos_len_dbg[li]=y_pos_len[li]+=PrepareLineAdj[li].LineLength;
			for (int lj=0;lj<y_pos_num_adj;lj++){
				if (li!=lj){
					double dst_t=DistancePoint2Line(PrepareLineAdj.at(lj).pointOne,PrepareLineAdj.at(li));
					if (dst_t<ri){
						//�����ٽ��ĳ���
						y_pos_len_dbg[li]=y_pos_len[li]+=PrepareLineAdj[lj].LineLength;
				    }
				}
				
			}
		}
		
#if 0  
		/********************ͳ�Ʒ�Χ�����ֵ***********/	
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
		/*********�ж���Ч************************/
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
		double line_30_pecsent=y_pos_len[sort_t[y_pos_num_adj/3]];//���ֵ��������
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
*����ֱ�߼��ĳ���,��ֱ�߼������ƵĲ��ֽ���ƴ�ӡ��ӳ���ɾ���Ȳ���
*
*@param [out] PrepareLine  ֱ�߼���
*@param radius ֱ�����ʱֱ�߼�ļ��
*/
/*------------------------------------------------------------------------------------------------------------*/
void CuiHough3::AdjustCutLineLength(
	vector<LINEDATA> *PrepareLine,
	int radius)
{	 
	//const int y_pos_num=PrepareLine->size();	
	double dst_t;
	/***************���㷶Χ��*******************/
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
*��������ֱ�ߵ�λ��\n
*���غ�ֱ�ߺϲ�,����ֱ�ߺϲ�����ֱ���ӳ���
*
*@param PrepareLine ֱ�߼���
*@param li ֱ��i
*@param lj ֱ��j
*@retval  true  ֱ�ߵ�����
*@retval  false ֱ��Ϊ������
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
			 // i��J
			  PrepareLine->erase(PrepareLine->begin()+lj);
			return false;	
		
	}else if((Line_j->pointOne.x<=Line_i->pointOne.x)&&
		     (Line_j->pointTwo.x>=Line_i->pointTwo.x)){
		  //j��I����
			PrepareLine->erase(PrepareLine->begin()+li);
			return false;

	}else if ((Line_i->pointOne.x<Line_j->pointOne.x)&&
			  (Line_i->pointTwo.x>=Line_j->pointOne.x))	{
		 //�ཻ i--j
			  	   PrepareLine->at(lj).pointOne=PrepareLine->at(li).pointOne;
				   PrepareLine->at(lj).LineLength=cui_GeneralImgProcess::P2PointDiatance(PrepareLine->at(lj).pointOne,PrepareLine->at(lj).pointTwo);
				  PrepareLine->erase(PrepareLine->begin()+li);
			  	  return false;	
	
	}else if((Line_j->pointOne.x<Line_i->pointOne.x)&&
		     (Line_j->pointTwo.x>=Line_i->pointOne.x))	{
			//�ཻ j---i
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
*����ֱ�����˵��ŵ�λ��\n
*X�����С�ķ��ұߣ�X�����С�ķ����
*
*@param [in out] ֱ�߼��� 
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
*�߶ε����������ֽ��ߵľ���
*
*@param line �߶�
*@return ƽ������
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
*�߶ε��������ֱ�ߵľ���
*
*@param line �߶�
*@return ƽ������
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
*��ˮƽ������λ������������գ����棬����
*@param[in] cui_ImgLables_SVG ��ա����桢����ı궨ͼ
*@param Width  ͼ����
*@param Height ͼ�񳤶�
*@param horLine ˮƽ�ߵ�ֱ�߷��̣�����û�У�
*@param[in] vpPoint ���λ��
*@param EyeLevel  ��ƽ��λ�ã��������꣩
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
	//����
	//1.˫���λ��--
	//2.��ƽ��
	//3.ˮƽ�ָ���
	//����������Ǹ����ָ���
	//cui_ImgLables_SVG--������Ϊ 321
	float max_vpy=std::max(vpPoint[0].point.y,vpPoint[1].point.y);
	
	for (register unsigned int x=0;x<(unsigned int)Width;x++){
		float  max_line=std::max((double)(horLine.a*x+horLine.b),(double)(EyeLevel));
		unsigned int Y_SegPosition=std::max(max_line,max_vpy);
		for (register  int y=Height-1;y>=0;y--){
			

			if (y<Y_SegPosition){
				//��ƽ��֮�ϵĵ�Ϊ����
				if (cui_ImgLables_SVG[y*Width+x]==Ground){
					cui_ImgLables_SVG[y*Width+x]=Vertical;
				}
			}
#if 0
			if (y>=Y_SegPosition){
				//��Ƶ��֮�µ����Ϊ����
				if (cui_ImgLables_SVG[y*Width+x]==Sky){
					cui_ImgLables_SVG[y*Width+x]=Ground;
				}
			}
#endif


		}
	}
	//����
}
/*------------------------------------------------------------------------------------------------------------*/
/**
*�����ʵ��ķ�������÷ָ�ͼ��ķ���\n
*���磬��Ҫ�����ʱ��������Ͷ�Ӧ�ķָ���
*
*/
/*------------------------------------------------------------------------------------------------------------*/
void CuiHough3::GetClassifyMethod(void){
	if (this->ImageEenvironment==BT_bigbuildingVP){
		//���컷��
		this->GetVPoint();

#if 0
		//ԭͼ�����ϵ���
		//hough.GetGroundSegmentLine(&hough.hist_scrimg);
		hough.Get3GroundSegmentLine(&hough.hist_scrimg,hough.cui_line_srcimg,hough.cui_line_src_len);
#else
		//VG�����ϵ���
		//hough.Get1GroundSegmentLine(&hough.hist_scrimg,hough.cui_line_srcimg,hough.cui_line_src_len);
		//ԭͼ�����ϵ���
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
*�ڵ�������һ�����ڷָ�ͼ���ˮƽ��
*
*@param histdata ֱ��ͼ����
*@return ֱ������  
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
				//����֮�ϣ���ѡ�Ƕ�֮��
				if (linedata[li].LineLength>Line_t.LineLength){
					Line_t=linedata[li];
				}
			}
		}
	}
    //ֱ��δ�ҵ���ηָ�//////////////////////////////////////////////////////////////////////////////////
	return Line_t;
}
/*-----------------------------------------------------------------------------------------------------------*/
/**
*����һ��ˮƽ�����и���棬����
*@param line �и��õ�ˮƽ��
*@deprecated ������
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
				//��ƽ��֮�ϵĵ�Ϊ����
				if (cui_ImgLables_SVG[y*cui_Width+x]==Ground){
					cui_ImgLables_SVG[y*cui_Width+x]=Vertical;
				}
			}
#endif	
#if 1
			if (y>=Y_SegPosition){
				//��Ƶ��֮�µ����Ϊ����
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

