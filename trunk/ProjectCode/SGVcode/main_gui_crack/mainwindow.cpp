#include "mainwindow.h"
#include "ui_mainwindow.h"

#ifdef QT_VERSION
#include <QMessageBox>
#include <QSerialPort>  
#include <QSerialPortInfo>
#include <QThread>
#endif // QT_VERSION



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	this->initUI();	

	m_DoWorkThread = new DoWorkThread(this);

	connect(m_DoWorkThread, SIGNAL(WorkProgress(QString)), this, SLOT(WorkProgressShow(QString)), Qt::QueuedConnection);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ClickButton_Test()
{

	QString test= QStringLiteral("test");

	boolean nvr_result = m_DoWorkThread->m_hw_nvr_ptr->hw_login();

	if (nvr_result) {

		QString msg_str = QStringLiteral("已登录NVR 设备！"); ;
		QMessageBox::information (this, test, msg_str);
	

	}else {
		QString msg_str = QStringLiteral("无法登录，NVR 设备！"); ;
		QMessageBox::warning(this,test,msg_str);

	}




}

void MainWindow::WhenCfgDone(boolean enable)
{
	ui->comboBox_SerialPort->setEnabled(enable);
	ui->checkBox_RunDirect->setEnabled(enable);
	ui->lineEdit_Speed->setEnabled(enable);
	if (QString::compare(ui->pushButton_SerialPort->text(), QStringLiteral("关闭")) == 0) {
		ui->pushButton_SerialPort->setText(QStringLiteral("打开"));
	}
	else if (QString::compare(ui->pushButton_SerialPort->text(), QStringLiteral("打开")) == 0) {
		ui->pushButton_SerialPort->setText(QStringLiteral("关闭"));
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
	this->ui->pushButton_CameraStart->setText(QStringLiteral("正在运行..."));
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
	this->ui->pushButton_CameraStart->setText(QStringLiteral("启动"));
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::WorkProgressShow(QString str)
{
	
	if (QString::compare(str, "success")==0) {
		QMessageBox::information(this, "", QStringLiteral("采集完成"));
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
	m_DoWorkThread->m_motor_port_name=ui->comboBox_SerialPort->currentText();
	m_DoWorkThread->m_motor_run_direct = ui->checkBox_RunDirect->isChecked();
	bool convert_ok;
	int convert_value=ui->lineEdit_Speed->text().toInt(&convert_ok);
	if (convert_ok) {
		m_DoWorkThread->m_motor_speed = convert_value;
	}
	bool combo_enable = ui->comboBox_SerialPort->isEnabled();
	if (combo_enable == true) {
#if TRUE
			QSerialPort serialport(m_DoWorkThread->m_motor_port_name);

			if (serialport.open(QIODevice::ReadWrite)) {
				serialport.close();
			}else{
				QMessageBox::warning(this, m_DoWorkThread->m_motor_port_name, QStringLiteral("串口不能被打开"));
				return;
			}
#endif // TRUE
			if (m_DoWorkThread->m_Step_Motor_ptr->open(m_DoWorkThread->m_motor_port_name.toStdString())==false) {
				QMessageBox::warning(this, m_DoWorkThread->m_motor_port_name, QStringLiteral("串口不能被打开"));
			}

	}else {
		m_DoWorkThread->m_Step_Motor_ptr->close();

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
	m_DoWorkThread->start();

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

			string port_name_t = m_DoWorkThread->m_motor_port_name.toStdString();
			m_DoWorkThread->m_Step_Motor_ptr->open(port_name_t);
	
	
			m_DoWorkThread->m_Step_Motor_ptr->SendQueryCmd(0);

			if (m_DoWorkThread->m_Step_Motor_ptr->Wait4CmdDone()) {	
				m_DoWorkThread->m_Step_Motor_ptr->SendCmd(BE_1105_RUN_NEG,65000,10);
				if (m_DoWorkThread->m_Step_Motor_ptr->Wait4CmdDone()) {
					QMessageBox::information(this, m_DoWorkThread->m_motor_port_name, QStringLiteral("步进电机：测试完毕"));
				}
				else {
					QMessageBox::warning(this, m_DoWorkThread->m_motor_port_name, QStringLiteral("步进电机：无法执行"));
				}

			}
			else {
		
				QMessageBox::warning(this, m_DoWorkThread->m_motor_port_name, QStringLiteral("步进电机：无法连接 "));
	
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
