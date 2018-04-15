#include "stdafx.h"

#include "cpp_stl.h"

#include "SerialPort/BE_1105_Dirver.hpp"

/*-------------------------------------------------------------------------*/
/**
*
*/
/*-------------------------------------------------------------------------*/

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	BE_1105_Driver *be_1105 = BE_1105_Driver::getInstance();
	
	be_1105->PrintAllSerialPort();

	while (be_1105->IsThreadRun()) {
		
		string cmd;
		std::getline(std::cin, cmd);

		if (cmd.compare("run neg") == 0) {
			be_1105->SendCmd(BE_1105_RUN_NEG, 55000);

		}else if (cmd.compare("open") == 0) {

#if defined(linux) || defined(__linux) || defined(__linux__)
			be_1105->open_ttyUSB();			
#endif
#if  defined(_WIN32) || defined(_WIN64)
			be_1105->open(3);			
#endif

		}else if (cmd.compare("close") == 0) {
			be_1105->close();
		}else if (cmd.compare("readResp") == 0) {
				be_1105->ReadResp();		
		}else if (cmd.compare("X") == 0) {

		}else if (cmd.compare("X") == 0) {
		
		}else if (cmd.compare("X") == 0) {
			
		}else if (cmd.compare("X") == 0) {
			
		}else if (cmd.compare("X") == 0) {
			
		}else if (cmd.compare("q") == 0) {
				break;
		}else {
			std::cout << "please input a cmd :" << std::endl;
			std::cout << "open" << std::endl;			
			std::cout << "run neg" << std::endl;			
			std::cout << "readResp" << std::endl;
			std::cout << "q" << std::endl;
			std::cout << ":" << std::endl;

		}





	}


	return 0;
}
