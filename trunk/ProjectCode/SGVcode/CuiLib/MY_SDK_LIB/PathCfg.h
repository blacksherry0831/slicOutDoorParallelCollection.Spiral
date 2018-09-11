#pragma once

#include "opencv_stl.h"

class PathCfg
{
public:
	PathCfg(void);
	~PathCfg(void);
public:
	const static std::string ImageDataBaseRootPath;
	static std::string ImageDataBaseRoot_CrackPath;
	/*-----------------------------------------*/
	const static std::string	PrefixChannel;
	const static std::string	PrefixTime;
	const static std::string    PrefixFrame;
};

