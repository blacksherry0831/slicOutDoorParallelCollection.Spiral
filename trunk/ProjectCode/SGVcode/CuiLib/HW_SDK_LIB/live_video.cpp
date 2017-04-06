#include "stdafx.h"
#include "live_video.h"
#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>
using namespace std;
//#include "Convert.h"
#include "modules.h"

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




int RGB_Y_tab[256];
int B_U_tab[256];
int G_U_tab[256];
int G_V_tab[256];
int R_V_tab[256];

//#include "cv.h"
//#include "highgui.h"
//using namespace cv;



void colorspace_init(void)
{
	int i;

	for (i = 0; i < 256; i++) {
		RGB_Y_tab[i] = FIX_OUT(RGB_Y_OUT) * (i - Y_ADD_OUT);
		B_U_tab[i] = FIX_OUT(B_U_OUT) * (i - U_ADD_OUT);
		G_U_tab[i] = FIX_OUT(G_U_OUT) * (i - U_ADD_OUT);
		G_V_tab[i] = FIX_OUT(G_V_OUT) * (i - V_ADD_OUT);
		R_V_tab[i] = FIX_OUT(R_V_OUT) * (i - V_ADD_OUT);
	}
}

live_video::live_video(const char* ip,int slot)
	:m_ip(ip),m_slot(slot),m_uh(-1),m_sh(-1),m_ph(INVALID_HANDLE),m_hwnd(0)
	,m_is_playing(false)
{
	colorspace_init();
	m_stream_len=0;
	m_stream_count=0;
	m_stream_ready=0;
	m_stream_frame_count=0;
	m_save_image_switch=0;
	m_save_video_switch=false;
	
	m_img_rgb_3=NULL;

#ifdef SHOW_IMAGE
#if SHOW_IMAGE
	m_img_rgb_4_for_show=NULL;
#endif
#endif
	m_frame_start=clock();
//	CvVideoWriter *writer = 0;
//int isColor = 1;
//int fps     = 25;  // or 30
//int frameW  = 640; // 744 for firewire cameras
//int frameH  = 480; // 480 for firewire cameras
//writer=cvCreateVideoWriter("out.avi",CV_FOURCC('P','I','M','1'),
//                           fps,cvSize(frameW,frameH),isColor);
	m_width=0;
	m_height=0;
	m_writer=NULL;
	m_writer_spiral=NULL;
	
	this->m_video_frame_count=0;

	this->get_directory();

}

live_video::~live_video()
{
	close();
	cvReleaseImage(&m_img_rgb_3);
#ifdef SHOW_IMAGE
#if SHOW_IMAGE
	cvReleaseImage(&m_img_rgb_4_for_show);
#endif
#endif
	cvReleaseVideoWriter(&m_writer);
	cvReleaseVideoWriter(&m_writer_spiral);

}





