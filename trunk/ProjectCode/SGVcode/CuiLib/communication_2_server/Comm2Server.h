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

class Comm2Server
{

  
public:
	Comm2Server(void);
	~Comm2Server(void);
public:

	int RegisterCar(void);
	
	
};

