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
void QtTcpClient::SendPlcResp(int _type)
{
	CMD_CTRL cmd;
	cmd.getRespPLCmd(_type);
	this->Send_1_cmd(&cmd);

	std::cout << "Client:" << "Send Resp";
	
	if (_type) {
		std::cout <<" OK  " << std::endl;
	}else {
		std::cout << " Error " << std::endl;
	}
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtTcpClient::SendPlcIntoInter(int _step)
{
	CMD_CTRL cmd;
	cmd.getPLCLRIntoCmd(_step);
	this->Send_1_cmd(&cmd);
	std::cout << "Client:" << "Send Into the internal!"<<_step << std::endl;
	
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtTcpClient::SendPlcRollerQualified(int _qualified)
{
	CMD_CTRL cmd;
	cmd.getRollerQualifiedCmd(_qualified);
	this->Send_1_cmd(&cmd);

	std::cout << "Client:" << "Send Roller qualified !" << ((_qualified ==1) ?"OK" :"Error") << std::endl;
	
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
	
		if (this->m_buffer.size() < HeaderSize) {
			this->readAllMy();
		}
		if (DataALLSize_ != -1 && this->m_buffer.size() < DataALLSize_) {
			this->readAllMy();
		}

		while (this->m_buffer.size()>=HeaderSize)
		{

			if(	(m_buffer[0]=='Y')&&
				(m_buffer[1]=='j')&&
				(m_buffer[2]=='k')&&
				(m_buffer[3] == 'j')){
#if TRUE
				const int CurrentBuffSize = this->m_buffer.size();
				if (CurrentBuffSize >= HeaderSize) {

					char *  header_data = m_buffer.data();

					BodySize_ = CMD_CTRL::GetCMDBodySize((CMD_CTRL::CMD_CTRL_HEADER*) header_data);

					if (BodySize_ < 2)	BodySize_ = 2;
				
					DataALLSize_ = HeaderSize + BodySize_ + 1;
				
					if (DataALLSize_ != -1 && this->m_buffer.size() >= DataALLSize_) {
						_cmd->Parse(m_buffer.data(), DataALLSize_);
						m_buffer.remove(0, DataALLSize_);
						return TRUE;
					}

				}

#endif // TRUE
				break;
			}
			else
			{
				m_buffer.remove(0, 1);
			}

		}
					
		if (this->IsSocketAlive()==FALSE) {
			return FALSE;
		}

	} while (TRUE);
	// a frame is ok
	



	return TRUE;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int QtTcpClient::Read_nSize_2_body(CMD_CTRL * _cmd)
{
	const int BODY_HEADER_SIZE = sizeof(IplImageUI);
	unsigned int body_size=_cmd->f_data_size-BODY_HEADER_SIZE;

	do
	{
		
		this->readAllMy();

		if (m_buffer.size() >= body_size) {
			//read enough data
			char *data_src = m_buffer.data();
			uchar *data_dst = (_cmd->f_data.data())+BODY_HEADER_SIZE;

			memcpy(data_dst, data_src, body_size);
			m_buffer.remove(0, body_size);
			break;	
		}

		if (this->IsSocketAlive() == FALSE) {
			return FALSE;
		}
		

	} while (TRUE);

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

		std::cout << "socket is closing" << std::endl;
		this->waitForDisconnected();
		return 0;

	}else if (stat_t == QAbstractSocket::SocketState::UnconnectedState) {
		
		std::cout << "socket is closed" << std::endl;
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