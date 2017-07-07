#include "CommData.h"
#include <stomp/StompCommandConstants.h>
#include <MY_SDK_LIB/Base.h>
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
#if 0
	this->m_url="http://58.240.85.222";
	this->m_websockets_url="ws://58.240.85.222";
	this->m_port=10000;
#else
	const string ip_addr="120.25.162.254";
	this->m_url="http://"+ip_addr;
	this->m_websockets_url="ws://"+ip_addr;
	this->m_port=9080;
#endif

	
	
	this->m_error="200";
	m_gps=GPS_WG_8020::getInstance();
	m_compass=Compass_HCM365::getInstance();
	this->m_random_count=1;
	m_car_status=CAR_STATUS_STOP;
	this->m_phone_number="15365078745";
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
	    random_ss_t<<frame_type_t<<m_random_count++;

		car["random"] =random_ss_t.str();

	}

	//root.append(person);
	
	std::string json_file = writer.write(car);

	Base::SaveString2Disk("heart_beat.json",json_file);

#if TRUE	
	while(json_file.c_str()[json_file.size()-1]!='}'){
		json_file.resize(json_file.size()-1);
	}
#endif



	return json_file;
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
	
	//const string ws_path_t="/car/websocket";

	const string ws_path_t="/car/hello";

#if 0
	ss<<m_websockets_url<<":"<<m_port<<ws_path_t<<"?"<<"car="<<m_car_id;
#else
	ss<<m_url<<":"<<m_port<<ws_path_t;
#endif


	const string ws_path_full=ss.str();
#if 0
	return "ws://120.25.162.254:7681";
#endif
	return ws_path_full;
}
/*----------------------------------*/
/**
*
*/
/*----------------------------------*/
string CommData::GetWsStompSendUrl()
{
	stringstream ss; //定义一个string流（使用s实例化） 
	const String AppDestPrefix="/app";
	
	const string ws_send_path_t="/hello";


	ss<<AppDestPrefix<<ws_send_path_t;
		

	return ss.str();
}
/*----------------------------------*/
/**
*
*/
/*----------------------------------*/
string CommData::GetWsStompSubscriptionUrl()
{
	stringstream ss; //定义一个string流（使用s实例化） 
	const String AppDestPrefix="";

	const string ws_send_path_t="/topic/greetings";

	ss<<AppDestPrefix<<ws_send_path_t;

	return ss.str();
}
/*----------------------------------*/
/**
*
*/
/*----------------------------------*/
string  CommData::GetWsStompSubscriptionP2pUrl()
{
	stringstream ss; //定义一个string流（使用s实例化） 
	const string UserDestinationPrefix="/user";
	const string UserId="/"+m_car_id;
	const string Msg="/message";

	ss<<UserDestinationPrefix<<UserId<<Msg;

	return ss.str();
}
/*----------------------------------*/
/**
*
*/
/*----------------------------------*/
StompFrame CommData::GetHeartBeatFrame()
{
	StompFrame sf_t;
	
	const string send_body_t=this->GetHeartBeat();

	sf_t.setCommand(StompCommandConstants::SEND);

	int  content_lengh_t=send_body_t.length();
	stringstream ss_t;
	ss_t<<content_lengh_t;

	sf_t.setProperty(StompCommandConstants::HEADER_DESTINATION,this->GetWsStompSendUrl());
	sf_t.setProperty(StompCommandConstants::HEADER_CONTENTLENGTH,ss_t.str());

//	sf_t.setPropertyContentType("application/json");

	sf_t.SetBody(send_body_t);

	return sf_t;
}

/*----------------------------------*/
/**
*
*/
/*----------------------------------*/
StompFrame CommData::GetSubscriptionFrame()
{
	string id_t=this->m_car_id;

	string dest_t=this->GetWsStompSubscriptionUrl();

	return StompFrame::GetSubscribeFrame(id_t,dest_t);
}
/*----------------------------------*/
/**
*
*/
/*----------------------------------*/
StompFrame CommData::GetSubscriptionFrameP2p()
{
	string id_t=this->m_car_id;

	string dest_t=this->GetWsStompSubscriptionP2pUrl();

	return StompFrame::GetSubscribeFrame(id_t,dest_t);
}
/*----------------------------------*/
/**
*
*/
/*----------------------------------*/