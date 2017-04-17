#include <stdio.h>

#if 1
#include "HW_SDK_LIB/ipc.h"
#include "HW_SDK_LIB/live_set.h"
#include "HW_SDK_LIB/live_video.h"
#include "HW_SDK_LIB/dc_save.h"
#endif


#if 1
#include "cv.h"
using namespace cv;
#endif

#include "opencv_stl.h"

#include "MY_SDK_LIB/ImageProcess.h"

#define SUB_EXE  TRUE
#define THRESH   TRUE

void SaveFrame2Disk(IplImage *image,int count_tmp,char* path,char* add)
{	  
	char tmpfile[1000]={'\0'};  
	
	sprintf(tmpfile,"%s//%d_%s.png",path,count_tmp,add);//使用帧号作为图片名  
	
	cvSaveImage(tmpfile,image);  
}

CvRect FindIgnoreArea(char *tmpfile,IplImage *image_rgb_org)
{

	IplImage* image_gray=cvCreateImage(cvGetSize(image_rgb_org),IPL_DEPTH_8U,1);

	cvCvtColor(image_rgb_org,image_gray, CV_RGB2GRAY);
	
	string tmpfile_str=tmpfile;

	ImageProcess ImageProcess;
	
	CvRect rect_t=ImageProcess.findTestArea(image_gray,tmpfile_str);

	cvReleaseImage(&image_gray);

	return rect_t;
}

