//ͼ���Canny��Ե���
//By MoreWindows (http://blog.csdn.net/MoreWindows)
#include <opencv2/opencv.hpp>
using namespace std;


#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
using namespace cv;
using namespace std;

#define	USE_CANNY 1
#define USE_Laplace 0

#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
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
	const char *pstrImageName = "D:\\ImageDataBase\\duanjian\\easy.jpg";
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
	int dist=5;
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

    waitKey(0);
}

int main()
{
	//Laplace_test();
	Hough_test();
	return 0;
}