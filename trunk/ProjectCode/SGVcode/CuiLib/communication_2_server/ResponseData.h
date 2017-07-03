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
public:
	int		m_random_count;
	int		m_resp_result;
	string	m_resp_type;
private:
	string m_result;
	string m_random;
public:
	int parse(string data_t);
	int IsHeartbeat();
	
public:
		
};