void yv12_to_rgb24_c(unsigned char * dst,
	int dst_stride,
	unsigned char * y_src,
	unsigned char * u_src,
	unsigned char * v_src,
	int y_stride,
	int uv_stride,
	int width,
	int height)
{
	const unsigned short dst_dif = 6 * dst_stride - 3 * width;
	int y_dif = 2 * y_stride - width;

	unsigned char *dst2 = dst + 3 * dst_stride;
	unsigned char *y_src2 = y_src + y_stride;
	unsigned int x, y;

	if (height < 0) {			/* flip image? */
		height = -height;
		y_src += (height - 1) * y_stride;
		y_src2 = y_src - y_stride;
		u_src += (height / 2 - 1) * uv_stride;
		v_src += (height / 2 - 1) * uv_stride;
		y_dif = -width - 2 * y_stride;
		uv_stride = -uv_stride;
	}

	for (y = height / 2; y; y--) {
		/* process one 2x2 block per iteration */
		for (x = 0; x < (unsigned int) width / 2; x++) {
			int u, v;
			int b_u, g_uv, r_v, rgb_y;
			int r, g, b;

			u = u_src[x];
			v = v_src[x];

			b_u = B_U_tab[u];
			g_uv = G_U_tab[u] + G_V_tab[v];
			r_v = R_V_tab[v];

			rgb_y = RGB_Y_tab[*y_src];
			b = (rgb_y + b_u) >> SCALEBITS_OUT;
			g = (rgb_y - g_uv) >> SCALEBITS_OUT;
			r = (rgb_y + r_v) >> SCALEBITS_OUT;
			dst[0] = MAX(0, MIN(255, b));
			dst[1] = MAX(0, MIN(255, g));
			dst[2] = MAX(0, MIN(255, r));

			y_src++;
			rgb_y = RGB_Y_tab[*y_src];
			b = (rgb_y + b_u) >> SCALEBITS_OUT;
			g = (rgb_y - g_uv) >> SCALEBITS_OUT;
			r = (rgb_y + r_v) >> SCALEBITS_OUT;
			dst[3] = MAX(0, MIN(255, b));
			dst[4] = MAX(0, MIN(255, g));
			dst[5] = MAX(0, MIN(255, r));
			y_src++;

			rgb_y = RGB_Y_tab[*y_src2];
			b = (rgb_y + b_u) >> SCALEBITS_OUT;
			g = (rgb_y - g_uv) >> SCALEBITS_OUT;
			r = (rgb_y + r_v) >> SCALEBITS_OUT;
			dst2[0] = MAX(0, MIN(255, b));
			dst2[1] = MAX(0, MIN(255, g));
			dst2[2] = MAX(0, MIN(255, r));
			y_src2++;

			rgb_y = RGB_Y_tab[*y_src2];
			b = (rgb_y + b_u) >> SCALEBITS_OUT;
			g = (rgb_y - g_uv) >> SCALEBITS_OUT;
			r = (rgb_y + r_v) >> SCALEBITS_OUT;
			dst2[3] = MAX(0, MIN(255, b));
			dst2[4] = MAX(0, MIN(255, g));
			dst2[5] = MAX(0, MIN(255, r));
			y_src2++;

			dst += 6;
			dst2 += 6;
		}

		dst += dst_dif;
		dst2 += dst_dif;

		y_src += y_dif;
		y_src2 += y_dif;

		u_src += uv_stride;
		v_src += uv_stride;
	}
}

void rgb24_to_rgb8(unsigned char * rgb_buf,int w,int h,char* dst_r,char* dst_g,char* dst_b){
	for(int i = 0; i < h; i++)
	{
		for(int j = 0; j < w ; j++)
		{
			//fwrite((char*)&rgb24[i * w * 3] + j * 3 + type ,1,1,ou);
			dst_r[i * w + j] = rgb_buf[i * w * 3 + j * 3 + 0];
			dst_g[i * w + j] = rgb_buf[i * w * 3 + j * 3 + 1];
			dst_b[i * w + j] = rgb_buf[i * w * 3 + j * 3 + 2];
		}		
	}
}

void CALLBACK live_video::on_stream(long lVideoID,char*buf,int len,int videoType,long nUser)
{
	live_video* lv = (live_video*)nUser;
	if(len != 40)
	{
		hwplay_input_data(lv->m_ph,buf,len);
#if 1
		if (videoType==4){

				lv->m_stream_len+=len;
				lv->m_stream_count++;

				if (lv->m_stream_len>6*1024&&lv->m_stream_ready==FALSE){
				
				if (lv->m_stream_ready==FALSE){
					printf("Rcv Data: %d ,Rcv Time: %d,Type: %d \n",lv->m_stream_len,lv->m_stream_count,videoType);
				}
					lv->m_stream_ready=TRUE;
					
				}

			
		}
#endif
	}



}

