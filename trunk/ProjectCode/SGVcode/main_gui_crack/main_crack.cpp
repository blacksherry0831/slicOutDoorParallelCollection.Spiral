#include "stdafx.h"
#include <MY_SDK_LIB/Base.h>
#include <MY_SDK_LIB/ImageProcess.h>


void hough(IplImage* pImg, size_t idx, CvRect rect_cut)
{
	CvSeq* lines = NULL;
	CvMemStorage* storage = NULL;
	storage = cvCreateMemStorage(0);	
	IplImage* pImg8u  = cvCreateImage(cvGetSize(pImg), IPL_DEPTH_8U, 1);
	IplImage* iplimg_tmp=cvCloneImage(pImg);
	IplImage* pImgCanny = cvCreateImage(cvGetSize(pImg), IPL_DEPTH_8U, 1);

#if 1
	cvCvtColor(pImg, pImg8u, CV_BGR2GRAY);
	cvEqualizeHist(pImg8u, pImg8u);
#endif // 0

	double low_thresh=-1;
	double high_thresh=-1;
	
#if 1
	ImageProcess::CannyAdaptiveFindThreshold(pImg8u, &low_thresh, &high_thresh);
#endif 

	cvCanny(pImg8u, pImgCanny, low_thresh,high_thresh, 3);
	double rho = 1;
	int lineGap = rect_cut.width/4.0;
	int lineLength = rect_cut.width/2;
	int threshold = rect_cut.width/10.0*3;
  	double theta = CV_PI / 180;
	lines = cvHoughLines2(pImgCanny, storage, CV_HOUGH_PROBABILISTIC,rho, theta, threshold, lineLength, lineGap);
	/*在pImgDst上画出检测到的直线*/
	for (int i = 0; i < lines->total; i++)
	{
		CvPoint* line = (CvPoint*)cvGetSeqElem(lines, i);
		cvLine(iplimg_tmp, line[0], line[1], CV_RGB(0, 0, 255), 1, 8);
	}
	stringstream ss;
	ss << idx;
	string idx_str_t = ss.str();

	cvShowImage((idx_str_t+"draw line").c_str(),iplimg_tmp);
	cvShowImage((idx_str_t + "binary").c_str(), pImgCanny);
	cvShowImage((idx_str_t + "gray").c_str(),pImg8u);

	cvReleaseImage(&pImg8u);
	cvReleaseImage(&iplimg_tmp);
	cvReleaseMemStorage(&storage);

}

void hough2(IplImage* pImg, size_t idx, CvRect rect_cut, int *pR, int *pTh)
{
	CvSeq* lines = NULL;
	CvMemStorage* storage = NULL;
	storage = cvCreateMemStorage(0);
	IplImage* pImg8u = cvCreateImage(cvGetSize(pImg), IPL_DEPTH_8U, 1);
	IplImage* iplimg_tmp = cvCloneImage(pImg);
	IplImage* pImgCanny = cvCreateImage(cvGetSize(pImg), IPL_DEPTH_8U, 1);

#if 1
	cvCvtColor(pImg, pImg8u, CV_BGR2GRAY);
	cvEqualizeHist(pImg8u, pImg8u);
	cvSmooth(pImg8u, pImg8u);
#endif // 0

	double low_thresh = -1;
	double high_thresh = -1;

#if 1
	ImageProcess::CannyAdaptiveFindThreshold(pImg8u, &low_thresh, &high_thresh);
#endif 

	cvCanny(pImg8u, pImgCanny, low_thresh, high_thresh, 3);
	
#if 0
	double rho = 1;
	int lineGap = rect_cut.width / 4.0;
	int lineLength = rect_cut.width / 2;
	int threshold = rect_cut.width / 10.0 * 3;
	double theta = CV_PI / 180;
	lines = cvHoughLines2(pImgCanny, storage, CV_HOUGH_PROBABILISTIC, rho, theta, threshold, lineLength, lineGap);
	/*在pImgDst上画出检测到的直线*/
	for (int i = 0; i < lines->total; i++)
	{
		CvPoint* line = (CvPoint*)cvGetSeqElem(lines, i);
		cvLine(iplimg_tmp, line[0], line[1], CV_RGB(0, 0, 255), 1, 8);
	}
#else
	int R;
	int Th;
	int iThreshold = 3;
	cv::Point pt1, pt2;
	ImageProcess::HoughLine(
		pImgCanny,
		&R, &Th, iThreshold,
		1.0*pImg->height/3.0,
		2.0*pImg->height/3.0,
		90,
		3);

	std::cout << "rho: " << R << "   " << "theta: " << Th << std::endl;

	ImageProcess::Draw_line_on_image(
		R,
		Th,
		rect_cut,
		iplimg_tmp,
		pt1,
		pt2);
	 *pR =R;
	 *pTh=Th;
#endif // 1


		
#if TRUE
	stringstream ss;
	ss << idx;
	string idx_str_t = ss.str();

	cvShowImage((idx_str_t + "draw line").c_str(), iplimg_tmp);
	cvShowImage((idx_str_t + "binary").c_str(), pImgCanny);
	cvShowImage((idx_str_t + "gray").c_str(), pImg8u);

#endif // TRUE

	cvReleaseImage(&pImg8u);
	cvReleaseImage(&iplimg_tmp);
	cvReleaseMemStorage(&storage);
}

