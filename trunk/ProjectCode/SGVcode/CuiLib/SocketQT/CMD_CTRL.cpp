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
		this->init();
		
}
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
CMD_CTRL::~CMD_CTRL() 
{
		this->destory();
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
		
	this->SetDataSize();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void CMD_CTRL::SetValue2Data(int _data)
{
	assert(f_data.size() >= 2);

	f_data[0] = _data%256;//low
	f_data[1] = _data/256;//height
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void CMD_CTRL::init()
{
	memset(&f_header, 0, sizeof(CMD_CTRL_HEADER));
	m_img = NULL;
	this->initHeader();
	this->initPc2Arm();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void CMD_CTRL::destory()
{
	m_img = NULL;
	this->f_data.clear();
	this->f_data_size = 0;
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
void CMD_CTRL::setFpgaConvertCmd(int _type, WorkMode _wm)
{
	if (_type == TRUE) {
		
		f_header.f_cmd[0] = CMD_TYPE::CT_CTRL;
		f_header.f_cmd[1] = CMD_TYPE_02::CT_START;

	}else if(_type == FALSE){

		f_header.f_cmd[0] = CMD_TYPE::CT_CTRL;
		f_header.f_cmd[1] = CMD_TYPE_02::CT_STOP;

	}else{

		assert(0);
	
	}
	
	this->SetValue2Data(_wm);

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void CMD_CTRL::setRespCmd(int _type, int work_mode)
{
	if (_type == TRUE) {

		f_header.f_cmd[0] = CMD_TYPE::CT_RESP;
		f_header.f_cmd[1] = CMD_TYPE_02::CT_OK;

	}
	else if (_type == FALSE) {

		f_header.f_cmd[0] = CMD_TYPE::CT_RESP;
		f_header.f_cmd[1] = CMD_TYPE_02::CT_ERROR;

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
	f_header.f_cmd[1] = CMD_TYPE_02::CT_LR_RUN_2;

	this->SetValue2Data(_step);

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void CMD_CTRL::setRollerQualified(int _qualified)
{
	f_header.f_cmd[0] = CMD_TYPE::CT_CTRL;
	f_header.f_cmd[1] = CMD_TYPE_02::CT_ROLLER_Q;

	this->SetValue2Data(_qualified);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void CMD_CTRL::initHearbeatCmd()
{
	f_header.f_cmd[0] = CMD_TYPE::CT_HEART;
	f_header.f_cmd[1] = CMD_TYPE_02::CT_BEAT;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void CMD_CTRL::setRectCutCmd(int _channel, CvRect _rect_cut)
{
	f_header.f_cmd[0] = CMD_TYPE::CT_IMG;
	f_header.f_cmd[1] = CMD_TYPE_02::CT_IMG_RECT;

	const size_t data_size = sizeof(IplImageUI);

	this->SetDataSize(data_size);
	
	Int2UChar(sizeof(IplImageU), f_data.data());

	m_img=this->getIplimageU();

	

	Int2UChar(_channel, m_img->IpAddrChannel);

	Int2UChar(_rect_cut.width, m_img->width_roi);
	Int2UChar(_rect_cut.height, m_img->height_roi);
	Int2UChar(_rect_cut.x, m_img->x_roi);
	Int2UChar(_rect_cut.y, m_img->y_roi);
	
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void CMD_CTRL::setModeChangeCmd(int _wm)
{
	f_header.f_cmd[0] = CMD_TYPE::CT_IMG;
	f_header.f_cmd[1] = CMD_TYPE_02::CT_IMG_MODE_CHANGE;
	
	this->SetValue2Data(_wm);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void CMD_CTRL::setSigmaChangeCmd(int _sigma)
{
	f_header.f_cmd[0] = CMD_TYPE::CT_IMG;
	f_header.f_cmd[1] = CMD_TYPE_02::CT_IMG_SIGMA_CHANGE;

	this->SetValue2Data(_sigma);
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
	this->destory();
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
int CMD_CTRL::isHeartbeatCmd()
{
	if (this->f_header.f_cmd[0] == CMD_TYPE::CT_HEART &&
		this->f_header.f_cmd[1] == CMD_TYPE_02::CT_BEAT) {
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
QSharedPointer<QImage> CMD_CTRL::getQimage()
{
	return IplImageToQImage(getIplimage());
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
IplImageU * CMD_CTRL::getIplimageU()
{
	IplImageU* img=NULL;

	if (this->f_data.size()>= sizeof(IplImageU)){
		
			unsigned char* buff_t = this->f_data.data();
			img = (IplImageU*)(buff_t);
			assert(UChar2Int(img->nSize, 8) == sizeof(IplImageU));

	}
	


	return img;
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
std::vector<unsigned char> CMD_CTRL::getFpgaStartCmd(int _type, WorkMode _wm)
{
	this->setFpgaConvertCmd(_type,_wm);
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
		this->f_header.f_cmd[1] == CMD_TYPE_02::CT_START) {
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
		this->f_header.f_cmd[1] == CMD_TYPE_02::CT_STOP) {
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
		this->f_header.f_cmd[1] == CMD_TYPE_02::CT_FRAME) {
		return TRUE;
	}

	return FALSE;;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int CMD_CTRL::CmdStat()
{
	int stat = (this->f_header.f_cmd[1] *256) + this->f_header.f_cmd[0];
	return stat;
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
			
			m_img = this->getIplimageU();
			
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

	int roi_width = this->UChar2Int(m_img->width_roi, ALIGN_SIZE_T);
	int roi_height = this->UChar2Int(m_img->height_roi, ALIGN_SIZE_T);
		
	int roi_x= this->UChar2Int(m_img->x_roi, ALIGN_SIZE_T);
	int roi_y= this->UChar2Int(m_img->y_roi, ALIGN_SIZE_T);
	
	

	if (nChannels == 1) {
		
		cvInitImageHeader(Iplimg_t, cvSize(WIDTH, HEIGHT), IPL_DEPTH_8U, 1);
				
	}else if (nChannels==8) {

		cvInitImageHeader(Iplimg_t, cvSize(WIDTH, HEIGHT), IPL_DEPTH_64F, 1);
	
	}else if (nChannels == 2) {

		cvInitImageHeader(Iplimg_t, cvSize(WIDTH, HEIGHT), IPL_DEPTH_8U, nChannels);

	}else {
	
		assert(0);
	}

#if _DEBUG
	assert(Iplimg_t->widthStep == Iplimg_t->width* Iplimg_t->nChannels);
#endif
	Iplimg_t->imageData = (char*)&buff_t[sizeof(IplImageUI)];
	
	if (roi_width + roi_height + roi_x + roi_y > 0) {
		cvSetImageROI(Iplimg_t, cvRect(roi_x, roi_y, roi_width, roi_height));
	}
	
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
int CMD_CTRL::InitImgCtrlHeader(int VideoCh,int _width, int _height, int _nChannels)
{
	
	const int body_size_t = _width*_height*_nChannels + sizeof(IplImageUI);
	
		SetDataSize(body_size_t);

	this->f_header.f_cmd[0] = CMD_TYPE::CT_IMG;
	this->f_header.f_cmd[1] = CMD_TYPE_02::CT_FRAME;
	

	
	this->Int2UChar(sizeof(IplImageU),this->f_data.data());

	IplImageU *img_u = this->getIplimageU();

	this->Int2UChar(_width, img_u->width);

	this->Int2UChar(_width, img_u->width);
	this->Int2UChar(_height, img_u->height);
	this->Int2UChar(_nChannels, img_u->nChannels );
	this->Int2UChar(1, img_u->sensor_stat);
	this->Int2UChar(VideoCh%8, img_u->IpAddrChannel);

	return 0;
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
int CMD_CTRL::Int2UChar(int _size, unsigned char * _data)
{
	
	
	_data[0]=_size%256;
	_data[1]=_size/256%256;
	_data[2]=_size/256/256%256;
	_data[3]=_size/256/256/256%256;

	return _data[0];
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
std::vector<unsigned char> CMD_CTRL::getRespPLCmd(int _type)
{
	this->setRespCmd(_type, WorkMode::RESP);
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
std::vector<unsigned char> CMD_CTRL::getHeartBeatCmd(int _type)
{

	this->initHeader();
	this->initHearbeatCmd();
	
	this->initCRC();
	return this->Data();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
std::vector<unsigned char> CMD_CTRL::getRectCfgCmd(int _channel,CvRect _rect_cut)
{
	this->initHeader();
	
	this->setRectCutCmd(_channel,_rect_cut);

	this->initCRC();
	return this->Data();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
std::vector<unsigned char> CMD_CTRL::getModeChangeCmd(int _wm)
{
	this->setModeChangeCmd(_wm);
	this->initHeader();
	this->initPc2Arm();

	return this->Data();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
std::vector<unsigned char> CMD_CTRL::getSigmaChangeCmd(int _sigma)
{
	this->setSigmaChangeCmd(_sigma);
	this->initHeader();
	this->initPc2Arm();

	return this->Data();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void CMD_CTRL::SetDataSize(const int _body_size)
{
	if (f_data.size() < 2) {
		f_data.resize(2, 0);
	}else{
		f_data.resize(_body_size);
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
QSharedPointer<QImage> CMD_CTRL::IplImageToQImage(IplImage* const img)
{	
	const uchar * imgData = (uchar *)img->imageData;
	
	QImage::Format f = QImage::Format::Format_Grayscale8;

	if (img->nChannels == 1) {
		f = QImage::Format::Format_Grayscale8;
	}

	QSharedPointer<QImage> image = QSharedPointer<QImage>(new QImage(imgData, img->width, img->height,f));
	return image;
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