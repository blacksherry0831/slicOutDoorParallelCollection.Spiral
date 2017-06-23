#include "CommData.h"
#include<iostream>
#include<sstream>
#include<string>
#include <fstream>
#include <assert.h>
/*----------------------------------*/
/**
*
*/
/*----------------------------------*/
CommData::CommData(void)
{
	this->m_user_id="";
	this->m_car_id="";
	this->m_current_task="task";
	this->m_url="http://58.240.85.222";
	this->m_websockets_url="ws://58.240.85.222";
	this->m_port=10000;
	this->m_error="200";
	m_gps=GPS_WG_8020::getInstance();
	m_compass=Compass_HCM365::getInstance();
	this->m_random_count=1;
	m_car_status=CAR_STATUS_STOP;
}
/*----------------------------------*/
/**
*
*/
/*----------------------------------*/
CommData::~CommData(void)
{

}
/*----------------------------------*/
/**
*
*/
/*----------------------------------*/
string CommData::GetHeartBeat()
{
	//Json::Value root;
	Json::FastWriter writer;
	Json::Value car;
	const string frame_type_t="heartbeat_req@";
	{
		car["car_id"] = m_car_id;
		car["task"] =m_current_task;
		car["longitude"] =m_gps->GetLonStr();
		car["latitude"] =m_gps->GetLatStr();
		car["compass"] =m_compass->GetHeadingStr();
		car["battery"] ="0.7";
		car["status"] =m_car_status;
		car["roadblock"] ="false";
		car["speed"] ="7";
		car["error"] =m_error;
		car["warning"] ="200";
		car["camera"] ="200";

		car["type"] ="car";
		car["to"] =m_user_id;
		car["frame_type"] =frame_type_t;
		
	    stringstream random_ss_t;
	    random_ss_t<<frame_type_t<<m_random_count;

		car["random"] =random_ss_t.str();

	}

	//root.append(person);
	
	std::string json_file = writer.write(car);

	SaveString2Disk("heart_beat.json",json_file);

	return json_file;
}
/*----------------------------------*/
/**
*
*/
/*----------------------------------*/
void CommData::SaveString2Disk(string file_name_t,string str_t)
{
#if _DEBUG	

	ofstream ofs;
	ofs.open(file_name_t);
	
	assert(ofs.is_open());
	ofs<<str_t;

	ofs.close();

#endif
}
/*----------------------------------*/
/**
*
*/
/*----------------------------------*/
string CommData::GetUrlRegisterCar()
{
	string url_path_t="/car/car/addCar";

	string param0="car_id="+m_car_id;
	string param1="userId="+m_user_id;

	stringstream ss; //定义一个string流（使用s实例化） 

	ss<<m_url<<":"<<m_port<<url_path_t<<"?"<<param0<<"&"<<param1;

	string url_p=ss.str();

	return url_p;
}
/*----------------------------------*/
/**
*
*/
/*----------------------------------*/
void  CommData::InputCarUserId()
{
	char buffer_t[1024];

	cout<<"Please Input Car Id:"<<endl;	
	fflush(stdin);
	cin.getline(buffer_t,1024);
	this->m_car_id=buffer_t;

	cout<<"Please Input User Id:"<<endl;
	fflush(stdin);
	cin.getline(buffer_t,1024);
	this->m_user_id=buffer_t;
	/*----*/

	if (this->m_user_id.compare("")==0){
		this->m_user_id="11111";
	}

	if (this->m_car_id.compare("")==0){
		this->m_car_id="1";
	}

#if _DEBUG
	cout<<"User Name Is :"<<this->m_user_id<<endl;
	cout<<"Car Id Is :"<<this->m_car_id<<endl;
#endif

}
/*----------------------------------*/
/**
*
*/
/*----------------------------------*/
string CommData::GetUrlTaskLongitudeLatitude()
{
	const string url_path_t="/car/car/one";
	
	stringstream ss; //定义一个string流（使用s实例化） 

	ss<<m_url<<":"<<m_port<<url_path_t<<"?"<<"task="<<m_current_task;

	string url_p=ss.str();

	return url_p;
}
/*----------------------------------*/
/**
*
*/
/*----------------------------------*/
string CommData::GetWsUrl()
{
	stringstream ss; //定义一个string流（使用s实例化） 
	const string ws_path_t="/car/websocket";

	ss<<m_websockets_url<<":"<<m_port<<ws_path_t<<"?"<<"car="<<m_car_id;

	const string ws_path_full=ss.str();

	return ws_path_full;
}
/*----------------------------------*/
/**
*
*/
/*----------------------------------*/

/*----------------------------------*/
/**
*
*/
/*----------------------------------*/
