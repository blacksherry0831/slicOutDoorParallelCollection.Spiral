#ifndef live_video_base_include_h
#define live_video_base_include_h


#ifdef _MSC_VER
#include "afxmt.h"
#endif

#include "pt_mutex.h"

#if TRUE
#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>
using namespace std;
#endif


#if _MSC_VER
#include "howellnetsdk.h"
#include "play_def.h"
#pragma comment(lib,"hwclient.lib")
#pragma comment(lib,"play_sdk.lib")
#endif

#include "cv.h"
#include "highgui.h"


#if TRUE
#include <tinyxml2/tinyxml2.h>
using namespace tinyxml2;
#endif

#include "MY_SDK_LIB/ICamera.hpp"

#define SAVE_IMAGE TRUE
#define SAVE_VIDEO TRUE
#define SHOW_IMAGE TRUE
#define	FPS_MY (25)

#define Y_R_IN			0.257
#define Y_G_IN			0.504
#define Y_B_IN			0.098
#define Y_ADD_IN		16

#define U_R_IN			0.148
#define U_G_IN			0.291
#define U_B_IN			0.439
#define U_ADD_IN		128

#define V_R_IN			0.439
#define V_G_IN			0.368
#define V_B_IN			0.071
#define V_ADD_IN		128

#define SCALEBITS_IN	8
#define FIX_IN(x)		((uint16_t) ((x) * (1L<<SCALEBITS_IN) + 0.5))


#define RGB_Y_OUT		1.164
#define B_U_OUT			2.018
#define Y_ADD_OUT		16

#define G_U_OUT			0.391
#define G_V_OUT			0.813
#define U_ADD_OUT		128

#define R_V_OUT			1.596
#define V_ADD_OUT		128


#define SCALEBITS_OUT	13
#define FIX_OUT(x)		((unsigned short) ((x) * (1L<<SCALEBITS_OUT) + 0.5))

#ifndef MIN
#define MIN(A,B)	((A)<(B)?(A):(B))
#endif

#ifndef MAX
#define MAX(A,B)	((A)>(B)?(A):(B))
#endif


class live_video_base :public ICamera
{
public:
	bool init();
	bool release();
	IplImage* QueryFrame();
	string IntrinsicName();
	string DistortionName();
public:
	live_video_base(const char* ip,int slot);
	~live_video_base();

	bool play(HWND hwnd);
	void close();

	bool zoom_rect(bool enable,RECT* src,RECT* dst);
	bool get_video_size(int* w,int * h);
	bool register_draw(draw_callback* fun,long user);
	bool save_to_jpg(const char* file,int quality);
	bool snap2jpg();
	bool save_to_bmp(const char* bmp);
	bool print_yuv(bool isYuv);
	bool wait_for_frame();
	static std::string ltos(long l);

	void turn_up();
	void turn_down();
	void turn_left();
	void turn_right();
	void turn_stop();
public:
	bool hw_login();
public:
	bool start_show_image_window_thread(int show);
	static unsigned opencv_show_image_thread(LPVOID lpParam);
	
	bool is_play();
	bool init_video_writer();

	bool init_xml_pos();
	bool write_xml_pos();
	
	static void string_replace(string&s1,const string s2,const string s3);

	string get_directory();
	static string get_time_stamp();
	
	void  SaveImage_rgb_3();
	void  SaveImage_rgb_4_for_show();
	void  SaveImage();
	void  SaveVideo();
protected:
	std::string m_ip;
	int m_slot;
	HWND m_hwnd;
	USER_HANDLE m_uh;
	REAL_HANDLE m_sh;
	PLAY_HANDLE m_ph;
	bool m_is_playing;
public:
	bool m_is_draw_spiral;
public:
	int m_width;
	int m_height;
public: 
	long m_stream_len;/**<buffer size*/
	long m_stream_count;/**<*/
	long m_stream_ready;/**<*/
	long m_stream_frame_count;/**<*/
	long m_save_image_switch;
	static bool m_save_video_switch;
	bool m_save_video_start;
public:
	clock_t m_frame_start;
public:
	clock_t m_frame_end;
public:

	IplImage * m_img_rgb_3;
#ifdef SHOW_IMAGE
#if SHOW_IMAGE
	IplImage * m_img_rgb_4_for_show;
#endif
#endif

	int m_video_frame_count;

	CvVideoWriter *m_writer;
	CvVideoWriter *m_writer_spiral;
	tinyxml2::XMLDocument m_video_pos;

private:
	static void CALLBACK on_stream(long lVideoID,char*buf,int len,int videoType,long nUser);
	static void CALLBACK on_yuv_ex(PLAY_HANDLE handle,
		const unsigned char* y,
		const unsigned char* u,
		const unsigned char* v,
		int y_stride,
		int uv_stride,
		int width,
		int height,
		INT64 time,
		long user);
protected:


	ThreadMutex m_ImgLock;


protected:
	static int RGB_Y_tab[256];
	static int B_U_tab[256];
	static int G_U_tab[256];
	static int G_V_tab[256];
	static int R_V_tab[256];
public:
	static void colorspace_init(void);
	static void yv12_to_rgb24_c(unsigned char * dst,
		int dst_stride,
		unsigned char * y_src,
		unsigned char * u_src,
		unsigned char * v_src,
		int y_stride,
		int uv_stride,
		int width,
		int height);
	static void rgb24_to_rgb8(unsigned char * rgb_buf, int w, int h, char* dst_r, char* dst_g, char* dst_b);

};

typedef live_video_base* live_video_base_ptr;

//typedef boost::shared_ptr<live_video> live_video_ptr;

#endif