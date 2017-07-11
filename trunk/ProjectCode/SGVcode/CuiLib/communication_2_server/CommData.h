#pragma once

#include <jsoncpp-master/include/json/json.h>
#include "pthread.h"

using namespace std;
#include <type.h>

#include <SerialPort/Gps_WG_8020.h>
#include <SerialPort/Compass_HCM365.h>
#include <SerialPort/AGV_Dirver.h>

#include <stomp/StompFrame.h>



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
	/*string m_car_status;*/
	string m_error;
	int    m_random_count;
	string m_phone_number;
public:
	void  InputCarUserId();


public:
	GPS_WG_8020* m_gps;
	Compass_HCM365 *m_compass;
	AGV_Dirver* m_agv_drive;
public:
	string GetHeartBeat();
	StompFrame GetSubscriptionFrame();
	StompFrame GetHeartBeatFrame();
	StompFrame GetSubscriptionFrameP2p();
public:
	string GetUrlRegisterCar();
	string GetUrlTaskLongitudeLatitude();
	string GetWsUrl();
	string GetWsStompSendUrl();
	string GetWsStompSubscriptionUrl();
	string GetWsStompSubscriptionP2pUrl();
public:
	
	
};

