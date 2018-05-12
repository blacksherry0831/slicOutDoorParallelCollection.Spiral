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
	mIpAddr = "192.168.100.102";
#else
	mIpAddr = "127.0.0.1";
#endif // 0

	mPort=TCP_PORT_VIDEO_TRANS;
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
	

	while (M_THREAD_RUN) {
	
		this->connect2ServerIfNoConnected();
		
		while (M_THREAD_RUN) {
		
#if 1
			{
					QSharedPointer<CMD_CTRL> cmd_t = QSharedPointer<CMD_CTRL>(new CMD_CTRL());
						
					if (m_socket->Read_1_cmd(cmd_t.data()) == 0) {
							break;
					}
				
					assert(cmd_t->IsImg());

					if (cmd_t->IsImg()) {
						this->ProcessCmd(cmd_t);
					}

			}

#endif // 1

		}
	
	}
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

		std::cout << "Image Data!" <<
			"Frame: " << cmd_t->FrameCount() <<
			"Channel:" << cmd_t->Channel() <<
			std::endl;
		const int CHANNEL = cmd_t->Channel();
		QSharedPointer<exCircleData> circleData = ChannelsData::channelsData()->getChannelData(CHANNEL);
		circleData->setImg(cmd_t);

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