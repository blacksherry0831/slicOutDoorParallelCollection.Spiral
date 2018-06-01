#include "StdAfx.h"
#include "module_all_cui.h"
//#include "HoughAnalyze.h"
//#include "SGVcode/Harr/CuiHarrTransformLet.h"
//#include "SGVcode/cui_GeneralImgProcess.h"
//typedef signed int INT32;
/*-----------------------------------------------------------------------------*/
/**
*��ʼ��
*
*@param  MemData_t ͼ������
*@param ImgMask ������
*  ImgMask==nullptr  ȡ��������
*@param HAParam Hough�任����
*  HAParam==nullptr  ʹ�����β��� 
*
*/
/*-----------------------------------------------------------------------------*/
HoughAnalyze::HoughAnalyze(ImageMemData* MemData_t,IplImage *ImgMask,HoughAnalyzeParam* HAParam)
{
	  this->pMD=MemData_t;
	  storage=cvCreateMemStorage(0);
	  gray_Color_img=nullptr;
	  lines=nullptr;
	  gray_binary_img=nullptr;
	  gray_img=nullptr;  
	  ImgMaskControus=ImgMask;
	  src_img=cvCreateImage(cvSize(pMD->ImgWidth,pMD->ImgHeight),IPL_DEPTH_8U,4);
	  memcpy(src_img->imageData,pMD->Src_ImgData,sizeof(UINT32)*pMD->ImgHeight*pMD->ImgWidth);
	  this->HAParam=(HAParam==NULL)?HoughAnalyzeParam::GetDefaultHAParam():*HAParam;
	  AdjustMaskImgSize(ImgMask);
}
/*-----------------------------------------------------------------------------*/
/**
*��������
*/
/*-----------------------------------------------------------------------------*/
HoughAnalyze::~HoughAnalyze(void)
{
	if (storage!=nullptr){
		  cvReleaseMemStorage(&storage);
	  }
	if (gray_Color_img!=nullptr){
		cvReleaseImage(&gray_Color_img);
	}
	
	if (gray_img!=nullptr){
		cvReleaseImage(&gray_img);
	}
	if (gray_binary_img!=nullptr){
		cvReleaseImage(&gray_binary_img);
	}

	if (src_img!=nullptr){
		cvReleaseImage(&src_img);
	}
	if(ImgMaskControus!=nullptr){
		cvReleaseImage(&ImgMaskControus);
	}
}
/*-----------------------------------------------------------------------------*/
/**
*�������ֵĳߴ�
*/
/*-----------------------------------------------------------------------------*/
void HoughAnalyze::AdjustMaskImgSize(IplImage *ImgMask)
{
	if (ImgMask==nullptr)
	{
		return;
	}
	float scale=1.0*ImgMask->width/pMD->ImgWidth;
	if (FloatIsInt(scale)){
	

	}else{
		
	}

   this->ImgMaskControus=cvCreateImage(cvGetSize(src_img),IPL_DEPTH_8U,ImgMask->nChannels);
   cvResize(ImgMask,this->ImgMaskControus);
#if 1
 cvSaveImage("HarrWaveletTransImg/Maskimg.png",this->ImgMaskControus);
#endif  
}
/*-----------------------------------------------------------------------------------------------------------*/
/**
*ͼ���Ԥ������ԭʼͼ����ж�ֵ������
*
*@param src_img_t ԭʼͼ����ͨ��
*@param  method  
*- 1 ԭͼ����Ϊ�ڰ�ͼ��ֱ�ӿ���
*- 2 ��������С�������ԭͼ�����˲�
*/
/*------------------------------------------------------------------------------------------------------------*/
void HoughAnalyze::ImgPreprocess(
	 IplImage **src_img_t,
	 int method){
if ((*src_img_t)->nChannels==3){
	cvCvtColor(*src_img_t, gray_img, CV_BGR2GRAY);  
}else if((*src_img_t)->nChannels==4){
	cvCvtColor(*src_img_t, gray_img, CV_BGRA2GRAY);  
}else{
	ASSERT(0);
}	 
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


  if (method==ContourImgMethod){
	  /*cvErode(gray_img,gray_img);
	  cvDilate(gray_img,gray_img);*/
	  cvCopyImage(gray_img,gray_binary_img);

  }else if (method==OriginalImgMethod){

#if TRUE
	if (pMD->ImgHeight*pMD->ImgWidth>1000000){
		CuiHarrTransformLet harr_smooth;
		harr_smooth.CuiLoadImgBuffer(pMD->Src_ImgData,pMD->ImgWidth,pMD->ImgHeight);
		IplImage *img_harr_smooth;
		img_harr_smooth=harr_smooth.HarrSmooth(1);
		cvResize(img_harr_smooth,gray_img);	
	}else{
		 cvErode(gray_img,gray_img);
		 cvDilate(gray_img,gray_img);
	}
	  cvCanny(gray_img,gray_binary_img, 40, 200, 3 );	
#endif

  }else if (method==HarrImgMethod){
	  /*cvErode(gray_img,gray_img);
	  cvDilate(gray_img,gray_img);
	  cvCanny(gray_img,gray_binary_img, 40, 200, 3 );	*/
	 // cvThreshold(gray_img,gray_binary_img,255,255,);
	  cvAdaptiveThreshold(gray_img,gray_binary_img,255, cv::ADAPTIVE_THRESH_MEAN_C,CV_THRESH_BINARY_INV,21);
	  //cvAdaptiveThreshold(gray_img,gray_binary_img,255,ADAPTIVE_THRESH_MEAN_C,CV_THRESH_BINARY_INV);
	  /*gray_binary_img.jpg,��ͼ����hough�任*/
  }
  this->ImgPreprocessAdjustByMaskImg();
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
/*-----------------------------------------------------------------------------------------------------------*/
/**
*ʹ�������ִ���Hough�任֮ǰ��ֵ��������
*
*/
/*-----------------------------------------------------------------------------------------------------------*/
void HoughAnalyze::ImgPreprocessAdjustByMaskImg(void)
{ 
	
	if (this->ImgMaskControus==nullptr)
	{
		return ;
	}
	ASSERT(this->gray_binary_img->nChannels==1);
	
	 if (this->ImgMaskControus==nullptr){
		 return;
	 }else{
		ASSERT(this->ImgMaskControus->nChannels==4);
	 }	
	 UINT32 *ColorPixel=(UINT32*)(this->ImgMaskControus->imageData); 
	 ASSERT(sizeof(char)==1);
	 char *GrayBinaryImg=this->gray_binary_img->imageData;
#if TRUE
  cvSaveImage("ImgMaskControus.png",this->ImgMaskControus);
  cvSaveImage("gray_binary_img.jpg",this->gray_binary_img);
#endif	 
	

	 for (int x=0;x<ImgMaskControus->width;x++){
		 for (int y=0;y<ImgMaskControus->height;y++)
		 {
			 //UINT32 RGB_NoA=ColorPixel[x+y*ImgMaskControus->width]&&0x00ffffff;
			 UINT32 RGBA=ColorPixel[x+y*ImgMaskControus->width];
			 if (RGBA!=BlackColorPNG){
				 //��Ҫ��ȥ�ĵ�
				GrayBinaryImg[x+y*ImgMaskControus->width]=0;
			 }
		 }
	 }
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
void HoughAnalyze::AdjustLine(
	 LINEDATA *cui_line_data,
	 int cui_line_len)
{
	 CvPoint point_t[4];
	 for (register int li=0;li<cui_line_len;li++){

		 point_t[0].x=cui_line_data[li].AdjPoint1.x=0;
		 point_t[0].y=cui_line_data[li].AdjPoint1.y=cui_line_data[li].a*point_t[0].x+cui_line_data[li].b;
		 point_t[1].x=cui_line_data[li].AdjPoint2.x=pMD->ImgWidth;
		 point_t[1].y=cui_line_data[li].AdjPoint2.y=cui_line_data[li].a*point_t[1].x+cui_line_data[li].b;
	 }	 

}
/*-----------------------------------------------------------------------------------------------------------*/
/**
*���ݽǶȴ�HSV��ɫ�ռ��ȡ��ɫ
*
*@param angle_color HSV��ɫ�ռ���H�ı仯��Χ0-180
*@return ��Ƕȶ�Ӧ����ɫ
*/
/*-----------------------------------------------------------------------------------------------------------*/
 CvScalar   HoughAnalyze::GetGradientColor(double angle_color){
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
 /*-----------------------------------------------------------------------------------------------------------*/
 /**
 *��ȡͼ�������е�ֱ��
 *
 *@param src_img_t[in]		ԭͼ�ڴ�����
 *@param gray_Color_img_t [out] �õ��Ҷ�ͼ��
 *@param plinedata [out] �����ֱ������
 *@param pline_len [out] ֱ������
 *@param int method [in] ͼ��ƽ������
 */
 /*-----------------------------------------------------------------------------------------------------------*/
 void HoughAnalyze::GetAllhoughLineData(
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
#if TRUE
		 if (method==ContourImgMethod){
					 //����---����
					 cvSaveImage("gray_binary_img.jpg",gray_binary_img);
					 IplImage* color_img_t=cvLoadImage("gray_binary_img.jpg");
					 cvConvertImage(color_img_t,gray_binary_img,CV_BGR2GRAY);
					 cvReleaseImage(&color_img_t);
					 //�����и���BUG
					 lines = cvHoughLines2(gray_binary_img, storage, CV_HOUGH_PROBABILISTIC, 1, CV_PI/180,
						 20,20,2);
		 }else if (method==OriginalImgMethod){  
					 //ԭͼ
					 lines = cvHoughLines2(gray_binary_img, storage, CV_HOUGH_PROBABILISTIC, 1, CV_PI/180,
						 35,20,4);
		}else if (method==HarrImgMethod){
					 lines = cvHoughLines2(gray_binary_img, storage, CV_HOUGH_PROBABILISTIC, 1, CV_PI/180,
						 HAParam.Threshold,HAParam.LineLengthMin,HAParam.GapMax);
		}else{
					 ASSERT(0);
		}

				 printf("%d\n",lines->total);
#endif
		
		 /************************************************/	
		 if (*plinedata){
			 delete [](*plinedata);
		 }
		 *pline_len=lines->total;
		 *plinedata=new LINEDATA[*pline_len];
	//	 ASSERT(FALSE);
		 int size_linedata=sizeof(LINEDATA);
		 
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
		 //memcpy(cui_line_cpy,*plinedata,min(MAXLINEDATA,lines->total)*sizeof(LINEDATA));
		 this->AdjustLine(*plinedata,*pline_len);


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
 void HoughAnalyze::GetHistogram180SVG(
	 string filename,
	 LINEDATA* linedata,
	 int linelen,
	 HistData180 *hist_data){
     
	 int HistDimension=180;
	 CvHistogram *hist[HistNUM]={NULL};
	 IplImage  *hist_matrix=NULL;
	 int	hist_size[1]={HistDimension};
	 float  angle_ranges[]={0,HistDimension+1};
	 float *ranges[]={angle_ranges};
	 /*********************************/
	 memset(hist_data,0,sizeof(HistData180));
	
	  for (int i=0;i<HistNUM;i++){
		 hist[i]=cvCreateHist(1,hist_size,CV_HIST_ARRAY,ranges);
	  }
	  /****ͳ��ֱ��ͼ*****************************/
#if TRUE
 HoughAnalyze::StatisticsHistogram180ByLine(filename,linedata,linelen,hist_data);
#endif
	 /****************************************************************/
	 for (int ci=0;ci<HistNUM;ci++){
		 for (register int i=0;i<HistDimension;i++){
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
	
/**************************************************************************************************************/
/** ����ֱ��ͼ��ʾͼ�� */
	 int height = 200;
	 char  text_buff_t[1024];
	 int bin_w=1800/HistDimension;
	 int h_bins=HistDimension;
	 int width =h_bins*bin_w;
	 IplImage* hist_img = cvCreateImage( cvSize(width,height*HistNUM), 8, 4 ); 
	 cvZero( hist_img );
	   for (int ci=0;ci<HistNUM;ci++){
#if TRUE
						cvNormalizeHist(hist[ci],1.0);
						if (hist_data->hist_all_sum!=0){
						if (ci==0){
						    memcpy(hist_data->hist_sky,cvGetHistValue_1D(hist[ci],0),sizeof(float)*HistDimension);
						}else if (ci==1){
						    memcpy(hist_data->hist_ground,cvGetHistValue_1D(hist[ci],0),sizeof(float)*HistDimension);
						}else if (ci==2){
							memcpy(hist_data->hist_vertical,cvGetHistValue_1D(hist[ci],0),sizeof(float)*HistDimension);
						}else if (ci==3){
							memcpy(hist_data->hist_all,cvGetHistValue_1D(hist[ci],0),sizeof(float)*HistDimension);							
						}else if (ci==4){
							memcpy(hist_data->hist_Multiply,cvGetHistValue_1D(hist[ci],0),sizeof(float)*HistDimension);
						}
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

							CvScalar color=GetGradientColor(1.0*h/HistDimension*180);
							
							CvFont font;
							cvInitFont(&font,CV_FONT_VECTOR0,0.3,0.3,0,1,8);
							//��ͼ������ʾ�ı��ַ���
							sprintf(text_buff_t,"%0.2f",bin_val*100/max_value);
							cvPutText(hist_img,text_buff_t,cvPoint(h*bin_w,height - intensity-10),&font,CV_RGB(255,255,255));
							
							cvRectangle( hist_img, cvPoint(h*bin_w,height),cvPoint((h+1)*bin_w,height - intensity),color,-1, 8, 0 );
							sprintf(text_buff_t,"%d",h*180/HistDimension);
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
/*************************************************************************************************************/
	   filename+="SGVAM.jpg";
	   cvSaveImage(filename.c_str(),hist_img);
	   cvReleaseImage(&hist_img); 
     for (int i=0;i<HistNUM;i++){
		   cvReleaseHist(&hist[i]);
	 }
 }
/*-----------------------------------------------------------------------------------------------------------*/
/**
*���ÿ�������ؿ��Manhattan 
*
*/
/*-----------------------------------------------------------------------------------------------------------*/
void HoughAnalyze::GetHist180All(void)
 {
	 //�ض������
#if TRUE
	 ASSERT(this->ImgMaskControus==nullptr);
	 int LineLength=pMD->GetSuperPixelDefaultEdgeLength();
	 //Ԥ��33
	 HoughAnalyzeParam hap(20,2,20,ContourImgMethod);
	 this->HAParam=hap;
#endif
	 cui_GeneralImgProcess::CalculateAllSpPropertyRange(
		 pMD->ImgLables.get(),
		 pMD->ImgWidth,
		 pMD->ImgHeight,
		 pMD->p_SpProperty,
		 pMD->slic_current_num);
	 int  HistDimension=180;
	 HistData180 *hist=pMD->Histogram180.get();

	 for (int i=0; i<pMD->slic_current_num;i++){
		 hist[i]=GetOneSPLineHistogram2(i);
	 }
	 
 }
/*-----------------------------------------------------------------------------------------------------------*/
/**
*����ֱ��ͼ��ƽ��ֵ
*
*@return ֱ��ͼ
*
*/
/*-----------------------------------------------------------------------------------------------------------*/
HistData180 HoughAnalyze::GetHist180VerticalAll(void)
 {
	 this->GetHist180All();
	 int HistDimension=180;
	 HistData180	HistVall;
	 memset(&HistVall,0,sizeof(HistData180));
	  HistData180 *hist=pMD->Histogram180.get();
	 /*************************************************/
#if InDoor
	 INT32* Matrix_Category_Lable=pMD->Matrix_Category_Lable_InDoor.get();
#endif
#if OutDoor
	 INT32* Matrix_Category_Lable=pMD->Matrix_Category_Lable.get();
#endif

	 for (int spi=0;spi<pMD->slic_current_num;spi++){
		 if (Matrix_Category_Lable[spi]==Vertical){
			 //ͼ��Ϊ����
			 for(int ai=0;ai<HistDimension;ai++){

				 HistVall.hist_all[ai]+=hist[spi].hist_all[ai];

			 }
		 }

	 }
	 /**************************************************/
	 /**************************************************/
	 double sum=0;
	 for(int ai=0;ai<HistDimension;ai++){

		 sum+=HistVall.hist_all[ai];

	 }
	 for(int ai=0;ai<HistDimension;ai++){

		 HistVall.hist_all[ai]/=sum;

	 }
	 /**************************************************/
	 return HistVall;
 }
/*----------------------------------------------------------------------------------*/
/**
*���һ�������ؿ��������ֱ��ͼ
*@param �����ؿ�������
*@return ������ֱ��ͼ
*@note 2014-04-03
*/
/*----------------------------------------------------------------------------------*/
HistData180 HoughAnalyze::GetOneSPLineHistogram2(int sp)
{ 
	HistData180 hist_data;

	SP_PROPERTY *p_SpProperty=pMD->p_SpProperty;
	int img_width=p_SpProperty[sp].max_x-p_SpProperty[sp].min_x;
	int img_height=p_SpProperty[sp].max_y-p_SpProperty[sp].min_y;
	std::auto_ptr<UINT32> ImgDataSP=std::auto_ptr<UINT32>(new UINT32[img_width*img_height]);
	int *Labes=pMD->ImgLables.get();
	UINT32 *LabesSP=ImgDataSP.get();


	/*���������ؿ�*/
#if 0
#else
	for(register int x=p_SpProperty[sp].min_x;x<p_SpProperty[sp].max_x;x++){
		for (register int y=p_SpProperty[sp].min_y;y<p_SpProperty[sp].max_y;y++){
			/////////////////////////////////
			ASSERT(x>=0&&x<pMD->ImgWidth);
			ASSERT(y>=0&&y<pMD->ImgHeight);
			/////////////////////////////////
			int x_t=x-p_SpProperty[sp].min_x;
			int y_t=y-p_SpProperty[sp].min_y;
			int index_org=x+y*pMD->ImgWidth;
			/////////////////////////////////
			if (Labes[index_org]==sp){
				LabesSP[y_t*img_width+x_t]=WhiteColorPNG;				 
			}else{				 
				LabesSP[y_t*img_width+x_t]=BlackColorPNG;
			}

		}
	}
#endif

	/*�Գ����ؿ�hough�任*/
	this->GetAllHoughLineByContour_SP(LabesSP,sp,img_width,img_height,p_SpProperty[sp].PosBorder);
	this->GetHistogram180SVG("Hist180",pMD->LineContour,pMD->LineContourLen,&hist_data);
	return hist_data;
}
/*----------------------------------------------------------------------------------*/
/**
*���㳬����ͼ���ֱ��ͼ
*@param ImgData_ContoursSP һ�������ص�����ͼ 
*@param sp �����ر�� 
*@param WidthSP �����ؿ�� 
*@param HeightSP �����ظ߶� 
*@param PosBorder ȥ���߽� 
*   -Ҫȥ���ı߽�
*   -# SpPosBorder_LEFT �����ؿ鿿ͼ����߽�
*   -#SpPosBorder_RIGHT �����ؿ鿿ͼ���ұ߽�
*   -#SpPosBorder_TOP �����ؿ鿿ͼ���ϱ߽�
*   -#SpPosBorder_BOTTOM �����ؿ鿿ͼ���±߽�
*/
/*----------------------------------------------------------------------------------*/
void  HoughAnalyze::GetAllHoughLineByContour_SP(
	const  UINT32* ImgData_ContoursSP,
	const  int sp,
	const  int WidthSP,
	const  int HeightSP,
	const UINT32 PosBorder)
{
	IplImage *imgContourBlock;
	IplImage *imgContourEdge;
	int cui_Width=WidthSP;
	int cui_Height=HeightSP;
	imgContourBlock=cvCreateImage(cvSize(cui_Width,cui_Height),IPL_DEPTH_8U,4);
	memcpy(imgContourBlock->imageData,ImgData_ContoursSP,imgContourBlock->imageSize);
	imgContourEdge=cvCreateImage(cvGetSize(imgContourBlock),IPL_DEPTH_8U,4);
	cvZero(imgContourEdge);
	/*
	*���������ҪԤ����
	*
	*/
	cui_GeneralImgProcess::GetContour2Draw(
		imgContourBlock,
		imgContourEdge,
		cvScalar(0xff,0xff,0xff,0xff));//0xff��͸�� 
#if TRUE
	if ((PosBorder&SpPosBorder_LEFT)!=0){		
		cvLine(imgContourEdge,cvPoint(0,0),cvPoint(0,HeightSP-1),cvScalar(0x00,0x00,0x00,0xff),4);//x==0;
	}
	if ((PosBorder&SpPosBorder_RIGHT)!=0){		
		cvLine(imgContourEdge,cvPoint(WidthSP-1,0),cvPoint(WidthSP-1,HeightSP-1),cvScalar(0x00,0x00,0x00,0xff),4);//x==width-1;
	}
	if ((PosBorder&SpPosBorder_TOP)!=0){		
		cvLine(imgContourEdge,cvPoint(0,0),cvPoint(WidthSP-1,0),cvScalar(0x00,0x00,0x00,0xff),4);//y==0;
	}
	if ((PosBorder&SpPosBorder_BOTTOM)!=0){		
		cvLine(imgContourEdge,cvPoint(0,HeightSP-1),cvPoint(WidthSP-1,HeightSP-1),cvScalar(0x00,0x00,0x00,0xff),4);//y==height-1;
	}
#endif
	
	cvSaveImage("contourSPedge.jpg",imgContourEdge);
	
	cvSaveImage("contourBlockSP.jpg",imgContourBlock);
	this->GetAllhoughLineData(
		&imgContourEdge,
		&gray_Color_img,
		&pMD->LineContour,
		&pMD->LineContourLen,
		 ContourImgMethod);

#if 1
	cui_GeneralImgProcess::DrawLine2Img(
		pMD->LineContour,
		pMD->LineContourLen,
		imgContourEdge,
		"InDoorSp");
#endif
	
	cvSaveImage("contourSP.jpg",imgContourEdge);
	
	cvReleaseImage(&imgContourEdge);
	cvReleaseImage(&imgContourBlock);
}
/*----------------------------------------------------------------------------------*/
/**
*������г����ؿ�������ֱ��ͼ
*@note
* ������������ֱ��ͼ
*/
/*----------------------------------------------------------------------------------*/
void HoughAnalyze::GetAllHoughLineBySourceImg(void)
{
	this->GetAllhoughLineData(
		&src_img,
		&gray_Color_img,
		&pMD->LineSource,
		&pMD->LineSourceLen,
		2);

#if 1
	cui_GeneralImgProcess::DrawLine2Img(
		pMD->LineSource,
		pMD->LineSourceLen,
		src_img,
		"InDoorAll");
#endif

	
#if 0
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
#endif
	
}
/*----------------------------------------------------------------------------------*/
/**
*�������г����ؿ������ֱ��ͼ
*
*/
/*----------------------------------------------------------------------------------*/
void HoughAnalyze::GetAllHoughLineByHarrImg(void)
{
	this->GetAllhoughLineData(
		&src_img,
		&gray_Color_img,
		&pMD->LineSource,
		&pMD->LineSourceLen,
		HAParam.peProcessMethod);
#if 1
	cui_GeneralImgProcess::DrawLine2Img(
		pMD->LineSource,
		pMD->LineSourceLen,
		src_img,
		"Harr");
#endif
}
/*----------------------------------------------------------------------------------*/
/**
*ͨ��ԭͼ���������ֱ��ͼ
*/
/*----------------------------------------------------------------------------------*/
HistData180 HoughAnalyze::GetALlImgHistogram_180(void)
{
	HistData180 hist_data;
	this->GetAllHoughLineBySourceImg();
	this->GetHistogram180SVG("Hist180",pMD->LineSource,pMD->LineSourceLen,&hist_data);
	return hist_data;
}
/*----------------------------------------------------------------------------------*/
/**
*
*@noteȫ����һ����
*/
/*----------------------------------------------------------------------------------*/
HistData180 HoughAnalyze::GetSaveALlImgHistogram_180(void)
{
	HistData180 hist_data;
	this->GetAllHoughLineByHarrImg();
	string time=cui_GeneralImgProcess::cuiGetCurrentTime();
	string filename="SrcIMG_B_180"+time;
	this->GetHistogram180SVG(filename,pMD->LineSource,pMD->LineSourceLen,&hist_data);
	return hist_data;
}
/*----------------------------------------------------------------------------------*/
/**
*��ֱ��ͼ�����ݱ��������������ֵΪ100����ֱ��ͼ����
*@param histdata ֱ��ͼ���� 
*@param Dim ֱ��ͼά�� 
*/
/*----------------------------------------------------------------------------------*/
void HoughAnalyze::AdjustHistFrom0to100(HistData180 *histdata,int Dim)
{
#if TRUE
	{
		float *histall=histdata->hist_all;
		float max_value=cui_GeneralImgProcess::GetMaxValue(histall,Dim);
		for (int i=0;i<Dim;i++){
			if (max_value!=0){
				histall[i]=histall[i]/max_value*100;
			}else{
				histall[i]=0;
			}
			
		}
	}

#endif
#if TRUE
	{
		float *histallcount=histdata->hist_all_count;
		float max_value=cui_GeneralImgProcess::GetMaxValue(histallcount,Dim);
		for (int i=0;i<Dim;i++){
			if (max_value!=0){
				histallcount[i]=histallcount[i]/max_value*100;
			}else{
				histallcount[i]=0;
			}
			
		}
	}	
#endif
	
}
/*----------------------------------------------------------------------------------*/
/**
*���ȫͼ��ֱ��ͼ���ݡ�������ֵת����100Ϊ������ֱ��ͼ
*@return ����ֱ��ͼ����
*/
/*----------------------------------------------------------------------------------*/
 HistData180 HoughAnalyze::GetSaveALlImgHistogram_180_0to100(void)
 {
	 HistData180 hist180;
	 hist180=this->GetSaveALlImgHistogram_180();
	 this->AdjustHistFrom0to100(&hist180,180);
	 return hist180;
 }
/*----------------------------------------------------------------------------------*/
/**
*ͳ��ͼƬ��������ֱ��ͼ������
*@param  filename 
*@param [out] linedata 
*@param  linelen 
*@param [out] hist_data 
*@Dim [in] ֱ��ͼά��
*/
/*----------------------------------------------------------------------------------*/
void HoughAnalyze::StatisticsHistogramByLine(
	 string filename,
	 LINEDATA* linedata,
	 int linelen,
	 HistData *hist_data,
	 int Dim){
  
	 memset(hist_data,0,sizeof(HistData));
	
	  /****ͳ��ֱ��ͼ*****************************/
	for (register int i=0;i<linelen;i++){
		 int angle_index=cvFloor(linedata[i].alpha_angle*Dim/180.0);	

		 if(linedata[i].Category==Ground){
		    hist_data->hist_ground[angle_index]+=linedata[i].LineLength;
		 }else if (linedata[i].Category==Sky){
			hist_data->hist_sky[angle_index]+=linedata[i].LineLength;
		 }else if (linedata[i].Category==Vertical){
		   	hist_data->hist_vertical[angle_index]+=linedata[i].LineLength;
		 } 
		 if (linedata[i].Category!=Remove){
		    hist_data->hist_all[angle_index]+=linedata[i].LineLength;
			hist_data->hist_all_count[angle_index]++;
		 }
		
	 } 
	  /***************************************************************/
	 for (register int ai=0;ai<Dim;ai++){
		hist_data->hist_Multiply[ai]=hist_data->hist_ground[ai]*hist_data->hist_vertical[ai];
	 }
	 /****************************************************************/
#if TRUE
	 //int a=__LINE__;
	 string xml_save_path=filename+"all.xml";
	 CvFileStorage *fsW=cvOpenFileStorage(xml_save_path.c_str(),0,CV_STORAGE_WRITE);
	
	  //cvStartWriteStruct(fsW,"LineHist",CV_NODE_MAP);

	 for (int i=0;i<Dim;i++){
		 cvStartWriteStruct(fsW,"AngleInterval",CV_NODE_MAP);
		 cvWriteInt(fsW,"From",i*(180/Dim));
		 cvWriteInt(fsW,"To",(i+1)*(180/Dim));
		 cvWriteInt(fsW,"count", hist_data->hist_all_count[i]);
		 cvWriteInt(fsW,"length",hist_data->hist_all[i]);
		 cvEndWriteStruct(fsW);
	 }
	  //cvEndWriteStruct(fsW);
	  CvFileNode* RootNode=cvGetRootFileNode(fsW);
	 cvReleaseFileStorage(&fsW);
#endif
}
/*----------------------------------------------------------------------------------*/
/**
*ͳ��ͼƬ��������ֱ��ͼ������
*@param  filename 
*@param [out] linedata 
*@param  linelen 
*@param [out] hist_data 
*@note ֱ��ͼ��180��Ԫ��
*/
/*----------------------------------------------------------------------------------*/
void  HoughAnalyze::StatisticsHistogram180ByLine(
	string filename,
	LINEDATA* linedata,
	int linelen,
	HistData180 *hist_data)
{
	int Dim=180;
	memset(hist_data,0,sizeof(HistData180));
	/****ͳ��ֱ��ͼ*****************************/
	for (register int i=0;i<linelen;i++){
		int angle_index=cvFloor(linedata[i].alpha_angle*Dim/180.0);	

		if(linedata[i].Category==Ground){
			hist_data->hist_ground[angle_index]+=linedata[i].LineLength;
		}else if (linedata[i].Category==Sky){
			hist_data->hist_sky[angle_index]+=linedata[i].LineLength;
		}else if (linedata[i].Category==Vertical){
			hist_data->hist_vertical[angle_index]+=linedata[i].LineLength;
		} 
		if (linedata[i].Category!=Remove){
			hist_data->hist_all[angle_index]+=linedata[i].LineLength;
			hist_data->hist_all_count[angle_index]++;
		}

	} 
	/***************************************************************/
	for (register int ai=0;ai<Dim;ai++){
		hist_data->hist_Multiply[ai]=hist_data->hist_ground[ai]*hist_data->hist_vertical[ai];
	}
	/****************************************************************/
	hist_data->hist_all_sum=cui_GeneralImgProcess::GetSumValue(hist_data->hist_all,Dim);
	/****************************************************************/
#if TRUE
	float* Counthist=hist_data->hist_all_count;
	hist_data->hist_all_count_sum=cui_GeneralImgProcess::GetSumValue(Counthist,180);
	for (int i=0;i<180;i++){
		if (hist_data->hist_all_count_sum!=0){
			hist_data->hist_all_count[i]/=hist_data->hist_all_count_sum;
		}else{
			hist_data->hist_all_count[i]=0;
		}

	}
#endif
	/****************************************************************/
#if TRUE
	//int a=__LINE__;
	string xml_save_path=filename+"all.xml";
	CvFileStorage *fsW=cvOpenFileStorage(xml_save_path.c_str(),0,CV_STORAGE_WRITE);

	//cvStartWriteStruct(fsW,"LineHist",CV_NODE_MAP);

	for (int i=0;i<Dim;i++){
		cvStartWriteStruct(fsW,"AngleInterval",CV_NODE_MAP);
		cvWriteInt(fsW,"From",i*(180/Dim));
		cvWriteInt(fsW,"To",(i+1)*(180/Dim));
		cvWriteInt(fsW,"count", hist_data->hist_all_count[i]);
		cvWriteInt(fsW,"length",hist_data->hist_all[i]);
		cvEndWriteStruct(fsW);
	}
	//cvEndWriteStruct(fsW);
	CvFileNode* RootNode=cvGetRootFileNode(fsW);
	cvReleaseFileStorage(&fsW);
#endif
	/****************************************************************/
}
/*----------------------------------------------------------------------------------*/
/**
*�жϸ������Ƿ�������
*/
/*----------------------------------------------------------------------------------*/
bool HoughAnalyze::DoubleIsInt(double n)
{
#if _MSC_VER
	__int64 i, a=*(__int64*)&n, e=(a>>52&2047)-1023, t=a&0xFFFFFFFFFFFFFll;
#endif
#if __GNUC__
	int64_t i, a=*(int64_t*)&n, e=(a>>52&2047)-1023, t=a&0xFFFFFFFFFFFFFll;
	ASSERT(sizeof(int64_t)==8);
#endif

	ASSERT(sizeof(double)==8);
	if (n>=-1e-16F && n<=1e-16F) return true; //1e-16���ж��Ƿ�Ϊ0ʱ��������
	for (i=0xFFFFFFFFFFFFFll; i; i>>=1)
		if (i>>e&t) return false;
	return true;
}
/*----------------------------------------------------------------------------------*/
/**
*�жϸ������Ƿ�������
*/
/*----------------------------------------------------------------------------------*/
bool HoughAnalyze::FloatIsInt(float n)
{
	ASSERT(sizeof(float)==4);
	int i, a=*(int*)&n, e=(a>>23&255)-127, t=a&8388607;
	if (n>=-1e-6F && n<=1e-6F) return true; //1e-6���ж��Ƿ�Ϊ0ʱ��������
	if (e<0) return false;
	if (0x7FFFFF>>e&t) return false;
	return true;
}
/*----------------------------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------------------------*/