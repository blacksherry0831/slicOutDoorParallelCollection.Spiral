#ifndef _PUBLIC_H                          // ָ��Ҫ������ƽ̨�� Windows Vista��
#define _PUBLIC_H   //public ��������ͷ�ļ�������ͷ�ļ�����public.h ;ÿ���ļ������Լ���.h


/*************************************************************************ȫ�ֱ���*********************************************************************/
extern HWND global_GGJZ_hwnd;//ȫ�ֱ������������ھ��
//extern  CDC *p_DC_picture;
extern CWnd *p_picture_handle;//ͼ��ؼ����
//extern	C_PAGE_STATE * global_page_state;//״̬ѡ����
//extern CvCapture* gloal_p_capture;
extern bool global_thread_synch;//�����߳�ͬ��--��ȫ�˳�
extern bool global_thread_restart;
extern bool is_thread_run;

extern int  global_cam_index;//cvCaptureFromCAM������
//extern  Store_database  global_record_set;
extern BOOL  global_net_work_is_connect;
/*************************���ݿ����***********************************************************/
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