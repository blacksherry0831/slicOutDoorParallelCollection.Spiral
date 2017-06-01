//ͼ���Canny��Ե���
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
const char *pstrWindowsCannyTitle = "��Ե���ͼ(http://blog.csdn.net/MoreWindows)";
//cvCreateTrackbar�Ļص�����

void SaveFrame2Disk(IplImage *image,int count_tmp,char* path,char* add)
{	  
	char tmpfile[1000]={'\0'};  
	
	sprintf(tmpfile,"%s//%d_%s.png",path,count_tmp,add);//ʹ��֡����ΪͼƬ��  
	
	cvSaveImage(tmpfile,image);  
}

void on_trackbar(int threshold)
{
#if USE_CANNY
	//canny��Ե���
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
//������
void Laplace_test()
{
	const char *pstrImageName = "D:\\ImageDataBase\\duanjian\\11_.png";
	const char *pstrWindowsSrcTitle = "ԭͼ(http://blog.csdn.net/MoreWindows)";
	const char *pstrWindowsToolBar = "Threshold";

	//���ļ�������ͼ��ĻҶ�ͼCV_LOAD_IMAGE_GRAYSCALE - �Ҷ�ͼ
#if USE_CANNY
	g_pSrcImage = cvLoadImage(pstrImageName, CV_LOAD_IMAGE_GRAYSCALE);
	g_pCannyImg = cvCreateImage(cvGetSize(g_pSrcImage), IPL_DEPTH_8U, 1);
#else
	g_pSrcImage = cvLoadImage(pstrImageName);

	g_pCannyImg = cvCreateImage(cvGetSize(g_pSrcImage), IPL_DEPTH_8U, g_pSrcImage->nChannels);
#endif


	//��������
	cvNamedWindow(pstrWindowsSrcTitle, CV_WINDOW_AUTOSIZE);
	cvNamedWindow(pstrWindowsCannyTitle, CV_WINDOW_AUTOSIZE);

//����������
	int nThresholdEdge = 1;
	cvCreateTrackbar(pstrWindowsToolBar, pstrWindowsCannyTitle, &nThresholdEdge, 100, on_trackbar);

	//��ָ����������ʾͼ��
	cvShowImage(pstrWindowsSrcTitle, g_pSrcImage);
	on_trackbar(1);

	//�ȴ������¼�
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
    //��˹ģ��ƽ��
	GaussianBlur( gray, gray, Size(9, 9), 2, 2 );

    vector<Vec3f> circles;
    //����任
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


    //��ԭͼ�л���Բ�ĺ�Բ
    for( size_t i = 0; i < circles.size(); i++ )
    {
        //��ȡ��Բ������
        Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        //��ȡ��Բ�뾶
        int radius = cvRound(circles[i][2]);
        //Բ��
        circle( src, center, 3, Scalar(0,255,0), -1, 8, 0 );
        //Բ
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
   
//���и�ʴ���� 
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
    //��˹ģ��ƽ��
	GaussianBlur( gray, gray, Size(9, 9), 2, 2 );

    vector<Vec3f> circles;
    //����任
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


    //��ԭͼ�л���Բ�ĺ�Բ
    for( size_t i = 0; i < circles.size(); i++ )
    {
        //��ȡ��Բ������
        Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        //��ȡ��Բ�뾶
        int radius = cvRound(circles[i][2]);
        //Բ��
        circle( src, center, 3, Scalar(0,255,0), -1, 8, 0 );
        //Բ
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

	results =ImageProcess::cvHoughCircles_Binary(  //cvHoughCircles������Ҫ����ÿһ�������ݶȵķ���
								      //��˻����ڲ��Զ�����cvSobel,����ֵ��Եͼ��Ĵ����ǱȽ��ѵ�
		src,
		storage,
		CV_HOUGH_GRADIENT,
		1,  //�ۼ���ͼ��ķֱ���
		20,//Բ�ļ��
		100,//canny
		36,//�ۼ���
		
		10,//С�뾶
		500//���뾶
		
		);
#endif
	for( int i = 0; i < results->total; i++ )
	{
		float* p = ( float* )cvGetSeqElem( results, i );
		//����Բ�任
		CvPoint pt = cvPoint( cvRound( p[0] ), cvRound( p[1] ) );
		cvCircle(
			dst,
			pt,  //ȷ��Բ��
			cvRound( p[2] ),  //ȷ���뾶
			CV_RGB( 0xff, 0, 0 )
		);  //��Բ����
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

	//cvSmooth( src, dst, CV_GAUSSIAN, 5, 5 );  //����


	results =ImageProcess::cvHoughCircles_Binary(  //cvHoughCircles������Ҫ����ÿһ�������ݶȵķ���
								      //��˻����ڲ��Զ�����cvSobel,����ֵ��Եͼ��Ĵ����ǱȽ��ѵ�
		src,
		storage,
		CV_HOUGH_GRADIENT,
		1,  //�ۼ���ͼ��ķֱ���
		1,//Բ�ļ��
		30,//canny
		61,//�ۼ���
		
		20,//С�뾶
		100//���뾶
		
		);
#endif
#if 1
	IplImage* src = cvLoadImage("D:\\ImageDataBase\\duanjian\\easy.jpg", 0 );
	IplImage* dst = cvLoadImage("D:\\ImageDataBase\\duanjian\\easy.jpg");

	cvSmooth( src, src, CV_GAUSSIAN, 5, 5 );  //����


	results =ImageProcess::cvHoughCircles_Binary(  //cvHoughCircles������Ҫ����ÿһ�������ݶȵķ���
								      //��˻����ڲ��Զ�����cvSobel,����ֵ��Եͼ��Ĵ����ǱȽ��ѵ�
		src,
		storage,
		CV_HOUGH_GRADIENT,
		1,  //�ۼ���ͼ��ķֱ���
		1,//Բ�ļ��
		30,//canny
		45,//�ۼ���
		
		30,//С�뾶
		100//���뾶
		
		);
#else
	IplImage* src = cvLoadImage("D:\\ImageDataBase\\duanjian\\easy.jpg", 0 );
	IplImage* dst = cvLoadImage("D:\\ImageDataBase\\duanjian\\easy.jpg");
	cvSmooth( src, src, CV_GAUSSIAN, 5, 5 );  //����
	results =cvHoughCircles(  //cvHoughCircles������Ҫ����ÿһ�������ݶȵķ���
								      //��˻����ڲ��Զ�����cvSobel,����ֵ��Եͼ��Ĵ����ǱȽ��ѵ�
		src,
		storage,
		CV_HOUGH_GRADIENT,
		1,  //�ۼ���ͼ��ķֱ���
		1,//Բ�ļ��
		30,//canny
		45,//�ۼ���
		
		30,//С�뾶
		100//���뾶
		
		);
#endif

	for( int i = 0; i < results->total; i++ )
	{
		float* p = ( float* )cvGetSeqElem( results, i );
		//����Բ�任
		CvPoint pt = cvPoint( cvRound( p[0] ), cvRound( p[1] ) );
		cvCircle(
			dst,
			pt,  //ȷ��Բ��
			cvRound( p[2] ),  //ȷ���뾶
			CV_RGB( 0xff, 0, 0 )
		);  //��Բ����
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