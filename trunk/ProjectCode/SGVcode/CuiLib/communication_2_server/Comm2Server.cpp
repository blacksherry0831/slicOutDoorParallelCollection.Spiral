#include "Comm2Server.h"
#include<iostream>
#include<sstream>
#include<string>
#include "ResponseData.h"
using namespace std; 
typedef websocketpp::client<websocketpp::config::asio_client> client;
/*----------------------------------*/
/**
*
*/
/*----------------------------------*/
Comm2Server::Comm2Server(void)
{

	this->m_thread_run=true;
	this->m_sip_client_connected=false;
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
	string url_p=m_CommData.GetUrlRegisterCar();
	
	string response_p;
	
	CHttpClient httpc_t;
	
	int code_r=httpc_t.Get(url_p,response_p);

#if _DEBUG
	cout<<"Register Car :"<<response_p<<endl;	
#endif

	if (code_r==0){
		if (response_p.compare("ALREADY")==0){
			return  code_r;
		}else if (response_p.compare("OK")){
			return code_r;
		}else if (response_p.compare("NO")){
			return -1;
		}else{
			return -1;
		}
	}

	return code_r;
}
/*----------------------------------*/
/**
*
*/
/*----------------------------------*/
void  Comm2Server::InputCarUserId()
{
	m_CommData.InputCarUserId();
}
/*----------------------------------*/
/**
*
*/
/*----------------------------------*/
int  Comm2Server::GetTaskLongitudeLatitude()
{
	
	string url_p=m_CommData.GetUrlTaskLongitudeLatitude();

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
	
	c2s->InitWebSocket();

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
	this->m_sip_client_connected=false;
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
void Comm2Server::InitWebSocket()
{
	const string uri=this->m_CommData.GetWsUrl();

	try {
		// We expect there to be a lot of errors, so suppress them
		m_sip_client.clear_access_channels(websocketpp::log::alevel::all);
		m_sip_client.clear_error_channels(websocketpp::log::elevel::all);

		// Initialize ASIO
		m_sip_client.init_asio();

		// Register our handlers
		m_sip_client.set_open_handler(websocketpp::lib::bind(&Comm2Server::on_open,this,&m_sip_client,::_1));
		m_sip_client.set_close_handler(websocketpp::lib::bind(&Comm2Server::on_close,this,&m_sip_client,::_1));
		m_sip_client.set_fail_handler(websocketpp::lib::bind(&Comm2Server::on_fail,this,&m_sip_client,::_1));
		m_sip_client.set_interrupt_handler(websocketpp::lib::bind(&Comm2Server::on_interrupt,this,&m_sip_client,::_1));
		m_sip_client.set_message_handler(websocketpp::lib::bind(&Comm2Server::on_message,this,&m_sip_client,::_1,::_2));

		websocketpp::lib::error_code ec;
		client::connection_ptr con = m_sip_client.get_connection(uri, ec);

		// Specify the SIP subprotocol:
		con->add_subprotocol("sip");

		m_sip_client.connect(con);

		// Start the ASIO io_service run loop
		m_sip_client.run();

		while(m_thread_run) {
			sleep(500);		
			std::cout << "#";
		}

		std::cout << "websocket done !" << std::endl;

	} catch (const std::exception & e) {
		std::cout << e.what() << std::endl;
	} catch (websocketpp::lib::error_code e) {
		std::cout << e.message() << std::endl;
	} catch (...) {
		std::cout << "other exception" << std::endl;
	}

}
/*----------------------------------*/
/**
*
*/
/*----------------------------------*/

void Comm2Server::on_open(client* c, websocketpp::connection_hdl hdl)
{
	// now it is safe to use the connection
	client::connection_ptr con = m_sip_client.get_con_from_hdl(hdl);
	std::cout << "connection ready" << std::endl;
		
	this->SendStompConnect(c,con);
}
/*----------------------------------*/
/**
*
*/
/*----------------------------------*/
void Comm2Server::on_close(client* c, websocketpp::connection_hdl hdl)
{
	this->m_thread_run=false;

	std::cout << "[ws][c]websocket connection close" << std::endl;

}
/*----------------------------------*/
/**
*
*/
/*----------------------------------*/
void Comm2Server::on_fail(client* c, websocketpp::connection_hdl hdl)
{
	std::cout << "[ws][f]websocket connection fail" << std::endl;
}
/*----------------------------------*/
/**
*
*/
/*----------------------------------*/
void Comm2Server::on_interrupt(client* c, websocketpp::connection_hdl hdl)
{
	std::cout << "[ws][i]websocket connection interrupt" << std::endl;
}
/*----------------------------------*/
/**
*
*/
/*----------------------------------*/
void Comm2Server::on_message(client* c, websocketpp::connection_hdl hdl, message_ptr msg)
{
	client::connection_ptr con = m_sip_client.get_con_from_hdl(hdl);
	string payload_t=msg->get_payload();

#if _DEBUG
	std::cout << "Received a reply:" << std::endl;
	
	fwrite(msg->get_payload().c_str(), msg->get_payload().size(), 1, stdout);
#endif

	StompFrame  sf_t;

	sf_t.Parse(payload_t);

	if (sf_t.IsConnected()){
		//连接成功
		this->m_sip_client_connected=true;

	}else if (sf_t.IsERROR()){
		//

	}else{


	}



}
/*----------------------------------*/
/**
*
*/
/*----------------------------------*/
void Comm2Server::sleep(int ms)
{

#if __GNUC__
	 usleep(ms * 1000);
#endif

#if _MSC_VER
		Sleep(ms);
#endif

}
/*----------------------------------*/
/**
*
*/
/*----------------------------------*/
void Comm2Server::SendHeartBeat(client* c, client::connection_ptr con)
{
	try {
		
		websocketpp::connection_hdl hdl=con->get_handle();

		if (m_sip_client_connected){

			string SIP_msg=m_CommData.GetHeartBeat();

			std::cout<<"HeartBeat !"<<endl;	

			m_sip_client.send(hdl, SIP_msg.c_str(), websocketpp::frame::opcode::text);

			sleep(1000);

		}

	} catch (const std::exception & e) {
		std::cout << e.what() << std::endl;
	} catch (websocketpp::lib::error_code e) {
		std::cout << e.message() << std::endl;
	} catch (...) {
		std::cout << "other exception" << std::endl;
	}
}
/*----------------------------------*/
/**
*
*/
/*----------------------------------*/
void Comm2Server::SendStompConnect(client* c, client::connection_ptr con)
{
	try {

		websocketpp::connection_hdl hdl=con->get_handle();

		if (m_sip_client_connected){

			string SIP_msg=StompFrame::GetConnectCmd();

			std::cout<<"[ws][stomp][connect]"<<endl;	

			m_sip_client.send(hdl, SIP_msg.c_str(),SIP_msg.size(), websocketpp::frame::opcode::binary);

			sleep(1000);

		}

	} catch (const std::exception & e) {
		std::cout << e.what() << std::endl;
	} catch (websocketpp::lib::error_code e) {
		std::cout << e.message() << std::endl;
	} catch (...) {
		std::cout << "other exception" << std::endl;
	}
}
/*----------------------------------*/
/**
*
*/
/*----------------------------------*/