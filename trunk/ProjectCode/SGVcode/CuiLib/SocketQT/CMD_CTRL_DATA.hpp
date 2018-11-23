#pragma once
/*-----------------------------------*/
#include "opencv_stl.h"
/*-----------------------------------*/
#include "qt_all.h"
/*-----------------------------------*/
#include "CMD_CTRL_DATA_LOCAL.hpp"
#include "CMD_CTRL_DATA_ARM_FPGA.hpp"
/*-----------------------------------*/
#define ALIGN_SIZE_T	(8)
/*-----------------------------------*/
/**
*
*/
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
class CMD_CTRL_DATA: public CMD_CTRL_DATA_LOCAL,public CMD_CTRL_DATA_ARM_FPGA
{
public:
	/*-----------------------------------*/
	/**
	*
	*/
	/*-----------------------------------*/
	enum DEV {
		DEV_IPC = 0x00,
		DEV_PLC = 0x01,
		DEV_PLC_LR = 0x11,
		DEV_FPGA_ARM = 0x02
	};
	/*-----------------------------------*/
	/**
	*
	*/
	/*-----------------------------------*/
	enum CMD_TYPE {
		CT_HEART = 'h',
		CT_QUERY = 'q',
		CT_CTRL = 'c',
		CT_RESP = 'r',
		CT_IMG = 'I'
	};
	/*-----------------------------------*/
	/**
	*
	*/
	/*-----------------------------------*/
	enum CMD_TYPE_02 {
		

		CT_BEAT = 'b',
		
		CT_LR_RUN_2 = 0x20,
		CT_ROLLER_Q = 0x10
	};
	/*-----------------------------------*/
	/**
	*
	*/
	/*-----------------------------------*/
	enum CMD_TYPE_02_C {

		CT_START = 0x00,
		CT_STOP = 0x01,

		CT_START_00 = 0x10,
		CT_STOP_00 = 0x11,
		CT_START_01 = 0x20,
		CT_STOP_01 = 0x21,
	};
	/*-----------------------------------*/
	/**
	*
	*/
	/*-----------------------------------*/
	enum CMD_TYPE_02_I {

		CT_IMG_FRAME = 'F',
		CT_IMG_RECT = 'R',
		CT_IMG_MODE_CHANGE = 'M',
		CT_IMG_SIGMA_CHANGE = 'S',

	};
	/*-----------------------------------*/
	/**
	*
	*/
	/*-----------------------------------*/
	enum CMD_TYPE_02_RESP {

		CT_OK = 0x00,
		CT_ERROR = 0x01,

	};
	/*-----------------------------------*/
	/**
	*
	*/
	/*-----------------------------------*/

public:
	typedef struct {
		unsigned char f_header[4];
		unsigned char f_reserve[2];
		unsigned char f_cmd_idx[4];
		unsigned char f_dst_dev[2];
		unsigned char f_src_dev[2];
		unsigned char f_cmd[2];
		unsigned char f_data_len[2];

	}CMD_CTRL_HEADER;
public:
	CMD_CTRL_DATA();
	~CMD_CTRL_DATA();

/*-------------------------------------*/
public:
		CMD_CTRL_HEADER f_header;
		std::vector<unsigned char> f_data;
		int f_data_size;
		unsigned char f_crc;
protected:
	IplImageU* m_img;
/*-------------------------------------*/
protected:
	void initHeader();
	void SetValue2Data(int _data);
	void init();
	void destory();
protected:
	void initPc2Arm();
	void initpc2plcLR();
	void initCRC();
public:
	void SetDataSize(const int _body_size = 2);
public:
	std::vector<unsigned char> Data();

	IplImage * getIplimage();
public:
	static int UChar2Int(unsigned char *_data,int _size);
	static int Int2UChar(int _size,unsigned char *_data );

public:
	static int GetCMDBodySize(CMD_CTRL_DATA::CMD_CTRL_HEADER* _cmd);
	static QSharedPointer<QImage> IplImageToQImage(IplImage * const img);

	void initHearbeatCmd();

public:
	QSharedPointer<QImage> getQimage();

	IplImageU* getIplimageU();
	
};