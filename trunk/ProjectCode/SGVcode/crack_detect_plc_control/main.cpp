


#include <QApplication>

#include "cpp_stl.h"
#include "opencv_stl.h"






#ifdef _MSC_VER
#ifndef NDEBUG
#pragma comment(linker, "/SUBSYSTEM:CONSOLE")
#endif
#endif


#include "SocketQT\QtThreadPLC.hpp"

int plcServer(int argc, char *argv[])
{

	QCoreApplication a(argc, argv);

	QSharedPointer<QtThreadPLC> plcdataServer = QSharedPointer<QtThreadPLC>(new QtThreadPLC(0));

	plcdataServer->start();

	return a.exec();
}


int main(int argc, char *argv[])
{
				
	return	plcServer(argc, argv);

	
}
