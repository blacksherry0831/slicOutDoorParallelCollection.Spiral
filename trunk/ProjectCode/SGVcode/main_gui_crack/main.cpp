#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	int EXEC_RESULT = 0;

	HW_NET_Init(0);{
		QApplication a(argc, argv);
		MainWindow w;
		w.show();
		EXEC_RESULT=a.exec();

	}HW_NET_Release();

	return  EXEC_RESULT;
}
