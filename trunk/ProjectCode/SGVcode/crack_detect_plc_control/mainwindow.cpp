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
	mPlcdataServer = QSharedPointer<QtThreadPLC>(new QtThreadPLC(0));
	mFlowServerServer = QSharedPointer<QtThreadFlowCtrlServer>(new QtThreadFlowCtrlServer(this));
	
	this->init_connect();

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
void MainWindow::init_connect()
{
	this->connect(mPlcdataServer.data(), 
		SIGNAL(status_sjts(int)),
		this,
		SLOT(sjts_status(int))
		);
	

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
		
		mFlowServerServer->NotifiedClientSession(CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_START);

	}else if (_sjts_status == CMD_CTRL::SJTS_MACHINE_STATUS::RollerDoneQualified ||
		_sjts_status == CMD_CTRL::SJTS_MACHINE_STATUS::RollerDoneUnqualified) {
		// 
		qDebug() << "roller qualified is done !";
	
	}else if (_sjts_status== CMD_CTRL::SJTS_MACHINE_STATUS::RollerDone) {
	
		mFlowServerServer->NotifiedClientSession(CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_STOP);
	
	}else if (_sjts_status == CMD_CTRL::SJTS_MACHINE_STATUS::StepMotorStart01) {
		
		mFlowServerServer->NotifiedClientSession(CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_START_01);
	
	}else if (_sjts_status == CMD_CTRL::SJTS_MACHINE_STATUS::StepMotorStop01) {
		
		mFlowServerServer->NotifiedClientSession(CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_STOP_01);
	
	}else if (_sjts_status == CMD_CTRL::SJTS_MACHINE_STATUS::StepMotorStart00) {
		
		mFlowServerServer->NotifiedClientSession(CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_START_00);
	
	}else if (_sjts_status == CMD_CTRL::SJTS_MACHINE_STATUS::StepMotorStop00) {
		
		mFlowServerServer->NotifiedClientSession(CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_STOP_00);
	
	}else if (_sjts_status == CMD_CTRL::SJTS_MACHINE_STATUS::SerialPortError) {
		printf_event("SIGNAL","SerialPortError");
	}else if (_sjts_status == CMD_CTRL::SJTS_MACHINE_STATUS::SerialPortIsOpen) {
		printf_event("SIGNAL","SerialPortIsOpen");
	}else{
		Q_ASSERT(0);
	}

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