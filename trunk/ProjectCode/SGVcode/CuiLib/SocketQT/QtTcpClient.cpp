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
QtTcpClient::QtTcpClient(QObject *parent):QTcpSocket(parent)
{
	
	MAX_MSECS =30000;
	m_buffer.clear();
#if _DEBUG

#else
m_buffer.reserve(2 * 1024 * 1024);
#endif // _DEBUG


	

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
int QtTcpClient::ReadAllMy()
{
	QByteArray qba;
	if (this->waitForReadyRead (MAX_MSECS)) {
		qba = this->readAll();
		m_buffer.append(qba);
		return TRUE;
	
	}else {

		return this->IsSocketError();

	}
	
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int QtTcpClient::ReadMy()
{
	//QByteArray qba;	
	 char buff[16*1024];

	if (this->waitForReadyRead(MAX_MSECS)) {
	
		

		int len_t=this->read(buff, 16*1024);

//		SocketError error = this->error();
//		qDebug() << this->errorString();
		
		if (len_t > 0) {
			m_buffer.append(buff,len_t);
			return TRUE;
		
		}else{
			return FALSE;
		}
	}else {

		SocketError error=this->error();
		QString  error_str_t = this->errorString();
		if (error == SocketError::ConnectionRefusedError
			&&  error_str_t.isEmpty()) {
			//maybe time out
			return TRUE;
		}else{
			qDebug() << error_str_t;
			return FALSE;
		}
		
	}

	return IsSocketAlive();
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
int QtTcpClient::WriteMy(const char* const _data,const int _size)
{
	assert(_size > 0);
	assert(_data!=NULL);

	const char* buff_t = _data;
	int left = _size;

	do
	{
		const int size = this->write(buff_t, left);

		if (size==-1){
			return FALSE;//error
		}else if(size>=0){
	
				if (this->waitForBytesWritten(MAX_MSECS)) {
					//success
					left -= size;
					buff_t += size;

				}else{
					return FALSE;//if the operation timed out, or if an error occurred
				}


		}else{
			assert(0);

		}
		

	} while (left>0);	
	
	assert(left == 0);

	return TRUE;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int QtTcpClient::Send_Start_CMD(int _type, CMD_CTRL::WorkMode _wm)
{
	CMD_CTRL cmd;
	cmd.getFpgaStartCmd(_type,_wm);
	
	if (_type) {
		std::cout << "Send CMD START FPGA" << std::endl;
	}else {
		std::cout << "Send CMD STOP FPGA" << std::endl;
	}
	return this->Send_1_cmd(&cmd);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int QtTcpClient::SendHearbeatCmd()
{
	CMD_CTRL cmd;
	cmd.getHeartBeatCmd(0);
	return this->Send_1_cmd(&cmd);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int QtTcpClient::SendPlcResp(int _type)
{
	CMD_CTRL cmd;
	cmd.getRespPLCmd(_type);
	

	std::cout << "Client:" << "Send Resp";
	
	if (_type) {
		std::cout <<" OK  " << std::endl;
	}else {
		std::cout << " Error " << std::endl;
	}
	return this->Send_1_cmd(&cmd);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int QtTcpClient::SendPlcIntoInter(int _step)
{
	CMD_CTRL cmd;
	cmd.getPLCLRIntoCmd(_step);
	
	std::cout << "Client:" << "Send Into the internal!"<<_step << std::endl;
	return this->Send_1_cmd(&cmd);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int QtTcpClient::SendPlcRollerQualified(int _qualified)
{
	CMD_CTRL cmd;
	cmd.getRollerQualifiedCmd(_qualified);
	std::cout << "Client:" << "Send Roller qualified !" << ((_qualified ==1) ?"OK" :"Error") << std::endl;
	
	return this->Send_1_cmd(&cmd);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int  QtTcpClient::Send_1_cmd(CMD_CTRL *_cmd)
{
	std::vector<unsigned char> data = _cmd->Data();
	return this->WriteMy((char*)data.data(), data.size());
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
			if (this->ReadAllMy() == FALSE) return FALSE;
		}
		if (DataALLSize_ != -1 && this->m_buffer.size() < DataALLSize_) {
			if (this->ReadAllMy() == FALSE) return FALSE;
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
			//	assert(0);
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
int QtTcpClient::Read_1_cmd_fast(CMD_CTRL * _cmd)
{
	

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
		
		if (this->ReadAllMy() == FALSE) return FALSE;

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
		
		std::cout << "socket is unconnected" << std::endl;
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
int QtTcpClient::IsSocketError()
{

	if (this->IsSocketAlive()==TRUE) {	
	
		SocketError error = this->error();
		QString  error_str_t = this->errorString();
		
		qDebug() << error_str_t;

		if (error == SocketError::ConnectionRefusedError) {

			if (error_str_t.isEmpty()) {				
				return TRUE;//maybe time out
			}

		}else if (error == SocketError::SocketTimeoutError) {

			return TRUE;//time out

		} else {
		

		}

		
	
	}

	return FALSE;

}

/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void QtTcpClient::disconnectFromHostMy()
{
	QAbstractSocket::SocketState stat_t = this->state();
	if (stat_t== QAbstractSocket::SocketState::ConnectedState) {
		this->disconnectFromHost();
		if (QAbstractSocket::ConnectedState == this->state())
			this->waitForDisconnected();
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