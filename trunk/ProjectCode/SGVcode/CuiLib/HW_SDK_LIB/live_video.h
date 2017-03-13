#ifndef live_video_include_h
#define live_video_include_h

#include <string>
#include "howellnetsdk.h"
#include "play_def.h"
#pragma comment(lib,"hwclient.lib")
#pragma comment(lib,"play_sdk.lib")

#include "cv.h"
#include "highgui.h"
using namespace cv;
//#include <boost/shared_ptr.hpp>

#define SAVE_IMAGE TRUE
#define SAVE_VIDEO TRUE
#define SHOW_IMAGE TRUE

class live_video
{
public:
	live_video(const char* ip,int slot);
	~live_video();

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

	static unsigned opencv_show_image_thread(LPVOID lpParam);
	
	bool is_play();
	bool init_video_writer();

	static void string_replace(string&s1,const string s2,const string s3);

	string get_directory();
	static string get_time_stamp();
	
private:
	std::string m_ip;
	int m_slot;
	HWND m_hwnd;
	USER_HANDLE m_uh;
	REAL_HANDLE m_sh;
	PLAY_HANDLE m_ph;
	bool m_is_playing;
public:
	int m_width;
	int m_height;
public: 
	long m_stream_len;/**<buffer size*/
	long m_stream_count;/**<*/
	long m_stream_ready;/**<*/
	long m_stream_frame_count;/**<*/
	long m_save_image_switch;
	bool m_save_video_switch;
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


	CvVideoWriter *m_writer;


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

};

typedef live_video* live_video_ptr;

//typedef boost::shared_ptr<live_video> live_video_ptr;

#endif