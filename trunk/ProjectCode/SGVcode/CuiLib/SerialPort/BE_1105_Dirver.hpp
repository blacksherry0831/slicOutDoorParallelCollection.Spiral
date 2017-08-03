#pragma once

#include "cpp_stl.h"

#include "pt_mutex.h"

#include "SerialPort.h"
#include "SerialPortBase.hpp"
#include "IStepMotor.hpp"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/

class BE_1105_Driver :public SerialPortBase,public IStepMotor
{
private:
	BE_1105_Driver(void);
	~BE_1105_Driver(void);	

	 void open(int com_num);
	 void init();


/*-------------------------------------------------------*/
private:	
	static  BE_1105_Driver* _instance;
public:	
	static BE_1105_Driver* getInstance();
private:
	class CGarbo // ����Ψһ��������������������ɾ��ʵ��      
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
	// ����һ����̬��Ա���ڳ������ʱ��ϵͳ�����������������    
/*-------------------------------------------------------*/

};