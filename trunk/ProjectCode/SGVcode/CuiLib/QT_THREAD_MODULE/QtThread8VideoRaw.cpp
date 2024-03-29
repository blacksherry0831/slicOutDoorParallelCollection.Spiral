//#include "stdafx.h"
#include "QtThread8VideoRaw.hpp"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
void QtThread8VideoRaw::initIpPort()
{
#if 1
	mIpAddr = BORD_VIDEO_OUT;
#else
	mIpAddr = "127.0.0.1";
#endif // 0

	mPort= TCP_PORT_VIDEO_RAW;
}
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
QtThread8VideoRaw::QtThread8VideoRaw()
{
	initIpPort();
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
QtThread8VideoRaw::~QtThread8VideoRaw(void)
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

/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThread8VideoRaw::run()
{
	
	const int WIDTH = 1920;
	const int HEIGHT = 1080;
	const int nChannels = 1;
	int VideoChannel = 0;

	while (M_THREAD_RUN) {
	
		this->connect2ServerIfNoConnected();
		
		while (M_THREAD_RUN) {
		
#if 1
			
					QSharedPointer<CMD_CTRL> cmd_t = QSharedPointer<CMD_CTRL>(new CMD_CTRL());
					
					cmd_t->InitImgCtrlHeader(VideoChannel++%8,WIDTH,HEIGHT,nChannels);

					if (m_socket->Read_nSize_2_body(cmd_t.data()) == 0) {
							break;
					}					

					if (cmd_t->IsImg()) {
						QtThread8Video::ProcessCmd(cmd_t);
					}else if (cmd_t->isHeartbeatCmd()) {
						//std::cout << "@" << std::endl;
					}else{
						 std::cout << "ErrorCmd" << std::endl;
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