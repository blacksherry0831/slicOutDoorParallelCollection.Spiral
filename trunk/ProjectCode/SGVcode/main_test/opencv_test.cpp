#include "stdafx.h"


void print_mat(string name_t,cv::Mat mat_t)
{

	std::cout <<name_t<< " = " << std::endl;
	std::cout << mat_t<<std::endl;

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

	inv_test();



    return 0;
}
