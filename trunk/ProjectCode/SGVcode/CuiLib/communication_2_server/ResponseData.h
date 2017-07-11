#pragma once

#include <jsoncpp-master/include/json/json.h>

#include "pthread.h"

#include "cpp_stl.h"

class ResponseData
{
public:
	ResponseData(void);
	ResponseData(string data_t);
	~ResponseData(void);
private:
	int		m_random_count;
	int		m_resp_result;
	string	m_resp_type;
private:
	string m_result;
	string m_random;
public:
	int parse(string data_t);
	int parseResp(Json::Value reader);
	int parseCmd(Json::Value value_t);
	
	int IsHeartbeat();
	int IsCmd();
	int IsAutoCmd();
	int IsManualCmd();
private:
	void Clear();
public:
	string m_car_id;
	string m_mode;
	string m_cmd;
	string m_longitude;
	string m_latitude;
	string m_speed;
public:
	string m_auto_task;
public:
	string m_manual_turn;
	string m_manual_run_direction;
};

