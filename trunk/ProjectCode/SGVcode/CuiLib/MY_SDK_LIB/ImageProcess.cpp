#include "StdAfx.h"
#include "ImageProcess.h"
#include "Base.h"

ImageProcess::ImageProcess(void)
{
}


ImageProcess::~ImageProcess(void)
{
}


void ImageProcess::findTestArea(void)
{

}

float ImageProcess::findAvgFloat(float* arr, int N)
{
	//float avg=0;
	float sum_t=0;
	
	for (int i = 1; i < N; i++){
		sum_t+=arr[i];
	}
	
	return (sum_t/N);

}
int ImageProcess::findMaxFloatIdx(float* arr, int N)
{
	float max = arr[0];
	int   max_idx=0;
	for (int i = 1; i < N; i++)
		if (max < arr[i]){
			max = arr[i];
			max_idx=i;
		}


		return max_idx;
}

void ImageProcess::adjustFloatMatrix2_100000(int height,float* matrix)
{
	float sum_t=0;

	//float sum_max=height*255;

	for (int i=0;i<height;i++){
		sum_t+=matrix[i];
	}

	for (int i=0;i<height;i++){		
		matrix[i]=10000*matrix[i]/sum_t;	
	}

}

CvRect ImageProcess::findTestArea(IplImage *image_gray,std::string readfile_name_t)
{
	assert(image_gray!=NULL);
	assert(image_gray->nChannels==1);

	//处理单色灰度图像

	IplImage *image_temp=cvCreateImage(cvGetSize(image_gray),IPL_DEPTH_8U,1);
	float * image_Y_sum=new float[image_temp->height];
	CvRect rect_cut_t; 

	memset(image_Y_sum,0,sizeof(float)*image_temp->height);
	{
#if 0
		cvSmooth(image_gray,image_temp);
		cvSaveImage(Base::comine_str(readfile_name_t,"test_out_smooth.jpg").c_str(),image_temp);
#else
		cvCopy(image_gray,image_temp);
#endif
		
		
#if 1
		uchar* data=(uchar *)image_temp->imageData;  
		int step = image_temp->widthStep/sizeof(uchar);  

		for(int i=0;i<image_temp->height;i++)  
			for(int j=0;j<image_temp->width;j++)  
				image_Y_sum[i]+=data[i*step+j];  //在高度方向累加
#endif		
		adjustFloatMatrix2_100000(image_temp->height,image_Y_sum);

#if _DEBUG
		{
			char data_t[1024];
			ofstream outfile;
			outfile.open(Base::comine_str(readfile_name_t,"Matrix_Y_SUM.txt").c_str(),ios::out);
			for(register int i = 0; i <image_temp->height; i++ ){

				float value_t=image_Y_sum[i];
				sprintf(data_t," %f ",value_t);
				outfile<<i;
				outfile<<' ';
				outfile<<data_t;
				outfile<<endl;
			} 
			outfile.close();
		}
#endif
		int max_idx=findMaxFloatIdx(image_Y_sum,image_temp->height);
		const float avg_t=findAvgFloat(image_Y_sum,image_temp->height);

		int top_idx=max_idx-1;
		int bottom_idx=max_idx+1;

		while(
			((image_Y_sum[top_idx]<=image_Y_sum[top_idx+1])||(image_Y_sum[top_idx-1]<=image_Y_sum[top_idx+1]))||
			(image_Y_sum[top_idx]>avg_t*1.5)
			){			

			if (top_idx--<10)
			{
				break;
			}
			if (avg_t>image_Y_sum[top_idx]){
				break;
			}

		}

		while(
			((image_Y_sum[bottom_idx]<=image_Y_sum[bottom_idx-1])||(image_Y_sum[bottom_idx+1]<=image_Y_sum[bottom_idx-1]))||
			(image_Y_sum[bottom_idx]>avg_t*1.5)
			){

			if (bottom_idx++>image_gray->height-10)
			{
				break;
			}
			if (avg_t>image_Y_sum[bottom_idx]){
				break;
			}


		}

		int delta=std::min(max_idx-top_idx,bottom_idx-top_idx);
#if _DEBUG&&0
		cvDrawRect(image_gray,cvPoint(0,max_idx-delta),cvPoint(image_gray->width,max_idx+delta),cvScalar(255,255,255));
		cvDrawLine(image_gray,cvPoint(0,max_idx),cvPoint(image_gray->width,max_idx),cvScalar(0,0,0));
		cvSaveImage(Base::comine_str(readfile_name_t,"test_out_org.jpg").c_str(),image_gray);
#endif
		rect_cut_t.x=0;
		rect_cut_t.y=max_idx-delta;
		rect_cut_t.width=image_gray->width;
		rect_cut_t.height=delta*2;



	}
	delete[] image_Y_sum;
	cvReleaseImage(&image_temp);

	return rect_cut_t;
}
/*--------------------------------------------------------------*/
/**
*
*
*/
/*--------------------------------------------------------------*/
void ImageProcess::SaveCutImage(IplImage* img_t,CvRect cut_t,string readfile_name_t)
{
	cvSetImageROI(img_t , cut_t);

	IplImage* img_cut_t=cvCreateImage(cvSize(cut_t.width,cut_t.height),IPL_DEPTH_8U,img_t->nChannels);
	
	cvCopy(img_t,img_cut_t,0);
	{
#if _DEBUG
		cvSaveImage(Base::comine_str(readfile_name_t,"_cut_image.jpg").c_str(),img_cut_t);
#endif
	}
	cvReleaseImage(&img_cut_t);
	
	cvResetImageROI(img_t);



	

}
/*--------------------------------------------------------------*/
/**
*
*
*/
/*--------------------------------------------------------------*/
void ImageProcess::FilterByMeanStdDev(IplImage* image_org_rgb,char *show_win)
{
	IplImage* Igray=cvCreateImage(cvGetSize(image_org_rgb),IPL_DEPTH_8U,1);
	IplImage* Ibinary=cvCreateImage(cvGetSize(image_org_rgb),IPL_DEPTH_8U,1);
#if 0
	IplImage  *Ibinary_old=cvCreateImage(cvGetSize(image_org_rgb),IPL_DEPTH_8U,1);
	IplImage  *Ibinary_sub=cvCreateImage(cvGetSize(image_org_rgb),IPL_DEPTH_8U,1);
#endif	  


	int width_t=image_org_rgb->width;
	int height_t=image_org_rgb->height;
	cvSetZero(Ibinary);
	cvCvtColor(image_org_rgb,Igray,CV_RGB2GRAY);
	
	//cvSmooth(Igray,Igray,CV_GAUSSIAN,3,3);
	
	{
		CvScalar mean;
		CvScalar std_dev;

		for (int yi=0;yi<image_org_rgb->height;yi++){
			
			CvRect rect_t=cvRect(0,yi,width_t,1);

			cvSetImageROI(Igray,rect_t);

			cvAvgSdv(Igray,&mean,&std_dev);

			//正态分布有两个参数，即期望（均数）μ和标准差σ，σ^2为方差。

			double     mean_t = mean.val[0];  //均值
			double     stddev_t = std_dev.val[0]; //标准差

			double sgm=3;

			double down_t=mean_t-sgm*stddev_t;
			double up_t=mean_t+sgm*stddev_t;			
			
			for(int xi=0;xi<width_t;xi++){
				double value_t=cvGetReal2D(Igray,0,xi);//ROI
				if (value_t<down_t||value_t>up_t){
					cvSet2D(Ibinary,yi,xi,CV_RGB(255,255,255));
				}

			}
#if 0
			printf("Mean : %f  , Stddev : %f \n",mean_t,std_dev);
#endif			

			cvResetImageROI(Igray);

		}

	}

#if _DEBUG&&0

	double sum_t=0;
	for (int i=0;i<image_org_rgb->height;i++){
		
		sum_t=0;
		for(int j=0;j<image_org_rgb->width;j++){

			sum_t+=cvGetReal2D(Igray,i,j);

		}

		double avg_t=sum_t/image_org_rgb->width;
		printf("Avg : %f  \n",avg_t);

	}

#endif

#if _DEBUG
	cvShowImage("gray",Igray);
	cvShowImage("binary",Ibinary);
	cvShowImage("org",image_org_rgb);
#endif
	cvReleaseImage(&Igray);
	cvReleaseImage(&Ibinary);

}
/*--------------------------------------------------------------*/
/**
*
*
*/
/*--------------------------------------------------------------*/
IplImage* ImageProcess::FilterByMeanStdDev(IplImage* image_org_rgb)
{
	IplImage* Igray=cvCreateImage(cvGetSize(image_org_rgb),IPL_DEPTH_8U,1);	
	IplImage* Ibinary=cvCreateImage(cvGetSize(image_org_rgb),IPL_DEPTH_8U,1);

	int width_t=image_org_rgb->width;
	int height_t=image_org_rgb->height;
	cvSetZero(Ibinary);
	cvCvtColor(image_org_rgb,Igray,CV_RGB2GRAY);
	cvShowImage("gray",Igray);

#if 1
	cvSmooth(Igray,Igray,CV_MEDIAN,3,3);
#endif


	{
		CvScalar mean;
		CvScalar std_dev;

		for (int yi=0;yi<image_org_rgb->height;yi++){

			CvRect rect_t=cvRect(0,yi,width_t,1);
			cvSetImageROI(Igray,rect_t);
			cvAvgSdv(Igray,&mean,&std_dev);
			//正态分布有两个参数，即期望（均数）μ和标准差σ，σ^2为方差。
			double     mean_t = mean.val[0];  //均值
			double     stddev_t = std_dev.val[0]; //标准差
			double sgm=3;
			double down_t=mean_t-sgm*stddev_t;
			double up_t=mean_t+sgm*stddev_t;			

			for(int xi=0;xi<width_t;xi++){
				double value_t=cvGetReal2D(Igray,0,xi);//ROI
				if (value_t<down_t||value_t>up_t){
					cvSet2D(Ibinary,yi,xi,CV_RGB(255,255,255));
				}

			}
#if 0
			printf("Mean : %f  , Stddev : %f \n",mean_t,std_dev);
#endif
			cvResetImageROI(Igray);

		}

	}

	cvReleaseImage(&Igray);
	
	return Ibinary;
}
/*--------------------------------------------------------------*/
/**
*
*
*/
/*--------------------------------------------------------------*/