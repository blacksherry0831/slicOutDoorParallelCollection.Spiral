#ifndef MAINWINDOW_H
#define MAINWINDOW_H
/*-------------------------------------*/
#define PLC_CTRL_ASYNC (1) 
/*-------------------------------------*/
#include <QMainWindow>
/*-------------------------------------*/
#include "QT_SDK_LIB/QBase.h"
/*-------------------------------------*/
#if PLC_CTRL_ASYNC
	#include "QT_THREAD_MODULE/QtThreadStepMotorServer.hpp"
	#include "QT_THREAD_MODULE/CMD_WORK_FLOW.hpp"
#endif // PLC_CTRL_ASYNC
/*-------------------------------------*/
#include "QT_THREAD_FLOW_CTRL/QtThreadFlowCtrlServer.hpp"
#include "QT_THREAD_FLOW_CTRL/QtThreadPlcNetClient.hpp"
/*-------------------------------------*/
namespace Ui {
class MainWindow;
}
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

#if PLC_CTRL_ASYNC	
	QSharedPointer<QtThreadPlcNetClient> mWorkFlowPlc;
	QSharedPointer<QtThreadStepMotorServer> mWorkFlowStepMotor;
#endif // PLC_CTRL_ASYNC

	QSharedPointer<QtThreadFlowCtrlServer> mFlowServerServer;
	QString   mCheckBoxRunStatus[2];
public:
	void tcpSvrNotifiedClientSessionM(CMD_WORK_FLOW::WF_FPGA_INNER _type_c, int _cmd_idx = 0);
	void tcpSvrBeforeNotifiedClientSessionM(CMD_WORK_FLOW::WF_FPGA_INNER _type_c, QString _msg="");

public:
	void closeDelay(int _ms=1000);
	void closeServerAsyncM();
private:
	void init_connect();
#if PLC_CTRL_ASYNC
	void init_connect_work_flow_plc();
	void init_connect_work_flow_setp_motor();
#endif
	void init_connect_work_flow_server();
private:
	void init_ctrols();
	void init_members();
	void init_class_member_base();
private:
	void check_box_ip_addr_clr();
private:
	QString GetWorkFlowRollerMsg(CMD_WORK_FLOW::SJTS_MACHINE_STATUS_ROLLER _sjts_s);
	QString GetWorkFlowMotorMsg(CMD_WORK_FLOW::SJTS_MACHINE_STATUS_MOTOR _sjts_s);
	int		GetServerClientSessions();
	void    SetSessionStatus2PlcNet();
private:
	void ShowStatusOnUI_Roller(CMD_WORK_FLOW::SJTS_MACHINE_STATUS_ROLLER __sjts_status);
	void ShowStatusOnUI_Motor(CMD_WORK_FLOW::SJTS_MACHINE_STATUS_MOTOR __sjts_status);
	
	void ShowStatusOnUI_ClientSessions_CheckBox(QString _ip,int _run,int _s);
	void ShowStatusOnUI_ClientSessions_Label(int _clients);
	void ShowStatusOnUI_ClientSessions_Label_Ex();
private:
	void dbg_checkRollerStatus(int _sjts_status_int, QString _msg);
	void dbg_checkMotorStatus(int _sjts_status_int, QString _msg);
private:
	void  SetCmdWorkFlow_StepMotor(int _sjts_status_int, QString _msg);
	void  SetCmdWorkFlow_Motor2PLcNet(int _sjts_status_int, QString _msg);
	void  SetCmdWorkFlow_Server_Roller(int _sjts_status_int, QString _msg);
	
	void  SetCmdWorkFlow_Server_Motor(int _sjts_status_int, QString _msg);
public:
	void  keyPressEvent(QKeyEvent *e);
public slots:
	  
	   void sjts_status_roller(int _sjts_status_int, QString _msg);
	   void sjts_status_motor(int _sjts_status_int, QString _msg);
	   	  
	   void socket_connect_state_Auto_equipment(int _status);

	   void thread_running_state_Auto_equipment(int _status);	   	 
	   void thread_running_state_step_motor(int _status);

	   void tcp_server_work_flow_dones(int _status, int _quality);
	   void work_flow_server_client_sessions(QString _ip_addr,int _run, int _s);
};

#endif // MAINWINDOW_H
