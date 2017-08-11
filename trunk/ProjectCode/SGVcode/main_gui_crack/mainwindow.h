#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>

#include "DoWork.hpp"

#ifdef QT_VERSION

#endif // QT_VERSION

#if _MSC_VER
	#ifndef _X86_
		#define _X86_
	#endif // !_X86_
	#include <minwindef.h>
	#include <minwinbase.h>
	#include <WinBase.h>
#endif

#if TRUE
	#include "cpp_stl.h"
	#include "SerialPort/BE_1105_Dirver.hpp"
	#include "HW_SDK_LIB/hw_nvr.h"
#endif

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    Ui::MainWindow *ui;

public:
	void initUI();
	void TestSerialPort();
	bool m_test_result;
public:
	void WhenCfgDone(boolean enable);
	void WorkStart();
	void WorkDone();
public:
	


	DoWorkThread*  m_DoWorkThread;

public slots:
	void ClickButton_Test();
	void ClickButton_SetSerialPort();
	void ClickButton_CameraStart();
	void ClickButton_MotorRun();
	void WorkProgressShow(QString str);
};

#endif // MAINWINDOW_H
