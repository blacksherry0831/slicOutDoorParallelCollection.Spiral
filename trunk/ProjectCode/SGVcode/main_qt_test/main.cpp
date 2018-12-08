#include "mainwindow.h"


#include <QApplication>

#include "cpp_stl.h"
#include "opencv_stl.h"
#if defined(QT_VERSION)
#include  "QT_THREAD_MODULE/QtThread8ImgProcess.hpp"
#include  "QT_THREAD_MODULE/QtThread8Video.hpp"
#include  "QT_THREAD_MODULE/QtThread8VideoProcess.hpp"
#include  "QT_THREAD_MODULE/QtThread8VideoRaw.hpp"
#endif

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
		
	return  EXEC_RESULT= a.exec();
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


int eightChannelVideo(int argc, char *argv[],int mode)
{
	QApplication app(argc, argv);
	
	QSharedPointer<QtThreadClientCtrl>		ctrlServer = QSharedPointer<QtThreadClientCtrl>(new QtThreadClientCtrl());
	
	QSharedPointer<QtThread8Video>			videoDataServer = QSharedPointer<QtThread8Video>(new QtThread8Video());

	ctrlServer->SetWorkMode(CMD_CTRL::WorkMode(mode));


	ctrlServer->start();

	videoDataServer->start();

	QtThread8VideoProcess::startTask();

	return app.exec();
}

int eightChannelVideoRaw(int argc, char *argv[])
{
	QApplication app(argc, argv);
	
	QSharedPointer<QtThread8VideoRaw>		videoRawDataServer = QSharedPointer<QtThread8VideoRaw>(new QtThread8VideoRaw());
	
	videoRawDataServer->start();
	
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

				return eightChannelVideo(argc, argv, (int)(CMD_CTRL::WorkMode::WM_ORG_IMG | CMD_CTRL::WorkMode::WM_SIZE_FULL));

			}
			else if (cmd.compare("8ChVideoTest") == 0) {

				return eightChannelVideo(argc,argv, (int)(CMD_CTRL::WorkMode::WM_DIFF_IMG | CMD_CTRL::WorkMode::WM_SIZE_FULL));

			}
			else if (cmd.compare("8ChVideoRaw") == 0) {

				return eightChannelVideoRaw(argc, argv);

			}
			else if (cmd.compare("ShowUI") == 0) {
				return mainGui(argc,argv);
			}else if (cmd.compare("XXX") == 0) {

			}else if (cmd.compare("q") == 0) {
				break;
			}
			else {
				std::cout << "please input a cmd :" << std::endl;
				std::cout << "8ChVideo" << std::endl;
				std::cout << "8ChVideoTest" << std::endl;
				std::cout << "8ChVideoRaw" << std::endl;
				std::cout << "ShowUI" << std::endl;
				std::cout << "PLC_M" << std::endl;
				std::cout << "q" << std::endl;
				std::cout << "......" << std::endl;

			}

	}	
	
}
