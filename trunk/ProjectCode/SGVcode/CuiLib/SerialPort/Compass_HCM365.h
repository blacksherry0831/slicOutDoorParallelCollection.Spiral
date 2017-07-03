#pragma once

#include "opencv_stl.h"
#include "SerialPort.h"

#include "pt_mutex.h"

class Compass_HCM365
{
	unsigned char	buffer_result[1024];
	//char	buffer_cmd[1024];

	int		buffer_result_idx;
	/*char CMD_VERS[]="AT^VERS\n";
	char CMD_GPRMC[]="AT^GPRMC\n";
	char CMD_ATE1[]="ate1\n";
	char CMD_ATE0[]="ate0\n";*/
public:
	//double g_Lat_float;
	//double g_Lon_float;

private:
	float m_pitch;
	float m_roll;
	float m_heading;
	//string g_UtcTime_hhmmss;//UTC时间
	//string g_Location_status;//定位状态
	//string g_Lat;//维度
	//String g_NS;
	//string g_Lon;//经度
	//string g_EW;
	//string g_Speed;
	//String g_Direction;
	//String g_UtcTime_ddmmyy;
	//string g_CiPianJiao;
	//string g_CiPianJiaoFangXiang;
	//String g_Mode;

protected:
	Compass_HCM365(void);
private:
	
	ThreadMutex m_MUTEX;  

	~Compass_HCM365(void);

private:
		CSerialPort m_sp;
public:
		void open(int com_num);
		void close();
		void init();
		/*void SendCmdEnterAT();
		void SendCmdVERS();
		void SendCmdGPRMC();*/
		void SendCmdPitchRollHeading();
		
		string GetPitchRollHeadingStr();
		string GetHeadingStr();
		/*string GetLatStr();
		string GetLonStr();*/

		void ReadCompassData();
		static DWORD readCompassThread(LPVOID lpParam);
private:     
	void process_compass_data();
	float ConvertBCD2Float(unsigned char* data_t);
public:
	
	 static Compass_HCM365* getInstance();
private:
	 static  Compass_HCM365* _instance;
	 class CGarbo // 它的唯一工作就是在析构函数中删除CSingleton的实例      
		  {    
			    public:    

			         ~CGarbo(){      
								if (Compass_HCM365::_instance){    
										delete Compass_HCM365::_instance;
										Compass_HCM365::_instance=NULL;
										printf("Release\n");
								}
					 }    
		  };    
		  
		static CGarbo Garbo; // 定义一个静态成员，在程序结束时，系统会调用它的析构函数    

};