#include "mainwindow.h"


#include <QApplication>

#include "cpp_stl.h"
#include "opencv_stl.h"


#include "module_my.h"



#ifdef _MSC_VER
#ifndef NDEBUG
#pragma comment(linker, "/SUBSYSTEM:CONSOLE")
#endif
#endif
int mainGui(int argc, char *argv[]) 
{
	int EXEC_RESULT = 0;
	QApplication a(argc, argv);
	
	MainWindow w;

	w.show();
		
	return  EXEC_RESULT= a.exec();;
}

int plcTest(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
#if 0
	QSharedPointer<QtTcpServer>  tcpServer = QSharedPointer<QtTcpServer>(new QtTcpServer(Q_NULLPTR, Q_NULLPTR));
	QSharedPointer<QtThreadServer> dataServer = QSharedPointer<QtThreadServer>(new QtThreadServer(10000,tcpServer));
	
	dataServer->start();
#endif // 0



	QSharedPointer<QtThreadPLC> plcdataServer = QSharedPointer<QtThreadPLC>(new QtThreadPLC(0));


	//dataServer->start();

	plcdataServer->start();

	return a.exec();
}


int eightChannelVideo(int argc, char *argv[])
{
	QApplication app(argc, argv);
	
	QSharedPointer<QtThreadClientCtrl>		ctrlServer = QSharedPointer<QtThreadClientCtrl>(new QtThreadClientCtrl());
	
	QSharedPointer<QtThread8Video>			videoDataServer = QSharedPointer<QtThread8Video>(new QtThread8Video());

#if 0
QSharedPointer<QtThread8VideoRaw>		videoRawDataServer = QSharedPointer<QtThread8VideoRaw>(new QtThread8VideoRaw());
#endif // 0

	

#if 0
	QSharedPointer<QtTcpServerTest>         data_test_Server = QSharedPointer<QtTcpServerTest>(new QtTcpServerTest(Q_NULLPTR,TCP_PORT_VIDEO_RAW));
#endif
	ctrlServer->start();

	videoDataServer->start();

#if 0
	videoRawDataServer->start();
#endif // 0


#if 0
	data_test_Server->StartListen();
#endif // 0

	

	QtThread8VideoProcess::startTask();

	return app.exec();
}








int main(int argc, char *argv[])
{
	string cmd;

	while (TRUE) {
			
			std::getline(std::cin, cmd);
			if (cmd.compare("PLC_M") == 0) {
				
				return	plcTest(argc, argv);

			}else if (cmd.compare("8ChVideo") == 0) {

				return eightChannelVideo(argc, argv);

			}else if (cmd.compare("ShowUI") == 0) {
				return mainGui(argc,argv);
			}else if (cmd.compare("XXX") == 0) {

			}else if (cmd.compare("q") == 0) {
				break;
			}
			else {
				std::cout << "please input a cmd :" << std::endl;
				std::cout << "8ChVideo" << std::endl;
				std::cout << "ShowUI" << std::endl;
				std::cout << "PLC_M" << std::endl;
				std::cout << "q" << std::endl;
				std::cout << "......" << std::endl;

			}

	}	
	
}
