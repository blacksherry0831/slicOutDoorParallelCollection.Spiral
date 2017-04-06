#pragma once

#include "opencv_stl.h"
#include "SerialPort.h"

#define NO_TURN		0x00
#define LEFT_TURN	0x01
#define RIGHT_TURN	0x02


class AGV_Dirver
{
	unsigned char	buffer_result[10];
	int		buffer_result_idx;
	unsigned char cmd_current[10];

protected:
	AGV_Dirver(void);
private:

#ifdef _MSC_VER
	CMutex m_MUTEX;
#endif

	~AGV_Dirver(void);

private:
		CSerialPort m_sp;
public:
		void open(int com_num);
		void close();
		void init();
public:
	void GetCmd(
	unsigned char* cmd,
	bool IsStart,
	int Direct,
	bool IsForward,
	bool IsRoadblock,
	unsigned int Speed);

	unsigned char GetLRC(unsigned char* cmd);

	void StartRun();
	void StopRun();
	void RunForward();
	void RunBack();
	void RunLeft();
	void RunRight();
	void RunStraight();
	/*	void SendCmdPitchRollHeading();
		
		string GetPitchRollHeadingStr();*/
	
		void ReadResultData();

		static DWORD readResultThread(LPVOID lpParam);
private:     
	void process_result_data();
	
public:
	 static vector<string> split(const string& s,char delim);
	 static AGV_Dirver* getInstance();
private:
	 static  AGV_Dirver* _instance;
	 class CGarbo // ����Ψһ��������������������ɾ��CSingleton��ʵ��      
		  {    
			    public:    

			         ~CGarbo(){      
								if (AGV_Dirver::_instance){    
										delete AGV_Dirver::_instance;
										AGV_Dirver::_instance=NULL;
										printf("Release\n");
								}
					 }    
		  };    
		  
		static CGarbo Garbo; // ����һ����̬��Ա���ڳ������ʱ��ϵͳ�����������������    

};