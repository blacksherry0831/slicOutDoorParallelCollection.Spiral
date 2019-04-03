#include "dialog.h"
#include "ui_dialog.h"
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
	setMouseTracking(true);
//	ui->labelImg->installEventFilter(this);
	this->initImageEvent();
	m_WidthImg=0;
	m_HeightImg = 0;
	m_Scale=0;
	mImgProcess.ShowCutArea = TRUE;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
Dialog::~Dialog()
{
    delete ui;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
bool Dialog::eventFilter(QObject * watched, QEvent * event)
{
	if (watched==ui->labelImg){

		if (event->type() == QEvent::MouseButtonPress)
		{
			/*if (event->button() == Qt::LeftButton)
			{
			
			}
			else if (event->button() == Qt::RightButton)
			{
			
			}
			else if (event->button() == Qt::MidButton)
			{
				
			}*/

		}else	if (event->type() == QEvent::MouseButtonRelease) {
		
		}
		else	if (event->type() == QEvent::MouseMove) {
			


		}
		else	if (event->type() == QEvent::KeyPress) {

		}
		else	if (event->type() == QEvent::KeyRelease) {

		}else{

		}

	}

	return QDialog::eventFilter(watched, event);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void Dialog::initImageEvent()
{
	connect(ui->labelImg, SIGNAL(mouseMoveEventSig(QMouseEvent *)), this, SLOT(mouseMoveEventSigImg(QMouseEvent *)));
	connect(ui->labelImg, SIGNAL(mousePressEventSig(QMouseEvent *)), this, SLOT(mousePressEventSigImg(QMouseEvent *)));
	connect(ui->labelImg, SIGNAL(mouseReleaseEventSig(QMouseEvent *)), this, SLOT(mouseReleaseEventSigImg(QMouseEvent *)));
	connect(ui->labelImg, SIGNAL(keyPressEventSig(QKeyEvent *)), this, SLOT(keyPressEventSigImg(QKeyEvent *)));
	connect(ui->labelImg, SIGNAL(keyReleaseEventSig(QKeyEvent *)), this, SLOT(keyReleaseEventSigImg(QKeyEvent *)));
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int  Dialog::initkeyboardModifiers()
{
	int flag = 0;

	if ((QApplication::keyboardModifiers() & Qt::ControlModifier ) >0){
		
		flag |= CV_EVENT_FLAG_CTRLKEY;
				
	}

	if ((QApplication::keyboardModifiers() & Qt::AltModifier) >0) {

		flag |= CV_EVENT_FLAG_ALTKEY;

	}

	return flag;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void Dialog::mousePressEvent(QMouseEvent * event)
{
	if (event->button() == Qt::LeftButton)
	{
		
	}
	else if (event->button() == Qt::RightButton)
	{
		
	}
	else if (event->button() == Qt::MidButton)
	{
		
	}
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void Dialog::mouseMoveEvent(QMouseEvent * event)
{
	if (event->buttons()&Qt::LeftButton)
	{
		
	}
	else if (event->buttons()&Qt::RightButton)
	{
		
	}
	else if (event->buttons()&Qt::MidButton)
	{
		
	}

#if TRUE
	event->pos();
#endif

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void Dialog::mouseReleaseEvent(QMouseEvent * event)
{
	if (event->button() == Qt::LeftButton)
	{
		
	}
	else if (event->button() == Qt::RightButton)
	{
		
	}
	else if (event->button() == Qt::MidButton)
	{
		
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
void Dialog::ResizeWindowSize()
{
	QRect rec = QApplication::desktop()->screenGeometry();
	int SCREEM_H = rec.height();
	int SCREEN_W = rec.width();
	int Dialog_H = 0;
	int Dialog_W = 0;

	if (!cmd_ctrl_image[mImgProcess.CurrentChannel].isNull()) {
	
		m_WidthImg =cmd_ctrl_image[mImgProcess.CurrentChannel]->Width();
		m_HeightImg =cmd_ctrl_image[mImgProcess.CurrentChannel]->Height();
		float scale =1.0* m_WidthImg / m_HeightImg;

		if (SCREEM_H*scale<=SCREEN_W){
				Dialog_W = SCREEM_H*scale;
				Dialog_H = SCREEM_H;
		}else{
				Dialog_W = SCREEN_W;
				Dialog_H = SCREEN_W /scale;
		}
#if 0
		if (this->width() != Dialog_W) {
			this->setFixedWidth(Dialog_W);
		}
		if (this->height() != Dialog_H) {
			this->setFixedHeight(Dialog_H);
		}
#else
		if (ui->labelImg->width() != Dialog_W) {
				ui->labelImg->setFixedWidth(Dialog_W);
				
		}
		if (ui->labelImg->height() != Dialog_H) {
				ui->labelImg->setFixedHeight(Dialog_H);
		}
		m_Scale = 1.0 * m_WidthImg / (ui->labelImg->width());
#endif // 0
		
					
	}

	



}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void Dialog::SetShowCutArea(int _show)
{
	mImgProcess.ShowCutArea = _show;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void Dialog::SetShowBinary(int _show_bin)
{
	mImgProcess.ImgProc_Binary = _show_bin;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void Dialog::SetImgProcChannel(int _ch)
{
	this->mImgProcess.CurrentChannel = _ch;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void Dialog::SetImgProc(IMG_PROC _img_proc)
{
	this->mImgProcess = _img_proc;
	this->mVideoProcessData = QSharedPointer<QtThread8VideoProcess>(new  QtThread8VideoProcess(this->mImgProcess.CurrentChannel, false));
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void Dialog::SetCmdCtrlImageBuffer(int _ch, QSharedPointer<CMD_CTRL> _cmd_ctrl_img)
{
	if (!_cmd_ctrl_img.isNull())
	{
		this->cmd_ctrl_image[_ch] = _cmd_ctrl_img;
		SetImgProcChannel(_ch);
		ShowCmdCtrlImage(_ch);
		this->update();
	}

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void Dialog::mouseMoveEventSigImg(QMouseEvent * event)
{
	QPoint qp = event->pos();
	int flags = initkeyboardModifiers();
	
	int x = qp.x()*m_Scale;
	int y = qp.y()*m_Scale;
	QtThread8VideoProcess* const ptr_vp = mVideoProcessData.data();


	ptr_vp->SetCutRectBorder(cvPoint(x, y), flags);
	ptr_vp->SetCutRect(x, y, flags);

	

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void Dialog::mousePressEventSigImg(QMouseEvent * event)
{
	QPoint qp = event->pos();
	int flags = initkeyboardModifiers();
	int x = qp.x()*m_Scale;
	int y = qp.y()*m_Scale;
	QtThread8VideoProcess* const ptr_vp = mVideoProcessData.data();

	if (event->button() == Qt::LeftButton){

		if (CV_EVENT_FLAG_ALTKEY & flags) {

			ptr_vp->ChangeRectMode();

		}
		else if (flags & CV_EVENT_FLAG_CTRLKEY) {

			if (ptr_vp->mRectM == QtThread8VideoProcess::RectMode::RECT) {
				ptr_vp->mCutAreaSet.x = x;
				ptr_vp->mCutAreaSet.y = y;
				ptr_vp->SetCutRectStart();
			}

		}else {

		}

	}else if (event->button() == Qt::RightButton){

		ptr_vp->SetCutRectStart();

	}else if (event->button() == Qt::MidButton){

	}

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void Dialog::mouseReleaseEventSigImg(QMouseEvent * event)
{
	QPoint qp = event->pos();
	int flags = initkeyboardModifiers();
	int x = qp.x()*m_Scale;
	int y = qp.y()*m_Scale;
	QtThread8VideoProcess* const ptr_vp = mVideoProcessData.data();

	if (event->button() == Qt::LeftButton)
	{
		if (ptr_vp->mRectM == QtThread8VideoProcess::RectMode::LEFT) {
			
		}else if (ptr_vp->mRectM == QtThread8VideoProcess::RectMode::UP) {
			
		}else if (ptr_vp->mRectM == QtThread8VideoProcess::RectMode::RIGHT) {
			
		}else if (ptr_vp->mRectM == QtThread8VideoProcess::RectMode::DOWN) {
			
		}else if (ptr_vp->mRectM == QtThread8VideoProcess::RectMode::IDLE) {

		}else if (ptr_vp->mRectM == QtThread8VideoProcess::RectMode::RECT) {

			if (flags & CV_EVENT_FLAG_CTRLKEY) {				
					ptr_vp->mCutAreaSet.width = std::abs(x - ptr_vp->mCutAreaSet.x);
					ptr_vp->mCutAreaSet.height = std::abs(y - ptr_vp->mCutAreaSet.y);
			}

		}else{

		}

	}
	else if (event->button() == Qt::RightButton)
	{
		ptr_vp->SetCutRectStop();
	}
	else if (event->button() == Qt::MidButton)
	{

	}

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void Dialog::keyPressEventSigImg(QKeyEvent * ev)
{
	
	int flags = initkeyboardModifiers();
	QtThread8VideoProcess* const ptr_vp = mVideoProcessData.data();
	const int KEY = ev->key();
	if (ev->key() == Qt::Key::Key_C) {
		if ((ptr_vp->mRectM == QtThread8VideoProcess::RectMode::RECT)) {
			ptr_vp->mCutAreaSet = cvRect(0, 0, 0, 0);
		}
	
	}else if (ev->key() == Qt::Key::Key_M) {
	
		ptr_vp->ChangeRectMode();

	}else if(ev->key() == Qt::Key::Key_Escape){
		this->close();
	}else if (ev->key() == Qt::Key::Key_Left) {
		ptr_vp->SetRectBorderAdj(-1);
	}else if (ev->key() == Qt::Key::Key_Right) {
		ptr_vp->SetRectBorderAdj(1);
	}else if (ev->key() == Qt::Key::Key_Up) {
		ptr_vp->SetRectBorderAdj(-1);
	}else if (ev->key() == Qt::Key::Key_Down) {
		ptr_vp->SetRectBorderAdj(1);
	
	}else if(KEY == Qt::Key_Enter || KEY == Qt::Key_Return){
		QSharedPointer<CMD_CTRL> cmd_t = ptr_vp->getCmdRectCfg();
		QtThreadClientCtrl::SetCmd(cmd_t);
	}else{

	}

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void Dialog::keyReleaseEventSigImg(QKeyEvent * ev)
{

	int flags = initkeyboardModifiers();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void Dialog::img_stat_show_ex(int _p_stat, int _channel, int _frames, void* _data)
{
	QtThread8VideoProcess* const ptr_vp = mVideoProcessData.data();
	QSharedPointer<exCircleData> circleData = ChannelsData4Show::getInstance()->getChannelData(_channel);
	
	if (circleData->QueueSize()) {
		cmd_ctrl_image[_channel] = circleData->getImg();
	}
	else {	
		return;
	}

	if ((_p_stat >> 8) == CMD_CTRL::CMD_TYPE_02_I::CT_IMG_FRAME) {
				
		if ((mImgProcess.CurrentChannel == _channel) && (!cmd_ctrl_image[_channel].isNull())) {
									
							IplImage* img_t = cmd_ctrl_image[_channel]->getIplimage();	
							if (mImgProcess.ShowCutArea) {
										mVideoProcessData->SetCurrentCutArea(img_t);
										mVideoProcessData->DrawCurrentCutArea(img_t);
										mVideoProcessData->DrawFutureCutArea(img_t);
										mVideoProcessData->DrawSelectedBoder(img_t);
							}
							
							this->ProcessImage(img_t);
							ShowCmdCtrlImage(_channel);
							
		
		}

	}else if ((_p_stat >> 8) == CMD_CTRL::CMD_TYPE_02_C::CT_START) {
		
	}else if ((_p_stat >> 8) == CMD_CTRL::CMD_TYPE_02_C::CT_STOP) {

	}else {

	}


	while (circleData->QueueSize()) {
		cmd_ctrl_image[_channel] = circleData->getImg();
	}

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void Dialog::ProcessImage(IplImage* img_t)
{

#if TRUE
	if (mImgProcess.ShowCutArea) {
		CvRect rect = cvGetImageROI(img_t);
		cvResetImageROI(img_t); {
			QtThread8VideoProcess::DrawArea(img_t, rect);
		}cvSetImageROI(img_t, rect);
	}
#endif // TRUE

#if TRUE
	if (mImgProcess.ImgProc_Binary) {

		CvRect rect = cvGetImageROI(img_t);
		cvResetImageROI(img_t); {
			float threshold = 0.5;
			float max_value = 255;
			int threshold_type = CV_THRESH_BINARY;
			cvThreshold(img_t, img_t, threshold, 255, threshold_type);//·§Öµ100

		}cvSetImageROI(img_t, rect);

	}
#endif // TRUE
	
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void Dialog::ShowQImage(QSharedPointer<QImage> _p_qimg)
{
	MainWindow::ShowImage(ui->labelImg, _p_qimg.data());
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void Dialog::ShowCmdCtrlImage(int _ch)
{
	QSharedPointer<QImage> qimg = cmd_ctrl_image[_ch]->getQimage();

	this->ResizeWindowSize();

	ShowQImage(qimg);


}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/