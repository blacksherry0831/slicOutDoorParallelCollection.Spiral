#include "mainwindow.h"
#include "ui_mainwindow.h"

#ifdef QT_VERSION
#include <QMessagebox>
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
	m_Step_Motor_ptr = BE_1105_Driver::getInstance();
	m_hw_nvr_ptr.reset(new hw_nvr("192.168.9.200", 0));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ClickButton_Test()
{

	QString test= QStringLiteral("test");

	boolean nvr_result = m_hw_nvr_ptr->hw_login();

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

void MainWindow::ClickButton_SetSerialPort()
{
	m_motor_port_name=ui->comboBox_SerialPort->currentText();
	m_motor_run_direct = ui->checkBox_RunDirect->isChecked();

	bool combo_enable = ui->comboBox_SerialPort->isEnabled();
	if (combo_enable == true) {
#if TRUE
			QSerialPort serialport(m_motor_port_name);

			if (serialport.open(QIODevice::ReadWrite)) {
				serialport.close();
			}else{
				QMessageBox::warning(this, m_motor_port_name, QStringLiteral("串口不能被打开"));
				return;
			}
#endif // TRUE
			if (this->m_Step_Motor_ptr->open(m_motor_port_name.toStdString())==false) {
				QMessageBox::warning(this, m_motor_port_name, QStringLiteral("串口不能被打开"));
			}

	}else {
		this->m_Step_Motor_ptr->close();

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
	QString TITLE="START";
#if TRUE
	
	if (this->m_Step_Motor_ptr->open(m_motor_port_name.toStdString())){

	}

	//一切就绪
	if (this->m_Step_Motor_ptr->Wait4CmdDone()==false){
		QMessageBox::warning(this, TITLE, QStringLiteral("步进电机：等待步进电机就绪"));
		return;
	}

	if (m_hw_nvr_ptr->hw_login()==false) {
		QMessageBox::warning(this, TITLE, QStringLiteral("NVR设备：无法登录"));
		return;
	}

	if (m_hw_nvr_ptr->Check7Link4Start() == false) {
		QMessageBox::warning(this, TITLE, QStringLiteral("NVR设备：0-7通道存在连接故障"));
		return;
	}

	if (m_hw_nvr_ptr->Wait4StopAllRecord() == false) {
		QMessageBox::warning(this, TITLE, QStringLiteral("NVR设备：停止录像异常"));
		return;
	}

	if (m_hw_nvr_ptr->Check7Status4Start()==false) {
		QMessageBox::warning(this, TITLE, QStringLiteral("NVR设备：通道存在开启"));
		return;
	}
#endif

#if TRUE
	//启动摄像
	if (m_hw_nvr_ptr->Wait4StartAllRecord() == false) {
		QMessageBox::warning(this, TITLE, QStringLiteral("NVR设备：通道不能全部开启"));
		return;
	}
	//启动步进电机
	this->m_Step_Motor_ptr->SendCmd(BE_1105_RUN_POS);
	if (this->m_Step_Motor_ptr->Wait4CmdDone()==false) {
		QMessageBox::warning(this, m_motor_port_name, QStringLiteral("步进电机：无法执行"));
	}
	//关闭录像
	if (m_hw_nvr_ptr->Wait4StopAllRecord() == false) {
		QMessageBox::warning(this, TITLE, QStringLiteral("NVR设备：停止录像异常"));
		return;
	}
#endif

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::ClickButton_MotorRun()
{
	this->ui->pushButton_MotorRun->setEnabled(false);{

			string port_name_t = m_motor_port_name.toStdString();
			this->m_Step_Motor_ptr->open(port_name_t);
	
	
			this->m_Step_Motor_ptr->SendQueryCmd(0);

			if (this->m_Step_Motor_ptr->Wait4CmdDone()) {	
				this->m_Step_Motor_ptr->SendCmd(BE_1105_RUN_POS);
				if (this->m_Step_Motor_ptr->Wait4CmdDone()) {
					QMessageBox::information(this, m_motor_port_name, QStringLiteral("步进电机：测试完毕"));
				}
				else {
					QMessageBox::warning(this, m_motor_port_name, QStringLiteral("步进电机：无法执行"));
				}

			}
			else {
		
				QMessageBox::warning(this, m_motor_port_name, QStringLiteral("步进电机：无法连接 "));
	
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
