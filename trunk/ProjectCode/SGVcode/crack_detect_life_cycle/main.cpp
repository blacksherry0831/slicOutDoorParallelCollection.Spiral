#include "qt_all.h"

#include "cpp_stl.h"
#include "opencv_stl.h"


#include "module_my.h"



#ifdef _MSC_VER
#ifndef NDEBUG
#pragma comment(linker, "/SUBSYSTEM:CONSOLE")
#endif
#endif



void lifecycle_init() 
{
	QSharedPointer<QtThreadClientLifeCycle> lc_p = QSharedPointer<QtThreadClientLifeCycle>(new QtThreadClientLifeCycle());
	
	lc_p->run_factorySetting();
	
}

void lifecycle_deadline(int argc, char *argv[])
{

	QApplication a(argc, argv);

	QSharedPointer<QtThreadClientLifeCycle> lc_p = QSharedPointer<QtThreadClientLifeCycle>(new QtThreadClientLifeCycle());

	lc_p->run_updateDeadline();

}


void print_tip() 
{
	std::cout << "please input a cmd :" << std::endl;
	std::cout << "deadline" << std::endl;
	std::cout << "q" << std::endl;
	std::cout << "......" << std::endl;
}

void main_cmd(int argc, char *argv[])
{
	string cmd;

	while (TRUE) {

		std::getline(std::cin, cmd);

		if (cmd.compare("init") == 0) {
			//lifecycle_init();
		}
		else if (cmd.compare("deadline") == 0) {
			lifecycle_deadline(argc, argv);
		}
		else if (cmd.compare("XXX") == 0) {

		}
		else if (cmd.compare("XXX") == 0) {

		}
		else if (cmd.compare("XXX") == 0) {

		}
		else if (cmd.compare("XXX") == 0) {

		}
		else if (cmd.compare("q") == 0) {
			break;
		}
		else {
			print_tip();

		}

	}


}

int main(int argc, char *argv[])
{
	
	lifecycle_deadline(argc, argv);

	string cmd;
	std::getline(std::cin, cmd);
}
