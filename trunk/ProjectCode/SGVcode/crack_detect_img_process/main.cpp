#include "mainwindow.h"


#include <QApplication>

#include "cpp_stl.h"
#include "opencv_stl.h"

#include "qt_all.h"

#include "module_my.h"

#if defined(QT_VERSION)

#include "QT_THREAD_MODULE/QtThread8ImgProcess.hpp"
#include "QT_THREAD_MODULE/QtThread8Video.hpp"
#include "QT_THREAD_MODULE/QtThread8VideoRaw.hpp"
#include "QT_THREAD_MODULE/QtThread8VideoProcess.hpp"

#endif


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
		return mainGui(argc,argv);
}
