#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>

#include "module_my.h"


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
#endif

#include <QtCore/QVariant>
#include <qtimer.h>
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

	bool m_test_result;
public:
	
	void WorkStart();
	void WorkDone();
public:
	QSharedPointer<QtThreadClientCtrl>		mCtrlServer;

	QSharedPointer<QtThread8Video>			mVideoDataServer;

	QSharedPointer<QThread>					mthread;

	QSharedPointer<QtLink>					mLink;
	
	QSharedPointer<QTimer>					mTimer;
private:
	void init_class_member();
public:
	void message_test();
public slots:
	
		void ClickButton_CutArea();
		void StartVideo();
		void StopVideo();
		void ConnectVideo();

public slots:
	void ClickButton_Test();
	void ClickButton_SetSerialPort();
	void ClickButton_CameraStart();
	void ClickButton_MotorRun();
	void WorkProgressShow(QString str);
	void CheckBox_ping(int _stat_t);
	void CheckBox_ssh(int _stat_t);
	void CheckBox_fpga_ctrl(int _stat_t);
	void CheckBox_fpga_image_video(int _stat_t);
	void main_test();
};

#endif // MAINWINDOW_H
