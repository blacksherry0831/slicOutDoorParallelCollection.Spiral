#pragma once

#include "cpp_stl.h"

#include "pt_mutex.h"

#include "SerialPort.h"
#include "SerialPortBase.hpp"
#include "IStepMotor.hpp"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
#define BE_1105_RUN_POS 0x01
#define BE_1105_RUN_NEG 0x02 
#define BE_1105_STOP	0x03 
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
class BE_1105_Driver :public SerialPortBase,public IStepMotor
{
private:
	BE_1105_Driver(void);
	~BE_1105_Driver(void);	
public:
	bool open(int com_num);
	bool open(string com);
	void init();
	void close();
	void Join();
	static void* readResultThread(void* lpParam);
	boolean IsThreadRun();
	void ReadRespData();
	bool IsReady();
	bool Wait4CmdDone();
private:
	void ProcessData();
private:
	boolean m_read_thread_run;
	int m_be_1105_addr;
	pthread_t m_pt_handle;
	unsigned char m_status[2];
/*-------------------------------------------------------*/
public:
	unsigned char m_cmd_ctrl[17];
	unsigned char m_cmd_query[5];
	unsigned char  m_resp_data[17];
	int		buffer_result_idx;
	int		m_wait_bytes;
	int     m_cmd_mode;
public:
	unsigned char * get_cmd(int run_mode);
	unsigned char * get_query_cmd();
	int  SendCmd(int mode);
	int  SendQueryCmd(int mode);
/*-------------------------------------------------------*/
private:	
	static  BE_1105_Driver* _instance;
public:	
	static BE_1105_Driver* getInstance();
private:
	class CGarbo // 它的唯一工作就是在析构函数中删除实例      
	{
	public:

		~CGarbo() {
			if (BE_1105_Driver::_instance) {
				delete BE_1105_Driver::_instance;
				BE_1105_Driver::_instance = NULL;
				std::cout <<"be 1105 driver close !"<<std::endl;
			}
		}
	};

	static CGarbo Garbo; 
	// 定义一个静态成员，在程序结束时，系统会调用它的析构函数    
/*-------------------------------------------------------*/

};