void CALLBACK live_video::on_yuv_ex(PLAY_HANDLE handle, const unsigned char* y, const unsigned char* u, const unsigned char* v, int y_stride, int uv_stride, int width, int height, INT64 time, long user)
{
	live_video* lv = (live_video*)user;
	IplImage* ipl_img=lv->m_img_rgb_3;
	unsigned char *image_buffer=(unsigned char *)ipl_img->imageData;
#if _DEBUG
	ASSERT(ipl_img->imageSize==width*height*3);
	ASSERT(ipl_img->width*3==ipl_img->widthStep);
#endif
#if 0
	if (user==1){
		TRACE("width=%d   height=%d   time=%ld   y=%02x     u=%02x    v=%02x\n",width,height,time,y[0],u[0],v[0]);
	}else{
		char *r = (char *)malloc(width*height);
		char *g =(char *)malloc(width*height);
		char *b =(char *)malloc(width*height);
		//convert::convert_yuv420_to_rgb8(y,u,v,y_stride,uv_stride,width,height,r,g,b);
		//yuv to rgb 
		unsigned char *image_buffer = new unsigned char[width * height * 3];	
		yv12_to_rgb24_c(image_buffer,width,(unsigned char*)y,(unsigned char*)u,(unsigned char*)v,y_stride,uv_stride,width,height);
		rgb24_to_rgb8(image_buffer,width,height,r,g,b);

		TRACE("width=%d   height=%d   time=%ld   image_buffer=%02x      r=%02x     g=%02x    b=%02x\n",width,height,time,image_buffer[0],r[0],g[0],b[0]);
		//for (int i=0;i<10;i++)
		//{
		//	TRACE("y=%02x image_buffer=%02x, ",y[i],image_buffer[i]);
		//}
		//TRACE("\n");
		free(image_buffer);
		free(r);
		free(g);
		free(b);
	}
#endif	
	/*static ImageData MemData(lv->m_img_rgb_4_for_show,
			"",
			1000,
			0,
			0.5);*/
	
	if (lv->m_stream_frame_count++==0)
	{
		lv->m_frame_start=clock();
		
		/*MemData.GetThetaMLXYSeeds_ByCircle_UseSpiral();*/
	}else{
		lv->m_frame_end=clock();
	}	
	
	/*MemData.Draw_Kseeds_Spiral(lv->m_img_rgb_4_for_show);*/
						
	if (lv->m_stream_frame_count%10==0)
	{
		double dur_ms=1.0*(lv->m_frame_end-lv->m_frame_start)/CLOCKS_PER_SEC*1000;
		double frame_snap_t=dur_ms/lv->m_stream_frame_count;
		printf("Frame From IP :%s , Frame Snap %0.2f (ms) \n",lv->m_ip.c_str(),frame_snap_t);
	}


		if (lv->m_stream_frame_count%1==0){
			
			{
				lv->m_ImgLock.Lock();
				yv12_to_rgb24_c(image_buffer,width,(unsigned char*)y,(unsigned char*)u,(unsigned char*)v,y_stride,uv_stride,width,height);
				lv->m_ImgLock.Unlock();
				//cvCvtColor(lv->m_img_rgb_3,lv->m_img_rgb_4_for_show,CV_BGR2BGRA);
			}	
		}

	
}
/**
*
*
*/
bool live_video::init_video_writer()
{				
		const int fps     = 5;
		//static int frame_count=0;
		static string time_stamp_t;
		/*-----------------------------------------------------*/	
		static string path_video_org;
		static string path_video_spiral;
		static string path_video_position;
		/*-----------------------------------------------------*/
		if(this->m_width+this->m_height==0) return false;

		/*-----------------------------------------------------*/
		if(m_writer!=NULL&&time_stamp_t!=""){
			if(this->m_save_video_start==false){
				return true;
			}else{
				cvReleaseVideoWriter(&m_writer);
				m_writer=NULL;
				cvReleaseVideoWriter(&m_writer_spiral);
				m_writer_spiral=NULL;

				m_video_frame_count=0;
				this->m_video_pos.SaveFile(path_video_position.c_str());
			}
		}
	/*-----------------------------------------------------*/
		time_stamp_t=get_time_stamp();
		path_video_position=this->get_directory()+"//"+this->m_ip+"$"+time_stamp_t+"$"+".xml";
		path_video_org=this->get_directory()+"//"+ this->m_ip+"$"+time_stamp_t+"$"+".avi";
		path_video_spiral=this->get_directory()+"//"+ this->m_ip+"$"+time_stamp_t+"$"+"spiral$.avi";

		  {
				//
				m_writer=cvCreateVideoWriter(path_video_org.c_str(),
					CV_FOURCC_DEFAULT,
					fps,
					cvSize(m_width,m_height),
					1);

				m_writer_spiral=cvCreateVideoWriter(path_video_spiral.c_str(),
					CV_FOURCC_DEFAULT,
					fps,
					cvSize(m_width,m_height),
					1);
				
				path_video_position=this->get_directory()+"//"+this->m_ip+"$"+time_stamp_t+"$"+".xml";
			
				this->init_xml_pos();

				this->m_save_video_start=false;
				ASSERT(m_writer!=NULL);
				//
			}
	  /*-----------------------------------------------------*/
	

	return false;
}
/**
*
*
*/
bool live_video::play(HWND hwnd)
{
	if(m_is_playing)
	{
		return false;
	}

	m_uh = HW_NET_Login(m_ip.c_str(),5198,"admin","12345");
	if(m_uh < 0)
	{
		printf("login error s% !\n",m_ip.c_str());
		return false;
	}


	BOOL iframe_sucess_t=FALSE;
	do 
	{
		iframe_sucess_t=HW_NET_SET_ForceIFrame(m_uh,m_slot);
		if (iframe_sucess_t==FALSE)
		{
				Sleep(0);
		}

	} while (iframe_sucess_t==FALSE);
		


	m_sh = HW_NET_OpenVideoEx(m_uh,m_slot,on_stream,(long)this);
	if(m_sh < 0)
	{
		printf("open video error s% !\n",m_ip.c_str());
		goto ONERR;
	}

	char head[100];
	int head_len = 0;
	if(HW_NET_GetVideoHead(m_sh,head,100,&head_len) == FALSE)
	{
		printf("Get Video Head error s% !\n",m_ip.c_str());
		goto ONERR;
	}

	m_ph = hwplay_open_stream(head,head_len,1*1024 * 1024,PLAY_LIVE);
	
	if(m_ph == INVALID_HANDLE)
	{
		printf("open stream error s% !\n",m_ip.c_str());
		goto ONERR;
	}

	m_hwnd = hwnd;
	hwplay_play(m_ph,hwnd);

	m_is_playing = true;

	return true;

ONERR:

	//printf("",);

	if(m_ph != INVALID_HANDLE)
	{
		hwplay_stop(m_ph);
		m_ph = INVALID_HANDLE;
	}

	if(m_sh != -1)
	{
		HW_NET_CloseVideo(m_sh);
		m_sh = -1;
	}

	if(m_uh != -1)
	{
		HW_NET_Logout(m_uh);
		m_uh = -1;
	}

	return false;
}

