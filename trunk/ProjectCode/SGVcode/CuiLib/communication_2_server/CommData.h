#pragma once

#include <jsoncpp-master/include/json/json.h>
#include "pthread.h"

using namespace std;
#include <type.h>

#include <SerialPort/Gps_WG_8020.h>
#include <SerialPort/Compass_HCM365.h>

#include <stomp/StompFrame.h>

#define  CAR_STATUS_RUN  "run"
#define  CAR_STATUS_STOP "stop"

class CommData
{
public:
	CommData(void);
	~CommData(void);
public:
	string m_url;
	string m_websockets_url;
	int    m_port;
	string m_current_task;
	string m_user_id;
	string m_car_id;
	string m_car_status;
	string m_error;
	int    m_random_count;
public:
	void  InputCarUserId();

	static void SaveString2Disk(
		string ffile_name_t,
		string str_t);
public:
	GPS_WG_8020* m_gps;
	Compass_HCM365 *m_compass;


	string GetHeartBeat();

	StompFrame GetHeartBeatFrame();

public:
	string GetUrlRegisterCar();
	string GetUrlTaskLongitudeLatitude();
	string GetWsUrl();
public:

	
};

