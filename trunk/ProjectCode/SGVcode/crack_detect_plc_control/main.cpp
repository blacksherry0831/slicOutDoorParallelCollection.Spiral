#include "mainwindow.h"


#include <QApplication>

#include "cpp_stl.h"
#include "opencv_stl.h"

#include "QT_THREAD_MODULE/QtThreadPLC.hpp"

#include "QT_THREAD_FLOW_CTRL/QtThreadFlowCtrlServer.hpp"




#ifdef _MSC_VER
#ifndef NDEBUG
#pragma comment(linker, "/SUBSYSTEM:CONSOLE")
#endif
#endif






int plcServer(int argc, char *argv[])
{

	QCoreApplication a(argc, argv);

	QSharedPointer<QtThreadPLC> plcdataServer = QSharedPointer<QtThreadPLC>(new QtThreadPLC(0));
	QSharedPointer<QtThreadFlowCtrlServer> flowServerServer = QSharedPointer<QtThreadFlowCtrlServer>(new QtThreadFlowCtrlServer(0));

	plcdataServer->startServer();
	flowServerServer->startServer();

	return a.exec();
}


int plcServerGUI(int argc, char *argv[])
{
		int EXEC_RESULT = 0;
		QApplication a(argc, argv);

		MainWindow w;

		w.show();

#if defined(linux) || defined(__linux) || defined(__linux__) ||defined( __GNUC__)
		w.hide();
#endif
		
		return  EXEC_RESULT = a.exec();
	
}




int main(int argc, char *argv[])
{
				
	return	plcServerGUI(argc, argv);

	
}
