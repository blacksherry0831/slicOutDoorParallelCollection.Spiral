#include "StdAfx.h"
#include "Base.h"
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
Base::Base(void)
{

}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
Base::~Base(void)
{

}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
int Base::str2int(int & int_temp, const string & string_temp)
{
	
		stringstream stream(string_temp);
		stream >> int_temp;
		return int_temp;

}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
string Base::int2str(int value_t)
{
	stringstream stream;
	stream << value_t;

	return stream.str();
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
string Base::base_name(string const & path)
{
	return path.substr(path.find_last_of("/\\") + 1);
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
std::string Base::file_name_without_ext(string const path)
{
	std::string base_name = Base::base_name(path);
	
	std::vector<std::string> name_ext=split(base_name, ".");

	string name;
	string ext;
	
	if (name_ext.size()==2) {
		name=name_ext.at(0);
		ext=name_ext.at(1);	
	}
	return name;
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
string Base::comine_str(string f_str,string s_str)
{
	return f_str.append(s_str);
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
std::vector<std::string> Base::split  (const std::string &s, char delim)
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
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
std::vector<std::string> Base::split  (const std::string &s, string delim)
{
	std::vector<std::string> v;
	std::string::size_type pos1, pos2;
	pos2 = s.find(delim);
	pos1 = 0;
	while(std::string::npos != pos2)
	{
		v.push_back(s.substr(pos1, pos2-pos1));

		pos1 = pos2 + delim.size();
		pos2 = s.find(delim, pos1);
	}
	if(pos1 != s.length())
		v.push_back(s.substr(pos1));

	return v;
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
void Base::SaveString2Disk(string file_name_t,string str_t)
{
#if _DEBUG	

	ofstream ofs;
	ofs.open(file_name_t);

	assert(ofs.is_open());
	ofs<<str_t;

	ofs.close();

#endif
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
int Base::IsEqual(std::string str_0,std::string str_1)
{
	int result_t=str_0.compare(str_1);

	if (result_t==0){
		return TRUE;
	}else{
		return FALSE;
	}
}
/*----------------------------------*/
/**
*
*/
/*----------------------------------*/
void Base::sleep(int ms)
{

#if __GNUC__
	usleep(ms * 1000);
#endif

#if _MSC_VER
	Sleep(ms);
#endif

}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
int  Base::is_file_exist(const char * file_path)
{
	if (file_path == NULL)
		return -1;
	if (access(file_path, 0) == 0)
		return 0;
	return -1;
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
int Base::get_number(string str_t)
{

	string str_new_t;

	for (size_t i = 0; i <str_t.size(); i++)
	{
			char char_t=str_t.at(i);

			if (isdigit(char_t)) {
				str_new_t += char_t;
			}
	}
		
	return atoi(str_new_t.c_str());
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
double Base::Math_GetAverageValue(double* Data, int DataNum)
{
	double sum = 0;
	ASSERT(DataNum>0);
	for (int i = 0; i<DataNum; i++) {
		sum += Data[i];
	}
	sum /= DataNum;
	return sum;
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
double Base::Math_GetVarianceValue(
	double* Data,
	int DataNum,
	double avg,
	double *variance) {

	*variance = 0;
	for (int i = 0; i<DataNum; i++) {
		*variance += (avg - Data[i])*(avg - Data[i]);
	}
	*variance /= DataNum;
	*variance = sqrtl(*variance);
	return *variance;
}