#include "Comm2Server.h"
#include<iostream>
#include<sstream>
#include<string>
using namespace std; 
typedef websocketpp::client<websocketpp::config::asio_client> client;
/*----------------------------------*/
/**
*
*/
/*----------------------------------*/
Comm2Server::Comm2Server(void)
{
	this->m_user_id="123456789";
	this->m_car_id="123456";
	this->m_current_task="task123456";
	this->m_url="http://58.240.85.222";
	this->m_websockets_url="ws://58.240.85.222";
	this->m_port=10000;
	this->m_thread_run=true;

}
/*----------------------------------*/
/**
*
*/
/*----------------------------------*/
Comm2Server::~Comm2Server(void)
{

}
/*----------------------------------*/
/**
*
*/
/*----------------------------------*/
int Comm2Server::RegisterCar(void)
{
	
	
	string url_path_t="/car/car/addCar";

	string param0="car_id="+m_car_id;
	string param1="userid="+m_user_id;

	stringstream ss; //定义一个string流（使用s实例化） 

	ss<<m_url<<":"<<m_port<<url_path_t<<"?"<<param0<<"&"<<param1;

	string url_p=ss.str();
	
	string response_p;




	CHttpClient httpc_t;
	
	int code_r=httpc_t.Get(url_p,response_p);

#if _DEBUG
	cout<<response_p<<endl;	
#endif

	return code_r;
}
/*----------------------------------*/
/**
*
*/
/*----------------------------------*/
void  Comm2Server::InputCarUserId()
{
	char buffer_t[1024];
	
	cout<<"Please Input Car Id:"<<endl;	
	fflush(stdin);
	cin.getline(buffer_t,1024);
	this->m_car_id=buffer_t;

	cout<<"Please Input User Id:"<<endl;
	fflush(stdin);
	cin.getline(buffer_t,1024);
	this->m_user_id=buffer_t;
	/*----*/

	if (this->m_user_id.compare("")==0){
		this->m_user_id="123456789";
	}

	if (this->m_car_id.compare("")==0){
		this->m_car_id="123456";
	}

#if _DEBUG
	cout<<"User Name Is :"<<this->m_user_id<<endl;
	cout<<"Car Id Is :"<<this->m_car_id<<endl;
#endif

}
/*----------------------------------*/
/**
*
*/
/*----------------------------------*/
int  Comm2Server::GetTaskLongitudeLatitude()
{
	
	const string url_path_t="/car/car/one";

	/*string param0="car_id="+m_car_id;
	string param1="userid="+m_user_id;*/

	stringstream ss; //定义一个string流（使用s实例化） 

	ss<<m_url<<":"<<m_port<<url_path_t<<"?"<<"task="<<m_current_task;

	string url_p=ss.str();

	string response_p;

	CHttpClient httpc_t;

	int code_r=httpc_t.Get(url_p,response_p);

#if _DEBUG
	cout<<response_p<<endl;	
#endif

	return code_r;

}
/*----------------------------------*/
/**
*
*/
/*----------------------------------*/
void*  Comm2Server::StartWebSocketConnection(void* pdata)
{
	Comm2Server* c2s=(Comm2Server*) pdata;

	stringstream ss; //定义一个string流（使用s实例化） 
	const string ws_path_t="/car/websocket";

	ss<<c2s->m_websockets_url<<":"<<c2s->m_port<<ws_path_t<<"?"<<"car="<<c2s->m_car_id;

	const string ws_path_full=ss.str();


	c2s->InitWebSocket(ws_path_full);

	return (void *)0;
}
/*----------------------------------*/
/**
*
*/
/*----------------------------------*/
int Comm2Server::StartWebSocketThread()
{	
	int ret;  
	/*创建线程一*/  
	ret=pthread_create(&m_pthread_t_id,NULL,StartWebSocketConnection,this);  
	if(ret!=0)  
	{  
		printf("Create pthread error!\n");  
		assert(ret!=0);
		return -1;  
	}
	return ret;
}
/*----------------------------------*/
/**
*
*/
/*----------------------------------*/
int Comm2Server::StopWebSocketThread()
{
	
	this->m_thread_run=FALSE;
		
	return 0;
}
/*----------------------------------*/
/**
*
*/
/*----------------------------------*/
void Comm2Server::Join()
{
	int ret=pthread_join(m_pthread_t_id,NULL);
}
/*----------------------------------*/
/**
*
*/
/*----------------------------------*/
void Comm2Server::InitWebSocket(string uri)
{
	try {
		// We expect there to be a lot of errors, so suppress them
		m_sip_client.clear_access_channels(websocketpp::log::alevel::all);
		m_sip_client.clear_error_channels(websocketpp::log::elevel::all);

		// Initialize ASIO
		m_sip_client.init_asio();

		// Register our handlers
		m_sip_client.set_open_handler(websocketpp::lib::bind(&Comm2Server::on_open,this,&m_sip_client,::_1));
		m_sip_client.set_message_handler(websocketpp::lib::bind(&Comm2Server::on_message,this,&m_sip_client,::_1,::_2));

		websocketpp::lib::error_code ec;
		client::connection_ptr con = m_sip_client.get_connection(uri, ec);

		// Specify the SIP subprotocol:
		con->add_subprotocol("sip");

		m_sip_client.connect(con);

		// Start the ASIO io_service run loop
		m_sip_client.run();

		while(m_thread_run) {
			boost::this_thread::sleep(boost::posix_time::milliseconds(100));
			std::cout<<"#";
		}

		std::cout << "done" << std::endl;

	} catch (const std::exception & e) {
		std::cout << e.what() << std::endl;
	} catch (websocketpp::lib::error_code e) {
		std::cout << e.message() << std::endl;
	} catch (...) {
		std::cout << "other exception" << std::endl;
	}

}


void Comm2Server::on_open(client* c, websocketpp::connection_hdl hdl)
{
	// now it is safe to use the connection
	std::cout << "connection ready" << std::endl;

	// Send a SIP OPTIONS message to the server:
	std::string SIP_msg="OPTIONS sip:carol@chicago.com SIP/2.0\r\nVia: SIP/2.0/WS df7jal23ls0d.invalid;rport;branch=z9hG4bKhjhs8ass877\r\nMax-Forwards: 70\r\nTo: <sip:carol@chicago.com>\r\nFrom: Alice <sip:alice@atlanta.com>;tag=1928301774\r\nCall-ID: a84b4c76e66710\r\nCSeq: 63104 OPTIONS\r\nContact: <sip:alice@pc33.atlanta.com>\r\nAccept: application/sdp\r\nContent-Length: 0\r\n\r\n";
	m_sip_client.send(hdl, SIP_msg.c_str(), websocketpp::frame::opcode::text);
}

void Comm2Server::on_message(client* c, websocketpp::connection_hdl hdl, message_ptr msg)
{
	client::connection_ptr con = m_sip_client.get_con_from_hdl(hdl);

	std::cout << "Received a reply:" << std::endl;
	fwrite(msg->get_payload().c_str(), msg->get_payload().size(), 1, stdout);
	
	m_sip_client.send(hdl,"xxxxxxxxxxxxxx", websocketpp::frame::opcode::text);
}

