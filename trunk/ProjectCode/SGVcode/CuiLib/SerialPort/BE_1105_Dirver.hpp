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
	class CGarbo // 它的唯一工作就是在析构函数中删除实例      
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
	// 定义一个静态成员，在程序结束时，系统会调用它的析构函数    
/*-------------------------------------------------------*/

};