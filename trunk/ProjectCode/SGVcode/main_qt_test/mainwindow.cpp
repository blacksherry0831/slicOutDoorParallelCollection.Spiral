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
    ui->setupUi(this);
	this->initUI();	

	

	

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ClickButton_Test()
{

	QString test= QStringLiteral("test");

	int nvr_result = true;

	if (nvr_result) {

		QString msg_str = QStringLiteral("1"); ;
		QMessageBox::information (this, test, msg_str);
	

	}else {
		QString msg_str = QStringLiteral("2"); ;
		QMessageBox::warning(this,test,msg_str);

	}




}

void MainWindow::WhenCfgDone(int enable)
{
	ui->comboBox_SerialPort->setEnabled(enable);
	ui->checkBox_RunDirect->setEnabled(enable);
	ui->lineEdit_Speed->setEnabled(enable);
	if (QString::compare(ui->pushButton_SerialPort->text(), QStringLiteral("3")) == 0) {
		ui->pushButton_SerialPort->setText(QStringLiteral("4"));
	}
	else if (QString::compare(ui->pushButton_SerialPort->text(), QStringLiteral("5")) == 0) {
		ui->pushButton_SerialPort->setText(QStringLiteral("6"));
	}
	else {

	}

	this->ui->groupBox_test->setEnabled(!enable);
	this->ui->groupBox_work->setEnabled(!enable);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::WorkStart()
{
	ui->pushButton_SerialPort->setEnabled(false);
	ui->groupBox_test->setEnabled(false);
	this->ui->pushButton_CameraStart->setEnabled(false);
	this->ui->pushButton_CameraStart->setText(QStringLiteral("7"));
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::WorkDone()
{
	ui->pushButton_SerialPort->setEnabled(true);
	ui->groupBox_test->setEnabled(true);
	this->ui->pushButton_CameraStart->setEnabled(true);
	this->ui->pushButton_CameraStart->setText(QStringLiteral("8"));
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::WorkProgressShow(QString str)
{
	
	if (QString::compare(str, "success")==0) {
		QMessageBox::information(this, "", QStringLiteral("9"));
		this->WorkDone();
	}else {
		QMessageBox::warning(this, "error", str);
		this->WorkDone();
	}


}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::ClickButton_SetSerialPort()
{

	bool convert_ok;
	int convert_value=ui->lineEdit_Speed->text().toInt(&convert_ok);
	if (convert_ok) {
		
	}
	bool combo_enable = ui->comboBox_SerialPort->isEnabled();
	if (combo_enable == true) {
#if TRUE
			QSerialPort serialport("com4");

			if (serialport.open(QIODevice::ReadWrite)) {
				serialport.close();
			}else{
				
				return;
			}
#endif // TRUE
		

	}else {
	

	}
CHANGE_STATE_SERIALPORT:
	{
		this->WhenCfgDone(!combo_enable);
	}
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
void MainWindow::ClickButton_MotorRun()
{
	this->ui->pushButton_MotorRun->setEnabled(false);{

		
			 {
		
				
	
			}

	}this->ui->pushButton_MotorRun->setEnabled(true);
}

/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::initUI()
{
#if _DEBUG
	this->TestSerialPort();
#endif
	foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
		if (info.isValid()) {
			ui->comboBox_SerialPort->addItem(info.portName());	
		}
	}

	ui->lineEdit_Speed->setValidator(new QRegExpValidator(QRegExp("[0-9]+$")));
	/*--------*/
	ui->checkBox_RunDirect->setChecked(true);
	/*--------*/
	connect(ui->pushButton_SerialPort, SIGNAL(clicked()), this, SLOT(ClickButton_SetSerialPort()));
	/*--------*/
	connect(ui->pushButton_CameraStart, SIGNAL(clicked()), this, SLOT(ClickButton_CameraStart()));
	/*--------*/
	connect(ui->pushButton_MotorRun, SIGNAL(clicked()), this, SLOT(ClickButton_MotorRun()));
	/*--------*/
	connect(ui->pushButton_test, SIGNAL(clicked()), this, SLOT(ClickButton_Test()));
	/*--------*/
}

void MainWindow::TestSerialPort()
{
	foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
		QString s = QObject::tr("Port: ") + info.portName() + "\n"
			+ QObject::tr("Location: ") + info.systemLocation() + "\n"
			+ QObject::tr("Description: ") + info.description() + "\n"
			+ QObject::tr("Manufacturer: ") + info.manufacturer() + "\n"
			+ QObject::tr("Serial number: ") + info.serialNumber() + "\n"
			+ QObject::tr("Vendor Identifier: ") + (info.hasVendorIdentifier() ? QString::number(info.vendorIdentifier(), 16) : QString()) + "\n"
			+ QObject::tr("Product Identifier: ") + (info.hasProductIdentifier() ? QString::number(info.productIdentifier(), 16) : QString()) + "\n"
			+ QObject::tr("Busy: ") + (info.isBusy() ? QObject::tr("Yes") : QObject::tr("No")) + "\n";

	}
}
