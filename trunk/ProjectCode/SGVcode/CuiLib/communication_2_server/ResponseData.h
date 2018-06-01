#pragma once

#include <jsoncpp-master/include/json/json.h>

#include "pthread.h"

#include "cpp_stl.h"

class ResponseData
{
public:
	ResponseData(void);
	ResponseData(std::string data_t);
	~ResponseData(void);
private:
	int		m_random_count;
	int		m_resp_result;
	std::string	m_resp_type;
private:
	std::string m_result;
	std::string m_random;
public:
	int parse(std::string data_t);
	int parseResp(Json::Value reader);
	int parseCmd(Json::Value value_t);
	
	int IsHeartbeat();
	int IsCmd();
	int IsAutoCmd();
	int IsManualCmd();
private:
	void Clear();
public:
	std::string m_car_id;
	std::string m_mode;
	std::string m_cmd;
	std::string m_longitude;
	std::string m_latitude;
	std::string m_speed;
public:
	std::string m_auto_task;
public:
	std::string m_manual_turn;
	std::string m_manual_run_direction;
};

