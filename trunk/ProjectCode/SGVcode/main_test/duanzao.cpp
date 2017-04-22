//ͼ���Canny��Ե���
//By MoreWindows (http://blog.csdn.net/MoreWindows)
#include <opencv2/opencv.hpp>
using namespace std;
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
#if 0
	//canny��Ե���
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
	const char *pstrWindowsSrcTitle = "ԭͼ(http://blog.csdn.net/MoreWindows)";
	const char *pstrWindowsToolBar = "Threshold";

	//���ļ�������ͼ��ĻҶ�ͼCV_LOAD_IMAGE_GRAYSCALE - �Ҷ�ͼ
#if 0
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
	return 0;
}