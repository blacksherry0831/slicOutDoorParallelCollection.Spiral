#include "stdafx.h"
#include <MY_SDK_LIB/Base.h>
#include <MY_SDK_LIB/ImageProcess.h>
#include <MY_SDK_LIB/OutCircleFeature.hpp>

/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
const std::string  MODULE_XML = "svm.module.learn.crack.old.xml";
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
int GetLineProperty(std::vector<CvPoint> point_set, std::vector<float> delta, float& sum_delta, int& idx)
{
	
	return ImageProcess::GetLineProperty(point_set, delta, sum_delta, idx);
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
std::string GetFrameChannelDiff(std::string file_base, std::string file_name, std::string add_str)
{
	return Base::CRACK_PATH_GetFrameChannelDiff(file_base, file_name, add_str);

}

void Save_Iplimage2DiskTXT(std::string file_base, std::string file_name,IplImage* img,int frame_count, std::string add_str)
{
	std::string file_full_name = file_base + file_name;
	int video_idx = Base::CRACK_FILE_NAME_get_channel(file_full_name);
	const std::string file_full_path = GetFrameChannelDiff(file_base, file_name, add_str);
	{

#if FALSE
	std::stringstream all_m_n;
	all_m_n << frame_count << add_str<<".txt";	
	std::string output_full_name= file_full_path+all_m_n.str();
	ofstream outfile;
	outfile.open(output_full_name); //myfile.bat是存放数据的文件名  
	
	if (outfile.is_open()){
		outfile <<"col" << " "
			<<"row" << " "
			<<"gray" << std::endl;
			
		for (size_t ih = 0; ih <img->height; ih++){
			for (size_t iw = 0; iw <img->width; iw++){		
			
								float pixel_data=cvGetReal2D(img,ih,iw);

								  outfile << iw << " "
									  << ih << " "
									  << pixel_data << std::endl;
						}
		}
			outfile.close();
	}else{
		cout << "不能打开文件!" << endl;
	}
#endif // TRUE
	
	}

	{

#if FALSE
	std::stringstream ss_file_path_2d;
	ss_file_path_2d << file_full_path << frame_count<<"\\";
	CreateDirectory(ss_file_path_2d.str().c_str(), NULL);

	for (size_t iw = 0; iw <img->width; iw++) {
		
		std::stringstream ss_file_name_tt ;
		ss_file_name_tt << ss_file_path_2d.str();
		ss_file_name_tt <<iw ;
		ss_file_name_tt << add_str << ".txt";		
		
		std::string output_full_name = ss_file_name_tt.str();
		ofstream outfile;
		outfile.open(output_full_name); //myfile.bat是存放数据的文件名  
			if (outfile.is_open()) {
							outfile << "row" << " "
							<< "gray" << std::endl;
							for (size_t ih = 0; ih <img->height; ih++) {
		
								float pixel_data = cvGetReal2D(img, ih, iw);

								outfile << ih << " " 
										<< pixel_data<<std::endl;
			
							}	
							outfile.close();
			}else {
				cout << "不能打开文件!" << endl;
			}


	}

#endif // TRUE
		
	}





}


/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/


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
	std::stringstream ss;
	ss << idx;
	std::string idx_str_t = ss.str();

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
	std::stringstream ss;
	ss << idx;
	std::string idx_str_t = ss.str();

	cvShowImage((idx_str_t + "draw line").c_str(), iplimg_tmp);
	cvShowImage((idx_str_t + "binary").c_str(), pImgCanny);
	cvShowImage((idx_str_t + "gray").c_str(), pImg8u);

#endif // TRUE

	cvReleaseImage(&pImg8u);
	cvReleaseImage(&iplimg_tmp);
	cvReleaseMemStorage(&storage);
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
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
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void init_mask_rect_2(std::string file_base, std::vector<CvRect>& rect_cut, int channel,int WIDTH)
{
	rect_cut.clear();

	const std::string file_name = "mask_x_y0_y1_width_height.xml";
	std::string mask_region_file = file_base + file_name;
	
	if (channel<10000){
				CvMat* mask_region = (CvMat*)(cvLoad(mask_region_file.c_str()));
				assert(mask_region != NULL);
				{
					float x = CV_MAT_ELEM(*mask_region, float, channel, 0);
					float y0 = CV_MAT_ELEM(*mask_region, float, channel, 1);
					float y1 = CV_MAT_ELEM(*mask_region, float, channel, 2);
					float width = CV_MAT_ELEM(*mask_region, float, channel, 3);
					float height = CV_MAT_ELEM(*mask_region, float, channel, 4);

					rect_cut.push_back(cvRect(x, y0, WIDTH, height));
					rect_cut.push_back(cvRect(x, y1, WIDTH, height));
				}
				cvReleaseMat(&mask_region);
	}
	


}

/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void capture_frame(std::string file_base,std::string file_name,std::vector<CvRect> rect_cut)
{



	std::string avi_1 = file_base+file_name;
	
	CvCapture* capture = cvCaptureFromAVI(avi_1.c_str());
	CvVideoWriter *video_writer_out;

	int ii = 99;
	CvRect rect0=cvRect(0,0,0,0);
	while (cvGrabFrame(capture))
	{
		ii++;
		cv::Mat matimg = cvRetrieveFrame(capture);        // retrieve the captured frame
		IplImage iplimg = matimg.operator _IplImage();
		
		if (rect0.x + rect0.y + rect0.width + rect0.height == 0) {
				rect0=get_detection_rect(iplimg, rect_cut);
#if _MSC_VER
				   std::string save_cut_path = file_base + "cut\\";
				   CreateDirectory(save_cut_path.c_str(), NULL);
				   std::string save_cut_str =save_cut_path+file_name + ".cut.avi";
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
				if (video_writer_out == nullptr) {
					std::cout << "create viveo writer [fail] :" << save_cut_str << std::endl;
				}else {
					std::cout << "create viveo writer [success] :" << save_cut_str << std::endl;
				}
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
		cvSaveImage("org.jpg", &iplimg);
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
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void process_frame_out(
	std::string file_base,
	std::string file_name,
	std::string svm_module,
	boolean SAVE_DATA_FLAG)
{
#if (CV_MAJOR_VERSION==2)&&(CV_MINOR_VERSION==4)
	CvSVM SvmTest;/**<支持向量机模型*/
	if (svm_module != "") {
		std::cout << "Now Load :" << svm_module << "as Svm Module" << std::endl;
		SvmTest.load(svm_module.c_str());
	}
#endif
	assert(Base::FS_checkUserPath(file_base));

	const std::string avi_1 = file_base + file_name;
	int m = 0, n = 0, frame_count = 0;
	IplImage* image_gray = nullptr;
	CvCapture* capture = cvCaptureFromAVI(avi_1.c_str());
	const int CHANNEL = Base::CRACK_FILE_NAME_get_channel(file_name);
	const int CIRCLE = Base::CRACK_FILE_NAME_get_circle(file_name);
	std::vector<CvRect> rect_mask;

	if (cvGrabFrame(capture)) {

		cv::Mat matimg = cvRetrieveFrame(capture);        // retrieve the captured frame
		IplImage iplimg = matimg.operator _IplImage();
		if (image_gray == nullptr) {
			image_gray = cvCreateImage(cvSize(iplimg.width, iplimg.height), IPL_DEPTH_8U, 1);
		}
		m = 1.0*iplimg.width;
		n = 1.0*iplimg.height;
		if (true) {
			init_mask_rect_2(file_base, rect_mask, CHANNEL, iplimg.width);
		}

	}

	IplImage* image_cut_statistic = cvCreateImage(cvSize(m, n), IPL_DEPTH_32F, 1);
	IplImage* image_cut_statistic_8U = cvCreateImage(cvSize(m, n), IPL_DEPTH_8U, 1);
	cvZero(image_cut_statistic);

	IplImage* image_cut_statistic_old = cvCloneImage(image_cut_statistic);
	IplImage* image_cut_statistic_diff = cvCloneImage(image_cut_statistic);
	IplImage* image_cut_statistic_diff_8U = cvCreateImage(cvSize(m, n), IPL_DEPTH_8U, 3);
	IplImage *image_gauss = cvCreateImage(cvSize(m, n), IPL_DEPTH_8U, 3);
	while (cvGrabFrame(capture)) {

		cv::Mat matimg = cvRetrieveFrame(capture);        // retrieve the captured frame
		IplImage iplimg = matimg.operator _IplImage();

		// convert 2 gray
		cvCvtColor(&iplimg, image_gray, CV_RGB2GRAY);
		for (size_t i = 0; i <rect_mask.size(); i++) {
			if (rect_mask.at(i).x >= 0 && rect_mask.at(i).y >= 0) {
				cvRectangleR(image_gray, rect_mask.at(i), cvScalar(0), CV_FILLED);
			}
		}
#if 0
		cvSmooth(image_gray, image_gray, CV_GAUSSIAN, 3, 3, 0, 0);
#endif // 0

		//convert 2 float
		

		cvConvert(image_gray, image_cut_statistic);
		//统计		


#if TRUE

		//差分
		if (frame_count == 0) {
			//第一次做差分
			cvZero(image_cut_statistic_diff);
		}else{
			//差分
			cvSub(image_cut_statistic, image_cut_statistic_old, image_cut_statistic_diff);
		}

#endif // TRUE
		std::vector<float> delta_out;

		std::vector<float> image_feature = ImageProcess::crack_get_image_feature_gauss(image_cut_statistic_diff, file_base, file_name, CIRCLE, CHANNEL, frame_count, image_gauss, delta_out, SAVE_DATA_FLAG);

#if TRUE
		//如果有学好的模型
		if (svm_module != "") {
			std::vector<float> feature_data = image_feature;
			CvMat m;
			cvInitMatHeader(&m, 1, feature_data.size(), CV_32FC1, feature_data.data());
			float category = SvmTest.predict(&m);
			if (category == 0) {
				//neg
			}
			else if (category == 1) {
				//pos
				cvCircle(image_cut_statistic_8U,
					cvPoint(image_cut_statistic_8U->width / 2, image_cut_statistic_8U->height / 2),
					3,
					CV_RGB(255, 255, 255), 2);
				{
					const std::string path_t = "svm_output_pos";
					std::string file_full_path_org = GetFrameChannelDiff(file_base, file_name, path_t);
					std::stringstream ss_image_name;
					ss_image_name << frame_count << ".jpg";
					std::string image_full_path = file_full_path_org + ss_image_name.str();
					cvSaveImage(image_full_path.c_str(), &iplimg);
				}
			}
			else {

			}
		}
		else {

		}
#endif // TRUE

#if TRUE
		cvConvert(image_cut_statistic, image_cut_statistic_8U);//float 转uchar
		cvConvertScale(image_cut_statistic_diff, image_cut_statistic_diff_8U, 0.5, 255.0 / 2);//float 转uchar

		if (SAVE_DATA_FLAG) {
			std::cout << "Save Frame : " << frame_count << std::endl;
			const std::string  diff = "diff";
			const std::string  org_gray = "org_gray";
			const std::string  org_rgb = "org_rgb";

			Save_Iplimage2DiskTXT(file_base, file_name, image_cut_statistic_diff, frame_count, diff);
			{
				std::string file_full_path_org = GetFrameChannelDiff(file_base, file_name, diff);
				std::stringstream ss_image_name;
				ss_image_name << frame_count << ".jpg";
				std::string image_full_path = file_full_path_org + ss_image_name.str();
				cvSaveImage(image_full_path.c_str(), image_cut_statistic_diff_8U);
			}
			Save_Iplimage2DiskTXT(file_base, file_name, image_cut_statistic, frame_count, org_gray);
			{
				std::string file_full_path_org = GetFrameChannelDiff(file_base, file_name, org_gray);
				std::stringstream ss_image_name;
				ss_image_name << frame_count << ".jpg";
				std::string image_full_path = file_full_path_org + ss_image_name.str();
				cvSaveImage(image_full_path.c_str(), image_cut_statistic_8U);
			}
#if TRUE
			{
				std::string file_full_path_org_rgb = GetFrameChannelDiff(file_base, file_name, org_rgb);
				std::stringstream ss_image_name;
				ss_image_name << frame_count << ".png";
				std::string image_full_path = file_full_path_org_rgb + ss_image_name.str();
				cvSaveImage(image_full_path.c_str(), &iplimg);
			}
#endif // TRUE


			
		}
		cvShowImage("diff_8U", image_cut_statistic_diff_8U);
		cvShowImage("m_n_gray_8U", image_cut_statistic_8U);
		cvShowImage("image_gauss", image_gauss);
		cvShowImage("org", &iplimg);
#endif // TRUE
#if TRUE
		//按键控制
		boolean PAUSE_FLAG = false;
		int key_data = cvWaitKey(10);
		if (key_data == 's') {
			std::cout << "Start Save Data !" << std::endl;
			SAVE_DATA_FLAG = true;
		}
		else if (key_data == 'e') {
			std::cout << "End Save Data !" << std::endl;
			SAVE_DATA_FLAG = false;
		}
		else if (key_data == 'p') {
			PAUSE_FLAG = !PAUSE_FLAG;
			if (PAUSE_FLAG == true) {
				std::cout << "now pause,input char 'c' to start run !" << std::endl;
			}
		}

		while (cvWaitKey(1) != 'c' &&  PAUSE_FLAG == true) {
			std::cout << "p";
		}
#endif

#if TRUE
		frame_count++;//end while
		cvCopyImage(image_cut_statistic, image_cut_statistic_old);
#endif // TRUE	
	}

	cvReleaseImage(&image_gauss);
	cvReleaseImage(&image_gray);
	cvReleaseImage(&image_cut_statistic);
	cvReleaseImage(&image_cut_statistic_8U);
	cvReleaseImage(&image_cut_statistic_old);
	cvReleaseImage(&image_cut_statistic_diff);
	cvReleaseImage(&image_cut_statistic_diff_8U);
	cvReleaseCapture(&capture);

}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/

/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/


/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/

/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/

/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void GetFeature(std::string src_file, std::string out_file,	const int CIRCLE,	const int CHANNEL, IplImage* avg, IplImage* sigma, int sigma_scale) 
{
	const int SIGMA = sigma_scale;
	const int WIDTH = avg->width;
	const int HEIGHT = avg->height;

	std::string tail=".bin.avi";
	
	const std::string binaryfilename = Base::base_name(src_file.c_str()) + ".sigma" + Base::int2str(SIGMA) + tail;
	const std::string binaryFileSavePath = out_file + binaryfilename;
		const std::string pn_base =			 Base::FS_createPath(out_file, "frame.sample.bin");
		const std::string org_gray_base =	 Base::FS_createPath(out_file, "frame.sample.org");
		const std::string org_gray_feature=	 Base::FS_createPath(out_file, "frame.sample.feature");
		const std::string pn_sigma_base =	 Base::FS_createPath(out_file, "frame.sample.bin.sigma");
	IplImage* image_gray = cvCreateImage(cvSize(WIDTH, HEIGHT), IPL_DEPTH_8U, 1);
	IplImage* image_gray_f = cvCreateImage(cvSize(WIDTH, HEIGHT), IPL_DEPTH_32F, 1);
	IplImage* calc_temp_f = cvCreateImage(cvSize(WIDTH, HEIGHT), IPL_DEPTH_32F, 1);
	IplImage* calc_temp_f_n = cvCreateImage(cvSize(WIDTH, HEIGHT), IPL_DEPTH_32F, 1);
	IplImage* calc_temp_f_p = cvCreateImage(cvSize(WIDTH, HEIGHT), IPL_DEPTH_32F, 1);
	IplImage* image_gray_binary_P = cvCreateImage(cvSize(WIDTH, HEIGHT), IPL_DEPTH_8U, 1);
	IplImage* image_gray_binary_N = cvCreateImage(cvSize(WIDTH, HEIGHT), IPL_DEPTH_8U, 1);
	IplImage* image_gray_binary_PN = cvCreateImage(cvSize(WIDTH, HEIGHT), IPL_DEPTH_8U, 1);

	IplImage* sigma_scale_m= cvCreateImage(cvSize(WIDTH, HEIGHT), IPL_DEPTH_32F, 1);
	cvConvertScale(sigma, sigma_scale_m, SIGMA);
	CvCapture* capture = cvCaptureFromAVI(src_file.c_str());
	const double FPS = cvGetCaptureProperty(capture, CV_CAP_PROP_FPS);
	const int FOURCC_DIVX = CV_FOURCC('D', 'I', 'V', 'X');
	const CvSize SIZE = cvSize((int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH), (int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT));
	assert(SIZE.width == WIDTH && SIZE.height == HEIGHT);
	CvVideoWriter* capture_W = cvCreateVideoWriter(binaryFileSavePath.c_str(), FOURCC_DIVX, FPS, SIZE, 0);
	assert(capture_W != NULL);
	const int TARGET_P = 255;
	const int TARTET_N = 128;
	const float COMP_MAX = 0;	

	for (int frame_count = 0; cvGrabFrame(capture) != NULL; frame_count++)
	{
		cv::Mat matimg = cvRetrieveFrame(capture);        // retrieve the captured frame
		IplImage iplimg = matimg.operator _IplImage();
		cvCvtColor(&iplimg, image_gray, CV_RGB2GRAY);
#if TRUE
		OutCircleFeature OCF(WIDTH, HEIGHT);
		OCF.SAVE_FLAG=TRUE;
		OCF.CIRCLE = CIRCLE;
		OCF.CHANNEL = CHANNEL;
		OCF.SIGMA = SIGMA;
		OCF.frame_count = frame_count;

		OCF.pn_base = pn_base;
		OCF.org_gray_base = org_gray_base;
		OCF.org_gray_feature=org_gray_feature;
		std::string pn_sigma_base;
		OCF.pn_sigma_base= pn_sigma_base;

		OCF.Process(image_gray,avg,sigma_scale_m);
		OCF.SaveB(image_gray);
#endif // TRUE
		cvAdd(image_gray_binary_P, image_gray_binary_N,image_gray_binary_PN);

		ASSERT(cvWriteFrame(capture_W, image_gray_binary_PN));
	}

	cvReleaseImage(&image_gray);
	cvReleaseImage(&image_gray_f);

	cvReleaseImage(&calc_temp_f);
	cvReleaseImage(&calc_temp_f_n);
	cvReleaseImage(&calc_temp_f_p);

	cvReleaseImage(&image_gray_binary_P);
	cvReleaseImage(&image_gray_binary_N);
	cvReleaseImage(&image_gray_binary_PN);
	cvReleaseImage(&sigma_scale_m);

	cvReleaseCapture(&capture);
	cvReleaseVideoWriter(&capture_W);
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void get_video_avg_sigma(std::string file,std::string save_path,IplImage* avg,IplImage* sigma)
{	
	const int WIDTH  = avg->width;
	const int HEIGHT = avg->height;
	const int FRAME_MAX_COUNT = 1024;
	const int CHANNEL = Base::CRACK_FILE_NAME_get_channel(file);
	const int CIRCLE = Base::CRACK_FILE_NAME_get_circle(file);
	

	std::vector<std::vector<std::vector<unsigned char> > > array3D;
	// 初始化  先resize有多少行
	array3D.resize(HEIGHT);
	//再resize 有多少高度
	for (int hi = 0; hi < HEIGHT; ++hi) {
		array3D[hi].resize(WIDTH);
		//要多少深度
		for (int wj = 0; wj < WIDTH; ++wj) {
			array3D[hi][wj].reserve(FRAME_MAX_COUNT);
			array3D[hi][wj].resize(FRAME_MAX_COUNT);
		}
			
	}

	int frame_count = 0;
	/*float Image_gray_count = 0;*/
	IplImage* image_gray = cvCreateImage(cvSize(WIDTH, HEIGHT), IPL_DEPTH_8U, 1);
			//cvZero(image_gray);
			cvZero(avg);
			cvZero(sigma);


			CvCapture* capture;			
			for (capture= cvCaptureFromAVI(file.c_str()),frame_count=0; frame_count<FRAME_MAX_COUNT;  ){

				if (cvGrabFrame(capture)) {
					
					cv::Mat matimg = cvRetrieveFrame(capture);        // retrieve the captured frame
					IplImage iplimg = matimg.operator _IplImage();
						// convert 2 gray
						cvCvtColor(&iplimg, image_gray, CV_RGB2GRAY);				
						for (size_t wi = 0; wi <WIDTH; wi++){
							for (size_t hi = 0; hi <HEIGHT; hi++){
								unsigned char data = cvGetReal2D(image_gray, hi, wi);
								array3D[hi][wi][frame_count]=data;
							}
						}
						frame_count++;
				}else{
					break;
				}

			}cvReleaseCapture(&capture);

	cvReleaseImage(&image_gray);


	for (size_t wi = 0; wi <WIDTH; wi++) {
		for (size_t hi = 0; hi <HEIGHT; hi++) {
			std::vector<unsigned char> &data_td = array3D[hi][wi];

			float avgF = Base::Math_GetAverageValue8U(data_td.data(), frame_count);
			float sigmaF_out;
			float sigmaF = Base::Math_GetVarianceValue8U(data_td.data(), frame_count,avgF);

			cvSetReal2D(avg, hi, wi, avgF);
			cvSetReal2D(sigma, hi, wi, sigmaF);
		}
	}
	//

	std::cout << "Now average && sigma is Done !!!" << std::endl;

#ifdef _DEBUG
#if  _DEBUG
{
	IplImage* image_8U = cvCreateImage(cvSize(WIDTH, HEIGHT), IPL_DEPTH_8U, 1);
		
		std::string	file_header;
		file_header.append("circle").append(Base::int2str(CIRCLE)).append(".")
			.append("ch").append(Base::int2str(CHANNEL)).append(".");
		const std::string avg_save = save_path + file_header + "avg.png";
		const std::string sigma_save = save_path + file_header + "sigma.png";

		cvConvert(avg, image_8U);
		cvSaveImage(avg_save.c_str(), image_8U);

		cvConvert(sigma, image_8U);
		cvSaveImage(sigma_save.c_str(), image_8U);

	cvReleaseImage(&image_8U);
	
}
#endif
#endif // 0


}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void output_binary_video_by_sigma_avg(std::string src_file,std::string out_file,IplImage* avg,IplImage* sigma,int sigma_scale,int TYPE)
{
	
	const int SIGMA = sigma_scale;
	const int WIDTH = avg->width;
	const int HEIGHT = avg->height;	
	std::string tail;
	if (TYPE == 0) {
		tail = ".bin.avi";
	}else if (TYPE == 1) {
		tail = ".BinGray.avi";
	}else {

	}
	const std::string binaryfilename = Base::base_name(src_file.c_str()) + ".sigma" + Base::int2str(SIGMA) + tail;
	const std::string binaryFileSavePath = out_file + binaryfilename;
	
	IplImage* image_gray = cvCreateImage(cvSize(WIDTH, HEIGHT), IPL_DEPTH_8U, 1);
	IplImage* image_gray_f = cvCreateImage(cvSize(WIDTH, HEIGHT), IPL_DEPTH_32F, 1);
	IplImage* image_gray_binary = cvCreateImage(cvSize(WIDTH, HEIGHT), IPL_DEPTH_8U, 1);
		
	CvCapture* capture = cvCaptureFromAVI(src_file.c_str());
		double FPS = cvGetCaptureProperty(capture, CV_CAP_PROP_FPS);
		int FOURCC_DIVX = CV_FOURCC('D', 'I', 'V', 'X');
		CvSize SIZE = cvSize((int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH), (int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT));
		assert(SIZE.width == WIDTH && SIZE.height == HEIGHT);
	CvVideoWriter* capture_W = cvCreateVideoWriter(binaryFileSavePath.c_str(), FOURCC_DIVX, FPS, SIZE, 0);
			assert(capture_W != NULL);
	

		

		for (int frame_count = 0; cvGrabFrame(capture) != NULL; frame_count++)
		{
			cv::Mat matimg = cvRetrieveFrame(capture);        // retrieve the captured frame
			IplImage iplimg = matimg.operator _IplImage();
			cvCvtColor(&iplimg, image_gray, CV_RGB2GRAY);
			cvConvert(image_gray, image_gray_f);

			if (TYPE==0){
			
				cvAbsDiff(image_gray_f, avg, image_gray_f);
				CvScalar scale_t = cvScalar(-1.0*SIGMA);
				cvScaleAdd(sigma, scale_t, image_gray_f, image_gray_f);
				cvThreshold(image_gray_f, image_gray_binary, 0, 255, CV_THRESH_BINARY);
			
			}else if (TYPE==1){
				
				cvAbsDiff(image_gray_f, avg, image_gray_f);
				CvScalar scale_t = cvScalar(-1.0*SIGMA);
				cvScaleAdd(sigma, scale_t, image_gray_f, image_gray_f);
				/*cvThreshold(image_gray_f, image_gray_binary, 0, 255, CV_THRESH_TOZERO);*/
				for (size_t wi = 0; wi <WIDTH; wi++){
					for (size_t hi = 0; hi <HEIGHT; hi++){

						float data_t = cvGetReal2D(image_gray_f, hi, wi);
						if (data_t > 0) {
							cvSetReal2D(image_gray_binary, hi, wi,cvGetReal2D(image_gray,hi,wi));
						}else{
							cvSetReal2D(image_gray_binary, hi, wi, cvGetReal2D(avg, hi, wi));
						}
						
					}
				}

			}else{
				
			}
			

			ASSERT(cvWriteFrame(capture_W, image_gray_binary));

		}

	cvReleaseImage(&image_gray);
	cvReleaseImage(&image_gray_f);
	cvReleaseImage(&image_gray_binary);

	cvReleaseCapture(&capture);
	cvReleaseVideoWriter(&capture_W);

}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void capture_frame_out(
	std::string file_base,
	std::string file_name,
	std::string svm_module,
	boolean SAVE_DATA_FLAG)
{
#if (CV_MAJOR_VERSION==2)&&(CV_MINOR_VERSION==4)
	CvSVM SvmTest;/**<支持向量机模型*/
	if (svm_module != "") {
		std::cout << "Now Load :" << svm_module << "as Svm Module" << std::endl;
		SvmTest.load(svm_module.c_str());
	}
#endif
	assert(Base::FS_checkUserPath(file_base));

	const std::string avi_1 = file_base + file_name;
		
	const int CHANNEL = Base::CRACK_FILE_NAME_get_channel(file_name);
	const int CIRCLE = Base::CRACK_FILE_NAME_get_circle(file_name);
	
	const std::string file_out_base = Base::FS_createPath(file_base, "out");
	const std::string file_out_circle = Base::FS_createPath(file_out_base,"circle" +Base::int2str(CIRCLE));
	const std::string file_out_channel = Base::FS_createPath(file_out_circle, "ch"+ Base::int2str(CHANNEL)  );


	int WIDTH, HEIGHT;
	ImageProcess::VIDEO_GetWidthHeight(avi_1, &WIDTH, &HEIGHT);
	

	
	
	IplImage* image_gray_avg = cvCreateImage(cvSize(WIDTH, HEIGHT), IPL_DEPTH_32F, 1);
	IplImage* image_gray_sigma = cvCreateImage(cvSize(WIDTH, HEIGHT), IPL_DEPTH_32F, 1);
	
	
	{
				get_video_avg_sigma(avi_1, file_out_channel, image_gray_avg,image_gray_sigma);

				GetFeature(avi_1, file_out_channel,CIRCLE,CHANNEL, image_gray_avg, image_gray_sigma,3);
#if FALSE
				output_binary_video_by_sigma_avg(avi_1, file_out_channel, image_gray_avg, image_gray_sigma, 3, 0);
				output_binary_video_by_sigma_avg(avi_1, file_out_channel, image_gray_avg, image_gray_sigma, 3, 1);
#endif // FALSE

	
	}

	cvReleaseImage(&image_gray_avg);
	cvReleaseImage(&image_gray_sigma);
	
	
	
	Base::sleep(1000);
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void capture_frame_statistics(
	std::string file_base,
	std::string file_name,
	std::string svm_module,
	boolean SAVE_DATA_FLAG)
{
#if (CV_MAJOR_VERSION==2)&&(CV_MINOR_VERSION==4)
	CvSVM SvmTest;/**<支持向量机模型*/
	if (svm_module != "") {
		std::cout << "Now Load :" << svm_module << "as Svm Module" << std::endl;
		SvmTest.load(svm_module.c_str());
	}
#endif
	assert(Base::FS_checkUserPath(file_base));
	
	const std::string avi_1 = file_base + file_name;
	int m = 0, n = 0, frame_count = 0;
	IplImage* image_gray = nullptr;
	CvCapture* capture = cvCaptureFromAVI(avi_1.c_str());
	const int CHANNEL = Base::CRACK_FILE_NAME_get_channel(file_name);
	const int CIRCLE = Base::CRACK_FILE_NAME_get_circle(file_name);
	std::vector<CvRect> rect_mask;	

	if (cvGrabFrame(capture)) {

		cv::Mat matimg = cvRetrieveFrame(capture);        // retrieve the captured frame
		IplImage iplimg = matimg.operator _IplImage();
		if (image_gray == nullptr) {
			image_gray = cvCreateImage(cvSize(iplimg.width, iplimg.height), IPL_DEPTH_8U, 1);
		}
		m = 1.0*iplimg.width;
		n = 1.0*iplimg.height;
		if (true) {
			init_mask_rect_2(file_base, rect_mask, CHANNEL, iplimg.width);
		}

	}

	IplImage* image_cut_statistic = cvCreateImage(cvSize(m, n), IPL_DEPTH_32F, 1);
	IplImage* image_cut_statistic_8U = cvCreateImage(cvSize(m, n), IPL_DEPTH_8U, 1);
	cvZero(image_cut_statistic);

	IplImage* image_cut_statistic_old = cvCloneImage(image_cut_statistic);
	IplImage* image_cut_statistic_diff = cvCloneImage(image_cut_statistic);
	IplImage* image_cut_statistic_diff_8U = cvCreateImage(cvSize(m, n), IPL_DEPTH_8U, 3);
	IplImage *image_gauss= cvCreateImage(cvSize(m, n), IPL_DEPTH_8U, 3);
	while (cvGrabFrame(capture)) {

		cv::Mat matimg = cvRetrieveFrame(capture);        // retrieve the captured frame
		IplImage iplimg = matimg.operator _IplImage();

		// convert 2 gray
		cvCvtColor(&iplimg, image_gray, CV_RGB2GRAY);	
		for (size_t i = 0; i <rect_mask.size(); i++) {
			if (rect_mask.at(i).x >= 0 && rect_mask.at(i).y >= 0) {
				cvRectangleR(image_gray, rect_mask.at(i), cvScalar(0), CV_FILLED);
			}		
		}
#if 0
		cvSmooth(image_gray, image_gray, CV_GAUSSIAN, 3, 3, 0, 0);
#endif // 0

		//convert 2 float
		cvConvert(image_gray, image_cut_statistic);
		//统计		
		

#if TRUE

		//差分
		if (frame_count == 0) {
			//第一次做差分
			cvZero(image_cut_statistic_diff);
		}
		else {
			//差分
			cvSub(image_cut_statistic, image_cut_statistic_old, image_cut_statistic_diff);
		}

#endif // TRUE
		std::vector<float> delta_out;
		
		std::vector<float> image_feature= ImageProcess::crack_get_image_feature_gauss(image_cut_statistic_diff, file_base,file_name, CIRCLE,CHANNEL, frame_count, image_gauss,delta_out, SAVE_DATA_FLAG);

#if TRUE
		//如果有学好的模型
		if (svm_module != "") {
			std::vector<float> feature_data = image_feature;
			CvMat m;
			cvInitMatHeader(&m, 1, feature_data.size(), CV_32FC1, feature_data.data());
			float category = SvmTest.predict(&m);
			if (category == 0) {
				//neg
			}
			else if (category == 1) {
				//pos
				cvCircle(image_cut_statistic_8U,
					cvPoint(image_cut_statistic_8U->width / 2, image_cut_statistic_8U->height / 2),
					3,
					CV_RGB(255, 255, 255), 2);
				{
					const std::string path_t = "svm_output_pos";
					std::string file_full_path_org = GetFrameChannelDiff(file_base, file_name, path_t);
					std::stringstream ss_image_name;
					ss_image_name << frame_count << ".jpg";
					std::string image_full_path = file_full_path_org + ss_image_name.str();
					cvSaveImage(image_full_path.c_str(), &iplimg);
				}
			}
			else {

			}
		}
		else {

		}
#endif // TRUE

#if TRUE
		cvConvert(image_cut_statistic, image_cut_statistic_8U);//float 转uchar
		cvConvertScale(image_cut_statistic_diff, image_cut_statistic_diff_8U, 0.5, 255.0 / 2);//float 转uchar

		if (SAVE_DATA_FLAG) {
			std::cout << "Save Frame : " << frame_count << std::endl;
			const std::string  diff = "diff";
			const std::string  org_gray = "org_gray";
			const std::string  org_rgb = "org_rgb";

			Save_Iplimage2DiskTXT(file_base, file_name, image_cut_statistic_diff, frame_count, diff);
			{
				std::string file_full_path_org = GetFrameChannelDiff(file_base, file_name, diff);
				std::stringstream ss_image_name;
				ss_image_name << frame_count << ".jpg";
				std::string image_full_path = file_full_path_org + ss_image_name.str();
				cvSaveImage(image_full_path.c_str(), image_cut_statistic_diff_8U);
			}
			Save_Iplimage2DiskTXT(file_base, file_name, image_cut_statistic, frame_count, org_gray);
			{
				std::string file_full_path_org = GetFrameChannelDiff(file_base, file_name, org_gray);
				std::stringstream ss_image_name;
				ss_image_name << frame_count << ".jpg";
				std::string image_full_path = file_full_path_org + ss_image_name.str();
				cvSaveImage(image_full_path.c_str(), image_cut_statistic_8U);
			}
#if TRUE
			{
				std::string file_full_path_org_rgb = GetFrameChannelDiff(file_base, file_name, org_rgb);
				std::stringstream ss_image_name;
				ss_image_name << frame_count << ".png";
				std::string image_full_path = file_full_path_org_rgb + ss_image_name.str();
				cvSaveImage(image_full_path.c_str(), &iplimg);
			}
#endif // TRUE


			
		}
		cvShowImage("diff_8U", image_cut_statistic_diff_8U);
		cvShowImage("m_n_gray_8U", image_cut_statistic_8U);
		cvShowImage("image_gauss", image_gauss);
		cvShowImage("org", &iplimg);
#endif // TRUE
#if TRUE
		//按键控制
		boolean PAUSE_FLAG = false;
		int key_data = cvWaitKey(10);
		if (key_data == 's') {
			std::cout << "Start Save Data !" << std::endl;
			SAVE_DATA_FLAG = true;
		}
		else if (key_data == 'e') {
			std::cout << "End Save Data !" << std::endl;
			SAVE_DATA_FLAG = false;
		}
		else if (key_data == 'p') {
			PAUSE_FLAG = !PAUSE_FLAG;
			if (PAUSE_FLAG == true) {
				std::cout << "now pause,input char 'c' to start run !" << std::endl;
			}
		}

		while (cvWaitKey(1) != 'c' &&  PAUSE_FLAG == true) {
			std::cout << "p";
		}
#endif

#if TRUE
		frame_count++;//end while
		cvCopyImage(image_cut_statistic, image_cut_statistic_old);
#endif // TRUE	
	}

	cvReleaseImage(&image_gauss);
	cvReleaseImage(&image_gray);
	cvReleaseImage(&image_cut_statistic);
	cvReleaseImage(&image_cut_statistic_8U);
	cvReleaseImage(&image_cut_statistic_old);
	cvReleaseImage(&image_cut_statistic_diff);
	cvReleaseImage(&image_cut_statistic_diff_8U);
	cvReleaseCapture(&capture);

}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void capture_frame_m_n_gray(std::string file_base,std::string file_name,const int m_step,const int n_step,std::string svm_module,boolean SAVE_DATA_FLAG)
{
#if (CV_MAJOR_VERSION==2)&&(CV_MINOR_VERSION==4)
	CvSVM SvmTest;/**<支持向量机模型*/
	if (svm_module != "") {
			
			std::cout << "Now Load :" << svm_module << "as Svm Module"<<std::endl;
			SvmTest.load(svm_module.c_str());
	}
#endif
	const std::string avi_1 = file_base + file_name;
	int m=0, n=0,frame_count=0;
	IplImage* image_gray = nullptr;
	CvCapture* capture = cvCaptureFromAVI(avi_1.c_str());
	const int CHANNEL=Base::CRACK_FILE_NAME_get_channel(file_name);
	std::vector<CvRect> rect_mask;
	
	if (cvGrabFrame(capture)) {

		cv::Mat matimg = cvRetrieveFrame(capture);        // retrieve the captured frame
		IplImage iplimg = matimg.operator _IplImage();
		if (image_gray == nullptr) {
			image_gray = cvCreateImage(cvSize(iplimg.width, iplimg.height), IPL_DEPTH_8U, 1);
		}
		m = 1.0*iplimg.width / m_step;
		n = 1.0*iplimg.height / n_step;
		if (false){
			init_mask_rect_2(file_base, rect_mask, CHANNEL, iplimg.width);
		}
		
	}

	
	
	IplImage* image_cut_statistic = cvCreateImage(cvSize(m, n), IPL_DEPTH_32F, 1);
	IplImage* image_cut_statistic_8U = cvCreateImage(cvSize(m, n), IPL_DEPTH_8U, 1);
	cvZero(image_cut_statistic);
	
	IplImage* image_cut_statistic_old = cvCloneImage(image_cut_statistic);
	IplImage* image_cut_statistic_diff = cvCloneImage(image_cut_statistic);
	IplImage* image_cut_statistic_diff_8U = cvCreateImage(cvSize(m, n), IPL_DEPTH_8U, 3);
	
	while (cvGrabFrame(capture)){
		
		cv::Mat matimg = cvRetrieveFrame(capture);        // retrieve the captured frame
		IplImage iplimg = matimg.operator _IplImage();
				
		cvCvtColor(&iplimg, image_gray,CV_RGB2GRAY);
		
		for (size_t i = 0; i <rect_mask.size(); i++){
			cvRectangleR(image_gray,rect_mask.at(i),cvScalar(0),CV_FILLED);
		}

		//统计
		ImageProcess::CuiResize(image_gray, image_cut_statistic,m_step,n_step);
		
#if TRUE
		
		//差分
		if (frame_count == 0) {
			//第一次做差分
			cvZero(image_cut_statistic_diff);
		}else{
			//差分
			cvSub(image_cut_statistic,image_cut_statistic_old,image_cut_statistic_diff);
		}

#endif // TRUE

#if TRUE
		//如果有学好的模型
		if (svm_module != "") {
			std::vector<float> feature_data=ImageProcess::crack_get_image_feature(image_cut_statistic_diff,"",frame_count);
			CvMat m;
			cvInitMatHeader(&m, 1, feature_data.size(), CV_32FC1, feature_data.data());
			float category = SvmTest.predict(&m);
				if (category == 0) {
					//neg
				}else if (category ==1){
					//pos
					cvCircle(image_cut_statistic_8U,
						cvPoint(image_cut_statistic_8U->width/2, image_cut_statistic_8U->height/2),
						3,
						CV_RGB(255, 255, 255), 2);
					{
						const std::string path_t = "svm_output_pos";
						std::string file_full_path_org = GetFrameChannelDiff(file_base, file_name,path_t);
						std::stringstream ss_image_name;
						ss_image_name << frame_count << ".jpg";
						std::string image_full_path = file_full_path_org + ss_image_name.str();
						cvSaveImage(image_full_path.c_str(), &iplimg);
					}
				}else{

				}
		}else{

		}
#endif // TRUE

#if TRUE
		cvConvert(image_cut_statistic, image_cut_statistic_8U, 1, 0);//float 转uchar
		cvConvertScale(image_cut_statistic_diff, image_cut_statistic_diff_8U, 0.5, 255.0 / 2);//float 转uchar
		
		if (SAVE_DATA_FLAG){
			std::cout << "Save Frame : " << frame_count<<std::endl;
			const std::string diff = "diff";
			const std::string  org_gray = "org_gray";
			
			Save_Iplimage2DiskTXT(file_base,file_name,image_cut_statistic_diff,frame_count,diff);
			{
					std::string file_full_path_org = GetFrameChannelDiff(file_base, file_name, diff);
					std::stringstream ss_image_name;
					ss_image_name << frame_count << ".jpg";
					std::string image_full_path = file_full_path_org + ss_image_name.str();
					cvSaveImage(image_full_path.c_str(), image_cut_statistic_diff_8U);
			}
			Save_Iplimage2DiskTXT(file_base, file_name, image_cut_statistic, frame_count,org_gray);
			{					
					std::string file_full_path_org=GetFrameChannelDiff(file_base, file_name, org_gray);
					std::stringstream ss_image_name;
					ss_image_name << frame_count << ".jpg";
					std::string image_full_path = file_full_path_org + ss_image_name.str();
					cvSaveImage(image_full_path.c_str(), image_cut_statistic_8U);
			}
			
		}
		cvShowImage("diff_8U", image_cut_statistic_diff_8U);
		cvShowImage("m_n_gray_8U", image_cut_statistic_8U);
		cvShowImage("gray", image_gray);
		cvShowImage("org", &iplimg);
#endif // TRUE
#if TRUE
		//按键控制
		boolean PAUSE_FLAG=false;
		int key_data = cvWaitKey(10);
		if (key_data == 's') {
			std::cout << "Start Save Data !"<<std::endl;
			SAVE_DATA_FLAG = true;
		}else if(key_data == 'e'){
			std::cout <<"End Save Data !"<<std::endl ;
			SAVE_DATA_FLAG = false;
		}else if (key_data == 'p') {
			PAUSE_FLAG = !PAUSE_FLAG;
			if (PAUSE_FLAG == true) {
				std::cout << "now pause,input char 'c' to start run !" << std::endl;
			}
		}

		while (cvWaitKey(1)!='c' &&  PAUSE_FLAG==true){
			std::cout << "p";
		}
#endif
#if TRUE
	frame_count++;//end while
	cvCopyImage(image_cut_statistic, image_cut_statistic_old);	
#endif // TRUE	
	} 

	cvReleaseImage(&image_gray);
	cvReleaseImage(&image_cut_statistic);
	cvReleaseImage(&image_cut_statistic_8U);
	cvReleaseImage(&image_cut_statistic_old);
	cvReleaseImage(&image_cut_statistic_diff);
	cvReleaseImage(&image_cut_statistic_diff_8U);
	cvReleaseCapture(&capture);

}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void init_rect(int idx,std::vector<CvRect>& rect_cut)
{
	int x_pos_0;//X 坐标
	int x_width;//X轴方向宽度
	int y_pos_0;//第一个矩形高
	int y_pos_1;//第二个矩形高
	const int y_step=25;//矩形0.5高
	const int WIDTH = 1920;
	const int HEIGHT = 1080;

	rect_cut.resize(0);

#if 0
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
#endif // 0
	if (idx == 1) {

		x_pos_0 = 680;
		x_width = 1456 - x_pos_0;
		y_pos_0 = 200;
		y_pos_1 = 950;

		

	}
	else if (idx == 2) {
		x_pos_0 = 727;
		x_width = 1389 - x_pos_0;
		y_pos_0 = 289;
		y_pos_1 = 841;
		
	}
	else if (idx == 3) {

		x_pos_0 = 599;
		x_width = 1469 - x_pos_0;
		y_pos_0 = 295;
		y_pos_1 = 927;
		
	}
	else if (idx == 4)
	{
		x_pos_0 = 498;
		x_width = 1046 - x_pos_0;
		y_pos_0 = 163;
		y_pos_1 = 773;
				
	}
	else if (idx == 5)
	{
		x_pos_0 = 613;
		x_width = 1487 - x_pos_0;
		y_pos_0 = 345;
		y_pos_1 = 857;

	}
	else if (idx == 6)
	{
		x_pos_0 = 535;
		x_width = 1685 - x_pos_0;
		y_pos_0 = 271;
		y_pos_1 = 781;

	}
	else if (idx == 7){
		x_pos_0 = 506;
		x_width = 1132 - x_pos_0;
		y_pos_0 = 423;
		y_pos_1 = 932;

	}else if (idx == 10000){
		x_pos_0 = 1;
		x_width = 1919 - x_pos_0;
		y_pos_0 = 90;
		y_pos_1 = 1000;
	}else{
		assert(0);
	}
	

	assert(x_pos_0>0   && x_pos_0<WIDTH);
	assert(x_width > 0 && x_width < WIDTH);
	assert(y_pos_0 > 0 && y_pos_0 < HEIGHT);
	assert(y_pos_1 > 0 && y_pos_1 < HEIGHT);
	

	rect_cut.push_back(cvRect(x_pos_0,//
		y_pos_0 - y_step,
		x_width,//
		y_step * 2));

	rect_cut.push_back(cvRect(x_pos_0,//
		y_pos_1 - y_step,
		x_width,//
		y_step * 2));

}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void init_mask_rect() 
{
	//x ,y0,y1,width,height
	CvMat* mat = cvCreateMat(30, 5, CV_32FC1);
	CvScalar value_tt;
	for (size_t ri = 0; ri <mat->rows; ri++){
				
			int x = 0, y0 = 150, y1 = 300, width = 0, height = 20;

#if 0
// 2018年1月10日17:23:56	
			if (ri == 1) {
					y0 = 180; y1= -1; height = 75;
				}
				else if (ri == 2) {
					y0 = 195; y1= -1; height = 75;
				}
				else if (ri == 3) {
					y0 = 175; y1 = -1; height = 75;
				}
				else if (ri == 4) {
					y0 = 195; y1 = -1; height = 75;
				}
				else if (ri == 5) {
					y0 = 215; y1 = -1; height = 75;
				}
				else if (ri == 6) {
					y0 = 215; y1 = -1; height = 75;
				}
				else if (ri == 7) {
					y0 = 240; y1 = -1; height = 75;
				}
				else {
					x = y0 = y1 = width = height = -1;
				}
#endif // 1
#if 1
				// 2018年1月10日17:23:56	
				if (ri == 1) {
					y0 = 160; y1 = -1; height = 135;
				}
				else if (ri == 2) {
					y0 = 130; y1 = -1; height = 115;
				}
				else if (ri == 3) {
					y0 = 125; y1 = -1; height = 125;
				}
				else if (ri == 4) {
					y0 = 130; y1 = -1; height = 130;
				}
				else if (ri == 5) {
					y0 = 120; y1 = -1; height = 85;
				}
				else if (ri == 6) {
					y0 = 105; y1 = -1; height = 105;
				}
				else if (ri == 7) {
					y0 = 120; y1 = -1; height = 85;
				}
				else {
					x = y0 = y1 = width = height = -1;
				}
#endif // 1
				
				CV_MAT_ELEM(*mat, float, ri,0) = x;
				CV_MAT_ELEM(*mat, float, ri, 1) = y0;
				CV_MAT_ELEM(*mat, float, ri, 2) = y1;
				CV_MAT_ELEM(*mat, float, ri, 3) = width;
				CV_MAT_ELEM(*mat, float, ri, 4) = height;
						
	}

	cvSave("mask_x_y0_y1_width_height.xml",mat);

	cvReleaseMat(&mat);

}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void cut_video_detection(std::string file_base,std::vector<std::string> files)
{
	std::vector<CvRect> rect_cut;

	//cvNamedWindow("org",CV_WINDOW_AUTOSIZE);
	cvNamedWindow("org", CV_WINDOW_NORMAL);

#if 0
	//cvNamedWindow("draw line", CV_WINDOW_NORMAL);
	//cvNamedWindow("binary", CV_WINDOW_NORMAL);
	//cvNamedWindow("gray", CV_WINDOW_NORMAL);
#endif // 0
		
	for (size_t i = 0; i <files.size(); i++)
	{
		
		int idx = Base::CRACK_FILE_NAME_get_channel(files.at(i));

		init_rect(idx, rect_cut);

		capture_frame(file_base,			
			Base::base_name(files.at(i)),
			rect_cut);
		 //	cvWaitKey(10 * 1000);
	}

}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void get_data_m_n_gray(std::string file_base, std::vector<std::string> files,std::string svm_module,boolean flag)
{
	cvNamedWindow("org", CV_WINDOW_NORMAL);
	cvNamedWindow("gray", CV_WINDOW_NORMAL);
	cvNamedWindow("m_n_gray_8U", CV_WINDOW_NORMAL);
	cvNamedWindow("diff_8U", CV_WINDOW_NORMAL);

	for (size_t i = 0; i <files.size(); i++){

		std::cout << "Start File Base: " << file_base << std::endl;
		std::cout << "Start File: " << files.at(i) << std::endl;
		capture_frame_m_n_gray(file_base,files.at(i),3,3,svm_module,flag);
				
	}

}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void process_video_out_data(std::string file_base, std::vector<std::string> files, std::string svm_module, boolean flag)
{
	/*cvNamedWindow("org", CV_WINDOW_NORMAL);
	cvNamedWindow("gray", CV_WINDOW_NORMAL);
	cvNamedWindow("diff_8U", CV_WINDOW_NORMAL);*/
	init_mask_rect();

	for (size_t i = 0; i <files.size(); i++) {
		
		std::cout << "Start File Base: " << file_base << std::endl;
		std::cout << "Start File: " << files.at(i) << std::endl;
		capture_frame_out(file_base,
			Base::base_name(files.at(i)),
			svm_module,
			flag);

	}

}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void adjust_video_by_dot_per_inch()
{
	std::cout << "no need to adjust" << std::endl;
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
int get_feature_by_path(std::string path_sample,std::string path_org,int neg_or_pos,std::vector<float>& features,std::vector<INT32>& classifies,int ch)
{
	int feature_dimension=-1;
	int ch_max;
	int ch_min;
	if (ch > 0) {
		ch_min = ch;
		ch_max = ch+1;		
	}else{
		ch_min = 1;
		ch_max = 50;
	}
		
	const boolean SAVE_FLAG = false;

	std::cout << "Sample Path:  " <<path_sample <<std::endl;
	std::cout << "Scan Feature Start From " << ch_min << " To " << ch_max-1 << std::endl;
	
	std::vector<std::string> files_circles_path;

	Base::FS_getDirs(path_sample,"circle", files_circles_path );
	
	for (size_t cfi = 0; cfi < files_circles_path.size(); cfi++){
		
		std::string circle_path = files_circles_path.at(cfi);
		const int CIRCLE =Base::CRACK_FILE_NAME_get_circle_byPath(circle_path);
		std::cout << "Current Circle:" << CIRCLE << std::endl;	//目录有效
		std::string org_circle_path=Base::FS_createPath(path_org, Base::FS_getDirName(circle_path),SAVE_FLAG);
		assert(CIRCLE != 0);
#if TRUE
		for (size_t chi = ch_min; chi <ch_max; chi++){
			std::stringstream channel_diff_ss;
			channel_diff_ss << "ch" << chi << "diff";
			std::stringstream channel_diff_cvmat_ss;
			channel_diff_cvmat_ss << "ch" << chi << "diff_cvmat";

			const std::string channel_diff_str = Base::FS_createPath(circle_path, channel_diff_ss.str(), SAVE_FLAG);
			const std::string org_channel_diff_cvmat_str = Base::FS_createPath(org_circle_path, channel_diff_cvmat_ss.str(), SAVE_FLAG);
			
			
			if (access(channel_diff_str.c_str(), 0) == 0) {
				std::vector<std::string> files;
				Base::FS_getFiles(channel_diff_str, "jpg", files);				

				std::cout << "Current Channel:" << chi << std::endl;	//目录有效
				for (size_t i = 0; i <files.size(); i++) {
					std::string file_name = Base::file_name_without_ext(files.at(i));
					int    frame_count;
					Base::str2int(frame_count, file_name);
					std::cout << "Current Channel :" << chi << "File Name :" << file_name << std::endl;	//目录有效
					std::string mat_name = file_name + ".xml";
					std::string mat_full_name = org_channel_diff_cvmat_str + "\\" + mat_name;

					CvMat *Cv_mat = (CvMat*)cvLoad(mat_full_name.c_str());
					assert(Cv_mat != nullptr);
					IplImage *Ipl_diff_img = cvCreateImage(cvSize(Cv_mat->width, Cv_mat->height), IPL_DEPTH_32F, 1);
					cvConvert(Cv_mat, Ipl_diff_img);

					//std::vector<float> feature_one= ImageProcess::crack_get_image_feature(Ipl_img, path_feature_path,frame_count);

					std::vector<float> delta_out;
					IplImage *image_gauss = cvCreateImage(cvGetSize(Ipl_diff_img), IPL_DEPTH_8U, 3);
					std::vector<float> feature_one = ImageProcess::crack_get_image_feature_gauss(Ipl_diff_img, path_sample, file_name, CIRCLE, chi, frame_count, image_gauss, delta_out, true);
					cvReleaseImage(&image_gauss);

					feature_dimension = feature_one.size();
					features.insert(features.end(), feature_one.begin(), feature_one.end());
					classifies.push_back(neg_or_pos);

					cvReleaseImage(&Ipl_diff_img);
					cvReleaseMat(&Cv_mat);
				}

			}

		}
#endif // TRUE

	}
 
	return feature_dimension;
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void test_feature_by_path(std::string path_sample, std::string path_org, int neg_or_pos, std::string svm_module,int ch)
{
	const bool SAVE_FLAG = true;
	int ch_max;
	int ch_min;
	if (ch > 0) {
		ch_min = ch;
		ch_max = ch + 1;
	}
	else {
		ch_min = 0;
		ch_max = 50;
	}

	CvSVM SvmTest;/**<支持向量机模型*/
	if (svm_module != "") {
		SvmTest.load(svm_module.c_str());
	}
		
	std::stringstream ss_svm_out;
	ss_svm_out << path_sample<<"\\"<<"svm_test_result"<<neg_or_pos<<".txt";
	const std::string myfile_path = ss_svm_out.str();

	std::ofstream myfile(myfile_path);

	const int CIRCLE = 0;
	assert(CIRCLE == 0);


	
	std::vector<std::string> files_circles_path;
	Base::FS_getDirs(path_sample, "circle", files_circles_path);

	for (size_t cfi = 0; cfi < files_circles_path.size(); cfi++) {
		std::string circle_path = files_circles_path.at(cfi);
		const int CIRCLE = Base::CRACK_FILE_NAME_get_circle_byPath(circle_path);
		std::cout << "Current Circle:" << CIRCLE << std::endl;	//目录有效
		std::string org_circle_path = Base::FS_createPath(path_org, Base::FS_getDirName(circle_path), 0);
		assert(CIRCLE != 0);
#if TRUE
	for (size_t chi = ch_min; chi <ch_max; chi++)
	{

		std::stringstream channel_diff_ss;
		channel_diff_ss << "ch" << chi << "diff";
		std::stringstream channel_diff_cvmat_ss;
		channel_diff_cvmat_ss << "ch" << chi << "diff_cvmat";
				
		const std::string channel_diff_str = Base::FS_createPath(circle_path, channel_diff_ss.str(), 0);

		const std::string channel_diff_histogram_str = Base::FS_createPath(channel_diff_str,"histogram");
		const std::string path_hist_right= Base::FS_createPath(channel_diff_histogram_str,"test_right");
		const std::string path_hist_wrong = Base::FS_createPath(channel_diff_histogram_str, "test_wrong");

		const std::string channel_diff_cvmat_str = Base::FS_createPath(circle_path, channel_diff_cvmat_ss.str(), 0);		
		const std::string org_channel_diff_cvmat_str = Base::FS_createPath(org_circle_path, channel_diff_cvmat_ss.str(), 0);
		const std::string org_channel_diff_str = Base::FS_createPath(org_circle_path, channel_diff_ss.str(), 0);
		const std::string org_channel_diff_histogram_str = Base::FS_createPath(org_channel_diff_str, "histogram", 0);
	
		if (access(channel_diff_str.c_str(), 0) == 0) {
			//目录有效
			std::vector<std::string> files;
			Base::FS_getFiles(channel_diff_str, "jpg", files);

			for (size_t i = 0; i <files.size(); i++) {
				
				const std::string file_name = Base::file_name_without_ext(files.at(i));
				int    frame_num;
				Base::str2int(frame_num, file_name);

				std::string mat_name = file_name + ".xml";
				std::string mat_full_name = org_channel_diff_cvmat_str + "\\" + mat_name;

				CvMat *Cv_mat = (CvMat*)cvLoad(mat_full_name.c_str());
				assert(Cv_mat != nullptr);
				IplImage *Ipl_diff_img = cvCreateImage(cvSize(Cv_mat->width, Cv_mat->height), IPL_DEPTH_32F, 1);
				cvConvert(Cv_mat, Ipl_diff_img);
				
#if 0
std::vector<float> feature_one = ImageProcess::crack_get_image_feature(Ipl_img, path_matrix_str, frame_num);
#endif // 0
#if TRUE
			std::vector<float> delta_out;
			IplImage *image_gauss = cvCreateImage(cvGetSize(Ipl_diff_img), IPL_DEPTH_8U, 3);
			std::vector<float> feature_one = ImageProcess::crack_get_image_feature_gauss(Ipl_diff_img, path_sample,file_name,CIRCLE, chi, frame_num, image_gauss, delta_out,false);
			cvReleaseImage(&image_gauss);
#endif // TRUE

				CvMat m;
				cvInitMatHeader(&m, 1, feature_one.size(), CV_32FC1, feature_one.data());

				if (svm_module != "") {
					float category_t=SvmTest.predict(&m);
#if TRUE
					const std::string file_full_name=file_name+".png";
					const std::string hist_src = org_channel_diff_histogram_str + file_full_name;
					std::string hist_dst;
					if (category_t==neg_or_pos){
						hist_dst= path_hist_right+ file_full_name;
					}else {
						hist_dst=path_hist_wrong+ file_full_name;
					}

					Base::FS_copyFile(hist_src,hist_dst);
#endif // TRUE
					myfile<<i<<" " 
						  << files.at(i) << "  "
						  << neg_or_pos << "  "
						  << category_t << "  ";
					if(category_t == neg_or_pos) {
						myfile<<"Y"<<" ";
					}else{
						myfile << "N" << " ";
					}

					for (size_t i = 0; i <feature_one.size(); i++){
						myfile <<feature_one.at(i)<< " ";
					}

					myfile << std::endl;
				}

				cvReleaseImage(&Ipl_diff_img);
				cvReleaseMat(&Cv_mat);
			}

		}

	}
#endif // TRUE	
	
	}





	myfile.close() ;
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void SaveFeature2Txt(std::vector<float> features,std::vector<INT32>  classifies,int feature_dimension,std::string file_base)
{

	assert(features.size() == classifies.size()*feature_dimension);
#if TRUE
	std::stringstream all_filename;
	all_filename <<file_base<<"\\" << "feature_"<< classifies.size()<<".txt";//样本个数2017年11月10日09:16:32
	std::string output_full_name =   all_filename.str();
	std::ofstream outfile;
	outfile.open(output_full_name); //myfile.bat是存放数据的文件名  

	outfile << std::setiosflags(std::ios::scientific);

	if (outfile.is_open()) {
		
		for (size_t i = 0; i <classifies.size(); i++){


			for (size_t vi = 0; vi <feature_dimension; vi++){
				outfile << features.at(i*feature_dimension+vi)<<" ";
			}			
			outfile << classifies.at(i)<<std::endl;
		}

		outfile.close();
	}
	else {
		std::cout << "不能打开文件!" << std::endl;
	}
#endif // TRUE
	std::cout <<"Save Feature Txt at : "<<output_full_name<<std::endl;
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
std::vector<float> GetFeatureByImage(const std::string _org_base, const std::string _imageName);
void do_svm_line_test(std::string org_path,std::string test_path, std::string svm_path)
{
	CvSVM svm_test;

	svm_test.load(svm_path.c_str());

	std::string test_pos_path = Base::FS_createPath(test_path, "sample.pos");
	std::string test_neg_path = Base::FS_createPath(test_path, "sample.neg");

	std::vector<std::string> test_pos_files;
	std::vector<std::string> test_neg_files;

	Base::FS_getFiles(test_pos_path, "png", test_pos_files);
	Base::FS_getFiles(test_neg_path, "png", test_neg_files);

	std::string temp = test_path;
	std::string predict_result = temp.append("predict.result.txt");



	int TP = 0;
	int FP = 0;
	int FN = 0;
	int TN = 0;

	std::ofstream outfile;
	outfile.open(predict_result); //myfile.bat是存放数据的文件名  

	outfile << std::setiosflags(std::ios::scientific);

	if (outfile.is_open()) {

		{

			for (size_t tpi = 0; tpi <test_pos_files.size(); tpi++) {

				std::vector<float> feature_test = GetFeatureByImage(org_path,test_pos_files.at(tpi));
				CvMat m;
				cvInitMatHeader(&m, 1, feature_test.size(), CV_32FC1, &feature_test[0]);
				const int cate_t = svm_test.predict(&m);


				outfile << test_pos_files.at(tpi) << " "
					<< "1" << " "
					<< cate_t << " "
					<< (1 == cate_t) << " " << std::endl;
				if (1 == cate_t) {
					TP++;
				}
				else {
					FN++;
				}



			}

			for (size_t tni = 0; tni <test_neg_files.size(); tni++) {

				std::vector<float> feature_test = GetFeatureByImage(org_path,test_neg_files.at(tni));
				CvMat m;
				cvInitMatHeader(&m, 1, feature_test.size(), CV_32FC1, &feature_test[0]);
				const int cate_t = svm_test.predict(&m);

				outfile << test_neg_files.at(tni) << " "
					<< "-1" << " "
					<< cate_t << " "
					<< (-1 == cate_t) << " " << std::endl;

				if (-1 == cate_t) {
					TN++;
				}
				else {
					FP++;
				}

			}

		}

		outfile << "Pre= " << 1.0*TP / (TP + FP) << " "
			<< "Rec= " << 1.0*TP / (TP + FN) << " " << std::endl;
		outfile.close();
	}
	else {
		std::cout << "不能打开文件!" << std::endl;

	}
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
std::vector<float> GetFeatureByImage(const std::string _org_base,const std::string _imageName)
{
	std::string file_name = Base::base_name(_imageName);
	std::vector<float> feature_r;
	const int CIRCLE=Base::CRACK_FILE_NAME_get_circle(_imageName);
	const int CHANNEL=Base::CRACK_FILE_NAME_get_channel(_imageName);
	const int FRAME = Base::CRACK_FILE_NAME_get_FRAME(_imageName);;

	std::string circle_str = "circle"; circle_str.append(Base::int2str(CIRCLE));
	std::string ch_str = "ch"; ch_str.append(Base::int2str(CHANNEL));
	std::string frame_str = "frame"; frame_str.append(Base::int2str(FRAME));

	int pos_t = Base::CRACK_FILE_NAME_get_PROPERTY(_imageName, ".pos");

	const std::string circle_path = Base::FS_createPath(_org_base, circle_str, 0);
	const std::string ch_path = Base::FS_createPath(circle_path, ch_str, 0);
	const std::string frame_path = Base::FS_createPath(ch_path,"frame.sample.feature",0);

	 std::string file_name_f;
	file_name_f.append("circle").append(Base::int2str(CIRCLE)).append(".ch").append(Base::int2str(CHANNEL)).append(".frame").append(Base::int2str(FRAME));
	if (pos_t==TRUE){
		file_name_f.append(".pos.mat.xml");
	}
	else
	{
		file_name_f.append(".neg.float.xml");
	}

	const std::string feature_full_path = frame_path+file_name_f;

	CvMat *Mf = (CvMat*)cvLoad(feature_full_path.c_str());
	ASSERT(Mf != NULL);
	for (size_t ci = 0; ci <Mf->cols; ci++){
		float data_t=cvGetReal2D(Mf, 0, ci);
		feature_r.push_back(data_t);
	}

	cvReleaseMat(&Mf);

	return feature_r;
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void extract_test_set_from_sample(std::string test_base,std::vector<std::string> data,int SAMPLE_SCALE,std::string PN) 
{

	ASSERT(data.at(0).find(PN) != std::string::npos);
	//测试样本抽取
	const unsigned long  SAMPLE_NUM= data.size();
	const unsigned long  STEP =SAMPLE_SCALE;
	const std::string dst_path = Base::FS_createPath(test_base,PN);
	for (unsigned int i = 0; i < SAMPLE_NUM; i+= STEP) {
	
		const std::string file_full_path_src = data.at(i);
		const std::string file_name = Base::base_name(file_full_path_src);

		const std::string dst_file_full_path=dst_path + file_name;

		ASSERT(Base::FS_copyFile(file_full_path_src, dst_file_full_path));
		Base::FS_deleteFile(file_full_path_src);
	
	}
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void do_svm_20180319161932(std::string base)
{
	const std::string org_path = Base::FS_createPath(base, "sample.org");
	const std::string pos_path = Base::FS_createPath(base, "sample.pos");
	const std::string neg_path = Base::FS_createPath(base, "sample.neg");
	const std::string test_path = Base::FS_createPath(base, "sample.test");

	std::vector<std::string> pos_files;
	std::vector<std::string> neg_files;

	Base::FS_getFiles(pos_path, "png", pos_files);
	Base::FS_getFiles(neg_path, "png", neg_files);

	const unsigned long  pos_num = pos_files.size();
	const unsigned long  neg_num = neg_files.size();
	

	int SAMPLE_SCALE = 10;
	float neg_step_f=0;
	float neg_num_f = 0;
	ASSERT(pos_num != 0 && neg_num != 0);

	if (neg_num>pos_num * 10  ) {
		neg_num_f =  pos_num * 10;
		neg_step_f = neg_num/(neg_num_f);
	
	}
	else
	{
		neg_num_f = neg_num;
		neg_step_f = neg_num / (neg_num_f);
	}

	std::vector<float> features;
	std::vector<INT32>  classifies;

	std::string temp = base;

#if TRUE
	int feature_dimension = 4;
	const int Gaussian = 1;
	const int Liner = 0;
	const int POLY = 2;
	const int CURRENT_KERNAL = Gaussian;
	const std::string svmsavepath = temp.append("svm_module").append(Base::int2str(pos_files.size() + neg_files.size()))
		.append(".Kernal").append(Base::int2str(CURRENT_KERNAL))
		.append(".Out")
		.append(".xml");
#endif // TRUE





	int STEP = 0;

	std::cout << "Please Input Execute Item:" << std::endl;
	std::cout << "1 process data learn test ! " << std::endl;
	std::cout << "2  learn ! " << std::endl;
	std::cout << "3  test ! " << std::endl;

	std::cin >> STEP;

	if (STEP == 1) {

		for (unsigned long pi = 0; pi <pos_files.size(); pi++) {

			std::vector<float> feature_one = GetFeatureByImage(org_path,pos_files.at(pi));
			feature_dimension = feature_one.size();
			features.insert(features.end(), feature_one.begin(), feature_one.end());
			classifies.push_back(1);

			if (pi % 100 == 0) {
				std::cout << "pos" << 1.0*pi / pos_files.size() << std::endl;
			}

		}

		for (float ni = 0; ni <neg_num; ni++) {

			std::vector<float> feature_one = GetFeatureByImage(org_path, neg_files.at(ni));
			feature_dimension = feature_one.size();
			features.insert(features.end(), feature_one.begin(), feature_one.end());
			classifies.push_back(-1);

			if (int(ni)% 100 == 0) {
				std::cout << "neg" << 1.0*ni / neg_files.size() << std::endl;
			}

		}

		SaveFeature2Txt(features, classifies, feature_dimension, base);

		STEP = 2;
	}

	if (STEP == 2) {

		ImageProcess::Svm_Lean(features, feature_dimension, classifies, CURRENT_KERNAL, svmsavepath, MODULE_XML);
		STEP = 3;
	}

	if (STEP == 3) {
#if 0
			extract_test_set_from_sample(test_path,pos_files,SAMPLE_SCALE,"sample.pos");
			extract_test_set_from_sample(test_path,neg_files,SAMPLE_SCALE,"sample.neg");
#endif // 0


		do_svm_line_test(org_path,test_path, svmsavepath);

	}

}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void do_svm()
{
	const std::string path_base_0 = "E:\\20180111out\\video-out-cal-cut-svm2\\";
	std::vector<std::string> path_base;
	std::vector<std::string> path_pos;
	std::vector<std::string> path_neg;
	std::vector<std::string> path_org;
	
	path_base.push_back(path_base_0);
	
	for (size_t i = 0; i < path_base.size(); i++){
		
		std::string file_base=path_base.at(i);
		if (
#if _WIN64 || _WIN32 || _MSC_VER
			access(file_base.c_str(), 0) == 0
#endif
			) {				
			path_pos.push_back(file_base+"sample.pos");
			path_neg.push_back(file_base+"sample.neg");
			path_org.push_back(file_base +"sample.org");
		}


	}

std::vector<float> features;
std::vector<INT32>  classifies;

std::string input_path;

#if FALSE

do {
	std::cout << "please input pos sample path: !" << std::endl;
	std::cout << "input q to exit !" << std::endl;
	std::cin >> input_path;
	if (
#if _WIN64 || _WIN32 || _MSC_VER
		access(input_path.c_str(), 0) == 0
#endif
		) {
		std::cout << "this is a path !" << std::endl;
		path_pos.push_back(input_path);

	}
	else {
		std::cout << "not a path !" << std::endl;
	}

} while (input_path.compare("q") != 0);
#endif // TRUE

#if FALSE
do {
	std::cout << "please input neg sample path: !" << std::endl;
	std::cout << "input q to exit !" << std::endl;
	std::cin >> input_path;
	if (
#if _WIN64 || _WIN32 || _MSC_VER
		access(input_path.c_str(), 0) == 0
#endif
		) {
		std::cout << "this is a path !" << std::endl;
		path_neg.push_back(input_path);

	}
	else {
		std::cout << "not a path !" << std::endl;

	}

} while (input_path.compare("q") != 0);
#endif//TRUE


#if TRUE
	
	int ChNum = -1;

	std::cout << "Please Input SVM Channel Numbwer:" << std::endl;

	std::cin >> ChNum;
	
	int ch_max;
	int ch_min;

	if (ChNum < 0) {
	//所以通道一起做
		ch_min = -1;
		ch_max = ch_min+1;
		std::cout << "Once" << std::endl;
	}else if (ChNum == 0) {
	//所以通道单独做
		ch_min = 1;
		ch_max = 50;
		std::cout << "All Channel Independent" << std::endl;
	}else if (ChNum > 0) {
	//每个通道单独做
		ch_min = ChNum;
		ch_max = ch_min + 1;
		std::cout << "single Channel" << std::endl;
	}else {
		assert(0);
	}

#endif // TRUE


std::cout << "Start Read Sample" << std::endl;

for (int chi = ch_min; chi <ch_max; chi++)
{
	int feature_dimension=0;
	features.clear();
	classifies.clear();	

#if TRUE
for (size_t i = 0; i < path_pos.size(); i++) {
	feature_dimension = get_feature_by_path(path_pos.at(i), path_org.at(i), 1, features, classifies,chi);
}

if (classifies.size() == 0) {
	continue;
}

for (size_t i = 0; i < path_neg.size(); i++) {
	feature_dimension = get_feature_by_path(path_neg.at(i), path_org.at(i), 0, features, classifies,chi);
}
#endif // TRUE

#if TRUE
const int Gaussian = 1;
const int Liner = 0;
std::stringstream ss;

if (chi == -1) {
	ss << "X://SvmModuleCh." << "All" << ".xml";
}else {
	ss << "X://SvmModuleCh." << chi << ".xml";
}

const	std::string svmsavepath = ss.str();

ImageProcess::Svm_Lean(features, feature_dimension, classifies,Gaussian, svmsavepath, MODULE_XML);
SaveFeature2Txt(features, classifies, feature_dimension, path_base.at(0));
#endif // TRUE

#if TRUE
for (size_t i = 0; i < path_pos.size(); i++) {
	test_feature_by_path(path_pos.at(i), path_org.at(i), 1, svmsavepath,chi);
}

for (size_t i = 0; i < path_neg.size(); i++) {
	test_feature_by_path(path_neg.at(i), path_org.at(i), 0, svmsavepath,chi);
}
#endif // TRUE
}






#if FALSE

	std::string   module_path = path_base.at(0)+"//"+"box_out.txt";

	ofstream outfile;
			 outfile.open(module_path); //myfile.bat是存放数据的文件名  
			 outfile << setiosflags(ios::scientific);
	float  STEP = 0.01;
	
	CvSVM svm_test;
	svm_test.load(svmsavepath.c_str());

	
CvMat m;
for (float i0 = 0; i0 <=1; i0+=STEP){
	for (float i1 = 0; i1 <= 1; i1 += STEP) {
		for (float i2 = 0; i2 <= 1; i2 += STEP) {
			for (float i3 = 0; i3 <= 1; i3 += STEP) {
				
				float feature_test[] = {i0,i1,i2,i3};
				
				cvInitMatHeader(&m, 1, sizeof(feature_test)/sizeof(float), CV_32FC1, &feature_test[0]);
				const float cate_t = svm_test.predict(&m);

				outfile << i0<<" " 
					<< i1 << " "
					<< i2 << " "
					<< i3 << " "
					<< cate_t<<std::endl;



			}
		}
	}
}

outfile.close();
#endif // TRUE


}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
void do_svm_test(std::string file_base, std::vector<std::string> files)
{
	std::string input_path;

	do {
		std::cout << "Please Input SVM module path :" << std::endl;

		std::cin >> input_path;

		if (
#if _WIN64 || _WIN32 || _MSC_VER
			access(input_path.c_str(), 0) == 0
#endif
			) {

			std::cout << "this is a path ! Now Start Test :" << std::endl;
			
			process_video_out_data(file_base, files,input_path,false);
			
		}
	} while (input_path != "q");

}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
int main(int argc, char *argv[])
{
	int EXEC_RESULT = 0;

	std::string file_base = "E:\\20180111out\\";
	std::string file_base_cal = Base::FS_createPath(file_base, "video-out-cal", 0);

	std::vector<std::string> files_cal;
	Base::FS_getFiles(file_base_cal,"avi" , files_cal);

	std::string file_base_cut = Base::FS_createPath(file_base, "video-out-cal-cut", 0);
	std::vector<std::string> files_cut;
	Base::FS_getFiles(file_base_cut, "avi", files_cut);

	std::string file_base_test;
	std::vector<std::string> files_test;

	int test_num = 0;

	std::cout << "Please Input Execute Item:" << std::endl;
	std::cout << "1 Cut Video! " << std::endl;
	std::cout << "2 Process Video Data ! " << std::endl;
	std::cout << "3 adjust video by size(dot per  inch) ! " << std::endl;
	std::cout << "4 SVM ! " << std::endl;
	std::cout << "5 SVM test !" << std::endl;
	std::cout << "6 undefined !" << std::endl;

	std::cin >> test_num;

	if (test_num == 1) {

 		cut_video_detection(file_base_cal,files_cal);//cut Video

	}else if (test_num == 2) {

		process_video_out_data(file_base_cut, files_cut,"", true);//process video data		

	}else if (test_num == 3) {

		adjust_video_by_dot_per_inch();

	}else if (test_num == 4){
#if 0
		do_svm();//svm
#endif // 0

		do_svm_20180319161932("E:\\20180111out\\video-out-cal-cut-svm2\\");
	
	}else if(test_num == 5){

		do_svm_test(file_base_test,files_test);//svm test

	}else {

	}

	std::cin.get();


		
	return  EXEC_RESULT;
}
