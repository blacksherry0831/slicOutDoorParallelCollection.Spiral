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


	mPlcdataServer->startServer();
	mFlowServerServer->startServer();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
MainWindow::~MainWindow()
{
	mPlcdataServer->closeServerSync();
	mFlowServerServer->closeServerSync();
	
	mPlcdataServer.clear();
	mFlowServerServer.clear();

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
	this->init_connect_plc();

#if  defined(QT_VERSION)
	this->connect(mFlowServerServer.data(),
		SIGNAL(work_flow_done(int)),
		this,
		SLOT(tcp_server_work_flow_dones(int)));

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
void MainWindow::init_connect_plc()
{
	Q_ASSERT(!mPlcdataServer.isNull());

#if  defined(QT_VERSION)
	const QObject * PlcdataServer_sender = mPlcdataServer.data();
	this->connect(PlcdataServer_sender,
		SIGNAL(status_sjts(int)),
		this,
		SLOT(sjts_status(int))
	);

	this->connect(PlcdataServer_sender,
		SIGNAL(socket_connect_state(int)),
		this,
		SLOT(socket_connect_state_Auto_equipment(int))
	);

	this->connect(PlcdataServer_sender,
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
void MainWindow::sjts_status(const int _sjts_status_int)
{
	const CMD_CTRL::SJTS_MACHINE_STATUS _sjts_status = (CMD_CTRL::SJTS_MACHINE_STATUS) _sjts_status_int;

	if (_sjts_status== CMD_CTRL::SJTS_MACHINE_STATUS::RoolerReady){
		
		beforeNotifiedClientSession(CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_START);
		NotifiedClientSession(CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_START);

	}else if (_sjts_status == CMD_CTRL::SJTS_MACHINE_STATUS::RollerDoneQualified ||
		_sjts_status == CMD_CTRL::SJTS_MACHINE_STATUS::RollerDoneUnqualified) {
		// 
		qDebug() << "roller qualified is done !";
	
	}else if (_sjts_status== CMD_CTRL::SJTS_MACHINE_STATUS::RollerDone) {
	
		NotifiedClientSession(CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_STOP);
	
	}else if (_sjts_status == CMD_CTRL::SJTS_MACHINE_STATUS::StepMotorStart01) {
		
		NotifiedClientSession(CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_START_01);
	
	}else if (_sjts_status == CMD_CTRL::SJTS_MACHINE_STATUS::StepMotorStop01) {
		
		NotifiedClientSession(CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_STOP_01);
	
	}else if (_sjts_status == CMD_CTRL::SJTS_MACHINE_STATUS::StepMotorStart00) {
		
		NotifiedClientSession(CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_START_00);
	
	}else if (_sjts_status == CMD_CTRL::SJTS_MACHINE_STATUS::StepMotorStop00) {
		
		NotifiedClientSession(CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_STOP_00);
	
	}else if (_sjts_status == CMD_CTRL::SJTS_MACHINE_STATUS::SerialPortError) {
		printf_event("SIGNAL","SerialPortError");
	}else if (_sjts_status == CMD_CTRL::SJTS_MACHINE_STATUS::SerialPortIsOpen) {
		printf_event("SIGNAL","SerialPortIsOpen");
	}else{
		Q_ASSERT(0);
	}

	this->ShowSjtsStatusOnUI(_sjts_status);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::printf_event(std::string _event, std::string _msg)
{
	std::cout << _event << ">>" << _msg << std::endl;
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
	
	mPlcdataServer = QSharedPointer<QtThreadPLC>(new QtThreadPLC(0));
	mFlowServerServer = QSharedPointer<QtThreadFlowCtrlServer>(new QtThreadFlowCtrlServer(this));
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
	
	this->update_work_flow_status_ex( mPlcdataServer.data(),mFlowServerServer.data());

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
void MainWindow::socket_connect_state_Auto_equipment(int _status)
{
	this->ui->checkBox_plc->setStyleSheet(mCheckBoxRunStatus[_status]);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/

void MainWindow::thread_running_state_Auto_equipment(int _status)
{
	this->ui->checkBox_plc->setChecked(_status);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void  MainWindow::tcp_server_work_flow_dones(int _status)
{
	if (_status) {
		printf_event("WORK FLOW", "all client thread done");
		this->mPlcdataServer->setWorkFlowDone(_status);
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
void MainWindow::NotifiedClientSession(CMD_CTRL::CMD_TYPE_LOCAL _type_c)
{
	mFlowServerServer->NotifiedClientSession(_type_c);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::tcp_server_running_client_sessions(int _running_sessions)
{
	mPlcdataServer->setClientSessionCount(_running_sessions);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::beforeNotifiedClientSession(CMD_CTRL::CMD_TYPE_LOCAL _type_c)
{

	mFlowServerServer->beforeNotifiedClientSession(_type_c);

}