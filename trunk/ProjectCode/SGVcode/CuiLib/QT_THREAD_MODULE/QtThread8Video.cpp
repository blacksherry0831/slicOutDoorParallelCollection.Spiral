//#include "stdafx.h"
#include "QtThread8Video.hpp"
#include "QtThread8VideoProcess.hpp"
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
void QtThread8Video::save_record(int _is_save)
{
	ChannelsData4Show::getInstance()->save_record(_is_save);
}
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
void QtThread8Video::before_enter_thread()
{
	this->setPriorityMy();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThread8Video::ProcessCmd(QSharedPointer<CMD_CTRL> _cmd)
{

	ChannelsData* channelsData = ChannelsData::getInstance();
	channelsData->EnqueueImgAll(_cmd);
#if _DEBUG && 0
	if (_cmd->IsImgStart()) {
		std::cout << "Image Start!" << std::endl;
	}else if (_cmd->IsImgEnd()) {
		std::cout << "Image Stop!" << std::endl;
	}else if (_cmd->IsImgFrame()) {	
		const int CHANNEL = _cmd->Channel();
		QSharedPointer<exCircleData> circleData = channelsData->getChannelData(CHANNEL);
		std::cout << "Image Data!" <<
			"Frame: " << _cmd->FrameCount() <<
			"Channel:" << _cmd->Channel() <<
			"FPS: "<< circleData->fps()<<
			std::endl;		
	}else {
		std::cout << "Image ERROR!" << std::endl;
		Q_ASSERT(FALSE);
	}

#endif // _DEBUG
	
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThread8Video::setPriorityMy()
{
#if USE_THREAD_CRACK_PRIORITY
	QThread::currentThread()->setPriority(QThread::Priority::HighPriority);
#endif
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
void QtThread8Video::emit_img_signals(QSharedPointer<CMD_CTRL> cmd_t)
{
	ChannelsData* channelsData = ChannelsData::getInstance();

	if (cmd_t->IsImgStart()) {
		emit img_stat(cmd_t->CmdStat(),0,0);
	}else if (cmd_t->IsImgEnd()) {	
		emit img_stat(cmd_t->CmdStat(),0,0);
	}else if (cmd_t->IsImgFrame()) {	
		const int CHANNEL = cmd_t->Channel();
		QSharedPointer<exCircleData> circleData = channelsData->getChannelData(CHANNEL);
		emit img_stat(cmd_t->CmdStat(),CHANNEL, circleData->QueueSize());
	}else {
			Q_ASSERT(FALSE);
	}
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void  QtThread8Video::set_record_time(QSharedPointer<CMD_CTRL> _cmd)
{
#if _DEBUG
	const uint seq = _cmd->GetCmdFrameSeq();
#endif
	if (_cmd->IsImgStart()) {

		mTimeCurrent = _cmd->mCurrentCircleTime;

	}else if (_cmd->IsImgEnd()) {

		mTimeCurrent.clear();

	}else if (_cmd->IsImgFrame()){

		
	}else {

	}

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtThread8Video::run_socket_work()
{

		QSharedPointer<CMD_CTRL> cmd_t = QSharedPointer<CMD_CTRL>(new CMD_CTRL());

		if (Read_1_cmd_process_hearbeat(cmd_t) == 0) {
			return;
		}

		if (cmd_t->IsImg()) {

			this->set_record_time(cmd_t);
			this->ProcessCmd(cmd_t);
			this->emit_img_signals(cmd_t);

		}
		else if (cmd_t->isHeartbeatCmd()) {
#if TRUE
			std::cout << "@ImgTransfer" << std::endl;
#endif // 0						
		}
		else {
			std::cout << "ErrorCmd" << std::endl;
#if  _DEBUG
			if (GetSocketConnected()){
				//	Q_ASSERT(0);
				std::cout << "Socket DisConnected" << std::endl;
			}
#endif //  _DEBUG
		}


}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
