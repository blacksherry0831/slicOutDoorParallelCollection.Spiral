﻿#include "StdAfx.h"
#include "public.h"
#define  USE_OPENCV TRUE
#define  OPENCV_GET_CAM TRUE
/***********************************/
int GlobalStatic::board_w=11;
int GlobalStatic::board_h=8;
int GlobalStatic::black=1;
int GlobalStatic::num_board_is_use=10;
int GlobalStatic::camera_id=0;
int GlobalStatic::per_frame=60;
int GlobalStatic::CAL_ROUTINE=TRUE;
/**************************全局变量******************************************/ 
	CWnd  *p_picture_handle;//图像控件句柄
    HWND global_GGJZ_hwnd;
//	//C_PAGE_STATE * global_page_state;//状态选项卡句柄
	bool global_thread_synch;//用于线程同步
	bool global_thread_restart=true;
	bool is_thread_run=false;
//	CSemaphore Thread_ctl_Sem(1,1,NULL,NULL);
	int  global_cam_index=0;//cvCaptureFromCAM的索引
///******************opencv相关变量***********************/
#if USE_OPENCV
static IplImage* p_frame_camera=NULL;
static IplImage ds_frame;

#endif
#if USE_OPENCV&&OPENCV_GET_CAM
CvCapture* gloal_p_capture=NULL;//全局的图像指针
#endif
#if !OPENCV_GET_CAM
static CCameraDS *pcamera;//摄像头
#endif
/***********************函数声明************************************************************/
#define DRAW_LINE  FALSE
/******************************************************************************/
void Calculate_Intrinsics_Distortion( CvCapture* capture,int board_w,int board_h,int n_boards,int board_dt){


	int board_n  = board_w * board_h;
	CvSize board_sz = cvSize( board_w, board_h );
	

	cvNamedWindow( "Calibration",CV_WINDOW_NORMAL );
	cvNamedWindow( "Raw Video",CV_WINDOW_NORMAL);
	//ALLOCATE STORAGE
	CvMat* image_points      = cvCreateMat(n_boards*board_n,2,CV_32FC1);
	CvMat* object_points     = cvCreateMat(n_boards*board_n,3,CV_32FC1);
	CvMat* point_counts      = cvCreateMat(n_boards,1,CV_32SC1);
	CvMat* intrinsic_matrix  = cvCreateMat(3,3,CV_32FC1);
	CvMat* distortion_coeffs = cvCreateMat(4,1,CV_32FC1);

	CvPoint2D32f* corners = new CvPoint2D32f[ board_n ];
	int corner_count;
	int successes = 0;
	int step, frame = 0;

	 IplImage *image = cvQueryFrame( capture );
	IplImage *gray_image = cvCreateImage(cvGetSize(image),8,1);//subpixel


	//cvNot(image,image);
	// CAPTURE CORNER VIEWS LOOP UNTIL WE’VE GOT n_boards 
	// SUCCESSFUL CAPTURES (ALL CORNERS ON THE BOARD ARE FOUND)
	//
//	help();
	while(successes < n_boards && global_thread_synch) {
		//Handle pause/unpause and ESC
	  cvWaitKey(20);
		image = cvQueryFrame( capture ); //Get next image
		cvShowImage("Raw Video", image);
		if (GlobalStatic::black==1){
			cvNot(image,image);//找角点需要反色
		}
		//Skip every board_dt frames to allow user to move chessboard
		if((frame++ % board_dt) == 0) {
						//Find chessboard corners:
						int found = cvFindChessboardCorners(
							image, board_sz, corners, &corner_count, 
							CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS
							);

						//Get Subpixel accuracy on those corners
			            cvCvtColor(image, gray_image, CV_BGR2GRAY);
						cvFindCornerSubPix(gray_image, corners, corner_count, 
							cvSize(11,11),cvSize(-1,-1), cvTermCriteria(    
							CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 30, 0.1 ));

						//Draw it
						cvDrawChessboardCorners(image, board_sz, corners, 
							corner_count, found);
						//      cvShowImage( "Calibration", image );
						if (GlobalStatic::black==1){
		            	   cvNot(image,image);//找角点需要反色
		                }
						// If we got a good board, add it to our data
						if( corner_count == board_n ) {
							cvShowImage( "Calibration", image ); //show in color if we did collect the image
							step = successes*board_n;
							for( int i=step, j=0; j<board_n; ++i,++j ) {
								CV_MAT_ELEM(*image_points, float,i,0) = corners[j].x;
								CV_MAT_ELEM(*image_points, float,i,1) = corners[j].y;
								CV_MAT_ELEM(*object_points,float,i,0) = j/board_w;
								CV_MAT_ELEM(*object_points,float,i,1) = j%board_w;
								CV_MAT_ELEM(*object_points,float,i,2) = 0.0f;
							}
							CV_MAT_ELEM(*point_counts, int,successes,0) = board_n;    
							successes++;
							printf("Collected our %d of %d needed chessboard images\n",successes,n_boards);
						}
						else{
							cvShowImage( "Calibration", gray_image ); //Show Gray if we didn't collect the image
						   //cvSaveImage("t.jpg",image);
						}
			
		} //end skip board_dt between chessboard capture

		
		
		//cvNot(image,image);
	} //END COLLECTION WHILE LOOP.








	cvDestroyWindow("Calibration");
	cvDestroyWindow("Raw Video");
	printf("\n\n*** CALLIBRATING THE CAMERA...");
	//ALLOCATE MATRICES ACCORDING TO HOW MANY CHESSBOARDS FOUND
	CvMat* object_points2  = cvCreateMat(successes*board_n,3,CV_32FC1);
	CvMat* image_points2   = cvCreateMat(successes*board_n,2,CV_32FC1);
	CvMat* point_counts2   = cvCreateMat(successes,1,CV_32SC1);
	//TRANSFER THE POINTS INTO THE CORRECT SIZE MATRICES
	for(int i = 0; i<successes*board_n; ++i){
		CV_MAT_ELEM( *image_points2, float, i, 0) = 
			CV_MAT_ELEM( *image_points, float, i, 0);
		CV_MAT_ELEM( *image_points2, float,i,1) =   
			CV_MAT_ELEM( *image_points, float, i, 1);
		CV_MAT_ELEM(*object_points2, float, i, 0) =  
			CV_MAT_ELEM( *object_points, float, i, 0) ;
		CV_MAT_ELEM( *object_points2, float, i, 1) = 
			CV_MAT_ELEM( *object_points, float, i, 1) ;
		CV_MAT_ELEM( *object_points2, float, i, 2) = 
			CV_MAT_ELEM( *object_points, float, i, 2) ;
	} 
	for(int i=0; i<successes; ++i){ //These are all the same number
		CV_MAT_ELEM( *point_counts2, int, i, 0) = 
			CV_MAT_ELEM( *point_counts, int, i, 0);
	}
	cvReleaseMat(&object_points);
	cvReleaseMat(&image_points);
	cvReleaseMat(&point_counts);

	// At this point we have all of the chessboard corners we need.
	// Initialize the intrinsic matrix such that the two focal
	// lengths have a ratio of 1.0
	//
	CV_MAT_ELEM( *intrinsic_matrix, float, 0, 0 ) = 1.0f;
	CV_MAT_ELEM( *intrinsic_matrix, float, 1, 1 ) = 1.0f;

	if (successes<10){
		return;
	}
	//CALIBRATE THE CAMERA!
	cvCalibrateCamera2(
		object_points2, image_points2,
		point_counts2,  cvGetSize( image ),
		intrinsic_matrix, distortion_coeffs,
		NULL, NULL,0  //CV_CALIB_FIX_ASPECT_RATIO
		);

	// SAVE THE INTRINSICS AND DISTORTIONS
	printf(" *** DONE!\n\nStoring Intrinsics.xml and Distortions.xml files\n\n");
	cvSave("Intrinsics.xml",intrinsic_matrix);
	cvSave("Distortion.xml",distortion_coeffs);

	

}
void calibration_image(CvCapture* capture){
		// EXAMPLE OF LOADING THESE MATRICES BACK IN:
	CvMat *intrinsic = (CvMat*)cvLoad("Intrinsics.xml");
	CvMat *distortion = (CvMat*)cvLoad("Distortion.xml");
	CvPoint cxcy;

	if (intrinsic == NULL || distortion == NULL) {
		std::cout << "没有校准文件";
		return;
	}
	cxcy.x=cvmGet(intrinsic,0,2);
	cxcy.y=cvmGet(intrinsic,1,2);
	// Build the undistort map which we will use for all 
	// subsequent frames.
	IplImage *image=cvQueryFrame( capture );
	int image_h=image->height;
	int image_w=image->width;
	IplImage* mapx = cvCreateImage( cvGetSize(image), IPL_DEPTH_32F, 1 );
	IplImage* mapy = cvCreateImage( cvGetSize(image), IPL_DEPTH_32F, 1 );
	cvInitUndistortMap(
		intrinsic,
		distortion,
		mapx,
		mapy
		);
	// Just run the camera to the screen, now showing the raw and
	// the undistorted image.
	cvNamedWindow( "Raw Video" ,CV_WINDOW_AUTOSIZE);
	cvNamedWindow( "Undistort" ,CV_WINDOW_AUTOSIZE);
	
	while(image&&global_thread_synch) 
	{
		IplImage *t = cvCloneImage(image);
		cvShowImage( "Raw Video", image ); // Show raw image
		cvRemap( t, image, mapx, mapy );     // Undistort image
		cvReleaseImage(&t);
#if DRAW_LINE
		for(int i=0;i<12;i++){
		 cvLine(image,cvPoint(0,image_h/12*i),cvPoint(image_w,image_h/12*i),CV_RGB(0,0,255));
		 cvLine(image,cvPoint(image_w/12*i,0),cvPoint(image_w/12*i,image_h),CV_RGB(0,0,255));
		 }
#else
		 for(int i=0;i<12;i++){	
		 cvCircle(image,cxcy,i*(50),CV_RGB(0,0,255));
		 } 
#endif
		cvShowImage("Undistort", image);     // Show corrected image

		//Handle pause/unpause and ESC
		 cvWaitKey(15);
		 image=cvQueryFrame( capture );
#if DRAW_LINE 
		 for(int i=0;i<12;i++){
		 cvLine(image,cvPoint(0,image_h/12*i),cvPoint(image_w,image_h/12*i),CV_RGB(255,0,0));
		 cvLine(image,cvPoint(image_w/12*i,0),cvPoint(image_w/12*i,image_h),CV_RGB(255,0,0));
		 }
#else
		 for(int i=0;i<12;i++){	
		 cvCircle(image,cxcy,i*(50),CV_RGB(255,0,0));
		 } 
#endif
		
	} 
	cvReleaseMat(&intrinsic);
	cvReleaseMat(&distortion);
	cvReleaseImage(&mapx);
	cvReleaseImage(&mapy);
	cvDestroyWindow( "Raw Video" );
	cvDestroyWindow( "Undistort" );

}
unsigned int __stdcall threadFunc_ctl(void* t)
 {	
//	 IplImage* one_frame_rgb;   
	 gloal_p_capture = cvCreateCameraCapture(global_cam_index);
	    is_thread_run=true; 			
	while(global_thread_synch){												
		//////////////////////////////////////////////////////////////////
		  
		if (GlobalStatic::CAL_ROUTINE){
              Calculate_Intrinsics_Distortion(gloal_p_capture,GlobalStatic::board_w,GlobalStatic::board_h,GlobalStatic::num_board_is_use,GlobalStatic::per_frame);
			  GlobalStatic::CAL_ROUTINE=FALSE;
		    }
			 calibration_image(gloal_p_capture);
	    }		


	cvReleaseCapture(&gloal_p_capture);

	is_thread_run=false;

	return FALSE;	

}

void * thread_opencv_calibration(void *)
{

	gloal_p_capture = cvCreateCameraCapture(global_cam_index);
	is_thread_run = true;
	while (global_thread_synch) {
		//////////////////////////////////////////////////////////////////

		if (GlobalStatic::CAL_ROUTINE) {
			Calculate_Intrinsics_Distortion(gloal_p_capture, GlobalStatic::board_w, GlobalStatic::board_h, GlobalStatic::num_board_is_use, GlobalStatic::per_frame);
			GlobalStatic::CAL_ROUTINE = FALSE;
		}
		calibration_image(gloal_p_capture);
	}


	cvReleaseCapture(&gloal_p_capture);

	is_thread_run = false;

	return NULL;
}

void StopCalibrationThread()
{

	while(is_thread_run){	
		  global_thread_synch=false;
		  Sleep(10);
		}
}


