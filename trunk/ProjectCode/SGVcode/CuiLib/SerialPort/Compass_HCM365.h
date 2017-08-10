#pragma once


#include "SerialPortBase.hpp"

#include "ICompass.h"

class Compass_HCM365:public SerialPortBase,public ICompass
{
	unsigned char	buffer_result[1024];
	//char	buffer_cmd[1024];

	int		buffer_result_idx;

protected:
	Compass_HCM365(void);
private:

	~Compass_HCM365(void);

public:
		bool open(int com_num);
		
		void init();
		
		void SendCmdPitchRollHeading();
		
		string GetPitchRollHeadingStr();
		string GetHeadingStr();
		double GetHead();

		void ReadCompassData();
		static DWORD readCompassThread(LPVOID lpParam);
private:     
	void process_compass_data();
	float ConvertBCD2Float(unsigned char* data_t);

/*-------------------------------------------------------*/
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
 /*-------------------------------------------------------*/
};