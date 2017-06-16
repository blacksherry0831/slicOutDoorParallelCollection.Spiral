#pragma once

#include "module_websocket.h"

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

class Comm2Server
{
private:
	client m_sip_client;
	string m_url;
	string m_websockets_url;
	int    m_port;
	bool   m_thread_run; 
private:

  string m_current_task;
  string m_user_id;
  string m_car_id;
  pthread_t m_pthread_t_id;
public:
	Comm2Server(void);
	~Comm2Server(void);
public:
	void InputCarUserId();
	int RegisterCar(void);

	int GetTaskLongitudeLatitude();

public:
	static void* StartWebSocketConnection(void *pdata);
	
	int StartWebSocketThread();
	int StopWebSocketThread();
	void Join();

	void InitWebSocket(string uri);
	void on_open(client* c, websocketpp::connection_hdl hdl);
	void on_message(client* c, websocketpp::connection_hdl hdl, message_ptr msg);
};

