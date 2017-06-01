//图像的Canny边缘检测
//By MoreWindows (http://blog.csdn.net/MoreWindows)



//#include "stdafx.h"

#include <opencv_stl.h>

#include <opencv2/opencv.hpp>
using namespace std;


#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
using namespace cv;
using namespace std;


#include "MY_SDK_LIB/ImageProcess.h"

#define	USE_CANNY 0
#define USE_Laplace 1

//#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
IplImage *g_pSrcImage, *g_pCannyImg;
const char *pstrWindowsCannyTitle = "边缘检测图(http://blog.csdn.net/MoreWindows)";
//cvCreateTrackbar的回调函数

void SaveFrame2Disk(IplImage *image,int count_tmp,char* path,char* add)
{	  
	char tmpfile[1000]={'\0'};  
	
	sprintf(tmpfile,"%s//%d_%s.png",path,count_tmp,add);//使用帧号作为图片名  
	
	cvSaveImage(tmpfile,image);  
}

void on_trackbar(int threshold)
{
#if USE_CANNY
	//canny边缘检测
	cvCanny(g_pSrcImage, g_pCannyImg, threshold, threshold * 3, 3);
	cvSaveImage("ohh.png",g_pCannyImg);
#endif

#if USE_Laplace
	if(threshold%2==1){
		cvLaplace(g_pSrcImage, g_pCannyImg,threshold);	
		SaveFrame2Disk(g_pCannyImg,threshold,"G:\\IMG_T","");
	}
#endif

	cvShowImage(pstrWindowsCannyTitle, g_pCannyImg);
	

}
//利德曼
void Laplace_test()
{
	const char *pstrImageName = "D:\\ImageDataBase\\duanjian\\11_.png";
	const char *pstrWindowsSrcTitle = "原图(http://blog.csdn.net/MoreWindows)";
	const char *pstrWindowsToolBar = "Threshold";

	//从文件中载入图像的灰度图CV_LOAD_IMAGE_GRAYSCALE - 灰度图
#if USE_CANNY
	g_pSrcImage = cvLoadImage(pstrImageName, CV_LOAD_IMAGE_GRAYSCALE);
	g_pCannyImg = cvCreateImage(cvGetSize(g_pSrcImage), IPL_DEPTH_8U, 1);
#else
	g_pSrcImage = cvLoadImage(pstrImageName);

	g_pCannyImg = cvCreateImage(cvGetSize(g_pSrcImage), IPL_DEPTH_8U, g_pSrcImage->nChannels);
#endif


	//创建窗口
	cvNamedWindow(pstrWindowsSrcTitle, CV_WINDOW_AUTOSIZE);
	cvNamedWindow(pstrWindowsCannyTitle, CV_WINDOW_AUTOSIZE);

//创建滑动条
	int nThresholdEdge = 1;
	cvCreateTrackbar(pstrWindowsToolBar, pstrWindowsCannyTitle, &nThresholdEdge, 100, on_trackbar);

	//在指定窗口中显示图像
	cvShowImage(pstrWindowsSrcTitle, g_pSrcImage);
	on_trackbar(1);

	//等待按键事件
	cvWaitKey();

	cvDestroyWindow(pstrWindowsSrcTitle);
	cvDestroyWindow(pstrWindowsCannyTitle);
	cvReleaseImage(&g_pSrcImage);
	cvReleaseImage(&g_pCannyImg);
}

