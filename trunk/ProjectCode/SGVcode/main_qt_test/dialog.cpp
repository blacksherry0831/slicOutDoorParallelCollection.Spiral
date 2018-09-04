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
	mShowCutArea = TRUE;
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
void Dialog::SetChannel(int _channel)
{
	this->mCurrentChannel =_channel;
	this->mVideoProcessData = QSharedPointer<QtThread8VideoProcess>(new  QtThread8VideoProcess(_channel,false));
}
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

	if (!cmd_ctrl_image[mCurrentChannel].isNull()) {
	
		m_WidthImg =cmd_ctrl_image[mCurrentChannel]->Width();
		m_HeightImg =cmd_ctrl_image[mCurrentChannel]->Height();
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
	this->mShowCutArea = _show;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void Dialog::SetShowBinary(int _show_bin)
{
	this->mShowBinaryImg = _show_bin;
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
		QtThreadClientCtrl::cmds.setCmd(cmd_t);
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
void Dialog::img_stat_show(int _p_stat, int _channel, int _frames)
{
	if ((_p_stat >> 8) == CMD_CTRL::CMD_TYPE_02::CT_FRAME) {
		
		QSharedPointer<exCircleData> circleData = ChannelsData::channelsData()->getChannelData(_channel);

		while (circleData->QueueSize()){
			cmd_ctrl_image[_channel] = circleData->getImg();
		
			if (mCurrentChannel == _channel) {
				
				IplImage* img_t = cmd_ctrl_image[_channel]->getIplimage();
#if TRUE
				if (this->mShowCutArea) {
					
					mVideoProcessData->SetCurrentCutArea(img_t);
					mVideoProcessData->DrawCurrentCutArea(img_t);
					mVideoProcessData->DrawFutureCutArea(img_t);
					mVideoProcessData->DrawSelectedBoder(img_t);

				}
#endif // TRUE

				MainWindow::ProcessImage(img_t, this->mShowCutArea,this->mShowBinaryImg);

#if TRUE
					QSharedPointer<QImage> qimg = cmd_ctrl_image[_channel]->getQimage();
					this->ResizeWindowSize();
					MainWindow::ShowImage(ui->labelImg, qimg.data());
#endif // TRUE
							
			}
		}

		

	}else if ((_p_stat >> 8) == CMD_CTRL::CMD_TYPE_02::CT_START) {
		
	}else if ((_p_stat >> 8) == CMD_CTRL::CMD_TYPE_02::CT_STOP) {

	}else {

	}
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/