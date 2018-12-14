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
	this->save_record(FALSE);
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
		this->mCmd->mCurrentCircleTime == "" ||
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

	this->mCmd = QSharedPointer < CMD_CTRL >(new CMD_CTRL());

	this->mChannel = -1;

	this->mFrameCount = -1;	
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
Saveframe * Saveframe::SetImgCmd(QSharedPointer<CMD_CTRL> _img_cmd)
{
	
	
	this->mCmd->mFeature = _img_cmd->mFeature;
	this->mCmd->mImgProc = _img_cmd->mImgProc;

	return this;
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
Saveframe* Saveframe::SetTime(std::string _time)
{
	this->mCmd->mCurrentCircleTime = _time;
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
		
		if (this->mIsSaveFrame) {

				this->initFileName();
				this->initPath();

				mImageNameFileFullPath = this->mChannelPath + mImageName;
				cvSaveImage(mImageNameFileFullPath.c_str(), img_t);

		}

	
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
		.append(PathCfg::PrefixTime).append(this->mCmd->mCurrentCircleTime).append(".")
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
	const float feature_t = this->mCmd->mFeature;
	const float feature_th_t = this->mCmd->mImgProc.ThresholdClassifyThickly;
	int pos_neg;

	if (feature_t==-1)
	{
			this->init_normal_path();
	}
	else
	{
			if (feature_t<feature_th_t) {
				pos_neg = 1;
			}
			else {
				pos_neg = 0;
			}
			this->init_pos_neg_path(pos_neg);
	}


	
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void Saveframe::init_pos_neg_path(int _pos_neg)
{
	std::string pos_neg_prefix;
	Q_ASSERT(_pos_neg==1 || _pos_neg==0);
	if (_pos_neg)
	{
		pos_neg_prefix ="p" ;
	}
	else
	{
		pos_neg_prefix = "n";
	}

	this->init_normal_path(pos_neg_prefix);

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void Saveframe::init_normal_path(std::string _prefix)
{
	if (this->isInitParam()) {	
#if TRUE
		this->mIpAddrPath = Base::FS_createPath(PathCfg::ImageDataBaseRoot_CrackPath, this->mIpaddr);
		this->mTimePath = Base::FS_createPath(this->mIpAddrPath, PathCfg::PrefixTime + this->mCmd->mCurrentCircleTime);
		this->mChannelPath = Base::FS_createPath(this->mTimePath, PathCfg::PrefixChannel + Base::int2str(this->mChannel)+_prefix);
#endif // TRUE	

	}
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/