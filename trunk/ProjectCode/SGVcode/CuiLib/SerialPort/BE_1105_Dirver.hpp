#pragma once

#include "cpp_stl.h"

#include "../pthreads-w32-2-9-1-release/include/pt_mutex.h"


#include "SerialPortBase/SerialPortBase.hpp"

#include "IStepMotor.hpp"

#include <QtCore>

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
	Q_OBJECT

public:
	enum BE_RESP {INIT=0X00,
		RCV_EXEC=0xB1,
		RCV_EXEC_REACH_LOC=0XB0,
		RCV_MEMORY_ERROR=0xB2,
		RCV_NO_EXEC=0xB5,
		REACH_NEG=0xA0,
		REACH_POS=0xA1,
		RUN_STATUS_RUN,
		RUN_STATUS_STOP,
		OK=0xFE,
		TIME_OUT=0xFF};
public:
	BE_1105_Driver(QObject *parent = nullptr);
	~BE_1105_Driver(void);	
public:

	int open_auto();
	int open(int com_num);
	int open(string com);
	
	void close();
	void Join();
	static void* readResultThread(void* lpParam);
	boolean IsThreadRun();
	void ReadRespData();
	void ReadRespDataAndProcess();


	int IsReady();
	int Wait4CmdDone();
	int Wait4CmdPosDone();
	

private:
	void ProcessData();
	std::vector<BE_RESP>  mLatestOrder;
private:
	
	int m_be_1105_addr;
	
	unsigned char m_status[2];
/*-------------------------------------------------------*/
public:
	unsigned char m_cmd_ctrl[17];
	unsigned char m_cmd_query[5];
	unsigned char  m_resp_data[17];
	int		buffer_result_idx;
	int		m_wait_bytes;
	int     m_cmd_mode;
	int		m_circle;
public:
	unsigned char * get_cmd(int run_mode, int speed, int circle=1);
	unsigned char * get_query_cmd();
	int  SendCmd(int run_mode, int speed, int circle=1);
	int  SendRunStatusCmd();
	int  SendCmd4Done(int run_mode, int speed, int circle = 1);
	int  SendQueryCmd(int mode);
	int  ReadResp();
	void ClearResp();

/*-------------------------------------------------------*/
private:	
	static  BE_1105_Driver* _instance;
public:	
	static BE_1105_Driver* getInstance(QThread* _thread=Q_NULLPTR);
#if TRUE
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
#endif
	public slots :
	int readComDataSlot();
};