void live_video::close()
{
	if(!m_is_playing)
	{
		return;
	}
	
	hwplay_stop(m_ph);
	HW_NET_CloseVideo(m_sh);
	HW_NET_Logout(m_uh);

	m_ph = INVALID_HANDLE;
	m_sh = -1;
	m_uh = -1;

	m_is_playing = false;
}

bool live_video::zoom_rect(bool enable,RECT* src,RECT* dst)
{
	if(!m_is_playing)
	{
		return false;
	}

	return hwplay_zoom_rect(m_ph,enable ? TRUE : FALSE,dst,src) ? true : false;
}

bool live_video::get_video_size(int* w,int * h)
{
	if(!m_is_playing)
	{
		return false;
	}

	this->wait_for_frame();

	BOOL return_value_t=hwplay_get_video_size(m_ph,w,h) ? true : false;;

	if (*w!=0&&*h!=0){
		this->m_width=*w;
		this->m_height=*h;
		if (m_img_rgb_3!=NULL){
			cvReleaseImage(&m_img_rgb_3);		
		}
		m_img_rgb_3=cvCreateImage(cvSize(*w,*h),IPL_DEPTH_8U,3);

#ifdef SHOW_IMAGE
#if SHOW_IMAGE
		 m_img_rgb_4_for_show=cvCreateImage(cvSize(*w,*h),IPL_DEPTH_8U,4);
#endif
#endif

		printf("Create Image\n");
				
		this->init_video_writer();
		
	}


	if (this->m_is_playing){
#if _MSC_VER
		HANDLE handle_t=::CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)(opencv_show_image_thread),this,0,NULL);
#endif
	}



	return return_value_t;
}

bool live_video::register_draw(draw_callback* fun,long user)
{
	if(!m_is_playing)
	{
		return false;
	}

	return hwplay_register_draw_fun(m_ph,fun,user) ? true : false;
}

bool live_video::save_to_jpg(const char* file,int quality)
{
	if(!m_is_playing)
	{
		return false;
	}

	this->wait_for_frame();

	return hwplay_save_to_jpg(m_ph,file,quality) ? true : false;
}

