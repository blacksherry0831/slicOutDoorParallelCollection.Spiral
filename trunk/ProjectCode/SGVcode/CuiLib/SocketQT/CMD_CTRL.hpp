#pragma once

#include "cpp_stl.h"
#include "opencv_stl.h"
#include  "qt_all.h"
/*-----------------------------------*/
/**
*
*/
/*-----------------------------------*/
#include "CMD_CTRL_DATA.hpp"
/*-----------------------------------*/
/**
*
*/
/*-----------------------------------*/
#define CAMERA_CHANNELS (8)
/*-----------------------------------*/
/**
*
*/
/*-----------------------------------*/
class CMD_CTRL :public CMD_CTRL_DATA
{
public:

enum CMD_TYPE_LOCAL {

		CT_FPGA_START_00,
		CT_FPGA_START_01,

		CT_FPGA_STOP_00,
		CT_FPGA_STOP_01,

		CT_FPGA_START,
		CT_FPGA_STOP

	};

	enum CMD_TYPE_LOCAL_02 {
		
		CT_ROLLER_Q = 0x10
	};

public:
	CMD_CTRL();
	~CMD_CTRL();
/*-------------------------------------*/
public:
	std::string mIpAddrRemote;
private:
	int mCmdRemoteFlag;
	
public:
	int IsCmdRemote();
	int IsCmdLocal();
	int SetCmdRemote();
	int SetCmdLocal();
	void SetIpAddrRemote(QTcpSocket* _pSocket);
/*-------------------------------------*/
public:
	void setFpgaConvertCmd(int _type, WorkMode _wm);
	void setRespCmd(int _type, int work_mode);
	void setPlcLrIntoIn(int _step);
	void setRollerQualified(int _qualified);

	void setRectCutCmd(int _channel, CvRect _rect_cut);
	void setModeChangeCmd(int _wm);
	void setSigmaChangeCmd(int _sigma);
public:
	std::vector<unsigned char>	getFpgaStartCmd(int _type, WorkMode _wm);
	std::vector<unsigned char>	getRespPLCmd(int _type);
	std::vector<unsigned char>	getPLCLRIntoCmd(int _step);
	std::vector<unsigned char>  getRollerQualifiedCmd(int _qualified);

	std::vector<unsigned char>  getLocalCmd(int _cmd00, int _cmd01=0x00);
	std::vector<unsigned char>  getHeartBeatCmd(int _type= DEV::DEV_FPGA_ARM);
	std::vector<unsigned char>  getRectCfgCmd(int _channel, CvRect _rect_cut);
	std::vector<unsigned char>  getModeChangeCmd(int _wm);
	std::vector<unsigned char>  getSigmaChangeCmd(int _sigma);
public:
	static void adjRect44(CvRect* rect);
public:
	
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
	int CmdStat();
	int FrameCount();
	int IsImg();
	int InitImgCtrlHeader(int VideoCh, int _width, int _height, int _nChannels);
	int InitImg();
	int InitIplimage();
	int Channel();
	int SensorStat();
	int Width();
	int Height();
	/*-------------------------------------*/
public:
	int IsThisCmd00(int _cmd);

	
};