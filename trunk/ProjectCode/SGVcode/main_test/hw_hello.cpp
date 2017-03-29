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
	live_video_ptr g_cur_video=NULL;
	BOOL login_success_t=FALSE;
	int save_num_t=0;
	BOOL is_save_t=false;

	g_cur_video = live_video_ptr(new live_video(ipc_p_t->ip,0));

	printf("connect to %s \n",ipc_p_t->ip);
	do{
		login_success_t=g_cur_video->play(NULL);
		if (login_success_t==FALSE){
			Sleep(0);
		}
	} while (login_success_t==FALSE);



#if 1

	int width=0,height=0;
	BOOL success_t;
	do 
	{
		success_t=g_cur_video->get_video_size(&width,&height);

	} while (success_t==FALSE);
	printf("Width: %d,Height:%d \n",width,height);
#endif
	
#if 1
	g_cur_video->print_yuv(TRUE);

	do 
	{
		Sleep(1000);

	}while (g_cur_video->is_play());

#endif



#if 0
do{


		std::string ip_addr_t=ipc_p_t->ip;
		long time=clock();
		std::string filename_t=ip_addr_t+"_"+ltos(time)+"hello.jpg";
		
	
		

		while(!is_save_t){

			char buffer[1024];
			clock_t start=clock();
			long time=start;

			
#if 0
			is_save_t=g_cur_video->save_to_jpg(filename_t.c_str(),100);
			//is_save_t=g_cur_video->snap2jpg();
#else
	
#endif

#if 0
			long error_t=HW_NET_GetLastError();

			if (error_t!=0){
				printf("Error Code: %d",error_t);
			}
#endif

			if (is_save_t){
				printf("±£´æ³É¹¦£º%d \n",save_num_t);
				Sleep(1000);
			}else{
				Sleep(0);
			}


		}
#if READ_FILE_MAX
		is_save_t=FALSE;
#endif

	} while (READ_FILE_MAX);
#endif
	
	if(g_cur_video){
		g_cur_video->close();
	}

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

int main()
{
	
	TimeCountStart();

	HW_NET_Init(0);

	
	std::vector<ipc_ptr> g_ipcs;
	std::vector<string>  g_ips;

	/*g_ips.push_back("192.168.3.1");*/
g_ips.push_back("192.168.3.2");
		//g_ips.push_back("192.168.3.22");
	/*g_ips.push_back("192.168.3.3");*/
	/*g_ips.push_back("192.168.3.4");
	
	g_ips.push_back("192.168.3.5");
	g_ips.push_back("192.168.3.6");
	g_ips.push_back("192.168.3.7");*/

	
	for (int i=0;i<g_ips.size();i++){
		
		get_ipcs_from_name(g_ips.at(i).c_str(),&g_ipcs);
#if 0
		cvNamedWindow(g_ips.at(i).c_str(),0);
		cvResizeWindow(g_ips.at(i).c_str(),480,270);
#endif
	}

#if _MSC_VER

	vector<HANDLE> handle;
	for (int i=0;i<g_ipcs.size();i++){

		HANDLE handle_t=::CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)THreadSuperPixel_DoOneImage_win,g_ipcs.at(i),0,NULL); 
	    handle.push_back(handle_t);
	
	}
	
	for(int i=0;i<handle.size();i++){
		WaitForSingleObject(handle.at(i),INFINITE);
	}

#endif

#if TRUE
	for (int i=0;i<g_ipcs.size();i++){

		delete g_ipcs.at(i);
	}
#endif

	HW_NET_Release();


	TimeCountStop("Threads Done : ");

	return 0;

}
/*----------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------*/