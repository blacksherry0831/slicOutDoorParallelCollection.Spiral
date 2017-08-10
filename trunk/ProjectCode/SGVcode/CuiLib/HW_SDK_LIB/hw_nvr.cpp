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
	std::cout << "Ӳ����:" << m_dev_config.disk_count<< std::endl;
	std::cout << "ͨ����:" << m_dev_config.dsp_count<< std::endl;
#endif // 1

#if _DEBUG
	/*----------------------------*/
	std::cout << "ͨ��:";
	for (size_t i = 0; i <m_dev_config.dsp_count; i++){
		std::cout << " " << i << " ";
	}
	std::cout << std::endl;
	/*----------------------------*/
	std::cout << "����:";
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
	std::cout << "¼��:";
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

	 const boolean disk_status= (m_dev_config.disk_count > 0) && this->GetHarddiskState();

	 const boolean STATUS = (m_uh != -1) && disk_status;

	return STATUS;
}
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/
boolean hw_nvr::Check7Status4Start()
{

	if (this->Check7Link4Start()) {
		//ȫ������
		if (this->Wait4StopAllRecord()) {
			//ȫ���ر�
			return true;
		}
	}

	return false;//��ǵ�7������ͷ��ȫ�����ڹر�״̬
}
boolean hw_nvr::Check7Link4Start()
{
	const int NVR_COUNT_7 = 7;

	if (this->status()) {
		/*----------------------------*/
		for (size_t i = 0; i < NVR_COUNT_7; i++) {
			if (m_channel_status.status[i] == 1) {//����״̬
				
			}
			else {
				return false;
			}
		}
		/*----------------------------*/
	}
	else {
		return false;
	}

	return true;//��ǵ�7������ͷ��ȫ�����ڹر�״̬
}
boolean hw_nvr::Wait4StartAllRecord()
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
boolean hw_nvr::Wait4StopAllRecord()
{
	boolean IsStop =false;
	
	
	do {
		this->StopAllRecord();
		IsStop = this->IsAllRecordStop();
	}while (IsStop==false);

	return IsStop;
}
/*----------------------------------------------------------------*/
/**
*
*
*
*/
/*----------------------------------------------------------------*/
boolean hw_nvr::IsAllRecordStop()
{
	this->status();
	/*----------------------------*/
	for (size_t i = 0; i < m_dev_config.dsp_count; i++) {
		if (m_channel_status.status[i] == 1) {//����״̬
			if (m_alarm_state.rec_state[i] == 1) {//¼��״̬
				return false;//��������״̬������ͷ����һ̨����¼��
			}			
		}		
	}
	/*----------------------------*/
	return true;
}
boolean hw_nvr::IsAllRecordStart()
{
	this->status();
	/*----------------------------*/
	for (size_t i = 0; i < m_dev_config.dsp_count; i++) {
		if (m_channel_status.status[i] == 1) {//����״̬
			if (m_alarm_state.rec_state[i] == 0) {//¼��״̬
				return false;//��������״̬������ͷ����һ̨��δ¼��
			}
		}
	}
	/*----------------------------*/
	return true;
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
boolean hw_nvr::SetManual()
{

#if 0
	if (HW_NET_SET_GetBlackWhite(m_uh, &m_net_blackwhite)) {

		if (m_net_blackwhite.control_mode == 1) {
			std::cout << "control mode is auto" << std::endl;
			m_net_blackwhite.control_mode == 0;
		}else {
			std::cout << "control mode is manual" << std::endl;
		}

		if(HW_NET_SET_SetBlackWhite(m_uh, &m_net_blackwhite)){
			return true;
		}else {
			std::cout << "set black white error" << std::endl;
		}

	}else{
		std::cout << "get black white error"<<std::endl;
	}	
#endif // 0




	return false;
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
boolean hw_nvr::Reboot()
{
	if (HW_NET_Reboot(m_uh)) {

		std::cout << "Reboot!" << std::endl;
		this->close();
		return true;
	}
	return false;
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
boolean hw_nvr::ShutDown()
{
	if (HW_NET_ShutDown(m_uh)) {

		std::cout << "Shutdown!" << std::endl;
		return true;
	}
	return false;
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
boolean hw_nvr::FormatDisk()
{
	const int  DISK_COUNT = m_dev_config.disk_count;
	int progress_f = 0;

	for (size_t i = 0; i < DISK_COUNT; i++){

		if (HW_NET_SET_FormatHarddisk(m_uh, i)) {
		
				while (progress_f != 100) {
						HW_NET_SET_GetFormatProgress(m_uh,&progress_f);//0-100
						Sleep(100);
						std::cout << "progress present:" << progress_f<<"%"<<std::endl ;

				}

				std::cout <<"disk"<<'['<<i<<']'<<"format success !"<<std::endl;		
		
		}



	}

	return true;
	



}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/
boolean hw_nvr::GetHarddiskState()
{

	boolean flag = false;
	
	INT64 volume=0; //Ӳ������
	INT64 free=0;//��������
	int state=0;//Ӳ��״̬ 0- ���� 1-���� 2-������
	
	const int  DISK_COUNT = m_dev_config.disk_count;
	if (DISK_COUNT> 0) {
		
		std::cout << "Ӳ������" << DISK_COUNT <<std::endl;
		flag = HW_NET_SET_GetHarddiskState(m_uh, &m_hard_disk_info);
		if (flag) {
		
			for (size_t i = 0; i <DISK_COUNT; i++)
			{
				if (m_hard_disk_info.disk[i].state == 0) {
					volume += m_hard_disk_info.disk[i].volume;
					free += m_hard_disk_info.disk[i].free;
				}else if (m_hard_disk_info.disk[i].state == 1) {
					std::cout << "Ӳ��" << "[" << i << "]" << "����" << std::endl;
				}else if (m_hard_disk_info.disk[i].state == 2){
					std::cout << "Ӳ��" << "[" << i << "]" << "������" << std::endl;
				}else {
					ASSERT(0);
				}

			
			}			
		}
	
	}else {
		std::cout << "Ӳ�̣�" << "û��Ӳ��" << std::endl;
	}

	if (volume > 0 ) {		
		float present_free = 1.0*free /volume;
		std::cout << "Ӳ��ʣ�ࣺ" << present_free*100 <<"%"<< std::endl;
		
		if (present_free<0.1) {
			flag = false;//ʣ��ռ䲻��
		}else {
			flag = true;
		}
	}

	return flag;
}
/*----------------------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------------------*/

