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
	this->init_ping_ssh();
	this->ConnectVideo();

	this->init_menu();
	this->init_controls();
	
	
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
MainWindow::~MainWindow()
{

	this->destory_all();
	
	 delete ui;
   
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::init_class_member()
{

#if TRUE
	mQimageGray = QSharedPointer<QImage>(new QImage(1920,1080, QImage::Format_Grayscale8));
	mQimageGray->fill(125);
#endif

	mStepMotor = QSharedPointer<QtThreadStepMotor>(new QtThreadStepMotor());
	mCtrlServer = QSharedPointer<QtThreadClientCtrl>(new QtThreadClientCtrl());
	mVideoDataServer = QSharedPointer<QtThread8Video>(new QtThread8Video());

	mCtrlServer->SetDataPipe(mVideoDataServer);
	mStepMotor->SetCmdCtrlPipe(mCtrlServer);

	//////////////////////////////////////////////////////////////////
	mthread = QSharedPointer<QThread>(new QThread());
	mLink = QSharedPointer<QtLink>(new QtLink());
	mTimer = QSharedPointer<QTimer>(new QTimer());
	//////////////////////////////////////////////////////////////////
#if TRUE
	this->mShowCutArea = TRUE;
	this->mShowBinaryImg = FALSE;
#endif // TRUE

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::init_ping_ssh()
{
	
	mLink->moveToThread(mthread.data());

	connect(mLink.data(), SIGNAL(ping_status(int)), this, SLOT(CheckBox_ping(int)));
	connect(mLink.data(), SIGNAL(ssh_status(int)), this, SLOT(CheckBox_ssh(int)));
	connect(mTimer.data(), SIGNAL(timeout()), mLink.data(), SLOT(line_check()));
	
	
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::start_ping_ssh()
{

	if (!mTimer->isActive())
	{
		mTimer->start(1000);
	}
	
	if (!mthread->isRunning())
	{
		mthread->start();
	}
		
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::destory_ping_ssh()
{
	
	mLink->Stop();

	if (mTimer->isActive())
	{
		mTimer->stop();
	}
	else {
	
		if (mthread->isRunning())
		{			
			mthread->quit();
#if 0
			mLink.clear();
#endif // 0


		}

	}

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::init_menu()
{
#if TRUE
	connect(ui->actionSetCutRect, SIGNAL(triggered()), this, SLOT(SetCutRectMethod()));
	connect(ui->action_stop_bg_srv, SIGNAL(triggered()), this, SLOT(StopVideo()));
	connect(ui->action_stop_bg_srv_force, SIGNAL(triggered()), this, SLOT(StopVideoForce()));
	connect(ui->actionEnable_ping_SSH, SIGNAL(triggered()), this, SLOT(start_ping_ssh()));
	connect(ui->actionShow_Cut_Area, SIGNAL(triggered()), this, SLOT(toggleShowCutArea()));
	connect(ui->action_Show_Binary_Img, SIGNAL(triggered()), this, SLOT(toggleShowBinaryImg()));
#endif // TRUE
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::init_controls()
{

#if TRUE
	connect(ui->checkBox_ImgModeOrg, SIGNAL(stateChanged(int)), this, SLOT(CheckBox_img_mode_update()));

	connect(ui->checkBox_ImgModeSizeOrg, SIGNAL(stateChanged(int)), this, SLOT(CheckBox_img_mode_update()));

	connect(ui->horizontalSlider_sigma,SIGNAL(valueChanged(int)),ui->label_sigma, SLOT(setNum(int)));

	connect(ui->horizontalSlider_sigma, SIGNAL(valueChanged(int)), this, SLOT(Slider_img_sigma_change(int)));
	
#endif // TRUE

#if TRUE
	connect(ui->pushButton_preview, SIGNAL(clicked()), this, SLOT(StartVideoModeSelected()));
	connect(ui->pushButton_cutArea, SIGNAL(clicked()), this, SLOT(ClickButton_CutArea()));
#endif // TRUE


#if TRUE

	ui->comboBox_IpAddr->addItem(BORD_VIDEO_IN_LONG);
	ui->comboBox_IpAddr->addItem(BORD_VIDEO_IN_SHORT);
	ui->comboBox_IpAddr->addItem(BORD_VIDEO_OUT);

	ui->comboBox_IpAddr->setCurrentIndex(ui->comboBox_IpAddr->findText(BORD_VIDEO_OUT));

	connect(ui->comboBox_IpAddr, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(ComboBox_IpAddr_changed(const QString &)));

	this->ComboBox_IpAddr_changed(this->ui->comboBox_IpAddr->currentText());

#endif // TRUE


}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::disableInputCtrls(bool _flag)
{
	ui->horizontalLayout_ctrls->setEnabled(!_flag);
	ui->groupBox_ctrls->setDisabled(_flag);
	ui->comboBox_IpAddr->setDisabled(_flag);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int MainWindow::IsBgThreadRunning()
{

#if _DEBUG
	if (mCtrlServer->isRunning()) {
		qDebug() << "Ctrl Server : Running !";
	}
	else
	{
		qDebug() << "Ctrl Server : Close !";
	}
	if (mVideoDataServer->isRunning()) {
		qDebug() << "Video Data Server : Running !";
	}
	else
	{
		qDebug() << "Video Data : Close !";
	}
#endif // _DEBUG

	if  (mCtrlServer->isRunning() ||
		mVideoDataServer->isRunning())
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
	
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::destory_all()
{
	this->destory_ping_ssh();

	this->stopVideoBasic();

	QtThread8VideoProcess::stopTask();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::connect_img_ch(int _connect,const QObject *receiver)
{

	if (_connect){
			
				connect(mVideoDataServer.data(), SIGNAL(img_stat(int, int, int)),receiver, SLOT(img_stat_show(int, int, int)));
			
	}else {	
				disconnect(mVideoDataServer.data(), SIGNAL(img_stat(int, int, int)),receiver, SLOT(img_stat_show(int, int, int)));

	
	}

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::ShowImage(QLabel* _qlab,QImage *_p_qimg)
{
	
	QPixmap pixmap2(QPixmap::fromImage(*_p_qimg));

	_qlab->setPixmap(pixmap2.scaled(_qlab->size(), Qt::KeepAspectRatio));
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::ProcessImage(IplImage* img_t,int isShowCut,int isShowBinary)
{
#if TRUE
	if (isShowCut) {
		CvRect rect = cvGetImageROI(img_t);
		cvResetImageROI(img_t); {
			QtThread8VideoProcess::DrawArea(img_t, rect);
		}cvSetImageROI(img_t, rect);

	}
#endif // TRUE

#if TRUE
	if (isShowBinary) {

		CvRect rect = cvGetImageROI(img_t);
		cvResetImageROI(img_t); {
			float threshold = 0.5;
			float max_value = 255;
			int threshold_type = CV_THRESH_BINARY;
			cvThreshold(img_t, img_t, threshold, 255, threshold_type);//阀值100

		}cvSetImageROI(img_t, rect);

	}
#endif // TRUE
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
	if (ui != NULL) {
		if (_stat_t == 0)
			ui->checkBox_ping->setCheckState(Qt::CheckState::Unchecked);
		else
			ui->checkBox_ping->setCheckState(Qt::CheckState::Checked);
	}
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::CheckBox_ssh(int _stat_t)
{
	if (ui != NULL) {
		if (_stat_t == 0)
			ui->checkBox_ssh->setCheckState(Qt::CheckState::Unchecked);
		else
			ui->checkBox_ssh->setCheckState(Qt::CheckState::Checked);
	}
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::CheckBox_fpga_ctrl(int _stat_t)
{

	if (ui != NULL) {
			if (_stat_t == 0)
				ui->checkBox_ctrl_port->setCheckState(Qt::CheckState::Unchecked);
			else
				ui->checkBox_ctrl_port->setCheckState(Qt::CheckState::Checked);
	}

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::CheckBox_fpga_image_video(int _stat_t)
{
	if (ui != NULL) {
		if (_stat_t == 0)
			ui->checkBox_video_port->setCheckState(Qt::CheckState::Unchecked);
		else
			ui->checkBox_video_port->setCheckState(Qt::CheckState::Checked);
	}
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::CheckBox_img_mode_update()
{
	bool imgModeOrg = ui->checkBox_ImgModeOrg->isChecked();
	bool imgModeSizeOrg = ui->checkBox_ImgModeSizeOrg->isChecked();

	mWorkMode = 0;

	if (imgModeOrg)
	{
		mWorkMode |= CMD_CTRL::WorkMode::WM_ORG_IMG;
	}
	else
	{
		mWorkMode |= CMD_CTRL::WorkMode::WM_DIFF_IMG;
	}

	if (imgModeSizeOrg)
	{
		mWorkMode |= CMD_CTRL::WorkMode::WM_SIZE_FULL;
	}
	else
	{
		mWorkMode |= CMD_CTRL::WorkMode::WM_SIZE_CUT;
	}

	mCtrlServer->SetWorkModeCmd((CMD_CTRL::WorkMode)mWorkMode);

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::ComboBox_IpAddr_changed(const QString& _str)
{

	statusBarshowMessage(_str);

	this->SetFpgaArmLinuxIpAddr(_str);

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::Slider_img_sigma_change(int _sigma)
{
	mCtrlServer->SetImgSigmaCmd(_sigma);
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
void MainWindow::SetCutRectMethod()
{
	QtThread8VideoProcess::SetRectCmds();
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
void MainWindow::ClickButton_MotorRun()
{

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::closeEvent(QCloseEvent * event)
{
	this->destory_all();

	if ((mTimer->isActive()==false)&&
		(mthread->isRunning()==false)&&
		(mCtrlServer->isRunning() == false)&&
		(mVideoDataServer->isRunning() == false))
	{		

		event->accept();
	}
	else
	{
		event->ignore();
	}

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::initUI()
{
	ui->setupUi(this);
	
	this->initUIlabelImage();

	this->SetMaxWidthMy();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::initUIlabelImage()
{
	QRect rec = QApplication::desktop()->screenGeometry();
	int SCREEM_H = rec.height();
	int SCREEN_W = rec.width();
	
	const int width_max = 1920;
	const int height_max = 1080;

	int lab_img_width = 1.0*  width_max / 4;
	int lab_img_height = 1.0* lab_img_width*(1.0*height_max / width_max);

	labelImage[0] = ui->label_img_ch0;
	labelImage[1] = ui->label_img_ch1;
	labelImage[2] = ui->label_img_ch2;
	labelImage[3] = ui->label_img_ch3;

	labelImage[4] = ui->label_img_ch4;
	labelImage[5] = ui->label_img_ch5;
	labelImage[6] = ui->label_img_ch6;
	labelImage[7] = ui->label_img_ch7;

	const int chs = sizeof(labelImage) / sizeof(void*);

	for (size_t chi = 0; chi < chs; chi++){
		//labelImage[chi]->setScaledContents(true);
		connect(labelImage[chi], SIGNAL(Mouse_DoubleClick(QLabel*)), this, SLOT(openImageShowQDialog(QLabel*)));
		labelImage[chi]->setMinimumHeight(lab_img_height);
#if TRUE
		((MouseLabel*)labelImage[chi])->mChannel = chi;
#endif // TRUE


	}


}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::initUIlabelImageView()
{

	const int chs = sizeof(labelImage) / sizeof(void*);

	for (size_t chi = 0; chi < chs; chi++) {		
			
			ShowImage(labelImage[chi], mQimageGray.data());

	}


}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int MainWindow::openImageShowQDialog(QLabel* _qabel)
{
	int result_t = 0;
	const int Channel = ((MouseLabel*)_qabel)->mChannel;
	connect_img_ch(FALSE, this);
	{
			QSharedPointer<Dialog> dialog = QSharedPointer<Dialog>( new Dialog(this));
			connect_img_ch(TRUE, dialog.data());

				{
					//dialog->setAttribute(Qt::WA_DeleteOnClose);
					dialog->SetChannel(Channel);
					dialog->SetShowCutArea(this->mShowCutArea);
					dialog->SetShowBinary(this->mShowBinaryImg);
					dialog->setModal(true);
					dialog->showFullScreen();
					dialog->show();
					result_t = dialog->exec();
				}
				
			connect_img_ch(FALSE, dialog.data());

	
	}
	connect_img_ch(TRUE, this);

	return result_t;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::toggleShowCutArea()
{

	if (ui->actionShow_Cut_Area->isChecked()) {
		this->mShowCutArea = TRUE;
	}else {
		this->mShowCutArea = FALSE;
	}

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::toggleShowBinaryImg()
{
	if (ui->action_Show_Binary_Img->isChecked()) {
		this->mShowBinaryImg = TRUE;
	}
	else {
		this->mShowBinaryImg = FALSE;
	}

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
void MainWindow::StartVideoBasic(int mode)
{
	mCtrlServer->SetWorkMode(CMD_CTRL::WorkMode(mode));
	mCtrlServer->startServer();

#if 1
	mVideoDataServer->startServer();
#endif // 0

	mStepMotor->startServer();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::StartVideoModeCutArea()
{

	const int mode = CMD_CTRL::WorkMode(CMD_CTRL::WorkMode::WM_ORG_IMG | CMD_CTRL::WorkMode::WM_SIZE_FULL);

	StartVideoBasic(mode);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/

/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::ClickButton_CutArea()
{
	

	if (!this->IsBgThreadRunning())
	{
			this->disableInputCtrls();
			this->initUIlabelImageView();
			this->StartVideoModeCutArea();
	}
	else
	{
		QMessageBox::critical(NULL, QStringLiteral("警告"), QStringLiteral("后台服务尚未停止"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
	}
	
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::StartVideoModeSelected()
{
	

	if (!this->IsBgThreadRunning())
	{
			this->CheckBox_img_mode_update();
			this->initUIlabelImageView();
			this->disableInputCtrls();
			this->StartVideoBasic(mWorkMode);
	}
	else
	{
		QMessageBox::critical(NULL, QStringLiteral("警告"), QStringLiteral("后台服务尚未停止"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
	}

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::StopVideo()
{
	
	this->stopVideoBasic();

	disableInputCtrls(false);

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::StopVideoForce()
{
	this->stopVideoBasicForce();

	disableInputCtrls(false);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::stopVideoBasic()
{
	if (mCtrlServer->isRunning()) {	
		mCtrlServer->closeServer();	
	}

	if (mVideoDataServer->isRunning()){
		mVideoDataServer->closeServer();
	}
	
	if (mStepMotor->isRunning()) {
		mStepMotor->closeServer();
	}
	
	this->IsBgThreadRunning();



}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::stopVideoBasicForce()
{
	this->stopVideoBasic();

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
	connect_img_ch(TRUE, this);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::img_stat_show(int _p_stat, int _channel, int _frames)
{
	if ((_p_stat >>8)== CMD_CTRL::CMD_TYPE_02_I::CT_IMG_FRAME){

		

		QSharedPointer<exCircleData> circleData = ChannelsData::channelsData()->getChannelData(_channel);
		
		while (circleData->QueueSize()){

				cmd_ctrl_image [_channel]=circleData->getImg();
				
				IplImage* img_t = cmd_ctrl_image[_channel]->getIplimage();

				ProcessImage(img_t, this->mShowCutArea, this->mShowBinaryImg);


				QSharedPointer<QImage> qimg = cmd_ctrl_image[_channel]->getQimage();
		
				ShowImage(labelImage[_channel], qimg.data());

		}		
		

	}else if ((_p_stat >> 8) == CMD_CTRL::CMD_TYPE_02_C::CT_START) {



	}else if((_p_stat >> 8) == CMD_CTRL::CMD_TYPE_02_C::CT_STOP) {

	}else {

	}
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
QImage *MainWindow::IplImageToQImage( IplImage* const img)
{
	QImage *image;
	cvCvtColor(img, img, CV_BGR2RGB);
	uchar *imgData = (uchar *)img->imageData;
	image = new QImage(imgData, img->width, img->height, QImage::Format_RGB888);
	return image;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::SetMaxWidthMy()
{
	QRect rec = QApplication::desktop()->screenGeometry();
	int SCREEM_H = rec.height();
	int SCREEN_W = rec.width();

	this->setFixedWidth(SCREEN_W);

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::statusBarshowMessage(QString _msg)
{
	ui->statusBar->showMessage(_msg);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void  MainWindow::SetFpgaArmLinuxIpAddr(QString _str)
{
	mFpgaArmLinuxIpAddr = _str;
	mCtrlServer->SetIpAddr(mFpgaArmLinuxIpAddr);
	mVideoDataServer->SetIpAddr(mFpgaArmLinuxIpAddr);
	mStepMotor->SetBordIPaddr(mFpgaArmLinuxIpAddr);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/