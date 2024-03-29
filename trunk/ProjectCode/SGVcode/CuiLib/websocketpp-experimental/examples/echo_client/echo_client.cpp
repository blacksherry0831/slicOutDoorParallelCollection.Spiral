#include <websocketpp/config/asio_no_tls_client.hpp>

#include <websocketpp/client.hpp>

#include <iostream>

typedef websocketpp::client<websocketpp::config::asio_client> client;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

// pull out the type of messages sent by our config
typedef websocketpp::config::asio_client::message_type::ptr message_ptr;

int case_count = 0;

void on_message(client* c, websocketpp::connection_hdl hdl, message_ptr msg) {
    client::connection_ptr con = c->get_con_from_hdl(hdl);
    
    if (con->get_resource() == "/getCaseCount") {
        std::cout << "Detected " << msg->get_payload() << " test cases." << std::endl;
        case_count = atoi(msg->get_payload().c_str());
    } else {
        c->send(hdl, msg->get_payload(), msg->get_opcode());
    }
	std::cout<<msg->get_payload();
}


void open_h_t(client* c, websocketpp::connection_hdl hdl) {
	// now it is safe to use the connection
	
	c->send(hdl,"xxxx", websocketpp::frame::opcode::text);
}
int main(int argc, char* argv[]) {
	// Create a server endpoint
    client echo_client;
	
	std::string uri = "ws://blacksherry.ticp.net:7681";
	
	if (argc == 2) {
	    uri = argv[1];
	}
	
	try {
        // We expect there to be a lot of errors, so suppress them
        echo_client.clear_access_channels(websocketpp::log::alevel::all);
        echo_client.clear_error_channels(websocketpp::log::elevel::all);
        
        // Initialize ASIO
        echo_client.init_asio();
        
        // Register our handlers
        echo_client.set_message_handler(bind(&on_message,&echo_client,::_1,::_2));
        
        websocketpp::lib::error_code ec;
		websocketpp::frame::opcode::value v_t=	websocketpp::frame::opcode::text;
		std::string msg="xxxx";
		 //websocketpp::lib::error_code ec;

        

		
		//echo_client.set_open_handler(&);
		 echo_client.set_open_handler(bind(&open_h_t,&echo_client,::_1));
	    //echo_client.send(con,msg,v_t);
	    // Start the ASIO io_service run loop
                client::connection_ptr con = echo_client.get_connection(uri, ec);
		echo_client.connect(con);
		echo_client.run();
        
        std::cout << "case count: " << case_count << std::endl;
        
        for (int i = 1; i <= case_count; i++) {
            echo_client.reset();
            
            std::stringstream url;
            
            url << uri << "/runCase?case=" << i << "&agent=WebSocket++/0.3.0-dev";
            
            con = echo_client.get_connection(url.str(), ec);
                        
            echo_client.connect(con);
            
            echo_client.run();
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
