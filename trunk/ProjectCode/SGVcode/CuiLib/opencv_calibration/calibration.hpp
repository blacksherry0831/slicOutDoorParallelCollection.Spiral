#pragma once


#include "opencv_stl.h"
#include "pt_mutex.h"

#include "MY_SDK_LIB/Base.h"
#include "MY_SDK_LIB/ICamera.hpp"

#include "checkerboard.hpp"

#ifndef DRAW_LINE 
#define DRAW_LINE  TRUE
#endif

/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
class calibration : public checkerboard
{

public:
	calibration(void);
	calibration(ICamera*  camera_t);
	

	~calibration(void);
public:

	void init();
	void release();

	void start(int routine);
	void stop();
	
	void Join();

	boolean IsThreadRun();

	static void * run(void * data);
	void Calculate_Intrinsics_Distortion(ICamera*  camera, int board_w, int board_h, int n_boards, int board_dt);
	void calibration_image(ICamera*  camera);
private:

	std::shared_ptr<ICamera>  m_camera;
	
	boolean m_read_thread_run;
	pthread_t m_pt_handle;
	
	
/*-------------------------------------------------------*/

};