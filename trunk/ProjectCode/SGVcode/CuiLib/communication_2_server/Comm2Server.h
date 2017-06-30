#pragma once

#include "module_websocket.h"
#include "CommData.h"
typedef websocketpp::client<websocketpp::config::asio_client> client;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;

// pull out the type of messages sent by our config
typedef websocketpp::config::asio_client::message_type::ptr message_ptr;

using websocketpp::lib::bind;

//client sip_client;
//
//bool received; 

#include "libcurl/CHttpClient.h"
#include "pthread.h"
using namespace std;

#include "stomp/StompFrame.h"

class Comm2Server
{
private:
	client m_sip_client;
	websocketpp::connection_hdl m_hdl_open;
	bool   m_sip_client_open;
private:
	bool   m_thread_run;
	pthread_t m_pthread_t_id;
public:
	Comm2Server(void);
	~Comm2Server(void);
private:
	CommData m_CommData;
public:
	void InputCarUserId();
	int RegisterCar(void);

	int GetTaskLongitudeLatitude();

public:
	static void* StartWebSocketConnection(void *pdata);
	
	int StartWebSocketThread();
	int StopWebSocketThread();
	void Join();

	void InitWebSocket();
	
	void on_open(client* c, websocketpp::connection_hdl hdl);
	void on_close(client* c, websocketpp::connection_hdl hdl);
	void on_message(client* c, websocketpp::connection_hdl hdl, message_ptr msg);
	void on_fail(client* c, websocketpp::connection_hdl hdl);
	void on_interrupt(client* c, websocketpp::connection_hdl hdl);

	void sleep(int ms);

	void SendHeartBeat(client* c, client::connection_ptr con);
	void SendStompConnect(client* c, client::connection_ptr con);
};

