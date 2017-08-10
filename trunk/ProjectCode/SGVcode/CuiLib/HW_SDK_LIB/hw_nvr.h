#pragma once

#ifndef hw_nvr_include_h
#define hw_nvr_include_h

#include "live_video_base.h"



class hw_nvr :public live_video_base
{
public:
	hw_nvr(const char* ip,int slot);
	~hw_nvr();
public:	
	net_record_ctrl_t m_net_record;
	tDevConfig m_dev_config;
	tChannelStatus m_channel_status;
	tAlarmState m_alarm_state;
	tHarddiskInfo m_hard_disk_info;
private:
	net_blackwhite_t    m_net_blackwhite;

public:
	boolean GetDevConfig();
	boolean GetChannelStatus();
	boolean GetNetRecord();
	boolean status();

	boolean Check7Status4Start();
	boolean Check7Link4Start();

	boolean Wait4StartAllRecord();	
	boolean Wait4StopAllRecord();
	
	boolean IsAllRecordStop();
	boolean IsAllRecordStart();

	boolean hw_login();
	boolean StopAllRecord();
	boolean StartAllRecoed();
	boolean SetManual();
	boolean Reboot();
	boolean ShutDown();
	boolean FormatDisk();
	boolean GetHarddiskState();
};

typedef hw_nvr* hw_nvr_ptr;

//typedef boost::shared_ptr<live_video> live_video_ptr;

#endif