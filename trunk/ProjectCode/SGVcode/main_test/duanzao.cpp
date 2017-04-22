//图像的Canny边缘检测
//By MoreWindows (http://blog.csdn.net/MoreWindows)
#include <opencv2/opencv.hpp>
using namespace std;
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
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
#if 0
	//canny边缘检测
	cvCanny(g_pSrcImage, g_pCannyImg, threshold, threshold * 3, 3);
	cvSaveImage("ohh.png",g_pCannyImg);
#endif

#if 1
	if(threshold%2==1){
		cvLaplace(g_pSrcImage, g_pCannyImg,threshold);	
		SaveFrame2Disk(g_pCannyImg,threshold,"G:\\IMG_T","");
	}
#endif

	cvShowImage(pstrWindowsCannyTitle, g_pCannyImg);
	

}
int main()
{
	const char *pstrImageName = "E:\\duanjian\\easy.jpg";
	const char *pstrWindowsSrcTitle = "原图(http://blog.csdn.net/MoreWindows)";
	const char *pstrWindowsToolBar = "Threshold";

	//从文件中载入图像的灰度图CV_LOAD_IMAGE_GRAYSCALE - 灰度图
#if 0
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
	return 0;
}