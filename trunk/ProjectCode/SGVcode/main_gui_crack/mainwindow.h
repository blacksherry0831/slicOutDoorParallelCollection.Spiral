#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>

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
	QString m_motor_port_name;
	bool m_motor_run_direct;
	bool m_test_result;
public:
	void WhenCfgDone(boolean enable);
public:
	BE_1105_Driver*		m_Step_Motor_ptr;
	std::auto_ptr<hw_nvr> m_hw_nvr_ptr;
public slots:
	void ClickButton_Test();
	void ClickButton_SetSerialPort();
	void ClickButton_CameraStart();
	void ClickButton_MotorRun();
};

#endif // MAINWINDOW_H
