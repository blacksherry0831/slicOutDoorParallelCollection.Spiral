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
QtTcpClient::QtTcpClient(QObject *parent):QtTcpClientBase(parent)
{
	
	MAX_MSECS =30000;
	mSocketRun = TRUE;
	m_buffer.clear();
	mSocketErrorOccur=0;
	mSocketReadMaxTimeOut = 0;
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

/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int QtTcpClient::Send_Start_CMD(CMD_CTRL::CMD_TYPE_02_C _type_c, CMD_CTRL::WorkMode _wm,uint _circle_seq)
{
	
	QSharedPointer<CMD_CTRL> qsp_cc_t = CMD_CTRL::getFpgaStartCmdEx(_type_c, _wm,_circle_seq);
	
	return this->Send_1_cmd(qsp_cc_t.data()  );

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int QtTcpClient::SendHearbeatCmd(int _need_resp)
{
	CMD_CTRL cmd;
	cmd.getHeartBeatCmd(_need_resp);
	return this->Send_1_cmd(&cmd);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int QtTcpClient::SendPlcResp(CMD_CTRL::CMD_TYPE_02_RESP _type)
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
	
	Q_ASSERT(_qualified == CMD_CTRL::BodyRollerQualified::Qualified ||
				_qualified == CMD_CTRL::BodyRollerQualified::UnQualified);
	

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
	const int HeaderSize = sizeof(CMD_CTRL_DATA::CMD_CTRL_HEADER);
	int DataALLSize_ = -1;
	int BodySize_ = -1;
	int READSIZE=HeaderSize;

#if TRUE
	_cmd->SetIpAddrRemote((QTcpSocket*)this);
#endif // TRUE
	

	if (mSocketRun==FALSE){
		return FALSE;
	}

	do {
	
		if (this->m_buffer.size() < HeaderSize) {
			if (this->ReadAllMy(HeaderSize) == SOCKET_STATUS::Error) return FALSE;
		}else {
			if (DataALLSize_ != -1 && this->m_buffer.size() < DataALLSize_) {
				if (this->ReadAllMy(DataALLSize_ - HeaderSize) == SOCKET_STATUS::Error) return FALSE;
			}		
		}
				
		const int CurrentBuffSize = this->m_buffer.size();

		while (CurrentBuffSize>=HeaderSize)
		{
			if (CurrentBuffSize >= HeaderSize) {			

				if ((m_buffer[0] == 'Y') &&
					(m_buffer[1] == 'j') &&
					(m_buffer[2] == 'k') &&
					(m_buffer[3] == 'j')) {					

						char *  header_data = m_buffer.data();

						BodySize_ = CMD_CTRL_DATA::GetCMDBodySize((CMD_CTRL_DATA::CMD_CTRL_HEADER*) header_data);

						if (BodySize_ < 2)	BodySize_ = 2;

						DataALLSize_ = HeaderSize + BodySize_ + 1;

						if (DataALLSize_ != -1 && CurrentBuffSize >= DataALLSize_) {
							_cmd->Parse(m_buffer.data(), DataALLSize_);
							m_buffer.remove(0, DataALLSize_);
							return TRUE;
						}

					break;
				}
				else
				{
					Q_ASSERT(0);
					m_buffer.remove(0, 1);
				}

			}




		}
					
		if (this->IsSocketAlive()==FALSE) {
			return FALSE;
		}

	} while (mSocketRun);
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
	const int body_size=_cmd->f_data_size-BODY_HEADER_SIZE;

	do
	{
		
		if (this->ReadAllMy(body_size) == SOCKET_STATUS::Error) return FALSE;

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
		

	} while (mSocketRun);

	return TRUE;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int QtTcpClient::getByteTcpRead()
{
	int data_result = 0xffffffff;
	const int GET_IDX = 0;
	const int GET_SIZE = 1;
	
	if (this->m_buffer.size()>0){

		data_result = this->m_buffer.at(GET_IDX);

		this->m_buffer.remove(GET_IDX, GET_SIZE);

	}

	return  data_result;

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
