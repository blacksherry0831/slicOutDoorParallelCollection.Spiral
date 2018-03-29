#pragma once

#ifndef hw_nvr_include_h
#define hw_nvr_include_h

class hw_dev 
{
public:
	hw_dev(const char* ip, int slot);
	~hw_dev();


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

typedef hw_dev* hw_dev_ptr;

//typedef boost::shared_ptr<live_video> live_video_ptr;

#endif