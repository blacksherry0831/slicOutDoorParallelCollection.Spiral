#include "stdafx.h"

#include "opencv_stl.h"
#include "pt_mutex.h"

#include "opencv_calibration/public.h"
#include "opencv_calibration/calibration.hpp"

#include "MY_SDK_LIB/OpencvCamera.hpp"
#include "HW_SDK_LIB/live_video.h"

void CalCamera(ICamera *camera_t)
{
	calibration cal(camera_t);
	string cmd;

	if (camera_t->is_file_exist(camera_t->IntrinsicName().c_str()) == 0) {
		std::cout << "校准文件 已存在!!" << std::endl;
	}

	std::cout << "please input Cmd :"<<std::endl;

	while (TRUE) {

		
		std::getline(std::cin, cmd);

		if (cmd.compare("start cal") == 0) {
			cal.start(TRUE);
		}
		else if (cmd.compare("stop") == 0) {
			cal.stop();
		}
		else if (cmd.compare("start test cal") == 0) {
			cal.start(0);
		}
		else if (cmd.compare("q") == 0) {
			cal.stop();
			break;
		}
		else {
			std::cout << "you can use this cmd:" << std::endl;
			std::cout << "1>start cal" << std::endl;
			std::cout << "2>start test cal" << std::endl;
			std::cout << "3>stop" << std::endl;
			std::cout << "4>q" << std::endl;


		}
	}

	cal.Join();
}

void opencv_camera()
{
	CalCamera(new OpencvCamera());
}

void howell_camera(string ip)
{
	HW_NET_Init(0);
	std::cout << "Connect Ip:" << ip << std::endl;
	CalCamera(new live_video_base(ip.c_str(), 0));

	HW_NET_Release();
}

int main( int argc, char** argv )
{

	
	int test_num = 0;

	std::cout << "Please Input Test Item:" << std::endl;
	std::cout << "1 Camera Test! " << std::endl;
	std::cout << "2 Howell Camera Test! " << std::endl;

	cin >> test_num;

	if (test_num == 1) {

		opencv_camera();

	}else if (test_num == 2) {
		
		string ip_addr;

		std::cout << "Please Input a IpAddr :" << std::endl;
		
		while (ip_addr=="") {
		std::getline(std::cin, ip_addr);
		}		
		howell_camera(ip_addr);

	}else {

	}

	cin.get();
	return 0;


}