CvRect  get_detection_rect(IplImage iplimg, std::vector<CvRect> rect_cut)
{
	int R[2];
	int Th[2];
	for (size_t i = 0; i < rect_cut.size(); i++)
	{
		IplImage* iplimg_t = cvCreateImage(cvSize(rect_cut.at(i).width, rect_cut.at(i).height), IPL_DEPTH_8U, 3); {

			cvSetImageROI(&iplimg, rect_cut.at(i));
			cvCopyImage(&iplimg, iplimg_t);
			cvResetImageROI(&iplimg);

			hough2(iplimg_t, i, rect_cut.at(i), &R[i], &Th[i]);

		}cvReleaseImage(&iplimg_t);
	}
	cv::Point pt[4];

	ImageProcess::Draw_line_on_image(R[0],Th[0],rect_cut.at(0),nullptr,pt[0],pt[1]);
	ImageProcess::Draw_line_on_image(R[1], Th[1],rect_cut.at(1), nullptr, pt[2], pt[3]);

	CvRect rect;
	rect.x = pt[0].x;
	rect.y = pt[0].y;

	rect.width = pt[1].x - pt[0].x;
	rect.height = pt[2].y - pt[0].y;

	return rect;

}


void capture_frame(string avi_1,std::vector<CvRect> rect_cut) {
	char FileName[80];
	memset(FileName, 0, sizeof FileName);

	CvCapture* capture = cvCaptureFromAVI(avi_1.c_str());
	CvVideoWriter *video_writer_out;

	int ii = 99;
	CvRect rect0=cvRect(0,0,0,0);
	while (cvGrabFrame(capture))
	{
		ii++;
		Mat matimg = cvRetrieveFrame(capture);        // retrieve the captured frame
		IplImage iplimg = matimg.operator _IplImage();
		
		if (rect0.x + rect0.y + rect0.width + rect0.height == 0) {
				rect0=get_detection_rect(iplimg, rect_cut);
#if _MSC_VER
				   string save_cut_str = avi_1 + ".cut.avi";
					video_writer_out = cvCreateVideoWriter(save_cut_str.c_str(),
					//CV_FOURCC_PROMPT,
					CV_FOURCC('D', 'I', 'V', 'X'),
					cvGetCaptureProperty(capture, CV_CAP_PROP_FPS),
					cvSize(rect0.width,rect0.height));
#else
				video_writer_out = cvCreateVideoWriter(video_out.c_str(),
					CV_FOURCC_DEFAULT,
					cvGetCaptureProperty(capture_in, CV_CAP_PROP_FPS),
					cvGetSize(frame_in));
#endif
		}
		
#if 1
		
		 {
			
			try{
				    IplImage *img_tt_org = cvCloneImage(&iplimg);
					IplImage *img_tt = cvCreateImage(cvSize(rect0.width,rect0.height), iplimg.depth, iplimg.nChannels);
						
						cvSetImageROI(img_tt_org, rect0);
							
						cvCopy(img_tt_org,img_tt);
							
						cvResetImageROI(img_tt_org);
					
						cvWriteFrame(video_writer_out, img_tt);
					cvReleaseImage(&img_tt);
					cvReleaseImage(&img_tt_org);

			}catch (const cv::Exception & msg){
				std::cout << msg.err;
			}



		}
#endif // 1



		
#if 1
		for (size_t i = 0; i < rect_cut.size(); i++)
		{
			cvRectangleR(&iplimg, rect_cut.at(i), CV_RGB(0, 255, 0), 2);
		}
		cvRectangleR(&iplimg,rect0, CV_RGB(0, 255, 255), 2);

		cvShowImage("org", &iplimg);
		static int flag_t = 0;
		cvWaitKey(flag_t);
		if (flag_t == 0) {			
			flag_t = 10;
		}
		
#endif // 1	
	}
	
	cvReleaseVideoWriter(&video_writer_out);

	cvReleaseCapture(&capture);
}

