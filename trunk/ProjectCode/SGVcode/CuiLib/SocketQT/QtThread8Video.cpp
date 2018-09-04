//#include "stdafx.h"
#include "QtThread8Video.hpp"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
void QtThread8Video::initIpPort()
{
#if 1
	mIpAddr = BORD_VIDEO_OUT;
#else
	mIpAddr = "127.0.0.1";
#endif // 0

	mPort=TCP_PORT_VIDEO_TRANS;

	this->mThreadName = "Image Data Thread ";
}
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
QtThread8Video::QtThread8Video()
{
	initIpPort();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
QtThread8Video::QtThread8Video(qintptr p)
{
	initIpPort();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
QtThread8Video::~QtThread8Video(void)
{
	qDebug() << "QtThread8Video is Release ! ";
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
void QtThread8Video::Run0()
{

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThread8Video::run1()
{

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThread8Video::run()
{
	this->emit_status_message(mStatusMessage = "Thread>> Ctrl Thread Start");

	while (M_THREAD_RUN) {
	
		this->connect2ServerIfNoConnected();
		
		while (M_THREAD_RUN) {
#if 1
			{
					QSharedPointer<CMD_CTRL> cmd_t = QSharedPointer<CMD_CTRL>(new CMD_CTRL());
						
					if (m_socket->Read_1_cmd(cmd_t.data()) == 0) {
							break;
					}					

					if (cmd_t->IsImg()) {
						  this->ProcessCmd(cmd_t);
						  this->emit_img_signals(cmd_t);
					}else if (cmd_t->isHeartbeatCmd()) {
#if 0
						std::cout << "@" << std::endl;
#endif // 0						
					}else{
						 std::cout << "ErrorCmd" << std::endl;
					}

			}

#endif // 1

		}

		this->closeSocket4Server();
	
	}

#if _DEBUG
	this->emit_status_message(mStatusMessage = "Thread>>  shutdown");
#endif // _DEBUG

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThread8Video::ProcessCmd(QSharedPointer<CMD_CTRL> cmd_t)
{
	if (cmd_t->IsImgStart()) {

		std::cout << "Image Start!" << std::endl;
		ChannelsData::channelsData()->init();
	}
	else if (cmd_t->IsImgEnd()) {

		std::cout << "Image Stop!" << std::endl;
		ChannelsData::channelsData()->destory();
	}
	else if (cmd_t->IsImgFrame()) {

		const int CHANNEL = cmd_t->Channel();
		QSharedPointer<exCircleData> circleData = ChannelsData::channelsData()->getChannelData(CHANNEL);
		circleData->setImg(cmd_t);
#if _DEBUG

		std::cout << "Image Data!" <<
			"Frame: " << cmd_t->FrameCount() <<
			"Channel:" << cmd_t->Channel() <<
			"FPS: "<<circleData->fps()<<
			std::endl;
#endif // _DEBUG


	}
	else {
		std::cout << "Image ERROR!" << std::endl;
		assert(FALSE);
	}
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThread8Video::emit_img_signals(QSharedPointer<CMD_CTRL> cmd_t)
{
	if (cmd_t->IsImgStart()) {
		emit img_stat(cmd_t->CmdStat(),0,0);
	}else if (cmd_t->IsImgEnd()) {	
		emit img_stat(cmd_t->CmdStat(),0,0);
	}else if (cmd_t->IsImgFrame()) {	
		const int CHANNEL = cmd_t->Channel();
		QSharedPointer<exCircleData> circleData = ChannelsData::channelsData()->getChannelData(CHANNEL);
		emit img_stat(cmd_t->CmdStat(),CHANNEL, circleData->QueueSize());
	}else {
			assert(FALSE);
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