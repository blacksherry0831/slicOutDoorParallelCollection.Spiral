#ifndef _PUBLIC_H                          // 指定要求的最低平台是 Windows Vista。
#define _PUBLIC_H   //public 包含所有头文件，所有头文件包含public.h ;每个文件包含自己的.h


/*************************************************************************全局变量*********************************************************************/
extern HWND global_GGJZ_hwnd;//全局变量保存主窗口句柄
//extern  CDC *p_DC_picture;
extern CWnd *p_picture_handle;//图像控件句柄
//extern	C_PAGE_STATE * global_page_state;//状态选项卡句柄
//extern CvCapture* gloal_p_capture;
extern bool global_thread_synch;//用于线程同步--安全退出
extern bool global_thread_restart;
extern bool is_thread_run;

extern int  global_cam_index;//cvCaptureFromCAM的索引
//extern  Store_database  global_record_set;
extern BOOL  global_net_work_is_connect;
/*************************数据库相关***********************************************************/
/************************************************************************************/
//extern DWORD WINAPI threadFunc_ctl(LPVOID thread_date_t);
extern unsigned int __stdcall threadFunc_ctl(void* t);
extern void * thread_opencv_calibration(void *);
extern void StopCalibrationThread();
/************************************************************************************/
class GlobalStatic{
public:
	
	static  int  board_w;	
	static  int  board_h;
	static  int  black;
    static  int  num_board_is_use;
	static  int  camera_id;
	static  int  per_frame;
	static  int  CAL_ROUTINE;
};

#endif