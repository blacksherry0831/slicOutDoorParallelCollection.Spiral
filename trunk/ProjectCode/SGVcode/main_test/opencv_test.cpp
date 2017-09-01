#include "stdafx.h"


void print_mat(string name_t,cv::Mat mat_t)
{

	std::cout <<name_t<< " = " << std::endl;
	std::cout << mat_t<<std::endl;

}

void capture_frame() {
	char FileName[80];
	memset(FileName, 0, sizeof FileName);
	string avi_1 = "C:\\Users\\cui-e460\\Desktop\\ch3_cal\\ch3.mp4out.avi";
	CvCapture* capture = cvCaptureFromAVI(avi_1.c_str());
	int i = 99;
	while (cvGrabFrame(capture))
	{
		i++;
		if (i % 100 == 0)
		{
			Mat img = cvRetrieveFrame(capture);        // retrieve the captured frame
			
			sprintf(FileName, "C:\\Users\\cui-e460\\Desktop\\ch3_cal\\002\\%d.png", i);
			imwrite(FileName, img);

		}
	}
	cvReleaseCapture(&capture);
}

void  inv_test()
{
	const int COLS = 1000;
	cv::Mat m1 = cv::Mat::eye(COLS, COLS, CV_32F);	

	m1.at<float>(0, 0) = 0.9;
	m1.at<float>(COLS - 1, COLS - 1) = 0.9;


	if(COLS<11) print_mat("m",m1);

	TimeCountStart();
			cv::Mat m_inv = m1.inv();

			
	TimeCountStop("inv cost time :");

	if(COLS<11) print_mat("m_inv",m_inv);

}


int main( int argc, char** argv )
{
#if 0
	inv_test();
#endif // 0



	capture_frame();

    return 0;
}
