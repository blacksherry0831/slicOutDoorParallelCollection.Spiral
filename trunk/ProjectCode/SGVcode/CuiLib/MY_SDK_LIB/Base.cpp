#include "StdAfx.h"
#include "Base.h"
//#include "opencv_stl.h"

Base::Base(void)
{
}


Base::~Base(void)
{
}


//string Base::GetNameExt4path(void)
//{
//	return string();
//}

string Base::base_name(string const & path)
{
	return path.substr(path.find_last_of("/\\") + 1);
}

string Base::comine_str(string f_str,string s_str)
{
	return f_str.append(s_str);
}
