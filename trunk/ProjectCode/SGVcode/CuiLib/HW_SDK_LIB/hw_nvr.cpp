#include "stdafx.h"
#include "hw_nvr.h"




#include "modules.h"
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/
hw_nvr::hw_nvr(const char* ip,int slot):live_video_base(ip,slot)
{
	


}
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/
hw_nvr::~hw_nvr()
{


}
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/
boolean hw_nvr::GetDevConfig()
{

	ASSERT(m_uh != -1);

	if (m_uh != -1) {
	
		return	HW_NET_SET_GetDevConfig(m_uh,&m_dev_config);
	
	}

	return false;
}
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/
boolean hw_nvr::GetChannelStatus()
{
	ASSERT(m_uh != -1);

	if (m_uh != -1) {

			return HW_NET_SET_GetChannelStatus(m_uh,&m_channel_status);

	}

	return false;

}
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/
boolean hw_nvr::GetNetRecord()
{

	ASSERT(m_uh != -1);

	if (m_uh != -1) {

		return HW_NET_SET_GetAlarmState(m_uh,&m_alarm_state);

	}

	return false;

}
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/
boolean hw_nvr::status()
{

	GetChannelStatus();
	GetNetRecord();

#if _DEBUG
	std::cout << "devName:" << m_dev_config.devName<< std::endl;
	std::cout << "硬盘数:" << m_dev_config.disk_count<< std::endl;
	std::cout << "通道数:" << m_dev_config.dsp_count<< std::endl;
#endif // 1

#if _DEBUG
	/*----------------------------*/
	std::cout << "通道:";
	for (size_t i = 0; i <m_dev_config.dsp_count; i++){
		std::cout << " " << i << " ";
	}
	std::cout << std::endl;
	/*----------------------------*/
	std::cout << "连接:";
	for (size_t i = 0; i <m_dev_config.dsp_count; i++){
		if (m_channel_status.status[i] == 1) {
			std::cout << " " << i << " ";		
		}else {
			if(i<10)	std::cout << " " << "X" << " ";
			if(i>=10&&i<100) std::cout << " " << "XX" << " ";
		}
	}
	std::cout << std::endl;
	/*----------------------------*/
	std::cout << "录像:";
	for (size_t i = 0; i <m_dev_config.dsp_count; i++) {
		if (m_alarm_state.rec_state[i] == 1) {
			std::cout << " " << i << " ";
		}
		else {
			if (i<10)	std::cout << " " << "X" << " ";
			if (i >= 10 && i<100) std::cout << " " << "XX" << " ";
		}
	}
	std::cout << std::endl;
	/*----------------------------*/
#endif // 1



	const boolean STATUS = (m_uh != -1) && (m_dev_config.disk_count >= 0);

	return STATUS;
}
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/
boolean hw_nvr::hw_login()
{
	if (live_video_base::hw_login()) {
		return this->GetDevConfig();
	}

	return false;
}
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/
boolean hw_nvr::StopAllRecord()
{
	for (size_t i = 0; i <m_dev_config.dsp_count; i++) {
		
		HW_NET_SET_StopRecord(m_uh, i);
	}

	return boolean();
}
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/
boolean hw_nvr::StartAllRecoed()
{
	for (size_t i = 0; i <m_dev_config.dsp_count; i++) {
		
		HW_NET_SET_StartRecord(m_uh,i);
	}
	return boolean();
}
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/

