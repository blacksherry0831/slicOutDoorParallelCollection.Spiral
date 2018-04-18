//#include "stdafx.h"
#include "CMD_CTRL.hpp"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/

/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
CMD_CTRL::CMD_CTRL() 
{
		memset(&f_header,0,sizeof(CMD_CTRL_HEADER));
		this->initHeader();
		this->initPc2Arm();
}

/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
CMD_CTRL::~CMD_CTRL() 
{

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void CMD_CTRL::initHeader()
{
	f_header.f_header[0] = 'Y';
	f_header.f_header[1] = 'j';
	f_header.f_header[2] = 'k';
	f_header.f_header[3] = 'j';
	memset(f_header.f_reserve, 0, sizeof(f_header.f_reserve));
	this->f_data.resize(2, 0);
	this->SetDataSize();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void CMD_CTRL::setGeneral()
{
	this->initHeader();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void CMD_CTRL::initPc2Arm()
{
	//
	f_header.f_dst_dev[0]=DEV::FPGA_ARM;
	f_header.f_dst_dev[1]=0x00;

	//
	f_header.f_src_dev[0]=DEV::IPC;
	f_header.f_src_dev[1]=0x00;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void CMD_CTRL::initpc2plcLR()
{
	//
	f_header.f_dst_dev[0] = DEV::PLC;
	f_header.f_dst_dev[1] = DEV::PLC_LR;

	//
	f_header.f_src_dev[0] = DEV::IPC;
	f_header.f_src_dev[1] = 0x00;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void CMD_CTRL::initCRC()
{
	std::vector<unsigned char> data_t = this->Data();
	int crc = data_t.at(0);
	for (size_t i = 1; i <data_t.size()-1; i++){
		crc ^= data_t.at(i);
	}
	this->f_crc = crc;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void CMD_CTRL::setFpgaConvertCmd(int _type)
{
	if (_type == TRUE) {
		
		f_header.f_cmd[0] = CMD_TYPE::CTRL;
		f_header.f_cmd[1] = CMD_TYPE::START;

	}else if(_type == FALSE){

		f_header.f_cmd[0] = CMD_TYPE::CTRL;
		f_header.f_cmd[1] = CMD_TYPE::STOP;

	}else{

		assert(0);
	
	}

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void CMD_CTRL::setRespCmd(int _type)
{
	if (_type == TRUE) {

		f_header.f_cmd[0] = CMD_TYPE::RESP;
		f_header.f_cmd[1] = CMD_TYPE::OK;

	}
	else if (_type == FALSE) {

		f_header.f_cmd[0] = CMD_TYPE::RESP;
		f_header.f_cmd[1] = CMD_TYPE::ERROR;

	}
	else {

		assert(0);

	}
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void CMD_CTRL::setPlcLrIntoIn(int _step)
{
	f_header.f_cmd[0] = CMD_TYPE::CTRL;
	f_header.f_cmd[1] = CMD_TYPE::LR_RUN_2;

	int _step_h = _step / 256;
	int _step_l = _step % 256;
	
	assert(f_data.size() >= 2);

	f_data[0] = _step_l;
	f_data[1] = _step_h;

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void CMD_CTRL::setRollerQualified(int _qualified)
{
	f_header.f_cmd[0] = CMD_TYPE::CTRL;
	f_header.f_cmd[1] = CMD_TYPE::ROLLER_Q;

	const int h_t = _qualified / 256;
	const int l_t = _qualified % 256;

	assert(f_data.size() >= 2);

	f_data[0] = l_t;
	f_data[1] = h_t;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
std::vector<unsigned char> CMD_CTRL::Data()
{
	std::vector<unsigned char> cmd_data;
		
	cmd_data.insert(cmd_data.end(), f_header.f_header, f_header.f_header+4);
	cmd_data.insert(cmd_data.end(), f_header.f_reserve, f_header.f_reserve + 2);
	cmd_data.insert(cmd_data.end(), f_header.f_cmd_idx, f_header.f_cmd_idx + 4);
	cmd_data.insert(cmd_data.end(), f_header.f_dst_dev, f_header.f_dst_dev + 2);

	cmd_data.insert(cmd_data.end(), f_header.f_src_dev, f_header.f_src_dev + 2);
	cmd_data.insert(cmd_data.end(), f_header.f_cmd, f_header.f_cmd + 2);
	cmd_data.insert(cmd_data.end(), f_header.f_data_len, f_header.f_data_len + 2);

	cmd_data.insert(cmd_data.end(), f_data.begin(), f_data.end());
	cmd_data.push_back(f_crc);
	
	return cmd_data;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void CMD_CTRL::Parse(char * _data,int _size)
{
	const int HeaderSize_ = sizeof(CMD_CTRL::CMD_CTRL_HEADER);
	const int BodySize_ = CMD_CTRL::GetCMDBodySize((CMD_CTRL::CMD_CTRL_HEADER*)_data);
	const char* body_data_= _data + HeaderSize_;
	/*----------------------------------------------------------*/
	memcpy(&(this->f_header), _data, HeaderSize_);
	/*----------------------------------------------------------*/
	f_data.clear();
	f_data.insert(f_data.end(),body_data_,body_data_ +BodySize_);
	/*----------------------------------------------------------*/
	f_crc = _data[_size - 1];
	/*----------------------------------------------------------*/
}
int CMD_CTRL::IsConvertDoneCmd()
{
	return 0;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int CMD_CTRL::IsResp()
{
	if (this->f_header.f_cmd[0] == 'r' && this->f_header.f_cmd[1] == 0x00) {
		return TRUE;
	}	
	return FALSE;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int CMD_CTRL::IsImageData()
{
	return 0;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int CMD_CTRL::IsIntoInnerReady()
{
	//工件就绪
	if (this->f_header.f_cmd[0] == 's' && this->f_header.f_cmd[1] == 0x20) {
		return TRUE;
	}
	return FALSE;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int CMD_CTRL::IsRoolerReady()
{
	//工件就绪
	if (this->f_header.f_cmd[0] == 's' && this->f_header.f_cmd[1] == 0x10) {
		return TRUE;
	}

	return 0;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
std::vector<unsigned char> CMD_CTRL::getFpgaStartCmd(int _type)
{
	this->setFpgaConvertCmd(_type);
	this->initHeader();
	this->initPc2Arm();
	
	return this->Data();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
std::vector<unsigned char> CMD_CTRL::getRespPLCmd(int _type)
{
	this->setRespCmd(_type);
	this->initHeader();
	this->initpc2plcLR();
	this->SetDataSize();
	this->initCRC();
	return this->Data();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
std::vector<unsigned char> CMD_CTRL::getPLCLRIntoCmd(int _step)
{
	
	this->initHeader();
	this->setPlcLrIntoIn(_step);
	this->initpc2plcLR();
	this->initCRC();
	return this->Data();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
std::vector<unsigned char> CMD_CTRL::getRollerQualifiedCmd(int _qualified)
{
	this->initHeader();
	this->setRollerQualified(_qualified);
	this->initpc2plcLR();
	this->initCRC();
	return this->Data();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void CMD_CTRL::SetDataSize() 
{
	if (f_data.size() < 2) {
		f_data.resize(2, 0);
	
	}
	this->f_data_size = f_data.size();
	
	int low0_8bit = this->f_data_size % 256;
	int high0_8bit = this->f_data_size/256%256;
	
	int low1_8bit = this->f_data_size/256/256 % 256;
	int high1_8bit = this->f_data_size /256/256/256;
	
	this->f_header.f_data_len[0] = low0_8bit ;
	this->f_header.f_data_len[1] = high0_8bit;
	
	this->f_header.f_reserve[0] = low1_8bit;
	this->f_header.f_reserve[1] = high1_8bit;
	
}


/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int CMD_CTRL::GetCMDBodySize(CMD_CTRL::CMD_CTRL_HEADER* _cmd)
{
	int size_t =_cmd->f_data_len[0] +
				_cmd->f_data_len[1] * 256 +
				_cmd->f_reserve[0] * 256*256 +
				_cmd->f_reserve[1] * 256*256*256;

	return size_t;

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