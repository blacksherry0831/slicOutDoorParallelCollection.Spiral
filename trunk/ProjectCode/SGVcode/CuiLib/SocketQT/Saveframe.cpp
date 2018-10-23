//#include "stdafx.h"
#include "Saveframe.hpp"

#include "MY_SDK_LIB/Base.h"
#include "QT_SDK_LIB/QBase.h"
#include "MY_SDK_LIB/PathCfg.h"
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
Saveframe::Saveframe()
{	
	this->initParam();
}
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
Saveframe::~Saveframe()
{
	
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void Saveframe::init()
{

	
	
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int Saveframe::isInitParam()
{

	if (this->mIpaddr == "" ||
		this->mTimeCurrent == "" ||
		this->mChannel == -1 ||
		this->mFrameCount == -1) {
	
		return FALSE;
	
	}

	return TRUE;

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void Saveframe::initParam()
{
	this->mIpaddr = "";
	this->mTimeCurrent = "";
	this->mChannel = -1;
	this->mFrameCount = -1;
	this->mIsSaveFrame =FALSE;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
Saveframe* Saveframe::start_record()
{
	this->initParam();
	return this;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
Saveframe* Saveframe::stop_record()
{
	this->initParam();
	return this;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
Saveframe * Saveframe::save_record(int _is_save)
{	
	this->mIsSaveFrame = _is_save;
	return this;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
Saveframe* Saveframe::SetTime(std::string _time)
{
	this->mTimeCurrent = _time;
	return this;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
Saveframe* Saveframe::SetChannel(int _ch)
{
	this->mChannel = _ch;
	return this;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
Saveframe* Saveframe::SetFrameCount(int _count)
{
	this->mFrameCount = _count;
	return this;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
Saveframe* Saveframe::SetIpAddr(std::string _ipAddr)
{
	this->mIpaddr = _ipAddr;
	return this;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void Saveframe::SaveFrame2Disk(IplImage* img_t)
{

	if (this->isInitParam()) {
	
		this->initFileName();
		this->initPath();

		mImageNameFileFullPath = this->mChannelPath+mImageName;

		 if(this->mIsSaveFrame)  cvSaveImage(mImageNameFileFullPath.c_str(),img_t);	
	
	}

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
std::string  Saveframe::initFileName()
{

	mImageName.clear();

	mImageName.append(this->mIpaddr).append(".")
		.append(PathCfg::PrefixTime).append(this->mTimeCurrent).append(".")
		.append(PathCfg::PrefixChannel).append(Base::int2str(this->mChannel)).append(".")
		.append(PathCfg::PrefixFrame).append(Base::int2str(this->mFrameCount)).append(".")
		.append("png");

	return mImageName;

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void Saveframe::initPath()
{
	if (this->isInitParam()) {

#if TRUE
		this->mIpAddrPath = Base::FS_createPath(PathCfg::ImageDataBaseRoot_CrackPath, this->mIpaddr);
		this->mTimePath = Base::FS_createPath(this->mIpAddrPath, PathCfg::PrefixTime+this->mTimeCurrent);
		this->mChannelPath = Base::FS_createPath(this->mTimePath, PathCfg::PrefixChannel +Base::int2str(this->mChannel));
#endif // TRUE	

	}
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/