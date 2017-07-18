#pragma once

#include "cpp_stl.h"
#include "SerialPort.h"

#include "pt_mutex.h"

#include "ICompass.h"

class Compass_HCM365:public ICompass
{
	unsigned char	buffer_result[1024];
	//char	buffer_cmd[1024];

	int		buffer_result_idx;
	/*char CMD_VERS[]="AT^VERS\n";
	char CMD_GPRMC[]="AT^GPRMC\n";
	char CMD_ATE1[]="ate1\n";
	char CMD_ATE0[]="ate0\n";*/
public:
	

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
		double GetHead();

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