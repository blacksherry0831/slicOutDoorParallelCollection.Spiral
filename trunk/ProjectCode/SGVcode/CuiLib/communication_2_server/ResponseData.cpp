#include "ResponseData.h"

#include "MY_SDK_LIB/Base.h"
/*----------------------------------*/
/**
*
*/
/*----------------------------------*/
ResponseData::ResponseData(void)
{
	this->Clear();
}
/*----------------------------------*/
/**
*
*/
/*----------------------------------*/
ResponseData::~ResponseData(void)
{

}
/*----------------------------------*/
/**
*
*/
/*----------------------------------*/
ResponseData::ResponseData(std::string data_t)
{
	this->parse(data_t);
}
/*----------------------------------*/
/**
*
*/
/*----------------------------------*/
void ResponseData:: Clear()
{
	this->m_result ="";
	this->m_random="";

	this->m_mode="";
	this->m_cmd="";
	this->m_random_count=-1;
	this->m_resp_result=-1;
}
/*----------------------------------*/
/**
*
*/
/*----------------------------------*/
int ResponseData::parse(std::string data_t)
{
	 Json::Reader Jreader;
	 Json::Value Jvalue;

	 this->Clear();

	if (Jreader.parse(data_t, Jvalue))
	{
		this->m_result = Jvalue["result"].asString();
		this->m_random= Jvalue["random"].asString();
		
		this->m_mode=Jvalue["mode"].asString();
		this->m_cmd=Jvalue["cmd"].asString();

		if(!Base::IsEqual(m_result,"") && !Base::IsEqual(m_random,"")){
			return this->parseResp(Jvalue);
		}

		if(this->IsCmd()){
			return this->parseCmd(Jvalue);
		}


	}

		return -1;
		
}
/*----------------------------------*/
/**
*
*/
/*----------------------------------*/
int ResponseData::parseResp(Json::Value reader)
{
		if (this->m_result.compare("success")==0){
			m_resp_result=1;
		}else{
			m_resp_result=0;
		}

		std::vector<std::string> random_data_t=Base::split(m_random,'@');
		if (random_data_t.size()>=2){
			this->m_random_count= atoi(random_data_t.at(1).c_str()) ;
			this->m_resp_type=random_data_t.at(0);
		}
		
		return m_resp_result;
	
}
/*----------------------------------*/
/**
*
*/
/*----------------------------------*/
int ResponseData::parseCmd(Json::Value value_t)
{

	m_car_id= value_t["car_id"].asString();
	m_mode= value_t["mode"].asString();
	m_cmd= value_t["cmd"].asString();
	
	m_longitude= value_t["longitude"].asString();
	m_latitude= value_t["latitude"].asString();
	
	m_speed= value_t["speed"].asString();

	m_auto_task= value_t["task"].asString();

	m_manual_turn= value_t["turn"].asString();
	m_manual_run_direction= value_t["direction"].asString();

	return TRUE;
}
/*----------------------------------*/
/**
*
*/
/*----------------------------------*/
int ResponseData::IsHeartbeat()
{

	if (this->m_resp_result==TRUE &&
		this->m_resp_type.compare("heartbeat_req")==0){
			return 1;
	}

	return 0;

}
/*----------------------------------*/
/**
*
*/
/*----------------------------------*/
int ResponseData::IsCmd()
{

	if(!Base::IsEqual(m_mode,"") && !Base::IsEqual(m_cmd,"")){
		return TRUE; //this is a cmd!
	}else{
		return FALSE;
	}

}
/*----------------------------------*/
/**
*
*/
/*----------------------------------*/
int ResponseData::IsAutoCmd()
{
	if(Base::IsEqual(m_mode,"auto") && !Base::IsEqual(m_cmd,"")){
		return TRUE; //this is a cmd!
	}else{
		return FALSE;
	}
}
/*----------------------------------*/
/**
*
*/
/*----------------------------------*/
int ResponseData::IsManualCmd()
{
	if(Base::IsEqual(m_mode,"manual") && !Base::IsEqual(m_cmd,"")){
		return TRUE; //this is a cmd!
	}else{
		return FALSE;
	}
}
/*----------------------------------*/
/**
*
*/
/*----------------------------------*/