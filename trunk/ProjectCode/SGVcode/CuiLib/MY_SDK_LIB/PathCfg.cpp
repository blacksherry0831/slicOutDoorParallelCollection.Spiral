#include "StdAfx.h"
#include "PathCfg.h"
#include "Base.h"
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
PathCfg::PathCfg(void)
{
	
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
PathCfg::~PathCfg(void)
{
	
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/

#if defined(linux) || defined(__linux) || defined(__linux__) ||defined( __GNUC__)
const std::string PathCfg::ImageDataBaseRootPath = "/media/X/";
#endif
#if defined(_WIN32) || defined(_WIN64) || defined( _MSC_VER)
const std::string PathCfg::ImageDataBaseRootPath = "X:\\ImageDataBase\\";
#endif

/*-----------------------------------------*/
std::string PathCfg::ImageDataBaseRoot_CrackPath = Base::FS_createPath(ImageDataBaseRootPath, "Crack");
/*-----------------------------------------*/
const std::string  PathCfg::PrefixChannel="ch";
/*-----------------------------------------*/
const std::string  PathCfg::PrefixTime="time";
/*-----------------------------------------*/
const std::string  PathCfg::PrefixFrame="frame";
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/