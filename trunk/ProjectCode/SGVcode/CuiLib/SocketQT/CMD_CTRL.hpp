#pragma once
/*-----------------------------------*/
#include "cpp_stl.h"
#include "opencv_stl.h"
#include  "qt_all.h"
/*-----------------------------------*/
/**
*
*/
/*-----------------------------------*/
#include "type_socket_qt.h"
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
	CMD_CTRL();
	~CMD_CTRL();
/*-------------------------------------*/
public:
	std::string mIpAddrRemote;
	std::string mIpAddrLocal;
	std::string mCurrentCircleTime;
	float mFeature;	
	IMG_PROC mImgProc;
public:
	
	void SetIpAddrRemote(QTcpSocket* _pSocket);
/*-------------------------------------*/
public:
	void setFpgaConvertCmd(int _type, WorkMode _wm);
	void setRespCmd(CMD_TYPE_02_RESP _type, int work_mode);
	void setPlcLrIntoIn(int _step);
	void setRollerQualified(int _qualified);

	void setRectCutCmd(int _channel, CvRect _rect_cut);
	void setModeChangeCmd(int _wm);
	void setSigmaChangeCmd(int _sigma);
public:
	std::vector<unsigned char>	getRespCmd(CMD_TYPE_02_RESP _type);
	std::vector<unsigned char>	getRespPLCmd(CMD_TYPE_02_RESP _type);
	std::vector<unsigned char>	getPLCLRIntoCmd(int _step);
	std::vector<unsigned char>  getRollerQualifiedCmd(int _qualified);

	std::vector<unsigned char>  getHeartBeatCmd(int _need_resp);

	std::vector<unsigned char>  getRectCfgCmd(int _channel, CvRect _rect_cut);
	std::vector<unsigned char>  getModeChangeCmd(int _wm);
	std::vector<unsigned char>  getSigmaChangeCmd(int _sigma);
private:
	std::vector<unsigned char>	getFpgaStartCmd(int _type, WorkMode _wm);
	std::vector<unsigned char>  getLocalCmd(int _cmd00, int _cmd01=0x00);
public:
	static QSharedPointer<CMD_CTRL> getLocalCmdEx(int _cmd00, int _cmd01 = 0x00);
	static QSharedPointer<CMD_CTRL> getFpgaStartCmdEx(int _type, WorkMode _wm);
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
	int IsAbortStop();
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
	int GetCmd00();
	int GetCmdParam();

	
};