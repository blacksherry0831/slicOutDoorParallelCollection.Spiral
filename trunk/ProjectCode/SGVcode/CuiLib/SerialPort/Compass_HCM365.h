#pragma once

#include "opencv_stl.h"
#include "SerialPort.h"

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
	//string g_UtcTime_hhmmss;//UTCʱ��
	//string g_Location_status;//��λ״̬
	//string g_Lat;//ά��
	//String g_NS;
	//string g_Lon;//����
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

#ifdef _MSC_VER
	CMutex m_MUTEX;
#endif

	~Compass_HCM365(void);
public:
		CSerialPort m_sp;

		void open();
		void close();

		/*void SendCmdEnterAT();
		void SendCmdVERS();
		void SendCmdGPRMC();*/
		void SendCmdPitchRollHeading();
		
		string GetPitchRollHeadingStr();
		/*string GetLatStr();
		string GetLonStr();*/

		void ReadCompassData();
		static DWORD readCompassThread(LPVOID lpParam);
private:     
	void process_compass_data();
	float ConvertBCD2Float(unsigned char* data_t);
public:
	static vector<string> split(const string& s,char delim);
	 static Compass_HCM365* getInstance();
private:
	 static  Compass_HCM365* _instance;
	 class CGarbo // ����Ψһ��������������������ɾ��CSingleton��ʵ��      
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
		  
		static CGarbo Garbo; // ����һ����̬��Ա���ڳ������ʱ��ϵͳ�����������������    

};