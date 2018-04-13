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
	f_header.f_header[0] = 'y';
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
void CMD_CTRL::setFpgaConvertCmd()
{
	f_header.f_cmd[0] = CMD_TYPE::CTRL;
	f_header.f_cmd[1] = CMD_TYPE::START;
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
std::vector<unsigned char> CMD_CTRL::getFpgaStartCmd()
{
	this->setFpgaConvertCmd();
	this->initHeader();
	this->initPc2Arm();
	this->SetDataSize();
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
	int high_8bit = this->f_data_size/256;
	int low_8bit = this->f_data_size % 256;

	this->f_header.f_data_len[0] = low_8bit ;
	this->f_header.f_data_len[1] = high_8bit;
	
}


/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int CMD_CTRL::GetCMDBodySize(CMD_CTRL::CMD_CTRL_HEADER* _cmd)
{
	int size_t = _cmd->f_data_len[0] + _cmd->f_data_len[1] * 256;

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