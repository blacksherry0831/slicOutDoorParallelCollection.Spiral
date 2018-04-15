//#include "stdafx.h"
#include "QtTcpClient.hpp"
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

/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
QtTcpClient::QtTcpClient(void)
{
	
	MAX_MSECS =30000;
	m_buffer.clear();

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
QtTcpClient::~QtTcpClient(void)
{


}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int QtTcpClient::init()
{
	return 0;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
QByteArray QtTcpClient::readAllMy()
{
	QByteArray qba;
	if (this->waitForReadyRead (MAX_MSECS)) {
		qba = this->readAll();
		m_buffer.append(qba);
	}
	return qba;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int QtTcpClient::WriteMy(QByteArray _data)
{
	return this->WriteMy(_data.data(),_data.size());
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int QtTcpClient::WriteMy(char * _data, int _size)
{

	if (_size> 0) {

		int size = this->write(_data, _size);
		
		assert(size == _size);
		
		if (this->waitForBytesWritten(MAX_MSECS)) {
			return size;
		}
		
	}

	return 0;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtTcpClient::Send_Start_CMD(int _type)
{
	CMD_CTRL cmd;
	cmd.getFpgaStartCmd(_type);
	this->Send_1_cmd(&cmd);
	if (_type) {
		std::cout << "Send CMD START FPGA" << std::endl;
	}else {
		std::cout << "Send CMD STOP FPGA" << std::endl;
	}

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void  QtTcpClient::Send_1_cmd(CMD_CTRL *_cmd)
{
	std::vector<unsigned char> data = _cmd->Data();
	this->WriteMy((char*)data.data(), data.size());
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int  QtTcpClient::Read_1_cmd(CMD_CTRL *_cmd)
{
	const int HeaderSize = sizeof(CMD_CTRL::CMD_CTRL_HEADER);
	int DataALLSize_ = -1;
	int BodySize_ = -1;
	int READSIZE=HeaderSize;
	do {
	
		this->readAllMy();
		const int CurrentBuffSize = this->m_buffer.size();

		if (CurrentBuffSize >= HeaderSize) {
			
			char *  header_data = m_buffer.data();

			BodySize_=CMD_CTRL::GetCMDBodySize((CMD_CTRL::CMD_CTRL_HEADER*) header_data);
			
			if (BodySize_ == 65535) {
			
			}else{
				DataALLSize_ = HeaderSize + BodySize_ + 1;
			}


		
		}

		if (DataALLSize_ > 0 && CurrentBuffSize >= DataALLSize_) {
			break;
		}
		
		if (this->IsSocketAlive()==FALSE) {
			return FALSE;
		}

	} while (TRUE);
	// a frame is ok
	

	_cmd->Parse(m_buffer.data(),DataALLSize_);

	m_buffer.remove(0, DataALLSize_);

	return TRUE;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int QtTcpClient::IsSocketAlive()
{
	QAbstractSocket::SocketState stat_t = this->state();

	if (stat_t == QAbstractSocket::SocketState::ClosingState) {

		std::cout << "Remote socket is closing" << std::endl;
		this->waitForDisconnected();
		return 0;

	}else if (stat_t == QAbstractSocket::SocketState::UnconnectedState) {
		
		std::cout << "Remote socket is closed" << std::endl;
		return 0;

	}else{

		return 1;

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