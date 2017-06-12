#pragma once

#include "opencv_stl.h"
#include "SerialPort.h"
#include "pt_mutex.h"

class GPS_WG_8020
{
	char	buffer_result[1024];
	char	buffer_cmd[1024];

	int		buffer_result_idx;
	/*char CMD_VERS[]="AT^VERS\n";
	char CMD_GPRMC[]="AT^GPRMC\n";
	char CMD_ATE1[]="ate1\n";
	char CMD_ATE0[]="ate0\n";*/
public:
	double g_Lat_float;
	double g_Lon_float;

public:
	string g_UtcTime_hhmmss;//UTC时间
	string g_Location_status;//定位状态
	string g_Lat;//维度
	String g_NS;
	string g_Lon;//经度
	string g_EW;
	string g_Speed;
	String g_Direction;
	String g_UtcTime_ddmmyy;
	string g_CiPianJiao;
	string g_CiPianJiaoFangXiang;
	String g_Mode;

protected:
	GPS_WG_8020(void);
private:

	ThreadMutex    m_MUTEX;

	~GPS_WG_8020(void);
public:
		CSerialPort m_sp;

		void open(int com_num);
		void init();
		void close();

		void SendCmdEnterAT();
		void SendCmdVERS();
		void SendCmdGPRMC();
		
		string GetLatLonStr();
		string GetLatStr();
		string GetLonStr();

		void ReadGpsData();
		string ReadString();
		static DWORD readGpsThread(LPVOID lpParam);
private:
	void process_gps_data();
	void Convert2Degree();
public:
	static vector<string> split(const string& s,char delim);
	 static GPS_WG_8020* getInstance();
private:
	 static  GPS_WG_8020* _instance;
	 class CGarbo // 它的唯一工作就是在析构函数中删除CSingleton的实例      
		  {    
			    public:    

			         ~CGarbo(){      
								if (GPS_WG_8020::_instance){    
										delete GPS_WG_8020::_instance;
										GPS_WG_8020::_instance=NULL;
										printf("Release\n");
								}
					 }    
		  };    
		  
		static CGarbo Garbo; // 定义一个静态成员，在程序结束时，系统会调用它的析构函数    

};