#pragma once
/*-------------------------------------*/
#include "cpp_stl.h"
/*-------------------------------------*/

/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
class IPrint
{
public:
	IPrint();
	~IPrint();
public:
	static void printf_event(std::string _event, std::string _msg="");
	
};