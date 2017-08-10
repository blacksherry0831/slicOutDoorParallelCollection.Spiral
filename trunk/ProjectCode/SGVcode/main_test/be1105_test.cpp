#include "stdafx.h"

#include "cpp_stl.h"

#include "SerialPort/BE_1105_Dirver.hpp"

/*-------------------------------------------------------------------------*/
/**
*
*/
/*-------------------------------------------------------------------------*/

int main()
{

	BE_1105_Driver *be_1105 = BE_1105_Driver::getInstance();

	

	

	while (be_1105->IsThreadRun()) {
		
		string cmd;
		std::getline(std::cin, cmd);

		if (cmd.compare("run neg") == 0) {
			be_1105->SendCmd(BE_1105_RUN_NEG);
		}else if (cmd.compare("open") == 0) {
			be_1105->open(4);
		}else if (cmd.compare("status") == 0) {
			
		}else if (cmd.compare("start record") == 0) {
			
		}else if (cmd.compare("stop record") == 0) {
			
		}else if (cmd.compare("set manual") == 0) {
		
		}else if (cmd.compare("reboot") == 0) {
			
		}else if (cmd.compare("hard disk") == 0) {
			
		}else if (cmd.compare("format disk") == 0) {
			
		}else if (cmd.compare("q") == 0) {
				break;
		}else {
			std::cout << "you can use this cmd:" << std::endl;
			std::cout << "open" << std::endl;			
			std::cout << "run neg" << std::endl;			
			std::cout << "q" << std::endl;
			std::cout << "please input a cmd :" << std::endl;

		}


	}


	return 0;
}