int Hough_test()
{
	Mat src, gray;

	//src=imread("D:\\ImageDataBase\\duanjian\\easy.jpg");
	src=imread("D:\\ImageDataBase\\duanjian\\Contours.png");

	//src=imread("D:\\ImageDataBase\\duanjian\\yuan.jpg");
	
	if( !src.data )  
		return -1;  
	
	cvtColor( src, gray, CV_BGR2GRAY );
    //高斯模糊平滑
	GaussianBlur( gray, gray, Size(9, 9), 2, 2 );

    vector<Vec3f> circles;
    //霍夫变换
	//30-70
	int dist=1;
	int canny=50;
	int r_min=20;
	int r_max=100;
	int accumulator=80;
#if 0
    HoughCircles(gray, circles, CV_HOUGH_GRADIENT, 1, dist,
		100, 30,		
		0, 0 );
#else
    HoughCircles(gray, circles, CV_HOUGH_GRADIENT, 1,
		dist,
		canny, 
		accumulator,
		r_min,
		r_max);
#endif


    //在原图中画出圆心和圆
    for( size_t i = 0; i < circles.size(); i++ )
    {
        //提取出圆心坐标
        Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        //提取出圆半径
        int radius = cvRound(circles[i][2]);
        //圆心
        circle( src, center, 3, Scalar(0,255,0), -1, 8, 0 );
        //圆
        circle( src, center, radius, Scalar(0,0,255), 3, 8, 0 );
   }

    namedWindow( "Circle", CV_WINDOW_AUTOSIZE );
    imshow( "Circle", src );

    return waitKey(0);

}

int Hough_Canny()
{
	Mat src, gray;

	//src=imread("D:\\ImageDataBase\\duanjian\\easy.jpg");
	//src=imread("D:\\ImageDataBase\\duanjian\\Contours.png");
	src=imread("D:\\ImageDataBase\\duanjian\\11_.png");



       Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));  
       Mat out;  
   
//进行腐蚀操作 
#if 0
	   Mat element_line_h = getStructuringElement(MORPH_RECT, Size(1, 3));
	   Mat element_line_v = getStructuringElement(MORPH_RECT, Size(3, 1)); 
       dilate(src,src, element);  
	   dilate(src,src, element);  
#endif	   

	  
#if 0	   
	   erode(src,src, element_line_h); 
	   erode(src,src, element_line_v); 
	    erode(src,src, element_line_h); 
	   erode(src,src, element_line_v); 
#endif	
	   //src=imread("D:\\ImageDataBase\\duanjian\\yuan.jpg");
	
	if( !src.data )  
		return -1;  
	
	cvtColor( src, gray, CV_BGR2GRAY );
    //高斯模糊平滑
	GaussianBlur( gray, gray, Size(9, 9), 2, 2 );

    vector<Vec3f> circles;
    //霍夫变换
	//30-70
	int dist=1;
	int canny=20;
	int r_min=30;
	int r_max=90;
	int accumulator=61;
#if 0
    HoughCircles(gray, circles, CV_HOUGH_GRADIENT, 1, dist,
		100, 30,		
		0, 0 );
#else
    HoughCircles(gray, circles, CV_HOUGH_GRADIENT, 1,
		dist,
		canny, 
		accumulator,
		r_min,
		r_max);
#endif


    //在原图中画出圆心和圆
    for( size_t i = 0; i < circles.size(); i++ )
    {
        //提取出圆心坐标
        Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        //提取出圆半径
        int radius = cvRound(circles[i][2]);
        //圆心
        circle( src, center, 3, Scalar(0,255,0), -1, 8, 0 );
        //圆
        circle( src, center, radius, Scalar(0,0,255), 2, 8, 0 );
   }

    namedWindow( "Circle", CV_WINDOW_AUTOSIZE );
    imshow( "Circle", src );
	imshow( "Gary", gray);

   return waitKey(0);
}

void UseMyHough_edges()
{
	CvSeq* results;
	CvMemStorage* storage = cvCreateMemStorage(0);
#if 1
	IplImage* src = cvLoadImage("D:\\ImageDataBase\\duanjian\\hough_edges.png", 0 );
	IplImage* dst = cvLoadImage("D:\\ImageDataBase\\duanjian\\hough_edges.png");

	results =ImageProcess::cvHoughCircles_Binary(  //cvHoughCircles函数需要估计每一个像素梯度的方向，
								      //因此会在内部自动调用cvSobel,而二值边缘图像的处理是比较难的
		src,
		storage,
		CV_HOUGH_GRADIENT,
		1,  //累加器图像的分辨率
		20,//圆心间距
		100,//canny
		36,//累加器
		
		10,//小半径
		500//最大半径
		
		);
#endif
	for( int i = 0; i < results->total; i++ )
	{
		float* p = ( float* )cvGetSeqElem( results, i );
		//霍夫圆变换
		CvPoint pt = cvPoint( cvRound( p[0] ), cvRound( p[1] ) );
		cvCircle(
			dst,
			pt,  //确定圆心
			cvRound( p[2] ),  //确定半径
			CV_RGB( 0xff, 0, 0 )
		);  //画圆函数
	}
	cvNamedWindow( "cvHoughCircles", 0 );
	cvShowImage( "cvHoughCircles", dst );
}

