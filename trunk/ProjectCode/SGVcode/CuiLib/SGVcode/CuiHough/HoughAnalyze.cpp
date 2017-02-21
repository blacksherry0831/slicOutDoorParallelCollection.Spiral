#include "StdAfx.h"
#include "module_all_cui.h"
//#include "HoughAnalyze.h"
//#include "SGVcode/Harr/CuiHarrTransformLet.h"
//#include "SGVcode/cui_GeneralImgProcess.h"
//typedef signed int INT32;
/*-----------------------------------------------------------------------------*/
/**
*初始化
*
*@param  MemData_t 图像数据
*@param ImgMask 屏蔽罩
*  ImgMask==nullptr  取消屏蔽罩
*@param HAParam Hough变换参数
*  HAParam==nullptr  使用屏蔽参数 
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
*析构参数
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
*将屏蔽罩的尺寸
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
*图像的预处理，对原始图像进行二值化处理
*
*@param src_img_t 原始图像三通道
*@param  method  
*- 1 原图本身为黑白图像，直接拷贝
*- 2 对其运用小波降噪对原图进行滤波
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
	 //中值滤波 
	 cvSmooth(gray_img,gray_img,CV_MEDIAN,3,3,0,0);
	   //邻域平均滤波
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
	  cvAdaptiveThreshold(gray_img,gray_binary_img,255,ADAPTIVE_THRESH_MEAN_C,CV_THRESH_BINARY_INV,21);
	  //cvAdaptiveThreshold(gray_img,gray_binary_img,255,ADAPTIVE_THRESH_MEAN_C,CV_THRESH_BINARY_INV);
	  /*gray_binary_img.jpg,此图用作hough变换*/
  }
  this->ImgPreprocessAdjustByMaskImg();
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
/*-----------------------------------------------------------------------------------------------------------*/
/**
*使用屏蔽罩处理Hough变换之前二值化的数据
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
				 //需要除去的点
				GrayBinaryImg[x+y*ImgMaskControus->width]=0;
			 }
		 }
	 }
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
*根据角度从HSV颜色空间获取颜色
*
*@param angle_color HSV颜色空间中H的变化范围0-180
*@return 与角度对应的颜色
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
 *获取图像上所有的直线
 *
 *@param src_img_t[in]		原图内存数据
 *@param gray_Color_img_t [out] 得到灰度图像
 *@param plinedata [out] 输出的直线数据
 *@param pline_len [out] 直线数据
 *@param int method [in] 图像平滑方法
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
#if TRUE
		 if (method==ContourImgMethod){
					 //轮廓---另算
					 cvSaveImage("gray_binary_img.jpg",gray_binary_img);
					 IplImage* color_img_t=cvLoadImage("gray_binary_img.jpg");
					 cvConvertImage(color_img_t,gray_binary_img,CV_BGR2GRAY);
					 cvReleaseImage(&color_img_t);
					 //其中有个大BUG
					 lines = cvHoughLines2(gray_binary_img, storage, CV_HOUGH_PROBABILISTIC, 1, CV_PI/180,
						 20,20,2);
		 }else if (method==OriginalImgMethod){  
					 //原图
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
		 //memcpy(cui_line_cpy,*plinedata,min(MAXLINEDATA,lines->total)*sizeof(LINEDATA));
		 this->AdjustLine(*plinedata,*pline_len);


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
	  /****统计直方图*****************************/
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
/** 设置直方图显示图像 */
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

							CvScalar color=GetGradientColor(1.0*h/HistDimension*180);
							
							CvFont font;
							cvInitFont(&font,CV_FONT_VECTOR0,0.3,0.3,0,1,8);
							//在图像中显示文本字符串
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
*获得每个超像素块的Manhattan 
*
*/
/*-----------------------------------------------------------------------------------------------------------*/
void HoughAnalyze::GetHist180All(void)
 {
	 //重定义参数
#if TRUE
	 ASSERT(this->ImgMaskControus==nullptr);
	 int LineLength=pMD->GetSuperPixelDefaultEdgeLength();
	 //预测33
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
*立面直方图的平均值
*
*@return 直方图
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
			 //图块为立面
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
*获得一个超像素块的轮廓的直方图
*@param 超像素块索引号
*@return 曼哈顿直方图
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


	/*拷贝超像素块*/
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

	/*对超像素块hough变换*/
	this->GetAllHoughLineByContour_SP(LabesSP,sp,img_width,img_height,p_SpProperty[sp].PosBorder);
	this->GetHistogram180SVG("Hist180",pMD->LineContour,pMD->LineContourLen,&hist_data);
	return hist_data;
}
/*----------------------------------------------------------------------------------*/
/**
*计算超像素图块的直方图
*@param ImgData_ContoursSP 一个超像素的轮廓图 
*@param sp 超像素标号 
*@param WidthSP 超像素宽度 
*@param HeightSP 超像素高度 
*@param PosBorder 去除边界 
*   -要去除的边界
*   -# SpPosBorder_LEFT 超像素块靠图像左边界
*   -#SpPosBorder_RIGHT 超像素块靠图像右边界
*   -#SpPosBorder_TOP 超像素块靠图像上边界
*   -#SpPosBorder_BOTTOM 超像素块靠图像下边界
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
	*这个轮廓需要预处理
	*
	*/
	cui_GeneralImgProcess::GetContour2Draw(
		imgContourBlock,
		imgContourEdge,
		cvScalar(0xff,0xff,0xff,0xff));//0xff不透明 
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
*获得所有超像素块轮廓的直方图
*@note
* 轮廓的曼哈顿直方图
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
#endif
	
}
/*----------------------------------------------------------------------------------*/
/**
*计算所有超像素块的轮廓直方图
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
*通过原图获得曼哈顿直方图
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
*@note全部归一化的
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
*将直方图的数据比例调整到以最大值为100的新直方图数据
*@param histdata 直方图数据 
*@param Dim 直方图维度 
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
*获得全图的直方图数据、并将峰值转换到100为比例的直方图
*@return 返回直方图数据
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
*统计图片中曼哈顿直方图的数据
*@param  filename 
*@param [out] linedata 
*@param  linelen 
*@param [out] hist_data 
*@Dim [in] 直方图维度
*/
/*----------------------------------------------------------------------------------*/
void HoughAnalyze::StatisticsHistogramByLine(
	 string filename,
	 LINEDATA* linedata,
	 int linelen,
	 HistData *hist_data,
	 int Dim){
  
	 memset(hist_data,0,sizeof(HistData));
	
	  /****统计直方图*****************************/
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
*统计图片中曼哈顿直方图的数据
*@param  filename 
*@param [out] linedata 
*@param  linelen 
*@param [out] hist_data 
*@note 直方图有180个元素
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
	/****统计直方图*****************************/
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
*判断浮点数是否是整数
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
	if (n>=-1e-16F && n<=1e-16F) return true; //1e-16：判断是否为0时允许的误差
	for (i=0xFFFFFFFFFFFFFll; i; i>>=1)
		if (i>>e&t) return false;
	return true;
}
/*----------------------------------------------------------------------------------*/
/**
*判断浮点数是否是整数
*/
/*----------------------------------------------------------------------------------*/
bool HoughAnalyze::FloatIsInt(float n)
{
	ASSERT(sizeof(float)==4);
	int i, a=*(int*)&n, e=(a>>23&255)-127, t=a&8388607;
	if (n>=-1e-6F && n<=1e-6F) return true; //1e-6：判断是否为0时允许的误差
	if (e<0) return false;
	if (0x7FFFFF>>e&t) return false;
	return true;
}
/*----------------------------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------------------------*/