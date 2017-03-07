#pragma once

#include "opencv_stl.h"

class Base
{
public:
	Base(void);
	~Base(void);
public:

	//static string GetNameExt4path(void);

	static std::string base_name(string const & path);	

	static std::string comine_str(string f_str,string s_str);
	
};

