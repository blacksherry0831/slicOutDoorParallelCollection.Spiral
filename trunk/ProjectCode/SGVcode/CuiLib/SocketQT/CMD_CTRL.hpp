#pragma once

#include "cpp_stl.h"

#include <QtCore>
#include <QtNetwork>
#include <QSharedPointer>
#include <QThread>

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
		IPC = 0x00,
		PLC = 0x01,
		PLC_LR = 0x11,
		FPGA_ARM=0x02};

	enum CMD_TYPE {
		QUERY='q',
		CTRL='c',
		RESP='r',
		START=0x00,
		STOP=0x01,
		OK=0x00,
		ERROR=0x01,
		LR_RUN_2=0x20,
		ROLLER_Q=0x10};

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
public:
		CMD_CTRL_HEADER f_header;
		std::vector<unsigned char> f_data;
		int f_data_size;
		unsigned char f_crc;
private:
	void initHeader();
	void setGeneral();
protected:
	void initPc2Arm();
	void initpc2plcLR();
	void initCRC();
public:
	void Convert2ByteStream();
	void SetDataSize();
	static int GetCMDBodySize(CMD_CTRL::CMD_CTRL_HEADER* _cmd);
public:
	void setFpgaConvertCmd(int _type);
	void setRespCmd(int _type);
	void setPlcLrIntoIn(int _step);
	void setRollerQualified(int _qualified);
	std::vector<unsigned char>	getFpgaStartCmd(int _type);
	std::vector<unsigned char>	getRespPLCmd(int _type);
	std::vector<unsigned char>	getPLCLRIntoCmd(int _step);
	std::vector<unsigned char>  getRollerQualifiedCmd(int _qualified);

public:
	std::vector<unsigned char> Data();
public:
	void Parse(char* _data,int _size);
public:
	int IsConvertDoneCmd();
	int IsResp();
	int IsImageData();
	int IsIntoInnerReady();
	int IsRoolerReady();

};