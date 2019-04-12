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

#if PLC_CTRL_SYNC
	mPlcdataServer->startServer();
#endif

#if PLC_CTRL_ASYNC
	this->mWorkFlowSocket->startServer();
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
	this->mWorkFlowSocket->closeServerSync();
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
void MainWindow::update_work_flow_status_ex(QtThreadFlowCtrlBase* _work_flow,
	QtThreadFlowCtrlServer* _Server)
{

	if (_work_flow != Q_NULLPTR &&
		_Server != Q_NULLPTR) {
	
		const int channels_enable = _Server->getRunningSessionIpAddr().size();

		_work_flow->setClientSessionCount(channels_enable);

	}

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::init_connect()
{
#if PLC_CTRL_SYNC
	this->init_connect_work_flow(mPlcdataServer.data());
#endif

#if PLC_CTRL_ASYNC
	this->init_connect_work_flow(mWorkFlowSocket.data());
	this->init_connect_work_flow(mWorkFlowStepMotor.data());
	this->init_connect_work_flow_async();
#endif


#if  defined(QT_VERSION)
	this->connect(mFlowServerServer.data(),
		SIGNAL(work_flow_done(int,int)),
		this,
		SLOT(tcp_server_work_flow_dones(int,int)));

#endif
	
#if PLC_CTRL_USE_TIMER
	connect(mFlowCtrlTimer,
		SIGNAL(timeout()),
		this, 
		SLOT(update_link_status()));	
#endif // PLC_CTRL_USE_TIMER

	
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::init_connect_work_flow_async()
{
#if PLC_CTRL_ASYNC
	QObject* sender_t=this->mWorkFlowSocket.data();
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
void MainWindow::sjts_status(const int _sjts_status_int, QString _msg)
{
	const CMD_CTRL::SJTS_MACHINE_STATUS _sjts_status = (CMD_CTRL::SJTS_MACHINE_STATUS) _sjts_status_int;
#if 0
	if (_sjts_status == CMD_CTRL::SJTS_MACHINE_STATUS::RoolerReady) {

		tcpSvrBeforeNotifiedClientSessionM(CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_START);
		tcpSvrNotifiedClientSessionM(CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_START, _msg.toUInt());

	}
	else if (_sjts_status == CMD_CTRL::SJTS_MACHINE_STATUS::RollerDoneQualified ||
		_sjts_status == CMD_CTRL::SJTS_MACHINE_STATUS::RollerDoneUnqualified) {
		
	}
	else if (_sjts_status == CMD_CTRL::SJTS_MACHINE_STATUS::RollerDone) {

		tcpSvrNotifiedClientSessionM(CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_STOP);

	}
	else if (_sjts_status == CMD_CTRL::SJTS_MACHINE_STATUS::StepMotorStart01) {

		tcpSvrNotifiedClientSessionM(CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_START_01);

	}
	else if (_sjts_status == CMD_CTRL::SJTS_MACHINE_STATUS::StepMotorStop01) {

		tcpSvrNotifiedClientSessionM(CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_STOP_01);

	}
	else if (_sjts_status == CMD_CTRL::SJTS_MACHINE_STATUS::StepMotorStart00) {

		tcpSvrNotifiedClientSessionM(CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_START_00);

	}
	else if (_sjts_status == CMD_CTRL::SJTS_MACHINE_STATUS::StepMotorStop00) {

		tcpSvrNotifiedClientSessionM(CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_STOP_00);

	}
	else if (_sjts_status == CMD_CTRL::SJTS_MACHINE_STATUS::SerialPortError) {
		QBase::printf_event("SIGNAL", "SerialPortError");
	}
	else if (_sjts_status == CMD_CTRL::SJTS_MACHINE_STATUS::SerialPortIsOpen) {
		QBase::printf_event("SIGNAL", "SerialPortIsOpen");
	}
	else if (_sjts_status == CMD_CTRL::SJTS_MACHINE_STATUS::ClientError){
		QBase::printf_event("ClientError", _msg.toStdString());
	}else if (_sjts_status == CMD_CTRL::SJTS_MACHINE_STATUS::SjtsMachineInnerError) {
		closeServerAsyncM();
#if 1
		QTimer *timer = new QTimer(this);
		connect(timer, SIGNAL(timeout()), this, SLOT(close()));
		timer->start(1000);
#endif

	}else {
		Q_ASSERT(0);
	}
#endif // 0
	this->ShowSjtsStatusOnUI(_sjts_status);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::ShowSjtsStatusOnUI(CMD_CTRL::SJTS_MACHINE_STATUS _sjts_status)
{
	QString status_t;
	bool status_serial_port_t = false;
	if (_sjts_status == CMD_CTRL::SJTS_MACHINE_STATUS::RoolerReady) {
			status_t="RoolerReady";
	}else if (_sjts_status == CMD_CTRL::SJTS_MACHINE_STATUS::RollerDoneQualified ) {
			status_t = "Qualified";
	}else if (_sjts_status == CMD_CTRL::SJTS_MACHINE_STATUS::RollerDoneUnqualified) {
			status_t = "Unqualified";
	}else if (_sjts_status == CMD_CTRL::SJTS_MACHINE_STATUS::RollerDone) {
			status_t = "RollerDone";
	}else if (_sjts_status == CMD_CTRL::SJTS_MACHINE_STATUS::StepMotorStart01) {
			status_t = "StepMotorStart01";
	}else if (_sjts_status == CMD_CTRL::SJTS_MACHINE_STATUS::StepMotorStop01) {
			status_t = "StepMotorStop01";
	}else if (_sjts_status == CMD_CTRL::SJTS_MACHINE_STATUS::StepMotorStart00) {
			status_t = "StepMotorStart00";
	}else if (_sjts_status == CMD_CTRL::SJTS_MACHINE_STATUS::StepMotorStop00) {
			status_t = "StepMotorStop00";
		
	}else if (_sjts_status == CMD_CTRL::SJTS_MACHINE_STATUS::SerialPortError) {
			status_t = "SerialPortError";
			status_serial_port_t = false;
			this->ui->checkBox_serial_port->setChecked(status_serial_port_t);
	}else if (_sjts_status == CMD_CTRL::SJTS_MACHINE_STATUS::SerialPortIsOpen) {
			status_t = "SerialPortIsOpen";		
			status_serial_port_t = true;			
			this->ui->checkBox_serial_port->setChecked(status_serial_port_t);
	}else if (_sjts_status == CMD_CTRL::SJTS_MACHINE_STATUS::ClientError) {
		
	}else if (_sjts_status == CMD_CTRL::SJTS_MACHINE_STATUS::SjtsMachineInnerError) {
		
	}else {
		Q_ASSERT(0);
	}
	this->ui->statusBar->showMessage(status_t);
	this->ui->progressBar_sjts_plc_work_flow->setValue(std::min(_sjts_status, CMD_CTRL::SJTS_MACHINE_STATUS::RollerDoneQualified));
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::ShowStatusOnUI_Roller(CMD_WORK_FLOW::SJTS_MACHINE_STATUS_ROLLER __sjts_status)
{

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::ShowStatusOnUI_Motor(CMD_WORK_FLOW::SJTS_MACHINE_STATUS_MOTOR __sjts_status)
{

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::init_ctrols()
{
	this->ui->progressBar_sjts_plc_work_flow->setRange(0, CMD_CTRL::SJTS_MACHINE_STATUS::RollerDoneQualified);
	this->ui->progressBar_sjts_plc_work_flow->setValue(0);
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
	mWorkFlowSocket = QSharedPointer<QtThreadPlcSocketClient> (new QtThreadPlcSocketClient());
	mWorkFlowStepMotor = QSharedPointer<QtThreadStepMotorServer>(new QtThreadStepMotorServer()) ;
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

	this->SetCmdWorkFlow_Socket(_sjts_status_int,_msg);
	this->SetCmdWorkFlow_Server_Motor(_sjts_status_int, _msg);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::update_link_status()
{
	
	if (!mFlowServerServer.isNull()) {

		this->check_box_clr();

		QVector<QString>  v_qstr_t= mFlowServerServer->getRunningSessionIpAddr();
		int channels_enable=v_qstr_t.size();
		QString ch_info = "Running session : "+QString::number(channels_enable);
		ui->label_channels_enable->setText(ch_info);
				for (size_t i = 0; i <v_qstr_t.size(); i++){
			
							QString qstr_t = v_qstr_t.at(i);
			
							if (0==qstr_t.contains(IPC_DATA_CENTER_ADDR_00)){
								this->ui->checkBox_101->setChecked(true);
							}else if (0==qstr_t.contains(IPC_DATA_CENTER_ADDR_01)){
								this->ui->checkBox_102->setChecked(true);
							}else if (0==qstr_t.contains(IPC_DATA_CENTER_ADDR_02)) {
								this->ui->checkBox_103->setChecked(true);
							}else{
								this->ui->checkBox_another->setChecked(true);
							}

				}


	}

	this->update_work_flow_status();

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::update_work_flow_status()
{

#if PLC_CTRL_SYNC
	this->update_work_flow_status_ex( mPlcdataServer.data(),mFlowServerServer.data());
#endif

#if PLC_CTRL_ASYNC
	this->update_work_flow_status_ex(mWorkFlowSocket.data(), mFlowServerServer.data());
#endif

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::check_box_clr()
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

		this->printf_event("STATUS ROLLER !","roller qualified is done !");

	}else if (_sjts_status == CMD_WORK_FLOW::SJTS_MACHINE_STATUS_ROLLER::MachineAbort){


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

		printf_event("SIGNAL", "SerialPortIsOpen");

	}else if (_sjts_status == CMD_WORK_FLOW::SJTS_MACHINE_STATUS_MOTOR::SerialPortError) {

		printf_event("ClientError", _msg.toStdString());

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
void MainWindow::SetCmdWorkFlow_Socket(int _sjts_status_int, QString _msg)
{
	const  CMD_WORK_FLOW::SJTS_MACHINE_STATUS_MOTOR sjts_status = (CMD_WORK_FLOW::SJTS_MACHINE_STATUS_MOTOR) _sjts_status_int;
	int cmd01 = _msg.toInt();
	unsigned int seq_no = 0;
		
	QSharedPointer<CMD_CTRL> qsp_cc_t = CMD_CTRL::getLocalCmdEx(_sjts_status_int, cmd01, seq_no);
	this->mWorkFlowSocket->SetMsgWhileRunning(qsp_cc_t);
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

		tcpSvrBeforeNotifiedClientSessionM(CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_START);
		tcpSvrNotifiedClientSessionM(CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_START, _msg.toUInt());
	}
	else if (_sjts_status == CMD_WORK_FLOW::SJTS_MACHINE_STATUS_ROLLER::RollerPosReady) {

	}
	else if (_sjts_status == CMD_WORK_FLOW::SJTS_MACHINE_STATUS_ROLLER::RollerIntoInnerReady) {

	}
	else if (_sjts_status == CMD_WORK_FLOW::SJTS_MACHINE_STATUS_ROLLER::RollerDoneEnd) {

		tcpSvrNotifiedClientSessionM(CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_STOP);

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

		tcpSvrNotifiedClientSessionM(CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_START_00);

	}else if (_sjts_status == CMD_WORK_FLOW::SJTS_MACHINE_STATUS_MOTOR::MotorStop00) {

		tcpSvrNotifiedClientSessionM(CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_STOP_00);

	}else if (_sjts_status == CMD_WORK_FLOW::SJTS_MACHINE_STATUS_MOTOR::MotorStart01) {

		tcpSvrNotifiedClientSessionM(CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_START_01);

	}else if (_sjts_status == CMD_WORK_FLOW::SJTS_MACHINE_STATUS_MOTOR::MotorStop01) {

		tcpSvrNotifiedClientSessionM(CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_STOP_01);

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
void  MainWindow::tcp_server_work_flow_dones(int _status, int _quality)
{

	if (_status) {
		QBase::printf_event("WORK FLOW", "all client thread work done !");
	}

	if (_status) {
#if PLC_CTRL_SYNC	
		this->mPlcdataServer->setWorkFlowDones(_status,_quality);
#endif

#if PLC_CTRL_ASYNC
		this->mWorkFlowSocket->setWorkFlowDones(_status, _quality);
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
void MainWindow::tcpSvrNotifiedClientSessionM(CMD_CTRL::CMD_TYPE_LOCAL _type_c, int _cmd_idx)
{
	mFlowServerServer->NotifiedClientSession(_type_c, _cmd_idx);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::tcp_server_running_client_sessions(int _running_sessions)
{
#if PLC_CTRL_SYNC
	mPlcdataServer->setClientSessionCount(_running_sessions);
#endif

#if PLC_CTRL_ASYNC
	mWorkFlowSocket->setClientSessionCount(_running_sessions);
#endif

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::tcpSvrBeforeNotifiedClientSessionM(CMD_CTRL::CMD_TYPE_LOCAL _type_c, QString _msg)
{

	mFlowServerServer->beforeNotifiedClientSession(_type_c);

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::init_connect_work_flow(QObject* _sender)
{
	Q_ASSERT(_sender!=Q_NULLPTR);

#if  defined(QT_VERSION)
	
	this->connect(_sender,
		SIGNAL(status_sjts(int, QString)),
		this,
		SLOT(sjts_status(int, QString))
		);

	this->connect(_sender,
		SIGNAL(socket_connect_state(int)),
		this,
		SLOT(socket_connect_state_Auto_equipment(int))
		);

	this->connect(_sender,
		SIGNAL(thread_running_state(int)),
		this,
		SLOT(thread_running_state_Auto_equipment(int))
		);

#endif //  defined(Q_VERSION)

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

#if PLC_CTRL_SYNC
	mPlcdataServer->closeServerAsync();
#endif

#if PLC_CTRL_ASYNC
	mWorkFlowSocket->closeServerAsync();
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
