#pragma once

#include <jsoncpp-master/include/json/json.h>
#include "pthread.h"
using namespace std;


#include <type.h>


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
	static	std::vector<std::string> split  (const std::string &s, char delim);
public:
		
};

