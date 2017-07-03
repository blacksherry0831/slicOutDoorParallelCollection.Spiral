#pragma once

#include "cpp_stl.h"

class Base
{
public:
	Base(void);
	~Base(void);
public:

	//static string GetNameExt4path(void);

	static std::string base_name(string const & path);	

	static std::string comine_str(string f_str,string s_str);

	static std::vector<std::string> split  (const std::string &s, char delim);

	static std::vector<std::string> split  (const std::string &s, string delim);
	
};

