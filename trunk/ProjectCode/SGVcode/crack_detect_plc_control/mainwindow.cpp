#include "mainwindow.h"
#include "ui_mainwindow.h"
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

	this->init_members();
	this->init_class_member_base();
	this->init_connect();
	this->init_ctrols();

#if PLC_CTRL_ASYNC
	this->mWorkFlowPlc->startServer();
	this->mWorkFlowStepMotor->startServer();
#endif

	mFlowServerServer->startServer();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
MainWindow::~MainWindow()
{

#if PLC_CTRL_SYNC
	mPlcdataServer->closeServerSync();
#endif

#if PLC_CTRL_ASYNC
	this->mWorkFlowPlc->closeServerSync();
	this->mWorkFlowStepMotor->closeServerSync();
#endif

	mFlowServerServer->closeServerSync();

    delete ui;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::init_connect_work_flow_plc()
{
	QObject* sender_t = this->mWorkFlowPlc.data();

#if  defined(QT_VERSION)	
	this->connect(sender_t,
		SIGNAL(socket_connect_state(int)),
		this,
		SLOT(socket_connect_state_Auto_equipment(int))
		);

	this->connect(sender_t,
		SIGNAL(thread_running_state(int)),
		this,
		SLOT(thread_running_state_Auto_equipment(int))
		);
#endif
#if PLC_CTRL_ASYNC
		this->connect(sender_t,
			SIGNAL(status_sjts_roller(int, QString)),
			this,
			SLOT(sjts_status_roller(int, QString))
			);
#endif
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::init_connect_work_flow_setp_motor()
{

	QObject* sender_t = this->mWorkFlowStepMotor.data();

#if  defined(QT_VERSION)	
	this->connect(sender_t,
		SIGNAL(thread_running_state(int)),
		this,
		SLOT(thread_running_state_step_motor(int))
		);
#endif

#if PLC_CTRL_ASYNC	
		this->connect(sender_t,
			SIGNAL(status_sjts_motor(int, QString)),
			this,
			SLOT(sjts_status_motor(int, QString))
			);	
#endif

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::init_connect_work_flow_server()
{

	this->connect(mFlowServerServer.data(),
		SIGNAL(work_flow_done(int, int)),
		this,
		SLOT(tcp_server_work_flow_dones(int, int)));

	this->connect(mFlowServerServer.data(),
		SIGNAL(client_sessions_status(QString, int, int)),
		this,
		SLOT(work_flow_server_client_sessions(QString, int, int)));

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::init_connect()
{

#if PLC_CTRL_ASYNC
	this->init_connect_work_flow_plc();
	this->init_connect_work_flow_setp_motor();
#endif
	
	this->init_connect_work_flow_server();
		
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::ShowStatusOnUI_Roller(CMD_WORK_FLOW::SJTS_MACHINE_STATUS_ROLLER _sjts_status)
{
	this->ui->statusBar->showMessage(this->GetWorkFlowRollerMsg(_sjts_status));
	this->ui->progressBar_sjts_plc_work_flow->setValue(_sjts_status);



}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::ShowStatusOnUI_Motor(CMD_WORK_FLOW::SJTS_MACHINE_STATUS_MOTOR _sjts_status)
{
	
	this->ui->statusBar->showMessage(this->GetWorkFlowMotorMsg(_sjts_status));
	this->ui->progressBar_sjts_step_motor->setValue(_sjts_status);

#if 1
	boolean status_serial_port_t = false;
	if (_sjts_status == CMD_WORK_FLOW::SJTS_MACHINE_STATUS_MOTOR::SerialPortOpen) {
		this->ui->checkBox_serial_port->setChecked(status_serial_port_t=true);
	}else if (_sjts_status == CMD_WORK_FLOW::SJTS_MACHINE_STATUS_MOTOR::SerialPortError) {
		this->ui->checkBox_serial_port->setChecked(status_serial_port_t=false);	
	}else {

	}
#endif

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::ShowStatusOnUI_ClientSessions_CheckBox(QString _ip, int _run, int _s)
{
	if (_ip.contains(IPC_DATA_CENTER_ADDR_00)) {
		this->ui->checkBox_101->setChecked(_run);
	}
	else if (_ip.contains(IPC_DATA_CENTER_ADDR_01)) {
		this->ui->checkBox_102->setChecked(_run);
	}
	else if (_ip.contains(IPC_DATA_CENTER_ADDR_02)) {
		this->ui->checkBox_103->setChecked(_run);
	}
	else {
		this->ui->checkBox_another->setChecked(_run);
	}
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::ShowStatusOnUI_ClientSessions_Label_Ex()
{
	ShowStatusOnUI_ClientSessions_Label(this->GetServerClientSessions());
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::ShowStatusOnUI_ClientSessions_Label(int _clients)
{
	QString ch_info = "Running session : " + QString::number(_clients);
	ui->label_channels_enable->setText(ch_info);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
QString MainWindow::GetWorkFlowRollerMsg(CMD_WORK_FLOW::SJTS_MACHINE_STATUS_ROLLER _sjts_s)
{
	QString status_t;
	if (_sjts_s == CMD_WORK_FLOW::SJTS_MACHINE_STATUS_ROLLER::RollerReadyStart) {
		status_t = "RollerReadyStart";
	}else if (_sjts_s == CMD_WORK_FLOW::SJTS_MACHINE_STATUS_ROLLER::RollerPosReady) {
		status_t = "RollerPosReady";
	}else if (_sjts_s == CMD_WORK_FLOW::SJTS_MACHINE_STATUS_ROLLER::RollerIntoInnerReady) {
		status_t = "RollerIntoInnerReady";
	}else if (_sjts_s == CMD_WORK_FLOW::SJTS_MACHINE_STATUS_ROLLER::RollerDoneEnd) {
		status_t = "RollerDoneEnd";
	}else if (_sjts_s == CMD_WORK_FLOW::SJTS_MACHINE_STATUS_ROLLER::RollerDoneQ) {
		status_t = "RollerDoneQ";
	}else if(_sjts_s == CMD_WORK_FLOW::SJTS_MACHINE_STATUS_ROLLER::RollerDoneUnQ){
		status_t = "RollerDoneUnQ";
	}else if (_sjts_s == CMD_WORK_FLOW::SJTS_MACHINE_STATUS_ROLLER::MachineAbort) {
		status_t = "MachineAbort";
	}else {
		Q_ASSERT(FALSE);
	}
	return status_t;

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
QString MainWindow::GetWorkFlowMotorMsg(CMD_WORK_FLOW::SJTS_MACHINE_STATUS_MOTOR _sjts_s)
{
	QString status_t;
	if (_sjts_s == CMD_WORK_FLOW::SJTS_MACHINE_STATUS_MOTOR::MotorStart00) {
		status_t = "MotorStart00";
	}else if (_sjts_s == CMD_WORK_FLOW::SJTS_MACHINE_STATUS_MOTOR::MotorStop00) {
		status_t = "MotorStop00";
	}else if (_sjts_s == CMD_WORK_FLOW::SJTS_MACHINE_STATUS_MOTOR::MotorStart01) {
		status_t = "MotorStart01";
	}else if (_sjts_s == CMD_WORK_FLOW::SJTS_MACHINE_STATUS_MOTOR::MotorStop01) {
		status_t = "MotorStop01";
	}else if (_sjts_s == CMD_WORK_FLOW::SJTS_MACHINE_STATUS_MOTOR::SerialPortOpen) {
		status_t = "SerialPortOpen";
	}else if (_sjts_s == CMD_WORK_FLOW::SJTS_MACHINE_STATUS_MOTOR::SerialPortError) {
		status_t = "SerialPortError";
	}else {
		Q_ASSERT(FALSE);
	}
	return status_t;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int MainWindow::GetServerClientSessions()
{

	if (this->mFlowServerServer.isNull()){
		return 0;
	}else{
		QVector<QString>  v_qstr_t = mFlowServerServer->getThreadRunningSessionIpAddr();
		return  v_qstr_t.size();
	}

	

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::init_ctrols()
{

	this->ui->progressBar_sjts_plc_work_flow->setRange(0,100);
	this->ui->progressBar_sjts_plc_work_flow->setValue(0);

	this->ui->progressBar_sjts_step_motor->setRange(0, 100);
	this->ui->progressBar_sjts_plc_work_flow->setValue(0);

	check_box_ip_addr_clr();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::init_members()
{
#if PLC_CTRL_USE_TIMER
	this->mFlowCtrlTimer = new QTimer(this);
	this->mFlowCtrlTimer->start(1000);	
#endif // PLC_CTRL_USE_TIMER

#if PLC_CTRL_SYNC
	mPlcdataServer = QSharedPointer<QtThreadPLC>(new QtThreadPLC(0));
#endif

#if PLC_CTRL_ASYNC
	mWorkFlowPlc = QSharedPointer<QtThreadPlcNetClient> (new QtThreadPlcNetClient(this));
	mWorkFlowStepMotor = QSharedPointer<QtThreadStepMotorServer>(new QtThreadStepMotorServer(this)) ;
#endif	

	mFlowServerServer = QSharedPointer<QtThreadFlowCtrlServer>(new QtThreadFlowCtrlServer(this));

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::sjts_status_roller(int _sjts_status_int, QString _msg)
{
	const  CMD_WORK_FLOW::SJTS_MACHINE_STATUS_ROLLER sjts_status = (CMD_WORK_FLOW::SJTS_MACHINE_STATUS_ROLLER) _sjts_status_int;

	this->dbg_checkRollerStatus(_sjts_status_int,_msg);

	this->ShowStatusOnUI_Roller(sjts_status);

	this->ShowStatusOnUI_ClientSessions_Label_Ex();

	this->SetCmdWorkFlow_StepMotor(_sjts_status_int,_msg);
	this->SetCmdWorkFlow_Server_Roller(_sjts_status_int,_msg);

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::sjts_status_motor(int _sjts_status_int, QString _msg)
{
	const  CMD_WORK_FLOW::SJTS_MACHINE_STATUS_MOTOR sjts_status = (CMD_WORK_FLOW::SJTS_MACHINE_STATUS_MOTOR) _sjts_status_int;


	this->dbg_checkMotorStatus(_sjts_status_int,_msg);

	this->ShowStatusOnUI_Motor(sjts_status);

	this->SetCmdWorkFlow_Motor2PLcNet(_sjts_status_int,_msg);
	this->SetCmdWorkFlow_Server_Motor(_sjts_status_int, _msg);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::check_box_ip_addr_clr()
{
	bool check_t = false;

	this->ui->checkBox_101->setChecked(check_t);

	this->ui->checkBox_102->setChecked(check_t);

	this->ui->checkBox_103->setChecked(check_t);

	this->ui->checkBox_another->setChecked(check_t);

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::dbg_checkRollerStatus(int _sjts_status_int, QString _msg)
{
	const  CMD_WORK_FLOW::SJTS_MACHINE_STATUS_ROLLER _sjts_status = (CMD_WORK_FLOW::SJTS_MACHINE_STATUS_ROLLER) _sjts_status_int;

	if (_sjts_status == CMD_WORK_FLOW::SJTS_MACHINE_STATUS_ROLLER::RollerReadyStart) {

	}else if (_sjts_status == CMD_WORK_FLOW::SJTS_MACHINE_STATUS_ROLLER::RollerPosReady) {

	}else if (_sjts_status == CMD_WORK_FLOW::SJTS_MACHINE_STATUS_ROLLER::RollerIntoInnerReady) {

	}else if (_sjts_status == CMD_WORK_FLOW::SJTS_MACHINE_STATUS_ROLLER::RollerDoneEnd) {
		
	}else if (_sjts_status == CMD_WORK_FLOW::SJTS_MACHINE_STATUS_ROLLER::RollerDoneQ ||		
		_sjts_status == CMD_WORK_FLOW::SJTS_MACHINE_STATUS_ROLLER::RollerDoneUnQ) {

		this->printf_event("STATUS ROLLER","roller qualified is done !");

	}else if (_sjts_status == CMD_WORK_FLOW::SJTS_MACHINE_STATUS_ROLLER::MachineAbort){
		this->printf_event("STATUS ROLLER", "Abort !!!");

	}else {
		Q_ASSERT(FALSE);
	}

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::dbg_checkMotorStatus(int _sjts_status_int, QString _msg)
{
	const  CMD_WORK_FLOW::SJTS_MACHINE_STATUS_MOTOR _sjts_status = (CMD_WORK_FLOW::SJTS_MACHINE_STATUS_MOTOR) _sjts_status_int;

	if (_sjts_status == CMD_WORK_FLOW::SJTS_MACHINE_STATUS_MOTOR::MotorStart00) {
				
	}else if (_sjts_status == CMD_WORK_FLOW::SJTS_MACHINE_STATUS_MOTOR::MotorStop00) {
		
	}else if (_sjts_status == CMD_WORK_FLOW::SJTS_MACHINE_STATUS_MOTOR::MotorStart01) {
		
	}else if (_sjts_status == CMD_WORK_FLOW::SJTS_MACHINE_STATUS_MOTOR::MotorStop01) {
		
	}else if (_sjts_status == CMD_WORK_FLOW::SJTS_MACHINE_STATUS_MOTOR::SerialPortOpen) {

		printf_event("SIG", "SerialPortIsOpen");

	}else if (_sjts_status == CMD_WORK_FLOW::SJTS_MACHINE_STATUS_MOTOR::SerialPortError) {

		printf_event("SIG","SerialPort Error!!!");

	}else {
		Q_ASSERT(FALSE);
	}
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::SetCmdWorkFlow_StepMotor(int _sjts_status_int, QString _msg)
{
	const  CMD_WORK_FLOW::SJTS_MACHINE_STATUS_ROLLER sjts_status = (CMD_WORK_FLOW::SJTS_MACHINE_STATUS_ROLLER) _sjts_status_int;
	int cmd01 = 0;
	unsigned int seq_no = 0;
	
	if (sjts_status == CMD_WORK_FLOW::SJTS_MACHINE_STATUS_ROLLER::RollerReadyStart) {
		seq_no = _msg.toUInt();
	}

	if (sjts_status == CMD_WORK_FLOW::SJTS_MACHINE_STATUS_ROLLER::RollerIntoInnerReady)
	{
		cmd01 = _msg.toInt();
	}

	QSharedPointer<CMD_CTRL> qsp_cc_t = CMD_CTRL::getLocalCmdEx(_sjts_status_int, cmd01, seq_no);
	this->mWorkFlowStepMotor->SetMsgWhileRunning(qsp_cc_t);

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::SetCmdWorkFlow_Motor2PLcNet(int _sjts_status_int, QString _msg)
{
	int cmd01 = _msg.toInt();
	unsigned int seq_no = 0;
		
	QSharedPointer<CMD_CTRL> qsp_cc_t = CMD_CTRL::getLocalCmdEx(_sjts_status_int, cmd01, seq_no);
	this->mWorkFlowPlc->SetMsgWhileRunning(qsp_cc_t);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::SetCmdWorkFlow_Server_Roller(int _sjts_status_int, QString _msg)
{
	const  CMD_WORK_FLOW::SJTS_MACHINE_STATUS_ROLLER _sjts_status = (CMD_WORK_FLOW::SJTS_MACHINE_STATUS_ROLLER) _sjts_status_int;

	if (_sjts_status == CMD_WORK_FLOW::SJTS_MACHINE_STATUS_ROLLER::RollerReadyStart) {

		tcpSvrBeforeNotifiedClientSessionM(CMD_WORK_FLOW::WF_FPGA_INNER::CT_FPGA_START);
		tcpSvrNotifiedClientSessionM(CMD_WORK_FLOW::WF_FPGA_INNER::CT_FPGA_START, _msg.toUInt());

	}
	else if (_sjts_status == CMD_WORK_FLOW::SJTS_MACHINE_STATUS_ROLLER::RollerPosReady) {

	}
	else if (_sjts_status == CMD_WORK_FLOW::SJTS_MACHINE_STATUS_ROLLER::RollerIntoInnerReady) {

	}
	else if (_sjts_status == CMD_WORK_FLOW::SJTS_MACHINE_STATUS_ROLLER::RollerDoneEnd) {

		tcpSvrNotifiedClientSessionM(CMD_WORK_FLOW::WF_FPGA_INNER::CT_FPGA_STOP);

	}else if (_sjts_status == CMD_WORK_FLOW::SJTS_MACHINE_STATUS_ROLLER::RollerDoneQ ||
		_sjts_status == CMD_WORK_FLOW::SJTS_MACHINE_STATUS_ROLLER::RollerDoneUnQ) {

	}else if (_sjts_status == CMD_WORK_FLOW::SJTS_MACHINE_STATUS_ROLLER::MachineAbort) {

		this->closeServerAsyncM();
		this->closeDelay();

	}else {
		
	}
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::SetCmdWorkFlow_Server_Motor(int _sjts_status_int, QString _msg)
{

	const  CMD_WORK_FLOW::SJTS_MACHINE_STATUS_MOTOR _sjts_status = (CMD_WORK_FLOW::SJTS_MACHINE_STATUS_MOTOR) _sjts_status_int;

	if (_sjts_status == CMD_WORK_FLOW::SJTS_MACHINE_STATUS_MOTOR::MotorStart00) {
		
		this->SetSessionStatus2PlcNet();
		tcpSvrNotifiedClientSessionM(CMD_WORK_FLOW::WF_FPGA_INNER::CT_FPGA_START_00);

	}else if (_sjts_status == CMD_WORK_FLOW::SJTS_MACHINE_STATUS_MOTOR::MotorStop00) {

		tcpSvrNotifiedClientSessionM(CMD_WORK_FLOW::WF_FPGA_INNER::CT_FPGA_STOP_00);

	}else if (_sjts_status == CMD_WORK_FLOW::SJTS_MACHINE_STATUS_MOTOR::MotorStart01) {

		tcpSvrNotifiedClientSessionM(CMD_WORK_FLOW::WF_FPGA_INNER::CT_FPGA_START_01);

	}else if (_sjts_status == CMD_WORK_FLOW::SJTS_MACHINE_STATUS_MOTOR::MotorStop01) {
		
		this->SetSessionStatus2PlcNet();
		tcpSvrNotifiedClientSessionM(CMD_WORK_FLOW::WF_FPGA_INNER::CT_FPGA_STOP_01);

	}else {
		
	}

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::socket_connect_state_Auto_equipment(int _status)
{
	this->ui->checkBox_plc->setChecked(_status);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::thread_running_state_Auto_equipment(int _status)
{
	this->ui->checkBox_plc->setStyleSheet(mCheckBoxRunStatus[_status]);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::thread_running_state_step_motor(int _status)
{
	this->ui->checkBox_serial_port->setStyleSheet(mCheckBoxRunStatus[_status]);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void  MainWindow::tcp_server_work_flow_dones(int _status, int _quality)
{

	if (_status) {
		printf_event("WORK FLOW", "all client thread work done !");

#if PLC_CTRL_ASYNC
		this->mWorkFlowPlc->setWorkFlowDones(_status, _quality);
#endif
	
	}

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::init_class_member_base()
{

	mCheckBoxRunStatus[0] = "QCheckBox{color:rgb(255,0,0)}";
	mCheckBoxRunStatus[1] = "QCheckBox{color:rgb(0,255,0)}";

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::tcpSvrNotifiedClientSessionM(CMD_WORK_FLOW::WF_FPGA_INNER _type_c, int _cmd_idx)
{
	mFlowServerServer->NotifiedClientSession(_type_c, _cmd_idx);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::work_flow_server_client_sessions(QString _ip_addr, int _run, int _s)
{
	ShowStatusOnUI_ClientSessions_CheckBox(_ip_addr, _run, _s);
	this->ShowStatusOnUI_ClientSessions_Label_Ex();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::tcpSvrBeforeNotifiedClientSessionM(CMD_WORK_FLOW::WF_FPGA_INNER _type_c, QString _msg)
{

	

	mFlowServerServer->beforeNotifiedClientSession(_type_c);

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::closeDelay(int _ms)
{
	QTimer *timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(close()));
	timer->start(_ms);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::closeServerAsyncM()
{
#if PLC_CTRL_ASYNC
	mWorkFlowPlc->closeServerAsync();
	mWorkFlowStepMotor->closeServerAsync();
#endif
	mFlowServerServer->closeServerAsync();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void  MainWindow::keyPressEvent(QKeyEvent *e)
{
#if 0
	// ²¶×½ Ctrl + Alt + A ×éºÏ¼ü;
	if (e->key() == Qt::Key_S && e->modifiers() == (Qt::AltModifier | Qt::ControlModifier)) 
	{ 
		if (this->isHidden()) {
			this->show();
		}
		else {
			this->hide();
		}
	}
#endif
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::SetSessionStatus2PlcNet()
{
	mWorkFlowPlc->set_external_session(this->GetServerClientSessions());
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/