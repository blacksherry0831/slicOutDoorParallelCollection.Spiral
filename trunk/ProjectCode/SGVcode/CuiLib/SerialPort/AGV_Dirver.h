#pragma once

#include "cpp_stl.h"
#include "SerialPort.h"
#include "MY_SDK_LIB/LatLng.h"

#define NO_TURN		0x00
#define LEFT_TURN	0x01
#define RIGHT_TURN	0x02

#define  CAR_STATUS_RUN  "run"
#define  CAR_STATUS_STOP "stop"

#include "pt_mutex.h"

#include "IGps.h"
#include "ICompass.h"

class AGV_Dirver
{
	unsigned char	buffer_result[10];
	int		buffer_result_idx;
	unsigned char cmd_current[10];

protected:
	AGV_Dirver(void);
private:

	ThreadMutex m_mutex ;  
	
private:

	~AGV_Dirver(void);

private:
		CSerialPort m_sp;
		IGps*       m_gps_ptr;
		ICompass*   m_compass_ptr;
public:
		void open(int com_num);
		void close();
		void init();
private:
	int status_car_ready;
	int status_car_battery_low;
	int status_car_roadblock;
	int status_car_internal_error;
	int status_car_communication_error;
	std::string m_car_status;
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
	void RunSimulation(LatLng laatlng_t,double angle_car,double angle_run);
public:
	int Send2Car();
	void Send2CarFeedBack();
	void SendByte2Car(unsigned char* buffer_t,int size_t);
	/*	void SendCmdPitchRollHeading();
		
	string GetPitchRollHeadingStr();*/
	void ReadResultData();
	static void* readResultThread(void* lpParam);
	/*-------------------------------------------*/
	int IsCarReady(void);
	int IsCarLowPower(void);
	int IsRoadblock(void);
	int IsCarInternalError(void);
	int IsCarCommunicationError(void);
	/*-------------------------------------------*/
public:
	string status();
private:     
	void process_result_data();
	
public:
	 static vector<string> split(const string& s,char delim);
	 static AGV_Dirver* getInstance();
private:
	 static  AGV_Dirver* _instance;
	 class CGarbo // 它的唯一工作就是在析构函数中删除CSingleton的实例      
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
		  
	 static CGarbo Garbo; // 定义一个静态成员，在程序结束时，系统会调用它的析构函数    

};