void FindAndDrawResut_rect(IplImage* Ibinary,IplImage* image_rgb_org_t,int count_tmp,char *AviSavePath)
{

	IplImage *image_rgb_org=cvCloneImage(image_rgb_org_t);
#if TRUE
	CvMemStorage* storage = cvCreateMemStorage (0);;
	CvSeq* contour = 0; 
	int contours = 0;  
	CvScalar external_color;  
	CvScalar hole_color;  
	contours = cvFindContours (Ibinary, storage, &contour, sizeof(CvContour),CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

	int count_t=0;

	for (;contour != 0; contour = contour->h_next)  
	{  
		if(contour->total<100) continue;
		count_t++;
		external_color = CV_RGB(255, 255,0);  
		hole_color = CV_RGB(0,0,255);  

		cvDrawContours (image_rgb_org, contour, external_color, hole_color,CV_FILLED);

		CvRect rect_b_t=cvBoundingRect(contour);
		cvRectangleR(image_rgb_org,rect_b_t,CV_RGB(0,0,255));

		//CvBox2D End_Rage2D_t = cvMinAreaRect2(contour);

	} 
	printf("Contours: %d \n",count_t);
	SaveFrame2Disk(image_rgb_org,count_tmp,AviSavePath,"ContoursRect");
#endif
#if _DEBUG
	cvShowImage("org",image_rgb_org);
#endif
	cvReleaseImage(&image_rgb_org);

}

void ThresholdTest(IplImage *sub_image_rgb,int count_tmp)
{

	IplImage *Igray = NULL;  
	IplImage *Iat = NULL;
	IplImage *Ismooth=NULL;

	
	Igray=cvCreateImage(cvSize(sub_image_rgb->width,sub_image_rgb->height),IPL_DEPTH_8U,1);
	Ismooth=cvCreateImage(cvSize(sub_image_rgb->width,sub_image_rgb->height),IPL_DEPTH_8U,1);


	Iat=cvCloneImage(Igray);

	cvCvtColor(sub_image_rgb,Igray, CV_RGB2GRAY);
	
	cvShowImage("gray",Igray);

#if 0
	int adaptive_method =CV_ADAPTIVE_THRESH_GAUSSIAN_C;
	int threshold_type  = CV_THRESH_BINARY;   
    int block_size =101;
	double offset=0;  

	cvAdaptiveThreshold( Igray, Iat, 255, adaptive_method, threshold_type, block_size, offset );  

#endif

#if 0
	cvThreshold(Igray,Iat,0,255,CV_THRESH_OTSU);
#endif

#if 0
	cvSmooth(Igray,Ismooth,CV_GAUSSIAN);
	cvShowImage("smooth",Ismooth);
	
	cvThreshold(Ismooth,Iat,10,255,CV_THRESH_BINARY);
	cvShowImage("binary",Iat);


#endif

#if 0	

	int mask[9] = {0, 0, 0,
				   1, 1, 1,
				   0, 0, 0};  
  
    IplConvKernel* element=cvCreateStructuringElementEx(3,3,1,1,CV_SHAPE_CUSTOM,mask );

	cvThreshold(Igray,Iat,10,255,CV_THRESH_BINARY);
	
	cvShowImage("binary",Iat); 

	cvErode(Iat,Ismooth,element,1);//腐蚀

	cvShowImage("erode",Ismooth); 

#endif

#if 1
		cvThreshold(Igray,Iat,5,255,CV_THRESH_BINARY);
#endif

#if 1
	
		cvShowImage("binary",Iat);
		char *AviSavePath = "G:\\crack detection\\crack detection_windows\\192_168_3_3\\cut\\";//图片保存的位置  
		char tmpfile[100]={'\0'};  
		sprintf(tmpfile,"%s//%d_binary.png",AviSavePath,count_tmp);//使用帧号作为图片名  
		cvSaveImage(tmpfile,Iat);  

#endif

#if 0
	 CvMemStorage* storage = cvCreateMemStorage (0);;
	 CvSeq* contour = 0; 
	 int contours = 0;  
	 CvScalar external_color;  
     CvScalar hole_color;  
	 contours = cvFindContours (Iat, storage, &contour, sizeof(CvContour),CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	 
	for (;contour != 0; contour = contour->h_next)  
    {  
		if(contour->total<10) continue;

        external_color = CV_RGB(rand()&255, rand()&255, rand()&255);  
        hole_color = CV_RGB(rand()&255, rand()&255, rand()&255);  
        cvDrawContours (sub_image_rgb, contour, external_color, hole_color, 1, 2, 8);  
    } 
#endif

#if 0
		char *AviSavePath = "G:\\crack detection\\crack detection_windows\\192_168_3_3\\cut\\";//图片保存的位置  
		char tmpfile[100]={'\0'};  
		sprintf(tmpfile,"%s//%d_binary.png",AviSavePath,count_tmp);//使用帧号作为图片名  
		cvSaveImage(tmpfile,Igray);  
#endif

	cvReleaseImage(&Iat);
	cvReleaseImage(&Igray);
	cvReleaseImage(&Ismooth);

}


void FindAndDrawResut_line_PROBABILISTIC(IplImage* Ibinary,IplImage* image_rgb_org_t,int count_tmp,char *AviSavePath)
{
	CvMemStorage* storage=cvCreateMemStorage(0); 
	
	CvSeq* lines = 0;
	
	IplImage *image_rgb_org=cvCreateImage(cvGetSize(Ibinary),IPL_DEPTH_8U,3);
	  
	cvCvtColor(Ibinary,image_rgb_org, CV_GRAY2RGB);
	
	double rho=1;  
	double theta=CV_PI/180;  
	double threshold=100;  //如果相应的累计值大于 threshold， 则函数返回的这个线段.
	double min_length=50;//CV_HOUGH_PROBABILISTIC  
	double sepration_connection=50;//CV_HOUGH_PROBABILISTIC 

	lines = cvHoughLines2(Ibinary, storage, CV_HOUGH_PROBABILISTIC,
		rho,
		theta,
		threshold,
		min_length,
		CV_HOUGH_PROBABILISTIC);

	{
		printf("%d\n",lines->total);
		int dis=0;
		int max=0;
		int j=0;
		CvPoint* line;
		CvPoint pointOne;
		CvPoint pointTwo;
		//int *a=new int[lines->total*4];
		RNG rng(0xFFFFFFFF);
		for(int i = 0; i < lines->total; i++ )
		{
			line = (CvPoint*)cvGetSeqElem(lines,i);

			//dis=(line[1].y-line[0].y)*(line[1].y-line[0].y)+(line[1].x-line[0].x)*(line[1].x-line[0].x);
			/*a[4*i]=line[0].x;
			a[4*i+1]=line[0].y;
			a[4*i+2]=line[1].x;
			a[4*i+3]=line[1].y;
			if(dis>max){
				max=dis;
				j=i;
			}*/
			
			rng.next();
			int icolor = (unsigned)rng;
			cvLine(image_rgb_org, line[i], line[i+1],Scalar(icolor&0xFF, 255, (icolor>>16)&0xFF),1);
		}
		//pointOne.x=a[4*j];
		//pointOne.y=a[4*j+1];
		//pointTwo.x=a[4*j+2];
		//pointTwo.y=a[4*j+3];
		//cvLine( color_dst, pointOne, pointTwo, CV_RGB(255,0,0), 3, 8 );    //画出最长的直线

	}

#if _DEBUG
	cvShowImage("org",image_rgb_org);
	SaveFrame2Disk(image_rgb_org,count_tmp,AviSavePath,"hough");
#endif
	cvReleaseMemStorage(&storage);
	cvReleaseImage(&image_rgb_org);
}


void FindAndDrawResut_line_STANDARD(IplImage* Ibinary,IplImage* image_rgb_org_t,int count_tmp,char *AviSavePath)
{
	CvMemStorage* storage=cvCreateMemStorage(0); 
	
	
	
	IplImage *image_rgb_org=cvCreateImage(cvGetSize(Ibinary),IPL_DEPTH_8U,3);
	  
	cvCvtColor(Ibinary,image_rgb_org, CV_GRAY2RGB);
	
	double rho=1;  
	double theta=CV_PI/180;  
	double threshold=200;  //如果相应的累计值大于 threshold， 则函数返回的这个线段.


	CvSeq * results = cvHoughLines2(Ibinary, storage,CV_HOUGH_STANDARD,
		rho,
		theta,
		threshold);

	{
		printf("%d\n",results->total);
		for(int i = 0 ; i < results ->total ; i ++)  
		{  
#if 1
			float* line = (float*)cvGetSeqElem(results,i);
			float rho = line[0];
			float theta = line[1];
			CvPoint pt1, pt2;
			double a = cos(theta), b = sin(theta);
			double x0 = a*rho, y0 = b*rho;
			pt1.x = cvRound(x0 + 2000*(-b));
			pt1.y = cvRound(y0 + 2000*(a));
			pt2.x = cvRound(x0 - 2000*(-b));
			pt2.y = cvRound(y0 - 2000*(a));
			cvLine(image_rgb_org,pt1,pt2,CV_RGB(0,255,0)); 
#else
			float * lines = (float*) cvGetSeqElem(results, i) ;  
			float rho =lines[0]  ;  
			float theta=lines[1];  
			CvPoint pt1,pt2 ;  
			double a = cos(theta),b=sin(theta) ;  
			if(fabs(a)<0.001)  
			{  
				pt1.x = pt2.x = cvRound(rho) ;  
				pt1.y=0 ;  
				pt2.y=Ibinary->height;  
			}  
			else if(fabs(b)<0.001)  
			{  
				pt1.y = pt2.y = cvRound(rho) ;  
				pt1.x=0 ;  
				pt2.x = Ibinary->width;  
			}  
			else  
			{  
				pt1.x=0;  
				pt1.y=cvRound(rho/b) ;  
				pt2.x=cvRound(rho/a) ;  
				pt2.y=0 ;  
			}
			cvLine(image_rgb_org,pt1,pt2,CV_RGB(0,255,0)); 
#endif
 
		}  
	}

#if _DEBUG
	cvShowImage("org",image_rgb_org);
	SaveFrame2Disk(image_rgb_org,count_tmp,AviSavePath,"hough_std");
#endif
	cvReleaseMemStorage(&storage);
	cvReleaseImage(&image_rgb_org);

}

void ThresholdTest_Adaptive_Thresh(IplImage *sub_image_rgb,int count_tmp)
{
	IplImage *Igray = NULL;  
	IplImage *Ibinary = NULL;
		
	IplImage *Ibinary00 = NULL;
	IplImage *Ibinary01 = NULL;

	IplImage *Ismooth=NULL;

#if 1

	char *AviSavePath = "G:\\crack detection\\crack detection_windows\\192_168_3_3\\cut\\";//图片保存的位置  
	
	char tmpfile[1000]={'\0'};  
	
	sprintf(tmpfile,"%s//%d_xxxxx.png",AviSavePath,count_tmp);//使用帧号作为图片名 

#endif

	Igray=cvCreateImage(cvSize(sub_image_rgb->width,sub_image_rgb->height),IPL_DEPTH_8U,1);
	
	Ismooth=cvCreateImage(cvSize(sub_image_rgb->width,sub_image_rgb->height),IPL_DEPTH_8U,1);
	
	Ibinary=cvCloneImage(Igray);
	Ibinary00=cvCloneImage(Igray);
	Ibinary01=cvCloneImage(Igray);


	cvCvtColor(sub_image_rgb,Igray, CV_RGB2GRAY);
	

 

	string tmpfile_str=tmpfile;

	ImageProcess ImageProcess;
	CvRect rect_t=ImageProcess.findTestArea(Igray,tmpfile_str);
	
	cvRectangleR(sub_image_rgb,rect_t,CV_RGB(0,255,0));

	cvShowImage("gray",Igray);



#if 1
	cvSmooth(Igray,Igray,CV_GAUSSIAN,3,3);
	cvShowImage("smooth",Ismooth);
#endif

#if 0
	cvSmooth(Igray,Igray,CV_MEDIAN,3,3);
	cvSmooth(Igray,Ismooth,CV_MEDIAN,3,3);
	cvShowImage("smooth",Ismooth);
#endif

#if 0
	int adaptive_method =CV_ADAPTIVE_THRESH_GAUSSIAN_C;
	int threshold_type  = CV_THRESH_BINARY;   
    int block_size =111;
	double offset=0;  

	cvAdaptiveThreshold( Igray, Ibinary00, 255, adaptive_method, threshold_type, block_size, offset );  

	cvShowImage("binary00",Ibinary00);

#endif

#if 1
	cvThreshold(Igray,Ibinary,5,255,CV_THRESH_BINARY);
	
	cvSmooth(Ibinary,Ibinary,CV_MEDIAN,5,3);

	cvShowImage("binary",Ibinary);

	
 
	sprintf(tmpfile,"%s//%d_binary.png",AviSavePath,count_tmp);//使用帧号作为图片名  
	cvSaveImage(tmpfile,Ibinary);  

#endif

#if 0
		cvThreshold(Igray,Ibinary01,5,255,CV_THRESH_BINARY);
		cvShowImage("binary01",Ibinary01);

		cvAnd(Ibinary01,Ibinary00,Ibinary);
		cvShowImage("binary",Ibinary);
#endif

#if 0

	int mask[9] = {0, 0, 0,
				   1, 1, 1,
				   0, 0, 0};  
  
    IplConvKernel* element=cvCreateStructuringElementEx(3,3,1,1,CV_SHAPE_CUSTOM,mask );

	cvThreshold(Igray,Iat,10,255,CV_THRESH_BINARY);
	
	cvShowImage("binary",Iat); 

	cvErode(Iat,Ismooth,element,1);//腐蚀

	cvShowImage("erode",Ismooth); 

#endif

#if 0
	
		cvShowImage("binary",Iat);
		char *AviSavePath = "G:\\crack detection\\crack detection_windows\\192_168_3_3\\cut\\";//图片保存的位置  
		char tmpfile[100]={'\0'};  
		sprintf(tmpfile,"%s//%d_binary.png",AviSavePath,count_tmp);//使用帧号作为图片名  
		cvSaveImage(tmpfile,Iat);  

#endif

#if 0
	 CvMemStorage* storage = cvCreateMemStorage (0);;
	 CvSeq* contour = 0; 
	 int contours = 0;  
	 CvScalar external_color;  
     CvScalar hole_color;  
	 contours = cvFindContours (Iat, storage, &contour, sizeof(CvContour),CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	 
	for (;contour != 0; contour = contour->h_next)  
    {  
		if(contour->total<10) continue;

        external_color = CV_RGB(rand()&255, rand()&255, rand()&255);  
        hole_color = CV_RGB(rand()&255, rand()&255, rand()&255);  
        cvDrawContours (sub_image_rgb, contour, external_color, hole_color, 1, 2, 8);  
    } 
#endif

#if 0
		char *AviSavePath = "G:\\crack detection\\crack detection_windows\\192_168_3_3\\cut\\";//图片保存的位置  
		char tmpfile[100]={'\0'};  
		sprintf(tmpfile,"%s//%d_binary.png",AviSavePath,count_tmp);//使用帧号作为图片名  
		cvSaveImage(tmpfile,Igray);  
#endif

	cvReleaseImage(&Ibinary);
	cvReleaseImage(&Ibinary00);
	cvReleaseImage(&Ibinary01);
	cvReleaseImage(&Igray);
	cvReleaseImage(&Ismooth);

}

/**
*
* 凹痕检测 轮廓法
*
*/
void find_pit(IplImage *sub_image_rgb,int count_tmp,IplImage* image_rgb_org)
{
	IplImage *Igray = NULL;  
	IplImage *Ibinary = NULL;

	IplImage *Ibinary00 = NULL;
	IplImage *Ibinary01 = NULL;

	IplImage *Ismooth=NULL;

#if 1

	char *AviSavePath = "G:\\crack detection\\crack detection_windows\\192_168_3_3\\cut\\";//图片保存的位置  

	char tmpfile[1000]={'\0'};  

	sprintf(tmpfile,"%s//%d_xxxxx.png",AviSavePath,count_tmp);//使用帧号作为图片名 

#endif

	Igray=cvCreateImage(cvSize(sub_image_rgb->width,sub_image_rgb->height),IPL_DEPTH_8U,1);

	Ismooth=cvCreateImage(cvSize(sub_image_rgb->width,sub_image_rgb->height),IPL_DEPTH_8U,1);

	Ibinary=cvCloneImage(Igray);
	Ibinary00=cvCloneImage(Igray);
	Ibinary01=cvCloneImage(Igray);


	cvCvtColor(sub_image_rgb,Igray, CV_RGB2GRAY);
	
	CvRect rect_t=FindIgnoreArea(tmpfile,image_rgb_org);



#if 1
	cvSmooth(Igray,Igray,CV_GAUSSIAN,3,3);
	//cvShowImage("smooth",Ismooth);
#endif

#if 1
	cvThreshold(Igray,Ibinary,5,255,CV_THRESH_BINARY);
	cvSmooth(Ibinary,Ibinary,CV_MEDIAN,5,3);
#endif


	cvRectangleR(Ibinary,rect_t,CV_RGB(0,0,0),CV_FILLED);
#if _DEBUG	
	SaveFrame2Disk(Ibinary,count_tmp,AviSavePath,"binary");
#endif

	
	FindAndDrawResut_rect(Ibinary,image_rgb_org,count_tmp,AviSavePath);



#if _DEBUG
	cvShowImage("binary",Ibinary);
#endif

#if _DEBUG
	cvRectangleR(Igray,rect_t,CV_RGB(255,255,255));
	cvShowImage("gray",Igray);
#endif
	cvReleaseImage(&Ibinary);
	cvReleaseImage(&Ibinary00);
	cvReleaseImage(&Ibinary01);
	cvReleaseImage(&Igray);
	cvReleaseImage(&Ismooth);
}


void find_line(IplImage *sub_image_rgb,int count_tmp,IplImage* image_rgb_org)
{
	IplImage *Igray = NULL;  
	IplImage *Ibinary = NULL;

	IplImage *Ibinary00 = NULL;
	IplImage *Ibinary01 = NULL;

	IplImage *Ismooth=NULL;

#if 1

	char *AviSavePath = "G:\\crack detection\\crack detection_windows\\192_168_3_3\\cut\\";//图片保存的位置  

	char tmpfile[1000]={'\0'};  

	sprintf(tmpfile,"%s//%d_xxxxx.png",AviSavePath,count_tmp);//使用帧号作为图片名 

#endif

	Igray=cvCreateImage(cvSize(sub_image_rgb->width,sub_image_rgb->height),IPL_DEPTH_8U,1);

	Ismooth=cvCreateImage(cvSize(sub_image_rgb->width,sub_image_rgb->height),IPL_DEPTH_8U,1);

	Ibinary=cvCloneImage(Igray);
	Ibinary00=cvCloneImage(Igray);
	Ibinary01=cvCloneImage(Igray);


	cvCvtColor(sub_image_rgb,Igray, CV_RGB2GRAY);

	CvRect rect_t=FindIgnoreArea(tmpfile,image_rgb_org);

#if 1
	int adaptive_method =CV_ADAPTIVE_THRESH_GAUSSIAN_C;
	int threshold_type  = CV_THRESH_BINARY;   
	int block_size =3;
	double offset=0;  
	cvAdaptiveThreshold( Igray, Ibinary, 255, adaptive_method, threshold_type, block_size, offset );  
#endif

#if 0
	//cvSmooth(Igray,Igray,CV_MEDIAN,3,3);
	cvThreshold(Igray,Ibinary,3,255,CV_THRESH_BINARY);
#endif
#if 0
	//cvSmooth(Igray,Igray,CV_GAUSSIAN,3,3);
	cvThreshold(Igray,Ibinary,5,255,CV_THRESH_BINARY);
	//cvSmooth(Ibinary,Ibinary,CV_MEDIAN,5,3);
#endif
#if 0
	cvCanny(Igray,Ibinary,1,10);
#endif

//	cvRectangleR(Ibinary,rect_t,CV_RGB(0,0,0),CV_FILLED);
//#if _DEBUG	
//	SaveFrame2Disk(Ibinary,count_tmp,AviSavePath,"binary");
//#endif
//
//
//	FindAndDrawResut_rect(Ibinary,image_rgb_org,count_tmp,AviSavePath);

	cvRectangleR(Ibinary,rect_t,CV_RGB(0,0,0),CV_FILLED);

#if 1
	FindAndDrawResut_line_PROBABILISTIC(Ibinary,image_rgb_org,count_tmp,AviSavePath);
#else
	FindAndDrawResut_line_STANDARD(Ibinary,image_rgb_org,count_tmp,AviSavePath);
#endif



#if _DEBUG
	cvShowImage("binary",Ibinary);
#endif

#if _DEBUG
	cvRectangleR(Igray,rect_t,CV_RGB(255,255,255));
	cvShowImage("gray",Igray);
#endif
	cvReleaseImage(&Ibinary);
	cvReleaseImage(&Ibinary00);
	cvReleaseImage(&Ibinary01);
	cvReleaseImage(&Igray);
	cvReleaseImage(&Ismooth);
	
}

void process_by_gaussian()
{
	CvCapture *capture = NULL;  
	IplImage *frame = NULL;  
	int count_tmp = 0;//计数总帧数  
	char tmpfile[100]={'\0'}; 
	char *AviFileName = "G:\\crack detection\\crack detection_windows\\192_168_3_3\\192.168.3.32016_12_16_17_15_13_.avi";//视频的目录  
	char *AviSavePath = "G:\\crack detection\\crack detection_windows\\192_168_3_3\\cut\\";//图片保存的位置  
	const int jiange = 1;//间隔两帧保存一次图片  
	capture = cvCaptureFromAVI(AviFileName);  

#if 1
	cvNamedWindow("sub",0);
	cvResizeWindow("sub",600,338);

	cvNamedWindow("org",0);
	cvResizeWindow("org",600,338);

	/*cvNamedWindow("smooth",0);
	cvResizeWindow("smooth",600,338);*/

	/*cvNamedWindow("erode",0);
	cvResizeWindow("erode",600,338);*/

	cvNamedWindow("binary",0);
	cvResizeWindow("binary",600,338);

	/*cvNamedWindow("binary00",0);
	cvResizeWindow("binary00",600,338);

	cvNamedWindow("binary01",0);
	cvResizeWindow("binary01",600,338);*/

	cvNamedWindow("gray",0);
	cvResizeWindow("gray",600,338);
#endif 



#if 0

	IplImage  *frame_old=NULL;
	IplImage  *frame_sub=NULL;

	while(true)  
	{  
		if(cvGrabFrame(capture))  
		{  
			if (count_tmp % jiange == 0)  
			{  
				if(frame!=NULL){
					cvCopyImage(frame,frame_old);
				}

				frame=cvRetrieveFrame(capture);

				if(frame_old==NULL){
					frame_old=cvCloneImage(frame);
				}
#if SUB_EXE
				if(frame_sub==NULL){
					frame_sub=cvCloneImage(frame);
					cvZero(frame_sub);
				}
#endif

#if 0
				cvShowImage("sub",frame);//显示当前帧  
				sprintf(tmpfile,"%s//%d.png",AviSavePath,count_tmp);//使用帧号作为图片名  
				cvSaveImage(tmpfile,frame);  
#endif

#if SUB_EXE				
				if(count_tmp>=1){
#if 1
					cvSub(frame,frame_old,frame_sub);
#else
					cvAbsDiff(frame,frame_old,frame_sub);
#endif

				}

#if 0
				find_pit(frame_sub,count_tmp,frame);

				find_line(frame_sub,count_tmp,frame);
#endif

#if 0
				ImageProcess iprocess_t;
				iprocess_t.FilterByMeanStdDev(frame,NULL);
#endif

#if 1

#endif


#if 0
				ThresholdTest_Adaptive_Thresh(frame_sub,count_tmp);
				//ThresholdTest(frame_sub,count_tmp);
#endif


				cvShowImage("sub",frame_sub);//显示当前帧
				sprintf(tmpfile,"%s//%d_sub.png",AviSavePath,count_tmp);//使用帧号作为图片名  
				cvSaveImage(tmpfile,frame_sub);  
#endif
			}                 
			if(cvWaitKey(10)>=0) //延时  
				break;  
			++count_tmp;  
		}  
		else  
		{  
			break;  
		}  
	}  

	//cvReleaseImage(&frame);
	cvReleaseImage(&frame_old);
	cvReleaseImage(&frame_sub);
	cvReleaseCapture(&capture);  
	cvDestroyWindow("AVI player");   
	std::cout<<"总帧数" << count_tmp << std::endl;  
#endif

#if 1
	IplImage* frame_old=NULL;

	IplImage* Ibinary_old=NULL;	

	IplImage* Ibinary=NULL;

	IplImage* Ibinary_sub=NULL;

	while(true)  
	{  
		if(cvGrabFrame(capture))  
		{  
			if (count_tmp % jiange == 0)  
			{ 
#if 1


				frame=cvRetrieveFrame(capture);

				ImageProcess img_process_t;
				Ibinary=img_process_t.FilterByMeanStdDev(frame);	

				if (Ibinary_old==NULL){
					Ibinary_old=cvCloneImage(Ibinary);
				}else{

				}

				if (Ibinary_sub==NULL){
					Ibinary_sub=cvCloneImage(Ibinary);;
				}

				if(count_tmp>=1){
#if 1
					cvSub(Ibinary,Ibinary_old,Ibinary_sub);
#else
					cvAbsDiff(frame,frame_old,frame_sub);
#endif

				}

				cvShowImage("binary",Ibinary);

				cvShowImage("org",frame);

				cvShowImage("sub",Ibinary_sub);

#if _DEBUG	
	SaveFrame2Disk(Ibinary,count_tmp,AviSavePath,"org");
#endif


				if (Ibinary!=NULL){
					cvCopyImage(Ibinary,Ibinary_old);
					cvReleaseImage(&Ibinary);
				}



#endif

			}                 
			if(cvWaitKey(10)>=0) //延时  
				break;  
			++count_tmp;  
		}  
		else  
		{  
			break;  
		}  
	}  

	//cvReleaseImage(&frame);
	cvReleaseImage(&frame_old);

	cvReleaseCapture(&capture);  
	cvDestroyWindow("AVI player");   
	std::cout<<"总帧数" << count_tmp << std::endl;  
#endif

}


int main()
{
	process_by_gaussian();

    return 0;
}