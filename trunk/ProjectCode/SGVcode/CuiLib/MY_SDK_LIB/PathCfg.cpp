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
const std::string PathCfg::ImageDataBaseRootPath = "X:\\ImageDataBase\\";
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