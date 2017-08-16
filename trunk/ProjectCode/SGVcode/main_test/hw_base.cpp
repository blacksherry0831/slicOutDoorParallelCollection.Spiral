#include <stdio.h>

#if 1
#include "cv.h"
using namespace cv;
#endif

#include "opencv_stl.h"

#if 1
#include "HW_SDK_LIB/ipc.h"
#include "HW_SDK_LIB/live_set.h"
#include "HW_SDK_LIB/live_video.h"
#include "HW_SDK_LIB/dc_save.h"
#include "HW_SDK_LIB/hw_nvr.h"
#endif

#ifndef  SHOW_IMAGE
#define SHOW_IMAGE FALSE
#endif


/*-------------------------------------------------------------------------*/
/**
*
*/
/*-------------------------------------------------------------------------*/
string ltos(long l)  
{  
	ostringstream os;  
	os<<l;  
	string result;  
	istringstream is(os.str());  
	is>>result;  
	return result;  

}
/*-------------------------------------------------------------------------*/
/**
*
*/
/*-------------------------------------------------------------------------*/
unsigned THreadSuperPixel_DoOneImage_win(LPVOID lpParam)
{
	ipc_ptr ipc_p_t=(ipc_ptr) lpParam;
	
	BOOL login_success_t=FALSE;
	int save_num_t=0;
	BOOL is_save_t=false;

	live_video_base_ptr g_cur_video =live_video_base_ptr(new live_video_base(ipc_p_t->ip, 0));

	printf("connect to %s \n",ipc_p_t->ip);
	
	
	g_cur_video->init();
	
	g_cur_video->start_show_image_window_thread(1);

#if 1

	do 
	{
		Sleep(1000);

	}while (g_cur_video->is_play());

#endif

	g_cur_video->release();

	delete g_cur_video;
	
#if __GUNC__||linux||__linux||__linux__

	

#endif
	return 0;
}
/*-------------------------------------------------------------------------*/
/**
*
*/
/*-------------------------------------------------------------------------*/
void ip_camera_test()
{
	TimeCountStart();

	HW_NET_Init(0);


	std::vector<ipc_ptr> g_ipcs;
	std::vector<string>  g_ips;

	
	g_ips.push_back("192.168.9.6");
#if 0
	g_ips.push_back("192.168.9.7");
	g_ips.push_back("192.168.3.22");
	g_ips.push_back("192.168.3.2");

	g_ips.push_back("192.168.24.102");
	g_ips.push_back("192.168.3.22");

	g_ips.push_back("192.168.3.3");
	g_ips.push_back("192.168.3.4");
	g_ips.push_back("192.168.3.5");
	g_ips.push_back("192.168.3.6");
	g_ips.push_back("192.168.3.7");
#endif // 0




	for (int i = 0; i<g_ips.size(); i++) {

		get_ipcs_from_name(g_ips.at(i).c_str(), &g_ipcs);

	}

#if _MSC_VER

	vector<HANDLE> handle;
	for (int i = 0; i<g_ipcs.size(); i++) {

		HANDLE handle_t = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)THreadSuperPixel_DoOneImage_win, g_ipcs.at(i), 0, NULL);
		handle.push_back(handle_t);

	}

	for (int i = 0; i<handle.size(); i++) {
		WaitForSingleObject(handle.at(i), INFINITE);
	}

#endif

#if TRUE
	for (int i = 0; i<g_ipcs.size(); i++) {

		delete g_ipcs.at(i);
	}
#endif

	HW_NET_Release();


	TimeCountStop("Threads Done : ");
}
/*-------------------------------------------------------------------------*/
/**
*
*/
/*-------------------------------------------------------------------------*/
void network_video_recorder()
{

	HW_NET_Init(0);

	
	std::auto_ptr<hw_nvr> g_hw_nvr(new hw_nvr("192.168.9.200", 0));

	string cmd;

	while (TRUE) {
		getline(std::cin, cmd);

		if (cmd.compare("login")==0) {
			g_hw_nvr->hw_login();
		}else if (cmd.compare("channel")==0) {
			g_hw_nvr->GetChannelStatus();
		}else if (cmd.compare("status")==0) {
			g_hw_nvr->status();
		}else if (cmd.compare("start record")==0) {		
			g_hw_nvr->StartAllRecoed();		
		}else if (cmd.compare("stop record")==0) {
			g_hw_nvr->StopAllRecord();
		}else if (cmd.compare("set manual") == 0) {
			g_hw_nvr->SetManual();
		}else if (cmd.compare("reboot") == 0) {
			g_hw_nvr->Reboot();			
		}else if (cmd.compare("hard disk") == 0) {
			g_hw_nvr->GetHarddiskState();
		}else if (cmd.compare("format disk") == 0) {
			g_hw_nvr->FormatDisk();
		}else if (cmd.compare("q")==0) {
			g_hw_nvr->StopAllRecord();
			break;
		}else {
			std::cout << "you can use this cmd:" << std::endl;
			std::cout << "login" << std::endl;
			std::cout << "status" << std::endl;
			std::cout << "start record" << std::endl;
			std::cout << "stop record" << std::endl;
			std::cout << "set manual" << std::endl;
			std::cout << "reboot" << std::endl;
			std::cout << "hard disk" << std::endl;
			std::cout << "format disk" << std::endl;
			std::cout << "q" << std::endl;
			std::cout << "please input a cmd :" << std::endl;

		}

	
	}




	HW_NET_Release();
}
/*-------------------------------------------------------------------------*/
/**
*
*/
/*-------------------------------------------------------------------------*/
int main()
{

	int test_num = 0;

	std::cout << "Please Input Test Item:" << std::endl;
	std::cout << "1 IP Camera Test! " << std::endl;
	std::cout << "2 Network Video Recorder Test! " << std::endl;
	
	cin >> test_num;

	if (test_num == 1) {
		
		ip_camera_test();

	}else if (test_num == 2) {		

		network_video_recorder();

	}else {

	}

	cin.get();
	return 0;

}
/*----------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------*/