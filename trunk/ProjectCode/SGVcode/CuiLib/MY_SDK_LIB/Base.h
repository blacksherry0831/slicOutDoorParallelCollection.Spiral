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

	static std::string file_name_without_ext(string const  path);

	static std::string comine_str(string f_str,string s_str);

	static std::vector<std::string> split  (const std::string &s, char delim);

	static std::vector<std::string> split  (const std::string &s, string delim);

	static void SaveString2Disk(
		string ffile_name_t,
		string str_t);
	
	static int IsEqual(std::string str_0,std::string str_1);
	
public:
	static void sleep(int ms);
	static int is_file_exist(const char * file_path);
	static int get_number(string str_t);
public:
	static double Math_GetAverageValue(double* Data, int DataNum);
	static double Math_GetVarianceValue(
		double* Data,
		int DataNum,
		double avg,
		double *variance);

};

