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

	}else if (_sjts_status == QtThreadPLC::RollerDoneQualified || _sjts_status == QtThreadPLC::RollerDoneUnqualified) {
	
	}else if (_sjts_status == QtThreadPLC::StepMotorStart01) {

	}else if (_sjts_status == QtThreadPLC::StepMotorStop01) {

	}else if (_sjts_status == QtThreadPLC::StepMotorStart00) {

	}else if (_sjts_status == QtThreadPLC::StepMotorStop00) {

	}else{
		Q_ASSERT(0);
	}

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/