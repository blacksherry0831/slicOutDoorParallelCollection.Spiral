#include "ResponseData.h"
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
ResponseData::ResponseData(void)
{
	this->m_random_count=-1;
	this->m_resp_result=-1;
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
ResponseData::ResponseData(string data_t)
{
	this->parse(data_t);
}
/*----------------------------------*/
/**
*
*/
/*----------------------------------*/
int ResponseData::parse(string data_t)
{
	 Json::Reader reader;
	 Json::Value value;

	if (reader.parse(data_t, value))
	{
		this->m_result = value["result"].asString();
		this->m_random= value["random"].asString();

		if (this->m_result.compare("success")==0){
			m_resp_result=1;
		}else{
			m_resp_result=0;
		}

		std::vector<std::string> random_data_t=split(m_random,'@');
		this->m_random_count= atoi(random_data_t.at(1).c_str()) ;
		this->m_resp_type=random_data_t.at(0);
		/*std::cout << out << std::endl;*/
		/*const Json::Value arrayObj = value["array"];
		for (int i=0; i<arrayObj.size(); i++)
		{
		out = arrayObj[i]["key2"].asString();
		std::cout << out;
		if (i != arrayObj.size() ¨C 1 )
		std::cout << std::endl;
		}*/
	}else{
		this->m_random_count=-1;
		this->m_resp_result=-1;
		return -1;
	}
}
/*----------------------------------*/
/**
*
*/
/*----------------------------------*/
std::vector<std::string> ResponseData::split  (const std::string &s, char delim)
{

	std::stringstream ss(s);
	std::string item;
	std::vector<std::string> elems;

	while (std::getline(ss, item, delim)) {

		elems.push_back(item);
		// elems.push_back(std::move(item)); // if C++11 (based on comment from @mchiasson)
	}
	return elems;
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