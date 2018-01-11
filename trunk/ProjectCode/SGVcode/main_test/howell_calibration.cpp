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
	std::cout << "3 Cal Video ! " << std::endl;

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

	}else if (test_num==3) {

#if 0
		string file_in;
		string file_out;
		string cal_file;
		
		std::cout << "Please Input In File :" << std::endl;
		while (file_in == "") {
			std::getline(std::cin, file_in);
		}
		
		file_out = file_in+"out.avi";

		std::cout << "Please Input Cal File:" << std::endl;
		while (cal_file == "") {
			std::getline(std::cin, cal_file);
		}
		calibration::opencv_cal_video(file_in, file_out, cal_file);
#else

		string file_base="";
		std::vector<string>  files_t;
		

		std::cout<< "please input video path !" << std::endl;

		while (file_base == "") {
			std::getline(std::cin, file_base);
		}

		Base::FS_getFiles(file_base, "mp4", files_t);
		
		std::cout << "Total Video to Calibration : "<<files_t.size() << std::endl;
		

		size_t length = files_t.size();
		for (size_t i = 0; i < length; i++)
		{
			calibration::opencv_cal_video(files_t[i],
											files_t[i]+ ".cal.avi",
											Base::CRACK_FILR_NAME_get_ipAddr(files_t[i]));
		}

#endif

	}else {


	}

	std::cout << "Please input \"Enter\"  to Exit" << std::endl;
	cin.get();
	return 0;


}