void init_rect(int idx,std::vector<CvRect>& rect_cut)
{
	int x_pos_0;//X 坐标
	int x_width;//X轴方向宽度
	int y_pos_0;//第一个矩形高
	int y_pos_1;//第二个矩形高
	const int y_step=25;//矩形0.5高

	rect_cut.resize(0);

	if (idx == 1) {
		
		x_pos_0 = 1130;
		x_width = 1400 - x_pos_0;
		y_pos_0 = 235;
		y_pos_1 = 660;
		
		rect_cut.push_back(cvRect(x_pos_0,//
			y_pos_0 - y_step,
			x_width,//
			y_step * 2));

		rect_cut.push_back(cvRect(x_pos_0,//
			y_pos_1 - y_step,
			x_width,//
			y_step * 2));

	}else if (idx==2){
		x_pos_0 = 1300;
		x_width = 1850 - x_pos_0;
		y_pos_0 = 360;
		y_pos_1 =880;
		
		rect_cut.push_back(cvRect(x_pos_0,//
			y_pos_0 - y_step,
			x_width,//
			y_step * 2));

		rect_cut.push_back(cvRect(x_pos_0,//
			y_pos_1 - y_step,
			x_width,//
			y_step * 2));
	}else if (idx == 3) {
			
			x_pos_0 = 850;
			x_width = 1920 - 850;
			y_pos_0 = 370;
			y_pos_1 = 855;
			
			rect_cut.push_back(cvRect(x_pos_0,//
										y_pos_0-y_step,
										x_width,//
										y_step*2));

			rect_cut.push_back(cvRect(x_pos_0,//
				y_pos_1-y_step,
				x_width,//
				y_step*2));
	}
	else if (idx == 4)
	{
		x_pos_0 = 850;
		x_width = 1161 - x_pos_0;
		y_pos_0 = 227;
		y_pos_1 = 800;
		
		rect_cut.push_back(cvRect(x_pos_0,//
			y_pos_0 - y_step,
			x_width,//
			y_step * 2));

		rect_cut.push_back(cvRect(x_pos_0,//
			y_pos_1 - y_step,
			x_width,//
			y_step * 2));
	}
	else if (idx == 5)
	{
		x_pos_0 = 549;
		x_width = 1497 - x_pos_0;
		y_pos_0 = 327;
		y_pos_1 = 919;
		
		rect_cut.push_back(cvRect(x_pos_0,//
			y_pos_0 - y_step,
			x_width,//
			y_step * 2));

		rect_cut.push_back(cvRect(x_pos_0,//
			y_pos_1 - y_step,
			x_width,//
			y_step * 2));
	}
	else if (idx == 6)
	{
		x_pos_0 = 960;
		x_width = 1920 - x_pos_0;
		y_pos_0 = 239;
		y_pos_1 = 830;
		
		rect_cut.push_back(cvRect(x_pos_0,//
			y_pos_0 - y_step,
			x_width,//
			y_step * 2));

		rect_cut.push_back(cvRect(x_pos_0,//
			y_pos_1 - y_step,
			x_width,//
			y_step * 2));
	}
	else if (idx == 7)
	{
		x_pos_0 = 740;
		x_width = 1479 - x_pos_0;
		y_pos_0 = 329;
		y_pos_1 = 980;
		
		rect_cut.push_back(cvRect(x_pos_0,//
			y_pos_0 - y_step,
			x_width,//
			y_step * 2));

		rect_cut.push_back(cvRect(x_pos_0,//
			y_pos_1 - y_step,
			x_width,//
			y_step * 2));
	}
	else
	{

	}

}

int get_idx(string str_t)
{
	string base_name=Base::base_name(str_t);
	vector<string>  vs= Base::split(base_name,'.');
	string idx_str = vs.at(0);
	return Base::get_number(idx_str);
}



int main(int argc, char *argv[])
{
	int EXEC_RESULT = 0;
	std::vector<CvRect> rect_cut;
	


	cvNamedWindow("org",CV_WINDOW_NORMAL);

#if 0
	//cvNamedWindow("draw line", CV_WINDOW_NORMAL);
	//cvNamedWindow("binary", CV_WINDOW_NORMAL);
	//cvNamedWindow("gray", CV_WINDOW_NORMAL);
#endif // 0
	
	string file_base = "X:\\MyProject\\Project\\原觉项目\\工业视觉探伤技术资料\\软件设计\\软件代码及说明\\2017年9月18日-视频采集\\12s\\obj0-12\\";
	
	vector<string> files;

	files.push_back("ch1.mp4out.avi");
	files.push_back("ch2.mp4out.avi");
	files.push_back("ch3.mp4out.avi");

	files.push_back("ch4.mp4out.avi");
	files.push_back("ch5.mp4out.avi");
	files.push_back("ch6.mp4out.avi");

	files.push_back("ch7.mp4out.avi");
	
	for (size_t i = 0; i <files.size(); i++)
	{
		string file_name=file_base+files.at(i);

		int idx=get_idx(file_name);

		init_rect(idx,rect_cut);
	
		capture_frame(file_name ,
						rect_cut);

	//	cvWaitKey(10 * 1000);
	}


		
	return  EXEC_RESULT;
}
