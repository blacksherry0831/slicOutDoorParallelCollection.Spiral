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
	f_data.resize(0);
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
	f_header.f_dst_dev[0]=DEV::DEV_FPGA_ARM;
	f_header.f_dst_dev[1]=0x00;

	//
	f_header.f_src_dev[0]=DEV::DEV_IPC;
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
	f_header.f_dst_dev[0] = DEV::DEV_PLC;
	f_header.f_dst_dev[1] = DEV::DEV_PLC_LR;

	//
	f_header.f_src_dev[0] = DEV::DEV_IPC;
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
		
		f_header.f_cmd[0] = CMD_TYPE::CT_CTRL;
		f_header.f_cmd[1] = CMD_TYPE::CT_START;

	}else if(_type == FALSE){

		f_header.f_cmd[0] = CMD_TYPE::CT_CTRL;
		f_header.f_cmd[1] = CMD_TYPE::CT_STOP;

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

		f_header.f_cmd[0] = CMD_TYPE::CT_RESP;
		f_header.f_cmd[1] = CMD_TYPE::CT_OK;

	}
	else if (_type == FALSE) {

		f_header.f_cmd[0] = CMD_TYPE::CT_RESP;
		f_header.f_cmd[1] = CMD_TYPE::CT_ERROR;

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
	f_header.f_cmd[0] = CMD_TYPE::CT_CTRL;
	f_header.f_cmd[1] = CMD_TYPE::CT_LR_RUN_2;

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
	f_header.f_cmd[0] = CMD_TYPE::CT_CTRL;
	f_header.f_cmd[1] = CMD_TYPE::CT_ROLLER_Q;

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
	this->Clear();
	/*----------------------------------------------------------*/
	memcpy(&(this->f_header), _data, HeaderSize_);	
	f_data.insert(f_data.end(),body_data_,body_data_ +BodySize_);	
	f_data_size = f_data.size();	
	f_crc = _data[_size - 1];
	/*----------------------------------------------------------*/
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void CMD_CTRL::Clear()
{
	f_data.clear();
	m_img = NULL;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
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
int CMD_CTRL::IsHeartbeat()
{
	if (this->f_header.f_cmd[0] == CMD_TYPE::CT_HEART &&
		this->f_header.f_cmd[1] == CMD_TYPE::CT_BEAT) {
		return TRUE;
	}
	return FALSE;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
IplImage* CMD_CTRL::getIplimage()
{
	IplImage*  IplImg_t = &(m_img->Iplimg);
	return IplImg_t;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int CMD_CTRL::SensorStat()
{
	const int buff_size = sizeof(m_img->sensor_stat);

	if (IsImg()) {
		return this->UChar2Int(m_img->sensor_stat, ALIGN_SIZE_T);
	}else{
		return  FALSE;
	}
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

	return FALSE;;
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
int CMD_CTRL::IsImgStart()
{
	//工件就绪
	if (IsImg() &&
		this->f_header.f_cmd[1] == CMD_TYPE::CT_START) {
		return TRUE;
	}

	return FALSE;;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int CMD_CTRL::IsImgEnd()
{
	//工件就绪
	if (IsImg() &&
		this->f_header.f_cmd[1] == CMD_TYPE::CT_STOP) {
		return TRUE;
	}

	return FALSE;;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int CMD_CTRL::IsImgFrame()
{
	//工件就绪
	if (IsImg() &&
		this->f_header.f_cmd[1] == CMD_TYPE::CT_FRAME) {
		return TRUE;
	}

	return FALSE;;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int  CMD_CTRL::FrameCount()
{
	if (IsImgFrame()) {
		
		return UChar2Int(m_img->frame, ALIGN_SIZE_T);
		
	}else{
		
		return -1;
	
	}

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int CMD_CTRL::InitImg()
{

	if (m_img == NULL) {
			unsigned char* buff_t = this->f_data.data();
			m_img = (IplImageU*)(buff_t);
			this->InitIplimage();
	}

	return TRUE;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int CMD_CTRL::InitIplimage()
{
	unsigned char* buff_t = this->f_data.data();
	IplImage* Iplimg_t = getIplimage();
	
	int WIDTH= this->UChar2Int(m_img->width, ALIGN_SIZE_T);
	int HEIGHT= this->UChar2Int(m_img->height, ALIGN_SIZE_T);
	int nChannels= this->UChar2Int(m_img->nChannels, ALIGN_SIZE_T);

	if (nChannels == 1) {
		
		cvInitImageHeader(Iplimg_t, cvSize(WIDTH, HEIGHT), IPL_DEPTH_8U, 1);
		

	}else if (nChannels==8) {

		cvInitImageHeader(Iplimg_t, cvSize(WIDTH, HEIGHT), IPL_DEPTH_64F, 1);
	
	}else {
	
		assert(0);
	}
#if _DEBUG
	assert(Iplimg_t->widthStep == Iplimg_t->width* Iplimg_t->nChannels);
#endif
	Iplimg_t->imageData = (char*)&buff_t[sizeof(IplImageUI)];
	return 0;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int CMD_CTRL::IsImg()
{
	//工件就绪
	if (this->f_header.f_cmd[0] == CMD_TYPE::CT_IMG) {		
		InitImg();
		return TRUE;
	}
	return FALSE;;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int CMD_CTRL::Channel()
{
	int buff_size = sizeof(m_img->IpAddrChannel);

	if (IsImg()) {
		return this->UChar2Int(m_img->IpAddrChannel, ALIGN_SIZE_T);
	}else{
		return  -1;
	}


	
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int CMD_CTRL::Width()
{	
	return getIplimage()->width;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int CMD_CTRL::Height()
{
	
	return getIplimage()->height;
	
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
int  CMD_CTRL::UChar2Int(unsigned char *_data, int _size)
{
	int value_t=0;
	assert(_size == ALIGN_SIZE_T);
	value_t =	_data[0]+
				_data[1]*256+
				_data[2]*256*256+
				_data[3]*256*256*256;

	return value_t;	
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