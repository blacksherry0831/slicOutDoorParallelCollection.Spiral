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
	mPlcdataServer->closeServer();
	mFlowServerServer->closeServer();
	
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
		SIGNAL(status_sjts(QtThreadPLC::SJTS_MACHINE_STATUS)),
		this,
		SLOT(sjts_status(QtThreadPLC::SJTS_MACHINE_STATUS))
		);
	

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::sjts_status(QtThreadPLC::SJTS_MACHINE_STATUS _sjts_status)
{

	if (_sjts_status==QtThreadPLC::RoolerReady){
		
		mFlowServerServer->NotifiedClientSession(CMD_CTRL::CMD_TYPE_02_C::CT_START);


	}else if (_sjts_status == QtThreadPLC::RollerDoneQualified || _sjts_status == QtThreadPLC::RollerDoneUnqualified) {
		
		// 
		qDebug() << "roller qualified is done !";
	
	}else if (_sjts_status==QtThreadPLC::RollerDone) {
	
		mFlowServerServer->NotifiedClientSession(CMD_CTRL::CMD_TYPE_02_C::CT_STOP);
	
	}else if (_sjts_status == QtThreadPLC::StepMotorStart01) {
		
		mFlowServerServer->NotifiedClientSession(CMD_CTRL::CMD_TYPE_02_C::CT_START_01);
	
	}else if (_sjts_status == QtThreadPLC::StepMotorStop01) {
		
		mFlowServerServer->NotifiedClientSession(CMD_CTRL::CMD_TYPE_02_C::CT_STOP_01);
	
	}else if (_sjts_status == QtThreadPLC::StepMotorStart00) {
		
		mFlowServerServer->NotifiedClientSession(CMD_CTRL::CMD_TYPE_02_C::CT_START_00);
	
	}else if (_sjts_status == QtThreadPLC::StepMotorStop00) {
		
		mFlowServerServer->NotifiedClientSession(CMD_CTRL::CMD_TYPE_02_C::CT_STOP_00);
	
	}else{
		Q_ASSERT(0);
	}

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/