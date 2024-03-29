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

	if (this->mIpAddrRemote == "" ||
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
	this->mIpAddrRemote = "";

	this->mCmd = QSharedPointer < CMD_CTRL >(new CMD_CTRL());

	this->mChannel = -1;

	this->mFrameCount = -1;	
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
Saveframe* Saveframe::start_record(QSharedPointer<CMD_CTRL> _img_cmd)
{
	const std::string time_t = _img_cmd->mCurrentCircleTime;

	this->initParam();
	this->init_CMD_CTRL(_img_cmd)->SetStartTime(time_t);

	this->init_ip_time_path();
	this->init_xml();

	return this;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
Saveframe* Saveframe::stop_record()
{	
	this->save_xml(QString::fromStdString(mTimePath+mFeatureFileName), mXml);
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
void Saveframe::init_xml()
{
	this->mXml = QSharedPointer<QDomDocument>(new QDomDocument());

	QDomProcessingInstruction Qdp_t=this->mXml->createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");

	this->mXml->appendChild(Qdp_t);

	QDomElement root = this->mXml->createElement("Feature");

	this->mXml->appendChild(root);

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void Saveframe::save_xml(QString _save_full_path,QSharedPointer<QDomDocument> _Xml)
{

	if (!mXml.isNull() && !_save_full_path.isEmpty())
	{
				QString  save_full_path = _save_full_path;

				QFile file(save_full_path);

				if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))

					return;

				QTextStream out(&file);

				out.setCodec("UTF-8");

				_Xml->save(out, 4, QDomNode::EncodingFromTextStream);

				file.close();	

	}


}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void Saveframe::add_xml()
{
	if (!mXml.isNull())
	{
					QDomElement root = this->mXml->documentElement();
					QDomElement frame = this->mXml->createElement("frame");

						{
								QDomElement filename = this->mXml->createElement("filename");
								QDomText filename_v=this->mXml->createTextNode(QString::fromStdString(mImageName));
								filename.appendChild(filename_v);

								QDomElement frameidx = this->mXml->createElement("frameidx");
								QDomText frameidx_v = this->mXml->createTextNode(QString::number(mFrameCount));
								frameidx.appendChild(frameidx_v);

								QDomElement channel = this->mXml->createElement("channel");
								QDomText channel_v= this->mXml->createTextNode(QString::number(mChannel));
								channel.appendChild(channel_v);

								QDomElement feature = this->mXml->createElement("feature");
								QDomText feature_v = this->mXml->createTextNode(GetVectorFloat(mCmd->mFeature));
								feature.appendChild(feature_v);

								frame.appendChild(filename);
								frame.appendChild(frameidx);
								frame.appendChild(channel);
								frame.appendChild(feature);

						}


						root.appendChild(frame);
	}



}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
QString Saveframe::GetVectorFloat(std::vector<float> _float)
{
	QString f_v_t;
	for (const auto f_data_t: _float)
	{
		QString f_str = QString("%1").arg(f_data_t);
		f_v_t.append(f_str).append(" ");
	}
	
	return f_v_t;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
Saveframe * Saveframe::set_Param(QSharedPointer<CMD_CTRL> _img_cmd)
{
	this->mChannel = _img_cmd->Channel();

	this->mIpAddrRemote = _img_cmd->mIpAddrRemote;

	this->mFrameCount = _img_cmd->FrameCount();
	
	return this;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
Saveframe * Saveframe::init_CMD_CTRL(QSharedPointer<CMD_CTRL> _img_cmd)
{
	
	return this->set_Param(_img_cmd)->set_CMD_CTRL(_img_cmd);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
Saveframe * Saveframe::set_CMD_CTRL(QSharedPointer<CMD_CTRL> _img_cmd)
{
	this->mCmd->mFeature = _img_cmd->mFeature;

	this->mCmd->mImgProc = _img_cmd->mImgProc;

	this->mCmd->mClassify = _img_cmd->mClassify;

	return this;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
Saveframe* Saveframe::SetStartTime(std::string _time)
{
	this->mCmd->mCurrentCircleTime = _time;
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
				this->add_xml();

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

	mImageName.append(this->mIpAddrRemote).append(".")
		.append(PathCfg::PrefixTime).append(this->mCmd->mCurrentCircleTime).append(".")
		.append(PathCfg::PrefixChannel).append(Base::int2str(this->mChannel)).append(".")
		.append(PathCfg::PrefixFrame).append(Base::int2str(this->mFrameCount)).append(".")
		.append(PathCfg::PrefixSigma).append(Base::int2str(this->mCmd->mImgProc.Sigma)).append(".")
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
	
	const int pos_neg = this->mCmd->mClassify;

	if (this->mCmd->mFeature.size() == 0)
	{
		this->init_normal_path();
	}
	else
	{
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
	Q_ASSERT(_pos_neg==1 || _pos_neg==0 || _pos_neg == -1);
	
	if (_pos_neg == 1 ){
		pos_neg_prefix ="p" ;
	}else if (_pos_neg == -1){
		pos_neg_prefix = "n";
	}else if(_pos_neg == 0){
		pos_neg_prefix = "";
	}else{

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
		this->mChannelPath = Base::FS_createPath(this->mTimePath, PathCfg::PrefixChannel + Base::int2str(this->mChannel)+_prefix);
	}
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void Saveframe::init_ip_time_path()
{		

		if (0 != this->mIpAddrRemote.compare("") &&
			0 != this->mCmd->mCurrentCircleTime.compare("")) {

					this->mIpAddrPath = Base::FS_createPath(PathCfg::ImageDataBaseRoot_CrackPath, this->mIpAddrRemote, mIsSaveFrame);
					this->mTimePath = Base::FS_createPath(this->mIpAddrPath, PathCfg::PrefixTime + this->mCmd->mCurrentCircleTime, mIsSaveFrame);
					this->mFeatureFileName.clear();

					this->mFeatureFileName.append(this->mIpAddrRemote).append(".")
						.append(this->mCmd->mCurrentCircleTime).append(".")
						.append("ch").append(Base::int2str(mChannel ))
						.append(".xml");

		}		
	
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/