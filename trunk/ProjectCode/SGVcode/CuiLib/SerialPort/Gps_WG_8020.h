#pragma once
#include "SerialPortBase.hpp"

#include "IGps.h"

class GPS_WG_8020 :public SerialPortBase,public IGps
{
	char	buffer_result[1024];
	char	buffer_cmd[1024];

	int		buffer_result_idx;
	

public:
	string g_UtcTime_hhmmss;//UTC时间
	string g_Location_status;//定位状态
	string g_Lat;//维度
	string g_NS;
	string g_Lon;//经度
	string g_EW;
	string g_Speed;
	string g_Direction;
	string g_UtcTime_ddmmyy;
	string g_CiPianJiao;//磁偏角方向
	string g_CiPianJiaoFangXiang;
	string g_Mode;

protected:
	GPS_WG_8020(void);
private:
	~GPS_WG_8020(void);
public:
		bool open(int com_num);
		void init();

		void SendCmdEnterAT();
		void SendCmdVERS();
		void SendCmdGPRMC();
		
		string GetLatLonStr();
		string GetLatStr();
		string GetLonStr();
		LatLng get();

		void ReadGpsData();
		string ReadString();
		static DWORD readGpsThread(LPVOID lpParam);
private:
	void process_gps_data();
	void Convert2Degree();
/*-------------------------------------------------------*/
public:
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
/*-------------------------------------------------------*/
};