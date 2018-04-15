#include "StdAfx.h"
#include "BE_1105_Dirver.hpp"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
BE_1105_Driver* BE_1105_Driver::_instance = new BE_1105_Driver(Q_NULLPTR);
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
BE_1105_Driver::BE_1105_Driver(QObject *parent):SerialPortBase(parent)
{
	this->m_baudrate = 9600;
	m_read_thread_run = true;
	m_be_1105_addr = 0;
	m_circle = 5;
	memset(m_status, 0xAA, sizeof(m_status));
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
BE_1105_Driver::~BE_1105_Driver(void)
{
	
	m_read_thread_run=false;

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int BE_1105_Driver::open(int com_num)
{

	if (SerialPortBase::open(com_num) == TRUE) {
		#if FALSE		
					int ret = 0;	
					this->m_read_thread_run = true;
					ret = pthread_create(&m_pt_handle, NULL, readResultThread,this);
					if (ret != 0)
					{
						std::cout << "Create pthread error!" << std::endl;
						ASSERT(ret != 0);
					}
		#endif		
	}

	return init();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int BE_1105_Driver::open(string com)
{
	const size_t length= com.length();
	string com_num;
	
	for (size_t i = 0; i < length; i++)
	{
		char char_t = com.at(i);
		if ( isdigit (char_t)) {
			com_num.push_back(char_t);
		}

	}

	 int com_number=std::stoi(com_num);

	 

	 return this->open(com_number);
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
void BE_1105_Driver::Join()
{
	int ret = pthread_join(m_pt_handle, NULL);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void BE_1105_Driver::close()
{
	
	this->m_read_thread_run = false;
	this->Join();
	SerialPortBase::close();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
BE_1105_Driver* BE_1105_Driver::getInstance(QThread* _thread)
{	
	

	if (_thread != Q_NULLPTR) {
			_instance->moveToThreadQSP(_thread);
			_instance->moveToThread(_thread);
	}

	return _instance;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void* BE_1105_Driver::readResultThread(void* lpParam)
{
	assert(lpParam!=NULL);

	BE_1105_Driver*  be_1105_driver=(BE_1105_Driver*)lpParam;

	while (be_1105_driver->IsThreadRun()) {
		
		be_1105_driver->ReadRespData();

	}

	return (void*)NULL;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
boolean BE_1105_Driver::IsThreadRun()
{

	/*m_timer->timeout();*/
	return m_read_thread_run;
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
void BE_1105_Driver::ReadRespData()
{
	DWORD read_count = 0;
	int  Timeout_t = 0;
	buffer_result_idx = 0;
	memset(m_resp_data, 0, sizeof(m_resp_data));

	//TimeCountStart();
	do {
		read_count =serial_read(&m_resp_data[buffer_result_idx], 1);

		
		if (read_count==1) {
			std::cout << hex << (unsigned int) (m_resp_data[buffer_result_idx])<<"-";
				if (m_resp_data[0] ==0xB1){
					if (++buffer_result_idx == 2) {
						std::cout <<"###OK收到信号"<<"按指令执行" << std::endl;
						break;						
					}
				}else if (m_resp_data[0] == 0xB5) {
					if (++buffer_result_idx == 2) {
						std::cout <<"###ERROR收到信号" <<"没有执行"<< std::endl;
						break;				
					}
				}else if (m_resp_data[0] == 0xB2) {
					if (++buffer_result_idx == 2) {
						std::cout << "###ERROR数据进行存储中发生错误" << std::endl;
						break;					
					}
				}else if (m_resp_data[0] == 0xB0) {					
					if (++buffer_result_idx == 2) {
						std::cout << "###OK执行位置控制完成：" << m_resp_data[1] << std::endl;
						break;
					} 
				}else if (m_resp_data[0] == 0xA0) {					
					if (++buffer_result_idx == 2) {
						std::cout << "###达负限位："<< m_resp_data[1] << std::endl;
						break;
					}
				}else if (m_resp_data[0] == 0xA1) {					
					if (++buffer_result_idx == 2) {
						std::cout << "###达正限位："<< m_resp_data[1] << std::endl;
						break;
					}
				}else if (m_resp_data[0] ==m_be_1105_addr) {

					if (++buffer_result_idx == 2) {
						std::cout << "###控制器编号："<<m_resp_data[0] <<"查询内容"<<m_resp_data[1]<< std::endl;
						break;
					}

				}else {
					std::cout << "###Error response" << std::endl;
				}
				
		}else if (read_count == 0) {
			
			QThread::msleep(1000);
			Timeout_t++;
			//TimeCountStop("BE_1105>>>Read Serial Port Timeout!");

		}else {

			ASSERT(0);
		}

	} while (Timeout_t<1 * 10 && IsThreadRun());

	if (Timeout_t < 1 * 10 && IsThreadRun()) {
		this->ProcessData();
	}

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void BE_1105_Driver::ReadRespDataAndProcess()
{
	
	QByteArray qba= this->m_qsp->readAll();

	for (size_t i = 0; i <qba.size(); i++){
		m_buffer.enqueue(qba.at(i));
	}


	const int BUF_SIZE = m_buffer.size();
	
	
	while (m_buffer.size()>=2){

		unsigned char header_t = m_buffer.dequeue();std::cout << hex <<(unsigned int)header_t << "-";
		unsigned char dev_t;
				
		if (header_t == 0xB0) {
			dev_t = m_buffer.dequeue(); std::cout << hex << (unsigned int)dev_t << "-"<< "###OK执行位置控制完成：" << (unsigned int)dev_t<< std::endl;
			this->mLatestOrder.push_back(BE_RESP::RCV_EXEC_REACH_LOC);
		}else	if (header_t == 0xB1) {
			dev_t = m_buffer.dequeue(); std::cout << hex << (unsigned int)dev_t << "-"<< "###OK收到信号" << "按指令执行" << std::endl;
			this->mLatestOrder.push_back(BE_RESP::RCV_EXEC);
		}else	if (header_t == 0xB2) {
			dev_t = m_buffer.dequeue(); std::cout << hex << (unsigned int)dev_t << "-"<< "###ERROR数据进行存储中发生错误" << std::endl;
			this->mLatestOrder.push_back(BE_RESP::RCV_MEMORY_ERROR);
		}else	if (header_t == 0xB5) {
			dev_t = m_buffer.dequeue(); std::cout << hex << (unsigned int)dev_t << "-"<< "###ERROR收到信号" << "没有执行" << std::endl;
			this->mLatestOrder.push_back(BE_RESP::RCV_NO_EXEC);
		}else	if (header_t == 0xA0) {
			dev_t = m_buffer.dequeue(); std::cout << hex << (unsigned int)dev_t << "-"<< "###达负限位：" << (unsigned int)dev_t << std::endl;
			this->mLatestOrder.push_back(BE_RESP::REACH_NEG);
		}else	if (header_t == 0xA1) {
			dev_t = m_buffer.dequeue(); std::cout << hex << (unsigned int)dev_t << "-"<< "###达正限位：" << (unsigned int)dev_t << std::endl;
			this->mLatestOrder.push_back(BE_RESP::REACH_POS);
		}else	if (header_t == m_be_1105_addr){
			dev_t = m_buffer.dequeue(); std::cout << hex << (unsigned int)dev_t << "-"<< "###控制器编号：" << (unsigned int)header_t << "查询内容" << (unsigned int)dev_t << std::endl;
			if (dev_t == 1) {
				this->mLatestOrder.push_back(BE_RESP::RUN_STATUS_RUN);
			}else{
				this->mLatestOrder.push_back(BE_RESP::RUN_STATUS_STOP);
			}
			
		}else{

		}
		
	}

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void BE_1105_Driver::ProcessData()
{
	m_MUTEX.lock();

	memcpy(m_status, m_resp_data, 2);


	m_MUTEX.unlock();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int BE_1105_Driver::IsReady()
{
	int FLAG = false;
	m_MUTEX.lock();

	if (m_status[0] == 0xB1) {
		
	}else if (m_status[0] == 0xB5) {
		
	}else if (m_status[0] == 0xB2) {
	
	}else if (m_status[0] == 0xB0) {
		FLAG = true;//执行位置控制完成
	}else if (m_status[0] == 0xA0) {
		
	}else if (m_status[0] == 0xA1) {
	
	}else if (m_status[0] == m_be_1105_addr){
		if (m_status[1] == 0)
			FLAG=true;
	}else if(m_status[0] == 0x55){
		FLAG = false;
	}else if (m_status[0]==0xAA) {
		FLAG = true;
	}else{
		ASSERT(0);
	}

	
	m_MUTEX.unlock();

	return FLAG;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int BE_1105_Driver::Wait4CmdDone()
{

	int COUNT = 0;
	do {
		QThread::msleep(100);
		if (COUNT++ > 5*60*10*m_circle) {
			break;//超时退出
		}
	} while (this->IsThreadRun() && (IsReady() == false));

	return IsReady();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int BE_1105_Driver::Wait4CmdPosDone()
{
	int COUNT = 0;

	do {		
			

		this->ReadResp();//1s
		
		QThread::msleep(1000);

		if (COUNT++ > 60  * m_circle) {
			//30秒没有返回数据??	
			std::cout << "M is time out !!!" << std::endl;
			return BE_RESP::TIME_OUT;
		}
		
		if (this->mLatestOrder.size() == 0) {
			this->SendRunStatusCmd();
		}

		for (size_t i = 0; i < this->mLatestOrder.size(); i++)
		{
			const BE_RESP ORDER=this->mLatestOrder.at(i);
			if (ORDER == BE_RESP::RCV_EXEC_REACH_LOC) {
			
				return ORDER;
			
			}else if(ORDER ==BE_RESP::RCV_EXEC){
				continue;
			}else if (ORDER == BE_RESP::RCV_NO_EXEC) {
				return ORDER;
			}else{

			}
		}
		mLatestOrder.clear();
		

	} while (TRUE);

	return BE_RESP::TIME_OUT;

	
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
unsigned char * BE_1105_Driver::get_cmd(int run_mode,int speed,int circle)
{	
	const unsigned int division_factor = speed;//运动速度 6.2K
	const unsigned int be_1105_addr = 0;
	const unsigned int one_circle = 25000;
	const unsigned int run_pulse =one_circle*circle;//一圈	
	const unsigned int run_up_down_pulse = one_circle*0.02;//平滑
	m_circle = circle;

	m_cmd_ctrl[0] = 0xBA;// 实时控制指令
	m_cmd_ctrl[1] = 0x01;//单步模式

	m_cmd_ctrl[2] = division_factor/256;
	m_cmd_ctrl[3] = division_factor%256;

	m_cmd_ctrl[4] = be_1105_addr;

	if (m_cmd_ctrl[1] == 0x04) {
		m_cmd_ctrl[5] = 0;//执行次数
	}else{
		m_cmd_ctrl[5] = 0;	
	}

	m_cmd_ctrl[6] =run_mode;

	m_cmd_ctrl[7] =0x31;

	m_cmd_ctrl[8] = run_pulse/256/256;//行进脉冲
	m_cmd_ctrl[9] = run_pulse/256%256;
	m_cmd_ctrl[10] = run_pulse%256;

	m_cmd_ctrl[11] = run_up_down_pulse /256;//加速脉冲
	m_cmd_ctrl[12] = run_up_down_pulse %256;	
	
	m_cmd_ctrl[13] = run_up_down_pulse /256;//减速脉冲
	m_cmd_ctrl[14] = run_up_down_pulse %256;
	
	m_cmd_ctrl[15] = m_cmd_ctrl[0];
	for (int i = 1; i < 15; i++) {
		m_cmd_ctrl[15]^=m_cmd_ctrl[i];
	}	
	m_cmd_ctrl[16] =0xFE;

	return m_cmd_ctrl;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
unsigned char * BE_1105_Driver::get_query_cmd()
{	
	const unsigned int be_1105_run_status = 0;	

	m_cmd_query[0] = 0xB6;// 查询

	m_cmd_query[1] = m_be_1105_addr;//地址

	m_cmd_query[2] = be_1105_run_status;//状态
	
	m_cmd_query[3] = m_cmd_query[0] ^ m_cmd_query[1] ^ m_cmd_query[2];

	m_cmd_query[4] = 0xFE;//结束符

	return m_cmd_query;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int BE_1105_Driver::SendCmd(int run_mode, int speed, int circle)
{
	m_cmd_mode = 0xBA;
	memset(m_status, 0x55, sizeof(m_status));
	this->mLatestOrder.clear();
	return this->serial_write(this->get_cmd(run_mode,speed,circle), 17);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int BE_1105_Driver::SendRunStatusCmd()
{
	unsigned char cmd_t[5] = { 0xB6,0x00,0x03,0xB5,0xFE };
	
	cmd_t[1] = m_be_1105_addr;
	cmd_t[2] = 0;
	cmd_t[3] = cmd_t[0] ^ cmd_t[1] ^ cmd_t[2];

	return this->serial_write(cmd_t,sizeof(cmd_t)/sizeof(unsigned char));
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int BE_1105_Driver::SendCmd4Done(int run_mode, int speed, int circle)
{
	int IsDone;
	this->ClearResp();
	do {

		this->SendCmd(BE_1105_RUN_NEG, 55000,circle);

		IsDone =this->Wait4CmdPosDone();

		if (IsDone == BE_RESP::RCV_EXEC_REACH_LOC) {
			break;
		}

	} while (TRUE);

	return 0;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int BE_1105_Driver::SendQueryCmd(int mode)
{		
	
	m_cmd_mode = 0xB6;
	memset(m_status, 0x55, sizeof(m_status));

	if (mode == 0) {
		m_wait_bytes = 2;
	}
	return this->serial_write(get_query_cmd(), 5);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int BE_1105_Driver::ReadResp()
{
	QThread::msleep(200);
	if (m_qsp->waitForReadyRead(1000)) {
		ReadRespDataAndProcess();
	}
	else{
		std::cout << "no response ! restart" << std::endl;
	}

	

	return 0;
}
void BE_1105_Driver::ClearResp()
{
	this->mLatestOrder.clear();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int BE_1105_Driver::readComDataSlot()
{

	this->ReadRespData();
	return 0;
}