bool live_video::save_to_bmp(const char* bmp)
{
	if(!m_is_playing)
	{
		return false;
	}

	return hwplay_save_to_bmp(m_ph,bmp) ? true : false;
}

bool live_video::print_yuv(bool isYuv)
{
	if (isYuv)
	{
		hwplay_register_yuv_callback_ex(m_ph,on_yuv_ex,(long)this);
	}else{
		hwplay_register_yuv_callback_ex(m_ph,on_yuv_ex,(long)this);
	}



	return true;
}

bool live_video::wait_for_frame()
{	
	int frame_num_t;
	int wait_time_t=0;
	BOOL success_t;
	
	do{
		success_t=hwplay_get_framenum_in_buf(m_ph,&frame_num_t);

		if ((success_t==TRUE)&&
			(frame_num_t>0)&&
			(m_stream_ready==TRUE)){

			break;
		}else{
			Sleep(100);
			printf(".");
			wait_time_t++;
			if (wait_time_t>10) break;
		}


	} while (TRUE);



	return true;
}
/*----------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------*/
bool live_video::snap2jpg()
{
	std::string ip_addr_t=this->m_ip;
	
	long time=clock();
	
	std::string filename_t=ip_addr_t+"_"+ltos(time)+"hello.jpg";
	
	return HW_NET_PLAY_SnapJpg(m_ph,filename_t.c_str());

}
/*----------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------*/
string live_video::ltos(long l)  
{  
	ostringstream os;  
	os<<l;  
	string result;  
	istringstream is(os.str());  
	is>>result;  
	return result;  

}
/*----------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------*/
unsigned live_video::opencv_show_image_thread(LPVOID lpParam)
{

	live_video* lv = (live_video*)lpParam;
	
	while(lv->m_img_rgb_3==NULL&&lv->m_is_playing);

	IplImage* ipl_img=lv->m_img_rgb_4_for_show;	
	ASSERT(lv->m_img_rgb_4_for_show!=NULL);


#ifdef SHOW_IMAGE
#if SHOW_IMAGE
	cvNamedWindow(lv->m_ip.c_str(),0);
	cvResizeWindow(lv->m_ip.c_str(),480,270);
#endif
#endif



	
	unsigned char *image_buffer=(unsigned char *)ipl_img->imageData;

		printf("Q Quit \n");
		printf("C Cut frame \n");
		printf("V Start save video \n");

	ImageData MemData(lv->m_img_rgb_4_for_show,
			"",
			1000,
			0,
			0.5);
	
	MemData.GetThetaMLXYSeeds_ByCircle_UseSpiral();
	
	while(lv->m_is_playing){

#ifdef SHOW_IMAGE	
#if SHOW_IMAGE

		cvCvtColor(lv->m_img_rgb_3,lv->m_img_rgb_4_for_show,CV_BGR2BGRA);
		
		MemData.Draw_Kseeds_Spiral(lv->m_img_rgb_4_for_show);

		cvShowImage(lv->m_ip.c_str(),lv->m_img_rgb_4_for_show);
		
		int key=cvWaitKey(1);

		if(key=='q'){
			lv->close();
			printf("关闭视频,%s \n",lv->m_ip.c_str());
		}else	if(key=='c'){
			lv->m_save_image_switch=1;
			printf("开始截图,%s \n",lv->m_ip.c_str());

		}else 	if(key=='v'){
			
			lv->m_save_video_switch=!lv->m_save_video_switch;
			if(lv->m_save_video_switch==true){
				printf("开始录制视频,%s \n",lv->m_ip.c_str());
			}else{
				printf("结束录制视频,%s \n",lv->m_ip.c_str());
			}

		}else if(key=='4'){
			lv->turn_left();
		}else if(key=='6'){
			lv->turn_right();
		}else if(key=='8'){
			lv->turn_up();
		}else if(key=='2'){
			lv->turn_down();
		}else if(key=='5'){
			lv->turn_stop();
		}else if(key==-1){
			Sleep(1000/25);
		}else{
			Sleep(1000/25);
		}
#else
		Sleep(100);
#endif
#endif
		
		lv->SaveImage();
		lv->SaveVideo();
		
		
	}

	cvDestroyWindow(lv->m_ip.c_str());


	return 0;
}
/*----------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------*/
bool live_video::is_play()
{
	return this->m_is_playing;
}
/*----------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------*/
void live_video::string_replace(string&s1,const string s2,const string s3)
{
	string::size_type pos=0;
	string::size_type a=s2.size();
	string::size_type b=s3.size();
	while((pos=s1.find(s2,pos))!=string::npos)
	{
		s1.replace(pos,a,s3);
		pos+=b;
	}
}
/*----------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------*/
string live_video::get_directory()
{
		string path_t=this->m_ip;
		string_replace(path_t,".","_");
#if _MSC_VER
		CreateDirectory(path_t.c_str(),NULL);
#endif
		return path_t;
}
/*----------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------*/
string live_video::get_time_stamp()
{
#if _MSC_VER

	SYSTEMTIME sys_tm;
	GetLocalTime(&sys_tm);

	/*char dir_data_path[255];
	sprintf(dir_data_path,"%s//%04d_%02d_%02d",g_set.snap_dir_path,sys_tm.wYear,sys_tm.wMonth,sys_tm.wDay);
	CreateDirectory(dir_data_path,NULL);*/

	char pic_path[255];
	sprintf(pic_path,"%04d_%02d_%02d_%02d_%02d_%02d_",
		sys_tm.wYear,sys_tm.wMonth,sys_tm.wDay,sys_tm.wHour,sys_tm.wMinute,sys_tm.wSecond);

	string time_t=pic_path;
	return time_t;
#endif
}
/*----------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------*/
void live_video::turn_up()
{
		struct tPtzControl data_t;
#if TRUE		
		//data_t.slot=0;
		data_t.controlType=0;//direct
		data_t.cmd=8;//up
		data_t.presetno=16;
#else
		data_t.slot=1;
		data_t.controlType=0;
		data_t.cmd=8;//up
		data_t.presetno=32;
#endif
		HW_NET_ControlPtz(m_sh,&data_t);
}
/*----------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------*/
void live_video::turn_down()
{
		struct tPtzControl data_t;
		data_t.slot=0;
		data_t.controlType=0;
		data_t.cmd=2;//down
		data_t.presetno=32;
		HW_NET_ControlPtz(m_sh,&data_t);
}
/*----------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------*/
void live_video::turn_left()
{
		struct tPtzControl data_t;
		data_t.slot=0;
		data_t.controlType=0;
		data_t.cmd=4;//left
		data_t.presetno=32;
		HW_NET_ControlPtz(m_sh,&data_t);
}
/*----------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------*/
void live_video::turn_right()
{
		struct tPtzControl data_t;
		data_t.slot=0;
		data_t.controlType=0;
		data_t.cmd=6;//right
		data_t.presetno=32;
		HW_NET_ControlPtz(m_sh,&data_t);
}
/*----------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------*/
void live_video::turn_stop()
{
		struct tPtzControl data_t;	
		data_t.controlType=0;//direct
		data_t.cmd=5;//stop
		HW_NET_ControlPtz(m_sh,&data_t);
}
/*----------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------*/
void  live_video::SaveImage_rgb_3()
{
#ifdef _MSC_VER
	m_ImgLock.Lock();
#endif

#ifdef SAVE_IMAGE
#if SAVE_IMAGE	
		{
					std::string gps_p=GPS_WG_8020::getInstance()->GetLatLonStr();
					std::string compass_prh=Compass_HCM365::getInstance()->GetPitchRollHeadingStr();

					std::string ip_addr_t=this->m_ip;

					std::string filename_org_t=ip_addr_t+
						"$"+get_time_stamp()+
						"$"+gps_p+
						"$"+compass_prh+
						"$"+"org.jpg";
					//std::string filename_srl_t=ip_addr_t+"_"+get_time_stamp()+"spiral.jpg";
					string path_t=this->m_ip;
					string_replace(path_t,".","_");
										
					cvSaveImage( (path_t+"//"+filename_org_t).c_str(),this->m_img_rgb_3);
					//cvSaveImage( (path_t+"//"+filename_srl_t).c_str(),lv->m_img_rgb_4_for_show);


					printf("SAVE_IMAGE\n");
		}
#endif
#endif

#ifdef _MSC_VER
	m_ImgLock.Unlock();
#endif
}
/*----------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------*/
void  live_video::SaveImage_rgb_4_for_show()
{
#ifdef _MSC_VER
	m_ImgLock.Lock();
#endif

#ifdef SAVE_IMAGE
#if SAVE_IMAGE
		std::string gps_p=GPS_WG_8020::getInstance()->GetLatLonStr();
		std::string compass_prh=Compass_HCM365::getInstance()->GetPitchRollHeadingStr();
		
		{				
			std::string ip_addr_t=this->m_ip;				
			std::string filename_srl_t=ip_addr_t+
				"$"+get_time_stamp()+
				"$"+gps_p+
				"$"+compass_prh+
				"$"+"spiral.jpg";
			string path_t=this->m_ip;
			string_replace(path_t,".","_");				
			cvSaveImage( (path_t+"//"+filename_srl_t).c_str(),this->m_img_rgb_4_for_show);
			printf("SAVE IMAGE SPIRAL\n");
		}
#endif
#endif

#ifdef _MSC_VER
		m_ImgLock.Unlock();
#endif
}
/*----------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------*/
void live_video::SaveImage()
{
	if(this->m_save_image_switch>0){
		this->SaveImage_rgb_4_for_show();
		this->SaveImage_rgb_3();
		this->m_save_image_switch--;
	}

}
/*----------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------*/
void live_video::SaveVideo()
{
	if(this->m_save_video_switch){

#ifdef SAVE_VIDEO
#if SAVE_VIDEO
		cvWriteFrame(this->m_writer,this->m_img_rgb_3);
		this->m_save_video_start=true;
		printf("SAVE_VIDEO\n");
#endif
#endif

#ifdef SAVE_VIDEO
#if SAVE_VIDEO	

		IplImage *img_t=cvCreateImage(cvGetSize(this->m_img_rgb_3),IPL_DEPTH_8U,this->m_img_rgb_3->nChannels);

		cvCvtColor(this->m_img_rgb_4_for_show,img_t,CV_BGRA2BGR);
		cvWriteFrame(this->m_writer_spiral,img_t);
		printf("SAVE VIDEO SPIRAL\n");

		cvReleaseImage(&img_t);
#endif
#endif
		
		this->write_xml_pos();
		this->m_video_frame_count++;

	}else{
		this->init_video_writer();
	}



}
/*----------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------*/
bool live_video::init_xml_pos()
{

	const char* declaration ="<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>";
	this->m_video_pos.Clear();
	this->m_video_pos.Parse(declaration);//会覆盖xml所有内容

	//添加申明可以使用如下两行
	//XMLDeclaration* declaration=doc.NewDeclaration();
	//doc.InsertFirstChild(declaration);

	tinyxml2::XMLElement* root=this->m_video_pos.NewElement("Root");
	this->m_video_pos.InsertEndChild(root);

	


	return 0;
}
/*----------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------*/
bool live_video::write_xml_pos()
{
	char  frame_count_t[1024];
	itoa(this->m_video_frame_count,frame_count_t,10);


	string lat_lon_str_t=GPS_WG_8020::getInstance()->GetLatLonStr();
	std::string compass_prh=Compass_HCM365::getInstance()->GetPitchRollHeadingStr();
	


	XMLElement* root=this->m_video_pos.RootElement();
	
	XMLElement* Node_t =this->m_video_pos.NewElement("VideoFrame");
	{	
		/*-----------*/
		XMLElement *frame_count=this->m_video_pos.NewElement("Frame"); 
					frame_count->InsertEndChild(this->m_video_pos.NewText(frame_count_t));

		Node_t->InsertEndChild(frame_count);
		/*-----------*/			     
		XMLElement* lat_lon_t = this->m_video_pos.NewElement("LatLon");
					lat_lon_t->InsertEndChild(this->m_video_pos.NewText(lat_lon_str_t.c_str()));
		
		Node_t->InsertEndChild(lat_lon_t);
		/*-----------*/
		XMLElement* compass_prh_e = this->m_video_pos.NewElement("Compass");
					compass_prh_e->InsertEndChild(this->m_video_pos.NewText(compass_prh.c_str()));
		
		Node_t->InsertEndChild(compass_prh_e);
		/*-----------*/
	}
	
	root->InsertEndChild(Node_t);

	return true;
}
/*----------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------*/
