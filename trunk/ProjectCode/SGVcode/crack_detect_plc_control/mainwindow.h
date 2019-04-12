#ifndef MAINWINDOW_H
#define MAINWINDOW_H
/*-------------------------------------*/
#define PLC_CTRL_SYNC (0) 
#define PLC_CTRL_ASYNC (1) 
/*-------------------------------------*/
#include <QMainWindow>
/*-------------------------------------*/
#include "QT_SDK_LIB/QBase.h"
/*-------------------------------------*/
#if PLC_CTRL_SYNC
	#include "QT_THREAD_MODULE/QtThreadPLC.hpp"
#endif // PLC_CTRL_SYNC
/*-------------------------------------*/
#if PLC_CTRL_ASYNC
	#include "QT_THREAD_MODULE/QtThreadPlcSocketClient.hpp"
	#include "QT_THREAD_MODULE/QtThreadStepMotorServer.hpp"
	#include "QT_THREAD_MODULE/CMD_WORK_FLOW.hpp"
#endif // PLC_CTRL_ASYNC
/*-------------------------------------*/
#include "QT_THREAD_FLOW_CTRL/QtThreadFlowCtrlServer.hpp"
/*-------------------------------------*/
namespace Ui {
class MainWindow;
}
/*-------------------------------------*/
#define PLC_CTRL_USE_TIMER (1)
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
class MainWindow : public QMainWindow,public IPrint
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

#if PLC_CTRL_SYNC
	QSharedPointer<QtThreadPLC> mPlcdataServer;
#endif // PLC_CTRL_SYNC
#if PLC_CTRL_ASYNC
	QSharedPointer<QtThreadPlcSocketClient> mWorkFlowSocket;
	QSharedPointer<QtThreadStepMotorServer> mWorkFlowStepMotor;
#endif // PLC_CTRL_ASYNC

	QSharedPointer<QtThreadFlowCtrlServer> mFlowServerServer;
	QString   mCheckBoxRunStatus[2];
	void update_work_flow_status_ex(QtThreadFlowCtrlBase* _work_flow,
									QtThreadFlowCtrlServer* _Server);

	void tcpSvrNotifiedClientSessionM(CMD_CTRL::CMD_TYPE_LOCAL _type_c, int _cmd_idx = 0);
	void tcpSvrBeforeNotifiedClientSessionM(CMD_CTRL::CMD_TYPE_LOCAL _type_c, QString _msg="");

public:
	void closeDelay(int _ms=1000);
	void closeServerAsyncM();
private:
	void init_connect();
	void init_connect_work_flow_async();
	void init_connect_work_flow(QObject* _sender);
	void init_ctrols();
	void init_members();
	void init_class_member_base();
private:
	void check_box_clr();
private:
	void ShowSjtsStatusOnUI(CMD_CTRL::SJTS_MACHINE_STATUS __sjts_status);
	void ShowStatusOnUI_Roller(CMD_WORK_FLOW::SJTS_MACHINE_STATUS_ROLLER __sjts_status);
	void ShowStatusOnUI_Motor(CMD_WORK_FLOW::SJTS_MACHINE_STATUS_MOTOR __sjts_status);
private:
	void dbg_checkRollerStatus(int _sjts_status_int, QString _msg);
	void dbg_checkMotorStatus(int _sjts_status_int, QString _msg);
private:
	void  SetCmdWorkFlow_StepMotor(int _sjts_status_int, QString _msg);
	void  SetCmdWorkFlow_Socket(int _sjts_status_int, QString _msg);
	void  SetCmdWorkFlow_Server_Roller(int _sjts_status_int, QString _msg);
	void  SetCmdWorkFlow_Server_Motor(int _sjts_status_int, QString _msg);
public:
	void  keyPressEvent(QKeyEvent *e);
public slots:
	   void sjts_status(int _sjts_status_int, QString _msg);
	   void sjts_status_roller(int _sjts_status_int, QString _msg);
	   void sjts_status_motor(int _sjts_status_int, QString _msg);

	   void update_link_status();
	   void update_work_flow_status();
	   void socket_connect_state_Auto_equipment(int _status);
	   void thread_running_state_Auto_equipment(int _status);
	   void tcp_server_work_flow_dones(int _status, int _quality);
	   void tcp_server_running_client_sessions(int _running_sessions);
};

#endif // MAINWINDOW_H
