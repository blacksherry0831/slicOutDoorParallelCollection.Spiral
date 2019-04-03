#include "mainwindow.h"
#include "ui_mainwindow.h"

#ifdef QT_VERSION
#include <QSerialPort>  
#include <QSerialPortInfo>
#include <QThread>
#include <QMessageBox>
#endif // QT_VERSION
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
 QString MainWindow::gRemoteBord[] = {	BORD_VIDEO_IN_LONG,
										BORD_VIDEO_IN_SHORT,
										BORD_VIDEO_OUT,
										BORD_VIDEO_SINGLE};
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
  
	this->initUI();	
	
	this->init_class_member();
	this->init_ping_ssh();
	
	this->init_menu();
	this->init_controls();
	this->init_controls_by_cfg();

	this->ConnectVideo();
	
	this->initGlobal();
		
	this->StartVideoModeSelected();
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
	this->init_class_member_base();

	this->init_class_member_ptr();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::init_class_member_ptr()
{
	mAppSetting = QSharedPointer<QSettings>(new QSettings(tr("Nanjing Yjkj"),tr("Visual inspection")));
#if TRUE
	mQimageGray = QSharedPointer<QImage>(new QImage(1920, 1080, QImage::Format_Grayscale8));
	mQimageGray->fill(125);
#endif

	mFlowCtrlLocal = QSharedPointer<QtThreadStepMotor>(new QtThreadStepMotor());

	mCtrlServer = QSharedPointer<QtThreadClientCtrl>(new QtThreadClientCtrl());
	mVideoDataServer = QSharedPointer<QtThread8Video>(new QtThread8Video());

	mImg8Process = QSharedPointer<QtThread8ImgProcess>(new QtThread8ImgProcess());
	mImg8Process->startTask();

	mCtrlServer->SetDataPipe(mVideoDataServer);

	mFlowCtrlClient = QSharedPointer<QtThreadFlowCtrlClient>(new QtThreadFlowCtrlClient());

#if FLOW_CTRL_USE_LOCAL_SERVER 
	mFlowServerServerLocal = QSharedPointer<QtThreadFlowCtrlServer>(new QtThreadFlowCtrlServer(this));
#endif
	
	//////////////////////////////////////////////////////////////////
	mthread = QSharedPointer<QThread>(new QThread());
	mLink = QSharedPointer<QtLink>(new QtLink());
	mTimer = QSharedPointer<QTimer>(new QTimer());
	//////////////////////////////////////////////////////////////////

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::init_class_member_base()
{
#if TRUE
	memset(&mImgProc, 0, sizeof(IMG_PROC));
#endif // TRUE

	mPaletteStatus[0].setBrush(QPalette::Base,Qt::red);
	mPaletteStatus[1].setBrush(QPalette::Base, Qt::green);
	
	mCheckBoxRunStatus[0]= "QCheckBox{color:rgb(255,0,0)}";
	mCheckBoxRunStatus[1]= "QCheckBox{color:rgb(0,255,0)}";

//	mFlowCtrlTimer = new QTimer(this);

	mAppKeyXilinxFpgaArm = "KeytXilinxFpgaArm";
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
void MainWindow::img_stat_show_ex(int _p_stat, int _channel, int _frames, void* _data)
{

	QSharedPointer<exCircleData> circleData = ChannelsData4Show::getInstance()->getChannelData(_channel);
	
	
	if (circleData->QueueSize()) {		
		cmd_ctrl_image[_channel] = circleData->getImg();
	}
	
	if ((_p_stat >> 8) == CMD_CTRL::CMD_TYPE_02_I::CT_IMG_FRAME) {

		if (!cmd_ctrl_image[_channel].isNull()) {
			
			QSharedPointer<QImage> qimg = cmd_ctrl_image[_channel]->getQimage();
			DrawUnqualified(cmd_ctrl_image[_channel]);
			ShowImage(labelImage[_channel], qimg.data());

		}

	}
	else if ((_p_stat >> 8) == CMD_CTRL::CMD_TYPE_02_C::CT_START) {

		ShowImageChGray(_channel);

	}
	else if ((_p_stat >> 8) == CMD_CTRL::CMD_TYPE_02_C::CT_STOP) {


	}
	else {

	}
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
std::string MainWindow::GetInOut()
{
	const QString ipaddr_qstr_t= this->ui->comboBox_IpAddr->currentText();
	
	return GetIpAddrProperty(ipaddr_qstr_t);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
std::string MainWindow::GetIpAddrProperty(QString _ipAddr)
{
	
	std::string property_t;

	if (QString::compare(_ipAddr, BORD_VIDEO_IN_LONG) == 0) {
		property_t = CrackDetection::CRACK_IN;
	}
	else if (QString::compare(_ipAddr, BORD_VIDEO_IN_SHORT) == 0) {
		property_t = CrackDetection::CRACK_IN;
	}
	else if (QString::compare(_ipAddr, BORD_VIDEO_OUT) == 0) {
		property_t = CrackDetection::CRACK_OUT;
	}
	else if (QString::compare(_ipAddr, BORD_VIDEO_SINGLE) == 0) {
		property_t = "single";
	}
	else {
		Q_ASSERT(0);
	}

	return property_t;
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
	connect(ui->action_stop_bg_srv, SIGNAL(triggered()), this, SLOT(StopVideo()));
	connect(ui->action_stop_bg_srv_force, SIGNAL(triggered()), this, SLOT(StopVideoForce()));
	connect(ui->action_block_in_step02, SIGNAL(triggered(bool)), this, SLOT(toggleBlockStep02(bool)));
#endif
#if TRUE
	connect(ui->actionSetCutRect, SIGNAL(triggered()), this, SLOT(SetCutRectMethod()));
	connect(ui->actionEnable_ping_SSH, SIGNAL(triggered()), this, SLOT(start_ping_ssh()));
#endif // TRUE

#if TRUE
	connect(ui->actionShow_Cut_Area, SIGNAL(triggered()), this, SLOT(toggleShowCutArea()));

	connect(ui->action_ImgProc_Binary, SIGNAL(triggered(bool)), this, SLOT(toggleImgProcBinary(bool)));
	connect(ui->action_ImgProc_Denoise, SIGNAL(triggered(bool)), this, SLOT(toggleImgProcDenoise(bool)));

	connect(ui->action_Show_Classify_Thickly, SIGNAL(triggered(bool)), this, SLOT(toggleShowClassifyThickly(bool)));
	connect(ui->action_img_collect, SIGNAL(triggered()), this, SLOT(toggleImgCollect()));

	connect(ui->actionHoughBlock_threshold, SIGNAL(triggered(bool)), this, SLOT(toggleImageClassifyHoughBlock(bool)));
#endif // TRUE

#if TRUE
	connect(ui->action_workflow_local, SIGNAL(triggered()), this, SLOT(workflow_local()));
	connect(ui->action_workflow_remote, SIGNAL(triggered()), this, SLOT(workflow_remote()));
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

	connect(ui->checkBox_ImgModeOrg, SIGNAL(stateChanged(int)), this, SLOT(CheckBox_img_mode_change()));

	connect(ui->checkBox_ImgModeSizeOrg, SIGNAL(stateChanged(int)), this, SLOT(CheckBox_img_mode_change()));

	connect(ui->horizontalSlider_sigma,SIGNAL(valueChanged(int)),ui->label_sigma, SLOT(setNum(int)));

	connect(ui->horizontalSlider_sigma, SIGNAL(valueChanged(int)), this, SLOT(Slider_img_sigma_change(int)));
	
#endif // TRUE

#if TRUE
	connect(ui->pushButton_preview, SIGNAL(clicked()), this, SLOT(StartVideoModeSelected()));
	connect(ui->pushButton_cutArea, SIGNAL(clicked()), this, SLOT(ClickButton_CutArea()));
#endif // TRUE

#if _DEBUG
	ui->comboBox_IpAddr->addItem(TEST_IPC);
#endif

#if TRUE

	std::vector<QString> RemoteBord(gRemoteBord, gRemoteBord + sizeof(gRemoteBord) / sizeof(QString));

	for (const auto bord_t : RemoteBord)
	{
		ui->comboBox_IpAddr->addItem(bord_t);
	}
		
	connect(ui->comboBox_IpAddr, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(ComboBox_IpAddr_changed(const QString &)));

#endif // TRUE

	this->ui->progressBar_flow_ctrl->setRange(0, CMD_CTRL::CMD_CTRL_DATA_LOCAL::CT_FPGA_STOP);
	
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::init_controls_by_cfg()
{

#if TRUE
	QString IpAddr_t = mAppSetting->value(mAppKeyXilinxFpgaArm).toString();

	if (IpAddr_t.isEmpty()) {
		ui->comboBox_IpAddr->setCurrentIndex(ui->comboBox_IpAddr->findText(BORD_VIDEO_OUT));
	}
	else
	{
		ui->comboBox_IpAddr->setCurrentIndex(ui->comboBox_IpAddr->findText(IpAddr_t));
	}

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

	if (mFlowCtrlClient->isRunning()) {
		qDebug() << "Flow Ctrl Client Server : Running !";
	}
	else
	{
		qDebug() << "Flow Ctrl Client Server: Close !";
	}
#endif // _DEBUG

	if  (mCtrlServer->isRunning() ||
		 mVideoDataServer->isRunning() ||
		 mFlowCtrlClient->isRunning() )
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

	if (!this->mImg8Process.isNull()) {
		this->mImg8Process->ConnectAllImg2View(_connect, receiver);
	}

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::DrawUnqualified(QSharedPointer<CMD_CTRL> _cmd)
{

	IplImage * img_t = _cmd->getIplimage();
	const int Width = img_t->width;
	const int Height = img_t->height;
	const int Thickness = 3;
	CvScalar Color=cvScalar(255,255,255);
	
	if (_cmd->IsImgFrame()) {
	
				if (_cmd->getQualified()==CMD_CTRL::BodyRollerQualified::UnQualified) {
#if _DEBUG 
		 			if (0) {
								cvLine(img_t, cvPoint(0, 0), cvPoint(Width,Height),Color, Thickness);
								cvLine(img_t, cvPoint(0, Height), cvPoint(Width,0),Color, Thickness);					
					}
#endif
				}

	}
		
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::ShowImageChGray(int _ch)
{
	ShowImageCh(_ch, mQimageGray.data());
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::ShowImageCh(int _ch, QImage *_p_qimg)
{
	ShowImage(labelImage[_ch], _p_qimg);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::ShowImage(QLabel* _qlab,QImage *_p_qimg)
{	
	ShowImageFast(_qlab, _p_qimg);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::ShowImageFast(QLabel * _qlab,  QImage * const  _p_qimg)
{
	QPixmap pixmap2(QPixmap::fromImage(*_p_qimg));

	_qlab->setPixmap(pixmap2.scaled(_qlab->size(), Qt::KeepAspectRatio));

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
				ui->checkBox_fpga_ctrl_port->setCheckState(Qt::CheckState::Unchecked);
			else
				ui->checkBox_fpga_ctrl_port->setCheckState(Qt::CheckState::Checked);
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
			ui->checkBox_image_video_port->setCheckState(Qt::CheckState::Unchecked);
		else
			ui->checkBox_image_video_port->setCheckState(Qt::CheckState::Checked);
	}
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::CheckBox_flow_ctrl_video(int _stat_t)
{
	if (_stat_t == 0)
		ui->checkBox_flow_ctrl->setCheckState(Qt::CheckState::Unchecked);
	else
		ui->checkBox_flow_ctrl->setCheckState(Qt::CheckState::Checked);

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::CheckBox_img_mode_change()
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
	this->mImgProc.Sigma = _sigma;
	mImg8Process->SetAllImgSigma(_sigma);
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
void MainWindow::workflow_local()
{
	this->mFlowCtrlClient->setLocalServer();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::workflow_remote()
{
	this->mFlowCtrlClient->setRemoteServer();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::tcp_server_work_flow_dones(int _status,int _quality)
{

	if (_status) {
		QBase::printf_event("WORK FLOW", "all client thread done");
		this->mFlowCtrlLocal->setWorkFlowDones(_status,_quality);
	}

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::tcp_server_running_client_sessions(int _running_sessions)
{
	this->mFlowCtrlLocal->setClientSessionCount(_running_sessions);
}
/*-------------------------------------*/
/**
*@note set 8 Channel video cut rect area;
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
void MainWindow::initGlobal()
{

#if 0
	JQCPUMonitor::initialize();
#endif // 0


}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::destoryGlobal()
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
		(mVideoDataServer->isRunning() == false) &&
		(mFlowCtrlClient->isRunning() == false)
		){
			event->accept();
		}else{
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

/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::initUIlabelImageView()
{

	const int chs = sizeof(labelImage) / sizeof(void*);

	for (size_t chi = 0; chi < chs; chi++) {			
			ShowImageChGray(chi);
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
					mImgProc.CurrentChannel =Channel;
					dialog->SetImgProc(mImgProc);	
				
					dialog->SetCmdCtrlImageBuffer(Channel,cmd_ctrl_image[Channel]);
					
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
		mImgProc.ShowCutArea = TRUE;
	}else {
		mImgProc.ShowCutArea = FALSE;
	}
	mImg8Process->SetAllImgShowCutRect(mImgProc.ShowCutArea);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::toggleImageClassifyHoughBlock(bool _status)
{

	QAction *action_t = qobject_cast<QAction *>(sender());
	
	std::vector<float> feature_t;

	mImgProc.ImgProc_Hough_Block_Classify= action_t->isChecked();
	
	mImg8Process->SetAllImgClassifyHoughBlock(mImgProc.ImgProc_Hough_Block_Classify,feature_t);

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::toggleImgProcDenoise(bool _status)
{
	QAction *action_t = qobject_cast<QAction *>(sender());

	mImgProc.ImgProc_Denoise = action_t->isChecked();

	mImg8Process->SetAllImgDenoise(mImgProc.ImgProc_Denoise);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::toggleImgProcBinary(bool _status)
{
	QAction *action_t = qobject_cast<QAction *>(sender());

	mImgProc.ImgProc_Binary = action_t->isChecked();

	mImg8Process->SetAllImgBinary(mImgProc.ImgProc_Binary);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::toggleShowClassifyThickly(bool _checked)
{
	bool ok;
	double double_min = 0;
	double double_max = 1;
	double threshold_default= mImg8Process->GetClassifyThicklyThreshold();
	const double double_step = 0.001;
	float threshold_t = -1;

	if (ui->action_Show_Classify_Thickly->isChecked()) {

		mImgProc.ImgProc_Binary_Thickly_Classify = TRUE;

		threshold_t = QInputDialog::getDouble(this,
										tr("classify thickly "),
										tr("threshold"),
										threshold_default,
										double_min,
										double_max,
										6,
										&ok,
										Qt::Dialog,
										double_step);

		if (!ok)	{
			threshold_t = threshold_default;
		}



	}else {
		mImgProc.ImgProc_Binary_Thickly_Classify = FALSE;
	}
	
	mImg8Process->SetAllImgClassifyThickly(mImgProc.ImgProc_Binary_Thickly_Classify, threshold_t);

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::toggleImgCollect()
{

	if (ui->action_img_collect->isChecked()) {
		this->mVideoDataServer->save_record(TRUE);
	}else {
		this->mVideoDataServer->save_record(FALSE);
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
#endif

	this->mFlowCtrlClient->startServer();

#if FLOW_CTRL_USE_LOCAL_SERVER 
	mFlowServerServerLocal->startServer();
	mFlowCtrlLocal->startServer();
#endif
	
#if TRUE
	while (mCtrlServer->isRunning() == false);

	mCtrlServer->GetImgSigmaCmd();
#endif

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
		QMessageBox::critical(NULL, tr("warning !"), tr("The background service is running !"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
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
			this->CheckBox_img_mode_change();
			this->initUIlabelImageView();
			this->disableInputCtrls();
			this->StartVideoBasic(mWorkMode);
	}
	else
	{
		QMessageBox::critical(NULL, tr("warning"), tr("后台服务尚未停止"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
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
	//同步

	mFlowCtrlClient->closeServerSync();
	
	mCtrlServer->closeServerSync();
		
	mVideoDataServer->closeServerSync();

#if FLOW_CTRL_USE_LOCAL_SERVER 
	mFlowServerServerLocal->closeServerSync();
#endif	

	mFlowCtrlLocal->closeServerSync();

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
	const QObject* sender[] = { mCtrlServer.data(),mVideoDataServer.data(),mFlowCtrlClient.data()};
	const char*   slot_tcp_port[1] = {};
	const char*   slot_thread_status[1] = {};
	const int sender_size = sizeof(sender)/sizeof(QObject);
		
	connect(mCtrlServer.data(), SIGNAL(socket_connect_state(int)), this, SLOT(CheckBox_fpga_ctrl(int)));
	connect(mCtrlServer.data(), SIGNAL(thread_running_state(int)), this, SLOT(CheckBox_thread_status_fpga_ctrl(int)));
	connect(mCtrlServer.data(), SIGNAL(SigmaChanged(int)), ui->horizontalSlider_sigma, SLOT(setValue(int)));

	connect(mVideoDataServer.data(), SIGNAL(socket_connect_state(int)), this, SLOT(CheckBox_fpga_image_video(int)));
	connect(mVideoDataServer.data(), SIGNAL(thread_running_state(int)), this, SLOT(CheckBox_thread_status_fpga_image_video(int)));

	connect(mFlowCtrlClient.data(), SIGNAL(socket_connect_state(int)), this, SLOT(CheckBox_flow_ctrl_video(int)));
	connect(mFlowCtrlClient.data(), SIGNAL(thread_running_state(int)), this, SLOT(CheckBox_thread_status_flow_ctrl_video(int)));
	
#if FLOW_CTRL_USE_LOCAL_SERVER 
	this->init_connect_work_flow();

	this->connect(mFlowServerServerLocal.data(),
		SIGNAL(work_flow_done(int,int)),
		this,
		SLOT(tcp_server_work_flow_dones(int,int)));

	this->connect(mFlowServerServerLocal.data(),
		SIGNAL(running_client_sessions(int)),
		this,
		SLOT(tcp_server_running_client_sessions(int)));
#endif

	this->connect(mFlowCtrlClient.data(),
		SIGNAL(status_sjts(int,QString)),
		this,
		SLOT(sjts_fpga_work_flow_status_rcv(int,QString))
		);

	connect_img_ch(TRUE, this);

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
	

#if defined(linux) || defined(__linux) || defined(__linux__) ||defined( __GNUC__)
	QRect rec = QApplication::desktop()->availableGeometry();
	int SCREEM_H = rec.height();
	int SCREEN_W = rec.width();
	this->setFixedWidth(SCREEN_W);
	this->setFixedHeight(SCREEM_H);
#endif

#if defined(_WIN32) || defined(_WIN64) || defined( _MSC_VER)
	QRect rec = QApplication::desktop()->availableGeometry();
	int SCREEM_H = rec.height();
	int SCREEN_W = rec.width();
	this->setFixedWidth(SCREEN_W);	
	this->setFixedHeight(SCREEM_H);
#endif



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
	mAppSetting->setValue(mAppKeyXilinxFpgaArm,_str);
	mImg8Process->SetAllImg_IpAddrProperty(GetIpAddrProperty(_str));
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::CheckBox_thread_status_ping(int _stat_t)
{

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::CheckBox_thread_status_ssh(int _stat_t)
{

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::CheckBox_thread_status_fpga_ctrl(int _stat_t)
{
	ui->checkBox_fpga_ctrl_port->setStyleSheet(mCheckBoxRunStatus[_stat_t]);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::CheckBox_thread_status_fpga_image_video(int _stat_t)
{
	ui->checkBox_image_video_port->setStyleSheet(mCheckBoxRunStatus[_stat_t]);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::CheckBox_thread_status_flow_ctrl_video(int _stat_t)
{
	ui->checkBox_flow_ctrl->setStyleSheet(mCheckBoxRunStatus[_stat_t]);
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
void MainWindow::sjts_status(const int _sjts_status_int,QString _msg)
{
	const CMD_CTRL::SJTS_MACHINE_STATUS _sjts_status = (CMD_CTRL::SJTS_MACHINE_STATUS) _sjts_status_int;

	if (_sjts_status == CMD_CTRL::SJTS_MACHINE_STATUS::RoolerReady) {

		beforeNotifiedClientSessionM(CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_START);
		NotifiedClientSessionM(CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_START,_msg.toUInt());

	}
	else if (_sjts_status == CMD_CTRL::SJTS_MACHINE_STATUS::RollerDoneQualified ||
		_sjts_status == CMD_CTRL::SJTS_MACHINE_STATUS::RollerDoneUnqualified) {
		// 
		qDebug() << "roller qualified is done !";

	}
	else if (_sjts_status == CMD_CTRL::SJTS_MACHINE_STATUS::RollerDone) {

		NotifiedClientSessionM(CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_STOP);

	}
	else if (_sjts_status == CMD_CTRL::SJTS_MACHINE_STATUS::StepMotorStart01) {

		NotifiedClientSessionM(CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_START_01);

	}
	else if (_sjts_status == CMD_CTRL::SJTS_MACHINE_STATUS::StepMotorStop01) {

		NotifiedClientSessionM(CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_STOP_01);

	}
	else if (_sjts_status == CMD_CTRL::SJTS_MACHINE_STATUS::StepMotorStart00) {

		NotifiedClientSessionM(CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_START_00);

	}
	else if (_sjts_status == CMD_CTRL::SJTS_MACHINE_STATUS::StepMotorStop00) {

		NotifiedClientSessionM(CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_STOP_00);

	}
	else if (_sjts_status == CMD_CTRL::SJTS_MACHINE_STATUS::SerialPortError) {
		QBase::printf_event("SIGNAL", "SerialPortError");
	}
	else if (_sjts_status == CMD_CTRL::SJTS_MACHINE_STATUS::SerialPortIsOpen) {
		QBase::printf_event("SIGNAL", "SerialPortIsOpen");
	}
	else if (_sjts_status == CMD_CTRL::SJTS_MACHINE_STATUS::ClientError){
		QBase::printf_event("ClientError", _msg.toStdString());
	}else {
		Q_ASSERT(0);
	}

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::sjts_fpga_work_flow_status_rcv(const int  _arm_linux_fpga_work_flow_int,QString _seq)
{	
	//RollerDoneQualified
	
	this->ui->progressBar_flow_ctrl->setValue(_arm_linux_fpga_work_flow_int);
#if _DEBUG
	if (_arm_linux_fpga_work_flow_int==CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_START) {
		
		
	}
	else if (_arm_linux_fpga_work_flow_int == CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_START_00) {

		
		
	}
	else if (_arm_linux_fpga_work_flow_int == CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_STOP_00) {

	
		
	}
	else if (_arm_linux_fpga_work_flow_int == CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_START_01) {

		
		
	}
	else if (_arm_linux_fpga_work_flow_int == CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_STOP_01) {

	
		

	}
	else if (_arm_linux_fpga_work_flow_int == CMD_CTRL::CMD_TYPE_LOCAL::CT_FPGA_STOP) {

	
		
	}
	else {

		Q_ASSERT(FALSE);
	}
#endif

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::NotifiedClientSessionM(CMD_CTRL::CMD_TYPE_LOCAL _type_c,int _cmd_idx)
{
#if FLOW_CTRL_USE_LOCAL_SERVER 	
	mFlowServerServerLocal->NotifiedClientSession(_type_c,_cmd_idx);
#endif	

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::beforeNotifiedClientSessionM(CMD_CTRL::CMD_TYPE_LOCAL _type_c,QString _msg)
{
#if FLOW_CTRL_USE_LOCAL_SERVER 
	mFlowServerServerLocal->beforeNotifiedClientSession(_type_c);
#endif	
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
#if FLOW_CTRL_USE_LOCAL_SERVER 
void MainWindow::init_connect_work_flow()
{
	this->connect(mFlowCtrlLocal.data(),
		SIGNAL(status_sjts(int,QString)),
		this,
		SLOT(sjts_status(int, QString)));
}
#endif
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void MainWindow::toggleBlockStep02(bool _block)
{
#if FLOW_CTRL_USE_LOCAL_SERVER 
	if (!mFlowCtrlLocal.isNull()) {
		mFlowCtrlLocal->SetBlock(_block);
	}
#endif
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
