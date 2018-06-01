#pragma once
#include "SerialPortBase/SerialPortBase.hpp"

#include "IGps.h"

class GPS_WG_8020 :public SerialPortBase,public IGps
{
	char	buffer_result[1024];
	char	buffer_cmd[1024];

	int		buffer_result_idx;
	

public:
	std::string g_UtcTime_hhmmss;//UTCʱ��
	std::string g_Location_status;//��λ״̬
	std::string g_Lat;//ά��
	std::string g_NS;
	std::string g_Lon;//����
	std::string g_EW;
	std::string g_Speed;
	std::string g_Direction;
	std::string g_UtcTime_ddmmyy;
	std::string g_CiPianJiao;//��ƫ�Ƿ���
	std::string g_CiPianJiaoFangXiang;
	std::string g_Mode;

protected:
	GPS_WG_8020(void);
private:
	~GPS_WG_8020(void);
public:
		int open(int com_num);
		int init();

		void SendCmdEnterAT();
		void SendCmdVERS();
		void SendCmdGPRMC();
		
		std::string GetLatLonStr();
		std::string GetLatStr();
		std::string GetLonStr();
		LatLng get();

		void ReadGpsData();
		std::string ReadString();
		static DWORD readGpsThread(LPVOID lpParam);
private:
	void process_gps_data();
	void Convert2Degree();
/*-------------------------------------------------------*/
public:
	 static GPS_WG_8020* getInstance();
private:
	 static  GPS_WG_8020* _instance;
	 class CGarbo // ����Ψһ��������������������ɾ��CSingleton��ʵ��      
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
		  
		static CGarbo Garbo; // ����һ����̬��Ա���ڳ������ʱ��ϵͳ�����������������    
/*-------------------------------------------------------*/
};