#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


#include "QT_THREAD_MODULE/QtThreadPLC.hpp"

#include "QT_THREAD_FLOW_CTRL/QtThreadFlowCtrlServer.hpp"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
	QTimer *mTimer;
	QSharedPointer<QtThreadPLC> mPlcdataServer;
	QSharedPointer<QtThreadFlowCtrlServer> mFlowServerServer;
	QString   mCheckBoxRunStatus[2];
private:
	void init_connect();
	void init_ctrols();
	void init_members();
	void init_class_member_base();
	void ShowSjtsStatusOnUI(CMD_CTRL::SJTS_MACHINE_STATUS __sjts_status);
	void printf_event(std::string _event, std::string _msg="");
	void check_box_clr();
public slots:
	   void sjts_status(int _sjts_status_int);
	   void update_link_status();

	   void socket_connect_state_Auto_equipment(int _status);
	   void thread_running_state_Auto_equipment(int _status);

};

#endif // MAINWINDOW_H
