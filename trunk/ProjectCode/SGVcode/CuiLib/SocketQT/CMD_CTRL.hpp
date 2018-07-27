#pragma once

#include "cpp_stl.h"
#include "opencv_stl.h"
/*-----------------------------------*/
/**
*
*/
/*-----------------------------------*/
#define CAMERA_CHANNELS (8)
#define ALIGN_SIZE_T	(8)
/*-----------------------------------*/
/**
*
*/
/*-----------------------------------*/
#include <QtCore>
#include <QtNetwork>
#include <QSharedPointer>
#include <QThread>
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
class CMD_CTRL
{
public:
	enum DEV {
		DEV_IPC = 0x00,
		DEV_PLC = 0x01,
		DEV_PLC_LR = 0x11,
		DEV_FPGA_ARM=0x02};

	enum CMD_TYPE {
		CT_HEART='h',
		CT_BEAT='b',
		CT_QUERY='q',
		CT_CTRL='c',
		CT_RESP='r',
		CT_IMG='I',
		CT_START=0x00,
		CT_STOP=0x01,
		CT_FRAME='F',
		CT_OK=0x00,
		CT_ERROR=0x01,
		CT_LR_RUN_2=0x20,
		CT_ROLLER_Q=0x10};
	enum WorkMode
	{
		CUT_AREA=1,
		ORG_IMAGE=2,
		DIFF=3,
		RESP = 4
	};
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
	CMD_CTRL();
	~CMD_CTRL();
/*-------------------------------------*/
public:
		CMD_CTRL_HEADER f_header;
		std::vector<unsigned char> f_data;
		int f_data_size;
		unsigned char f_crc;
private:
	IplImageU* m_img;
/*-------------------------------------*/
private:
	void initHeader();
	void init();
	void destory();
protected:
	void initPc2Arm();
	void initpc2plcLR();
	void initCRC();
	
public:
	void Convert2ByteStream();
	void SetDataSize(const int _body_size=2);
	static int GetCMDBodySize(CMD_CTRL::CMD_CTRL_HEADER* _cmd);
public:
	void setFpgaConvertCmd(int _type, WorkMode _wm);
	void setRespCmd(int _type, int work_mode);
	void setPlcLrIntoIn(int _step);
	void setRollerQualified(int _qualified);
	void initHearbeatCmd();
public:
	std::vector<unsigned char>	getFpgaStartCmd(int _type, WorkMode _wm);
	std::vector<unsigned char>	getRespPLCmd(int _type);
	std::vector<unsigned char>	getPLCLRIntoCmd(int _step);
	std::vector<unsigned char>  getRollerQualifiedCmd(int _qualified);
	std::vector<unsigned char>  getHeartBeatCmd(int _type= DEV::DEV_FPGA_ARM);

public:
	std::vector<unsigned char> Data();
public:
	void Parse(char* _data,int _size);
public:
	int IsConvertDoneCmd();
	int IsResp();
	int isHeartbeatCmd();
	int IsIntoInnerReady();
	int IsRoolerReady();
	/*-------------------------------------*/
	int IsImgStart();
	int IsImgEnd();
	int IsImgFrame();
	int FrameCount();
	int IsImg();
	int InitImgCtrlHeader(int VideoCh, int _width, int _height, int _nChannels);
	int InitImg();
	int InitIplimage();
	int Channel();
	int SensorStat();
	int Width();
	int Height();
	IplImage* getIplimage();
	/*-------------------------------------*/

public:
	static int UChar2Int(unsigned char *_data,int _size);
	static int Int2UChar(int _size,unsigned char *_data );
	
};