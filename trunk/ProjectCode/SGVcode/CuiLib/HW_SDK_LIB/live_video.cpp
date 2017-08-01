#include "stdafx.h"
#include "live_video.h"
#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>
using namespace std;
//#include "Convert.h"
#include "modules.h"

#include "../SerialPort/Compass_HCM365.h"
#include "../SerialPort/Gps_WG_8020.h"




live_video::live_video(const char* ip,int slot):live_video_base(ip,slot)
{
	
	m_stream_len=0;
	m_stream_count=0;
	m_stream_ready=0;
	m_stream_frame_count=0;
	m_save_image_switch=0;
	m_save_video_switch=false;
	m_is_draw_spiral=false;
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
/**
*
*
*/
bool live_video::init_video_writer()
{				
		const int fps     = FPS_MY;
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






/*----------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------*/

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
		
		if(lv->m_is_draw_spiral){

				MemData.Draw_Kseeds_Spiral(lv->m_img_rgb_4_for_show);
				std::string gps_p=GPS_WG_8020::getInstance()->GetLatLonStr();
				std::string compass_prh=Compass_HCM365::getInstance()->GetPitchRollHeadingStr();
				{
					CvFont font;
					CvPoint pt1=cvPoint(20,30);
					CvPoint pt0=cvPoint(20,100);;
					cvInitFont(&font,CV_FONT_HERSHEY_SIMPLEX, 1.0f,1.0f,2);
					cvPutText(lv->m_img_rgb_4_for_show, gps_p.c_str(), pt1, &font,CV_RGB(0, 0, 0) );
					cvPutText(lv->m_img_rgb_4_for_show, compass_prh.c_str(), pt0, &font,CV_RGB(0, 0, 0) );

				}
		}

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
		}else if(key=='d'){

			lv->m_is_draw_spiral=!lv->m_is_draw_spiral;
		
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
