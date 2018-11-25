#pragma once
/*-----------------------------------*/
/**
*
*/
/*-----------------------------------*/
#include "cpp_stl.h"
/*-----------------------------------*/
#include "opencv_stl.h"
/*-----------------------------------*/
#include "qt_all.h"
/*-----------------------------------*/
#define ALIGN_SIZE_T	(8)
/*-----------------------------------*/
typedef struct _IplImageU
{
	unsigned char nSize[ALIGN_SIZE_T];//this struct size
	char prefix[ALIGN_SIZE_T];
	unsigned char IpAddrChannel[ALIGN_SIZE_T];
	unsigned char frame[ALIGN_SIZE_T];

	unsigned char width[ALIGN_SIZE_T];
	unsigned char height[ALIGN_SIZE_T];

	unsigned char width_roi[ALIGN_SIZE_T];
	unsigned char height_roi[ALIGN_SIZE_T];
	unsigned char x_roi[ALIGN_SIZE_T];
	unsigned char y_roi[ALIGN_SIZE_T];

	unsigned char sensor_stat[ALIGN_SIZE_T];
	unsigned char nChannels[ALIGN_SIZE_T];

	IplImage  	Iplimg;
} IplImageU;

/*-----------------------------------*/
/**
*
*/
/*-----------------------------------*/
typedef union _IplImageUI
{
	unsigned char buff[ALIGN_SIZE_T * 32];
	IplImageU iplImgU;
}IplImageUI;
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
class CMD_CTRL_DATA_ARM_FPGA
{
public:
	enum WorkMode
	{
		WM_SIZE_FULL = 0x80,
		WM_SIZE_CUT = 0x40,
		WM_ORG_IMG = 0x20,
		WM_DIFF_IMG = 0x10,
		RESP = 0x01
	};
public:
	CMD_CTRL_DATA_ARM_FPGA();
	~CMD_CTRL_DATA_ARM_FPGA();
public:
	IplImage * getIplimage();
protected:
	IplImageU* m_img;

};