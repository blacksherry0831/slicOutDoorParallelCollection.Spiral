#include "stdafx.h"
#include "hw_dev.hpp"

#include "modules.h"
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/
hw_dev::hw_dev(const char* ip, int slot) 
{



}
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/
hw_dev::~hw_dev()
{


}
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/
boolean hw_dev::GetDevConfig()
{

	return false;
}
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/
boolean hw_dev::GetChannelStatus()
{
	
	return false;

}
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/
boolean hw_dev::GetNetRecord()
{

	
	return false;

}
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/
boolean hw_dev::status()
{

	

	const boolean STATUS = 0;

	return STATUS;
}
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/
boolean hw_dev::Check7Status4Start()
{

	

	return true;//标记的7个摄像头，全部处于关闭状态
}
boolean hw_dev::Wait4StartAllRecord()
{
	boolean FLAG = false;


	do {
		this->StartAllRecoed();
		FLAG = this->IsAllRecordStart();
	} while (FLAG == false);

	return FLAG;
}
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/
boolean hw_dev::Wait4StopAllRecord()
{
	boolean IsStop = false;


	

	return IsStop;
}
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/
boolean hw_dev::IsAllRecordStop()
{
	
	return true;
}
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/
boolean hw_dev::hw_login()
{
	

	return false;
}
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/
boolean hw_dev::StopAllRecord()
{


	return false;
}
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/
boolean hw_dev::StartAllRecoed()
{
	
	return false;
}
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/
boolean hw_dev::SetManual()
{
	return false;
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
boolean hw_dev::Reboot()
{
	return false;
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
boolean hw_dev::ShutDown()
{
	return false;
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
boolean hw_dev::FormatDisk()
{
	return false;
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
boolean hw_dev::GetHarddiskState()
{

	boolean flag = false;
	return false;
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/

