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
	enum DEV{
		IPC=0x00,
		PLC=0x01,
		FPGA_ARM=0x02};

	enum CMD_TYPE {
		QUERY='q',
		CTRL='c',
		RESP='r',
		START=0x00,
		STOP=0x01};

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
protected:
	void initPc2Arm();
public:
	void Convert2ByteStream();
	void SetDataSize();
	static int GetCMDBodySize(CMD_CTRL::CMD_CTRL_HEADER* _cmd);
public:
	void setFpgaConvertCmd();
	std::vector<unsigned char>	getFpgaStartCmd();
public:
	std::vector<unsigned char> Data();
public:
	void Parse(char* _data,int _size);
public:
	int IsConvertDoneCmd();
	int IsResp();
	int IsImageData();
};