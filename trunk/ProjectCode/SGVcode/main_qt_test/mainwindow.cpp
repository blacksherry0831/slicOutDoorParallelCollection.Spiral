#include "mainwindow.h"
#include "ui_mainwindow.h"

#ifdef QT_VERSION
#include <QSerialPort>  
#include <QSerialPortInfo>
#include <QThread>
#include <QMessageBox>
#endif // QT_VERSION



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
  
	this->initUI();	
	
	this->init_class_member();


	connect(mLink.data(), SIGNAL(ping_status(int)), this, SLOT(CheckBox_ping(int)));
	connect(mLink.data(), SIGNAL(ssh_status(int)), this, SLOT(CheckBox_ssh(int)));
	connect(mTimer.data(), SIGNAL(timeout()), mLink.data(), SLOT(line_check()));
	
	this->ConnectVideo();
	
	mthread->start();
	mTimer->start(1*1000);	

	
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
MainWindow::~MainWindow()
{

	this->StopVideo();
	
    delete ui;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::init_class_member()
{
	mCtrlServer = QSharedPointer<QtThreadClientCtrl>(new QtThreadClientCtrl());
	mVideoDataServer = QSharedPointer<QtThread8Video>(new QtThread8Video());

	mthread = QSharedPointer<QThread>(new QThread());
	mLink = QSharedPointer<QtLink>(new QtLink());
	mTimer = QSharedPointer<QTimer>(new QTimer());

	mLink->moveToThread(mthread.data());
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::ClickButton_Test()
{

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::WorkStart()
{

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::WorkDone()
{
	
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::WorkProgressShow(QString str)
{
	


}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::CheckBox_ping(int _stat_t)
{
	if(_stat_t==0)
		ui->checkBox_ping->setCheckState(Qt::CheckState::Unchecked);
	else
		ui->checkBox_ping->setCheckState(Qt::CheckState::Checked);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::CheckBox_ssh(int _stat_t)
{
	if (_stat_t == 0)
		ui->checkBox_ssh->setCheckState(Qt::CheckState::Unchecked);
	else
		ui->checkBox_ssh->setCheckState(Qt::CheckState::Checked);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::CheckBox_fpga_ctrl(int _stat_t)
{
	if (_stat_t == 0)
		ui->checkBox_ctrl_port->setCheckState(Qt::CheckState::Unchecked);
	else
		ui->checkBox_ctrl_port->setCheckState(Qt::CheckState::Checked);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::CheckBox_fpga_image_video(int _stat_t)
{
	if (_stat_t == 0)
		ui->checkBox_video_port->setCheckState(Qt::CheckState::Unchecked);
	else
		ui->checkBox_video_port->setCheckState(Qt::CheckState::Checked);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::main_test()
{
	ui->checkBox_ping->setChecked(1);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::ClickButton_SetSerialPort()
{

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::ClickButton_CameraStart()
{
#if  0
	this->WorkStart();
#else

	this->WorkStart();
	

#endif //  0

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::ClickButton_CutArea()
{
	
	this->StartVideo();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::ClickButton_MotorRun()
{

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::initUI()
{
	ui->setupUi(this);

	connect(ui->pushButton_cutArea, SIGNAL(clicked()), this, SLOT(ClickButton_CutArea()));
	
}

/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::message_test()
{
	QMessageBox::information(NULL, "TestTitle", "test", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::StartVideo()
{
	mCtrlServer->SetWorkMode(CMD_CTRL::WorkMode::CUT_AREA);
	mCtrlServer->start();
	mVideoDataServer->start();
	QtThread8VideoProcess::startTask();

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::StopVideo()
{
	mCtrlServer->closeServer();
	mVideoDataServer->closeServer();
	QtThread8VideoProcess::stopTask();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::ConnectVideo()
{
	connect(mCtrlServer.data(), SIGNAL(socket_connect_state(int)), this, SLOT(CheckBox_fpga_ctrl(int)));
	connect(mVideoDataServer.data(), SIGNAL(socket_connect_state(int)), this, SLOT(CheckBox_fpga_image_video(int)));
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
