#include "StdAfx.h"
#include "calibration.hpp"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
calibration::calibration(ICamera*  camera_t)

{
	this->m_camera.reset(camera_t);
	m_read_thread_run = true;
	m_pt_handle.p=NULL;
}
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
calibration::calibration(void)
{
	
	m_read_thread_run = true;
	m_pt_handle.p = NULL;

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
calibration::~calibration(void)
{
	
	m_read_thread_run=false;

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void calibration::init()
{
	m_read_thread_run = true;
	m_camera->init();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void calibration::Join()
{
	if (m_pt_handle.p != NULL) {
		int ret = pthread_join(m_pt_handle, NULL);
		m_pt_handle.p = NULL;
	}

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void calibration::release()
{	
	this->m_read_thread_run = false;
	m_camera->release();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void calibration::stop()
{
	this->m_read_thread_run = false;
	
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void calibration::start(int routine)
{
	m_read_thread_run = true;
	CAL_ROUTINE = routine;

	int ret = pthread_create(&m_pt_handle, NULL, run,this);

	if (ret != 0)
	{
		std::cout << "Create pthread error!" << std::endl;
		ASSERT(ret != 0);
	}else {
		std::cout << "Create pthread Success!" << std::endl;
	}

	
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/

/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/

/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
boolean calibration::IsThreadRun()
{
	return m_read_thread_run;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void *  calibration::run(void * data)
{
	calibration * cal = (calibration*)data;
	
	cal->init();
	
	while (cal->IsThreadRun()) {
		//////////////////////////////////////////////////////////////////

		if (cal->CAL_ROUTINE) {
			cal->Calculate_Intrinsics_Distortion(cal->m_camera.get(),board_w,board_h,num_board_is_use, per_frame);
			cal->CAL_ROUTINE = FALSE;
		}
		cal->calibration_image(cal->m_camera.get());
	}


	cal->release();


	return NULL;

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void calibration::Calculate_Intrinsics_Distortion(ICamera*  camera, int board_w, int board_h, int n_boards, int board_dt) {


	int board_n = board_w * board_h;
	CvSize board_sz = cvSize(board_w, board_h);


	cvNamedWindow("Calibration", CV_WINDOW_NORMAL);
	cvNamedWindow("Raw Video", CV_WINDOW_NORMAL);
	//ALLOCATE STORAGE
	CvMat* image_points = cvCreateMat(n_boards*board_n, 2, CV_32FC1);
	CvMat* object_points = cvCreateMat(n_boards*board_n, 3, CV_32FC1);
	CvMat* point_counts = cvCreateMat(n_boards, 1, CV_32SC1);
	CvMat* intrinsic_matrix = cvCreateMat(3, 3, CV_32FC1);
	CvMat* distortion_coeffs = cvCreateMat(4, 1, CV_32FC1);

	CvPoint2D32f* corners = new CvPoint2D32f[board_n];
	int corner_count;
	int successes = 0;
	int step, frame = 0;

	IplImage *image =camera->QueryFrame();
	IplImage *gray_image = cvCreateImage(cvGetSize(image), 8, 1);//subpixel


																 //cvNot(image,image);
																 // CAPTURE CORNER VIEWS LOOP UNTIL WE’VE GOT n_boards 
																 // SUCCESSFUL CAPTURES (ALL CORNERS ON THE BOARD ARE FOUND)
																 //
																 //	help();
	while (successes < n_boards && IsThreadRun()) {
		//Handle pause/unpause and ESC
		cvWaitKey(40);
		image = m_camera->QueryFrame(); //Get next image
		cvShowImage("Raw Video", image);
		if (black == 1) {
			cvNot(image, image);//找角点需要反色
		}
		//Skip every board_dt frames to allow user to move chessboard
		if ((frame++ % board_dt) == 0) {
			//Find chessboard corners:
			int found = cvFindChessboardCorners(
				image, board_sz, corners, &corner_count,
				CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS
			);

			//Get Subpixel accuracy on those corners
			cvCvtColor(image, gray_image, CV_BGR2GRAY);
			cvFindCornerSubPix(gray_image, corners, corner_count,
				cvSize(11, 11), cvSize(-1, -1), cvTermCriteria(
					CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));

			//Draw it
			cvDrawChessboardCorners(image, board_sz, corners,
				corner_count, found);
			//      cvShowImage( "Calibration", image );
			if (black == 1) {
				cvNot(image, image);//找角点需要反色
			}
			// If we got a good board, add it to our data
			if (corner_count == board_n) {
				cvShowImage("Calibration", image); //show in color if we did collect the image
				step = successes*board_n;
				for (int i = step, j = 0; j<board_n; ++i, ++j) {
					CV_MAT_ELEM(*image_points, float, i, 0) = corners[j].x;
					CV_MAT_ELEM(*image_points, float, i, 1) = corners[j].y;
					CV_MAT_ELEM(*object_points, float, i, 0) = j / board_w;
					CV_MAT_ELEM(*object_points, float, i, 1) = j%board_w;
					CV_MAT_ELEM(*object_points, float, i, 2) = 0.0f;
				}
				CV_MAT_ELEM(*point_counts, int, successes, 0) = board_n;
				successes++;
				printf("Collected our %d of %d needed chessboard images\n", successes, n_boards);
			}
			else {
				cvShowImage("Calibration", gray_image); //Show Gray if we didn't collect the image
														//cvSaveImage("t.jpg",image);
			}

		} //end skip board_dt between chessboard capture



		  //cvNot(image,image);
	} //END COLLECTION WHILE LOOP.








	cvDestroyWindow("Calibration");
	cvDestroyWindow("Raw Video");
	printf("\n\n*** CALLIBRATING THE CAMERA...");
	//ALLOCATE MATRICES ACCORDING TO HOW MANY CHESSBOARDS FOUND
	CvMat* object_points2 = cvCreateMat(successes*board_n, 3, CV_32FC1);
	CvMat* image_points2 = cvCreateMat(successes*board_n, 2, CV_32FC1);
	CvMat* point_counts2 = cvCreateMat(successes, 1, CV_32SC1);
	//TRANSFER THE POINTS INTO THE CORRECT SIZE MATRICES
	for (int i = 0; i<successes*board_n; ++i) {
		CV_MAT_ELEM(*image_points2, float, i, 0) =
			CV_MAT_ELEM(*image_points, float, i, 0);
		CV_MAT_ELEM(*image_points2, float, i, 1) =
			CV_MAT_ELEM(*image_points, float, i, 1);
		CV_MAT_ELEM(*object_points2, float, i, 0) =
			CV_MAT_ELEM(*object_points, float, i, 0);
		CV_MAT_ELEM(*object_points2, float, i, 1) =
			CV_MAT_ELEM(*object_points, float, i, 1);
		CV_MAT_ELEM(*object_points2, float, i, 2) =
			CV_MAT_ELEM(*object_points, float, i, 2);
	}
	for (int i = 0; i<successes; ++i) { //These are all the same number
		CV_MAT_ELEM(*point_counts2, int, i, 0) =
			CV_MAT_ELEM(*point_counts, int, i, 0);
	}
	cvReleaseMat(&object_points);
	cvReleaseMat(&image_points);
	cvReleaseMat(&point_counts);

	// At this point we have all of the chessboard corners we need.
	// Initialize the intrinsic matrix such that the two focal
	// lengths have a ratio of 1.0
	//
	CV_MAT_ELEM(*intrinsic_matrix, float, 0, 0) = 1.0f;
	CV_MAT_ELEM(*intrinsic_matrix, float, 1, 1) = 1.0f;

	if (successes<10) {
		return;
	}
	//CALIBRATE THE CAMERA!
	cvCalibrateCamera2(
		object_points2, image_points2,
		point_counts2, cvGetSize(image),
		intrinsic_matrix, distortion_coeffs,
		NULL, NULL, 0  //CV_CALIB_FIX_ASPECT_RATIO
	);

	// SAVE THE INTRINSICS AND DISTORTIONS
	printf(" *** DONE!\n\nStoring Intrinsics.xml and Distortions.xml files\n\n");
	
	cvSave(camera->IntrinsicName().c_str(), intrinsic_matrix);
	cvSave(camera->DistortionName().c_str(), distortion_coeffs);



}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void calibration::calibration_image(ICamera*  camera) {
	// EXAMPLE OF LOADING THESE MATRICES BACK IN:
	CvMat *intrinsic = (CvMat*)cvLoad(camera->IntrinsicName().c_str());
	CvMat *distortion = (CvMat*)cvLoad(camera->DistortionName().c_str());
	CvPoint cxcy;

	if (intrinsic == NULL || distortion == NULL) {
		std::cout << "没有校准文件";
		return;
	}
	cxcy.x = cvmGet(intrinsic, 0, 2);
	cxcy.y = cvmGet(intrinsic, 1, 2);
	// Build the undistort map which we will use for all 
	// subsequent frames.
	IplImage *image = m_camera->QueryFrame();
	int image_h = image->height;
	int image_w = image->width;
	IplImage* mapx = cvCreateImage(cvGetSize(image), IPL_DEPTH_32F, 1);
	IplImage* mapy = cvCreateImage(cvGetSize(image), IPL_DEPTH_32F, 1);
	
	try
	{
		cvInitUndistortMap(
			intrinsic,
			distortion,
			mapx,
			mapy
		);
	}
	catch (cv::Exception& e)
	{
		std::cout << e.msg << std::endl;
	}

	
	
	// Just run the camera to the screen, now showing the raw and
	// the undistorted image.
	cvNamedWindow("Raw Video", CV_WINDOW_NORMAL);
	cvNamedWindow("Undistort", CV_WINDOW_NORMAL);

	while (image&&IsThreadRun())
	{
		IplImage *t = cvCloneImage(image);
		cvShowImage("Raw Video", image); // Show raw image
		cvRemap(t, image, mapx, mapy);     // Undistort image
		cvReleaseImage(&t);
#if DRAW_LINE
		for (int i = 0; i<12; i++) {
			cvLine(image, cvPoint(0, image_h / 12 * i), cvPoint(image_w, image_h / 12 * i), CV_RGB(0, 0, 255));
			cvLine(image, cvPoint(image_w / 12 * i, 0), cvPoint(image_w / 12 * i, image_h), CV_RGB(0, 0, 255));
		}
#else
		for (int i = 0; i<12; i++) {
			cvCircle(image, cxcy, i*(50), CV_RGB(0, 0, 255));
		}
#endif
		cvShowImage("Undistort", image);     // Show corrected image

											 //Handle pause/unpause and ESC
		cvWaitKey(15);
		image = m_camera->QueryFrame();
#if DRAW_LINE 
		for (int i = 0; i<12; i++) {
			cvLine(image, cvPoint(0, image_h / 12 * i), cvPoint(image_w, image_h / 12 * i), CV_RGB(255, 0, 0));
			cvLine(image, cvPoint(image_w / 12 * i, 0), cvPoint(image_w / 12 * i, image_h), CV_RGB(255, 0, 0));
		}
#else
		for (int i = 0; i<12; i++) {
			cvCircle(image, cxcy, i*(50), CV_RGB(255, 0, 0));
		}
#endif

	}
	cvReleaseMat(&intrinsic);
	cvReleaseMat(&distortion);
	cvReleaseImage(&mapx);
	cvReleaseImage(&mapy);
	cvDestroyWindow("Raw Video");
	cvDestroyWindow("Undistort");

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
bool calibration::opencv_cal_video(string video_in, string video_out, string cal_prefix)
{
	std::cout << "IN file : " << video_in << std::endl
				<< "OUT file : " << video_out << std::endl
				<< "Ip Addr : " << cal_prefix << std::endl;

	IplImage *frame_in = NULL;
	IplImage *frame_out = NULL;
	CvCapture *capture_in = NULL;
	const string window_in = "video_in";
	const string window_out = "video_out";
	capture_in = cvCreateFileCapture(video_in.c_str());
	frame_in = cvQueryFrame(capture_in);
	frame_out = cvCloneImage(frame_in);
	cvNamedWindow(window_in.c_str(),0);
	cvNamedWindow(window_out.c_str(),0);

#if _MSC_VER
	CvVideoWriter *video_writer_out = cvCreateVideoWriter(video_out.c_str(),
		//CV_FOURCC_PROMPT,
		CV_FOURCC('D', 'I', 'V', 'X'),
		cvGetCaptureProperty(capture_in, CV_CAP_PROP_FPS),
		cvGetSize(frame_in));
#else
	CvVideoWriter *video_writer_out = cvCreateVideoWriter(video_out.c_str(),
		CV_FOURCC_DEFAULT,
		cvGetCaptureProperty(capture_in, CV_CAP_PROP_FPS),
		cvGetSize(frame_in));
#endif

	assert(video_writer_out!=NULL);


	while (frame_in) {

		opencv_cal_img2img(frame_in, frame_out, cal_prefix);
		cvWriteFrame(video_writer_out, frame_out);
#if 0
		cvShowImage(window_in.c_str(), frame_in);
		cvShowImage(window_out.c_str(), frame_out);
#endif // 0

		cvWaitKey(1);




		frame_in = cvQueryFrame(capture_in);
		

	}

	cvReleaseImage(&frame_out);

	cvReleaseCapture(&capture_in);
	cvReleaseVideoWriter(&video_writer_out);

	cvDestroyWindow(window_in.c_str());
	cvDestroyWindow(window_out.c_str());

	std::cout << " convert done!" << std::endl;

	return true;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
bool calibration::opencv_cal_img2img(IplImage * img_src, IplImage * img_dest, string cal_prefix)
{

	stringstream intrinsic_file;
	stringstream distortion_file;	

	intrinsic_file << "Intrinsics_";
	intrinsic_file << cal_prefix;
	intrinsic_file << ".xml";

	distortion_file << "Distortion_";
	distortion_file << cal_prefix;
	distortion_file << ".xml";

	CvMat *intrinsic = (CvMat*)cvLoad(intrinsic_file.str().c_str());//内参
	CvMat *distortion = (CvMat*)cvLoad(distortion_file.str().c_str());//外参
	if (intrinsic == NULL || distortion == NULL) {
		std::cout << "没有校准文件";
		return false;
	}
	// Build the undistort map which we will use for all 
	// subsequent frames.		
	IplImage* mapx = cvCreateImage(cvGetSize(img_src), IPL_DEPTH_32F, 1);
	IplImage* mapy = cvCreateImage(cvGetSize(img_src), IPL_DEPTH_32F, 1);

	try
	{
		cvInitUndistortMap(
			intrinsic,
			distortion,
			mapx,
			mapy
		);
	}
	catch (cv::Exception& e)
	{
		std::cout << e.msg << std::endl;
	}
	// Just run the camera to the screen, now showing the raw and
	// the undistorted image.

	cvRemap(img_src, img_dest, mapx, mapy);     // Undistort image
	
	cvReleaseMat(&intrinsic);
	cvReleaseMat(&distortion);
	cvReleaseImage(&mapx);
	cvReleaseImage(&mapy);
	

	return false;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/

/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/

/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/

/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/

/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/

/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/