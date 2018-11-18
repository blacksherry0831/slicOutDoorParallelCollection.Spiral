


#include <QApplication>

#include "cpp_stl.h"
#include "opencv_stl.h"






#ifdef _MSC_VER
#ifndef NDEBUG
#pragma comment(linker, "/SUBSYSTEM:CONSOLE")
#endif
#endif


#include "QT_THREAD_MODULE/QtThreadPLC.hpp"

#include "QT_THREAD_FLOW_CTRL/QtThreadFlowCtrlServer.hpp"



int plcServer(int argc, char *argv[])
{

	QCoreApplication a(argc, argv);

	QSharedPointer<QtThreadPLC> plcdataServer = QSharedPointer<QtThreadPLC>(new QtThreadPLC(0));
	QSharedPointer<QtThreadFlowCtrlServer> flowServerServer = QSharedPointer<QtThreadFlowCtrlServer>(new QtThreadFlowCtrlServer(0));

	plcdataServer->start();
	flowServerServer->start();

	return a.exec();
}


int main(int argc, char *argv[])
{
				
	return	plcServer(argc, argv);

	
}