void UseMyHough_SuperPixel()
{
	CvSeq* results;
	CvMemStorage* storage = cvCreateMemStorage(0);
#if 0	
	IplImage* src = cvLoadImage("D:\\ImageDataBase\\duanjian\\Contours.png", 0 );
	IplImage* dst = cvLoadImage("D:\\ImageDataBase\\duanjian\\Contours.png");

	//cvSmooth( src, dst, CV_GAUSSIAN, 5, 5 );  //降噪


	results =ImageProcess::cvHoughCircles_Binary(  //cvHoughCircles函数需要估计每一个像素梯度的方向，
								      //因此会在内部自动调用cvSobel,而二值边缘图像的处理是比较难的
		src,
		storage,
		CV_HOUGH_GRADIENT,
		1,  //累加器图像的分辨率
		1,//圆心间距
		30,//canny
		61,//累加器
		
		20,//小半径
		100//最大半径
		
		);
#endif
#if 1
	IplImage* src = cvLoadImage("D:\\ImageDataBase\\duanjian\\easy.jpg", 0 );
	IplImage* dst = cvLoadImage("D:\\ImageDataBase\\duanjian\\easy.jpg");

	cvSmooth( src, src, CV_GAUSSIAN, 5, 5 );  //降噪


	results =ImageProcess::cvHoughCircles_Binary(  //cvHoughCircles函数需要估计每一个像素梯度的方向，
								      //因此会在内部自动调用cvSobel,而二值边缘图像的处理是比较难的
		src,
		storage,
		CV_HOUGH_GRADIENT,
		1,  //累加器图像的分辨率
		1,//圆心间距
		30,//canny
		45,//累加器
		
		30,//小半径
		100//最大半径
		
		);
#else
	IplImage* src = cvLoadImage("D:\\ImageDataBase\\duanjian\\easy.jpg", 0 );
	IplImage* dst = cvLoadImage("D:\\ImageDataBase\\duanjian\\easy.jpg");
	cvSmooth( src, src, CV_GAUSSIAN, 5, 5 );  //降噪
	results =cvHoughCircles(  //cvHoughCircles函数需要估计每一个像素梯度的方向，
								      //因此会在内部自动调用cvSobel,而二值边缘图像的处理是比较难的
		src,
		storage,
		CV_HOUGH_GRADIENT,
		1,  //累加器图像的分辨率
		1,//圆心间距
		30,//canny
		45,//累加器
		
		30,//小半径
		100//最大半径
		
		);
#endif

	for( int i = 0; i < results->total; i++ )
	{
		float* p = ( float* )cvGetSeqElem( results, i );
		//霍夫圆变换
		CvPoint pt = cvPoint( cvRound( p[0] ), cvRound( p[1] ) );
		cvCircle(
			dst,
			pt,  //确定圆心
			cvRound( p[2] ),  //确定半径
			CV_RGB( 0xff, 0, 0 )
		);  //画圆函数
	}
	cvNamedWindow( "cvHoughCircles", 0 );
	cvShowImage( "cvHoughCircles", dst );
	/*cvWaitKey(10);
		cvWaitKey(0);*/


}


int main(int *argc,char *argv[])
{
	
	TimeCountStart();

	{
		vector<string> paths;
		paths.push_back("D:\\ImageDataBase\\duanjian\\suck_ohaha.jpg");
		paths.push_back("D:\\ImageDataBase\\duanjian\\test.jpg");
		
		for(int i=0;i<paths.size();i++){
			string path_t=paths.at(i);
			ImageProcess::zhangjiagang_hongbao_duanzao(path_t);
			destroyAllWindows();
		}
		

	}

	TimeCountStop("cost time:");
	
	/*double cost_s=dur/CLOCKS_PER_SEC;*/

	return 0;
}