#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "QT_SDK_LIB/QBase.h"

#include "QT_THREAD_MODULE/QtThreadPLC.hpp"

#include "QT_THREAD_FLOW_CTRL/QtThreadFlowCtrlServer.hpp"

namespace Ui {
class MainWindow;
}

/*-------------------------------------*/
#define PLC_CTRL_USE_TIMER (1)
/*-------------------------------------*/


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

#if PLC_CTRL_USE_TIMER
	QTimer *mFlowCtrlTimer;
#endif // PLC_CTRL_USE_TIMER


	QSharedPointer<QtThreadPLC> mPlcdataServer;
	QSharedPointer<QtThreadFlowCtrlServer> mFlowServerServer;
	QString   mCheckBoxRunStatus[2];
	void update_work_flow_status_ex(QtThreadFlowCtrlBase* _work_flow,
									QtThreadFlowCtrlServer* _Server);

	void tcpSvrNotifiedClientSessionM(CMD_CTRL::CMD_TYPE_LOCAL _type_c, int _cmd_idx = 0);
	void tcpSvrBeforeNotifiedClientSessionM(CMD_CTRL::CMD_TYPE_LOCAL _type_c, QString _msg="");
public:
	void closeServerAsync();
private:
	void init_connect();
	
	void init_connect_work_flow(QObject* _sender);
	void init_ctrols();
	void init_members();
	void init_class_member_base();
	void ShowSjtsStatusOnUI(CMD_CTRL::SJTS_MACHINE_STATUS __sjts_status);
	
	void check_box_clr();
public slots:
	   void sjts_status(int _sjts_status_int, QString _msg);
	   void update_link_status();
	   void update_work_flow_status();
	   void socket_connect_state_Auto_equipment(int _status);
	   void thread_running_state_Auto_equipment(int _status);
	   void tcp_server_work_flow_dones(int _status, int _quality);
	   void tcp_server_running_client_sessions(int _running_sessions);
};

#endif // MAINWINDOW_H
