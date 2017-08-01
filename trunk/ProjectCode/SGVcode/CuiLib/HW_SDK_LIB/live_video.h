#ifndef live_video_include_h
#define live_video_include_h




#include "live_video_base.h"

class live_video :public live_video_base
{
public:
	live_video(const char* ip,int slot);
	~live_video();
public:
	static unsigned opencv_show_image_thread(LPVOID lpParam);
	
	bool init_video_writer();

	bool write_xml_pos();	
	
	void  SaveImage_rgb_3();
	void  SaveImage_rgb_4_for_show();
	void  SaveImage();
};

typedef live_video* live_video_ptr;

//typedef boost::shared_ptr<live_video> live_video_ptr;

#endif