#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

#if 1
#include "module_my.h"

#include "QT_THREAD_MODULE/QtThread8VideoProcess.hpp"
#endif // TRUE



#include "mainwindow.h"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
protected:
	bool eventFilter(QObject *watched, QEvent *event);
	void initImageEvent();
	int initkeyboardModifiers();
protected:
	//鼠标点击事件
	void mousePressEvent(QMouseEvent* event);
	//鼠标移动事件
	void mouseMoveEvent(QMouseEvent *event);
	//鼠标释放事件
	void mouseReleaseEvent(QMouseEvent *event);

public:
	
	void ResizeWindowSize();
	void SetShowCutArea(int _show);
	void SetShowBinary(int _show_bin);
	
	void SetImgProc(IMG_PROC _img_proc);

private:
	int m_WidthImg;
	int m_HeightImg;
	float m_Scale;

	IMG_PROC mImgProcess;
	 
	 QSharedPointer<CMD_CTRL> cmd_ctrl_image[8];
	 QSharedPointer<QtThread8VideoProcess> mVideoProcessData;
public slots:
	void img_stat_show_ex(int _p_stat, int _channel, int _frames, void* _data);

	void mouseMoveEventSigImg(QMouseEvent *event);
	void mousePressEventSigImg(QMouseEvent *event);
	void mouseReleaseEventSigImg(QMouseEvent *event);

	void keyPressEventSigImg(QKeyEvent *ev);
	void keyReleaseEventSigImg(QKeyEvent *ev);

	void ProcessImage(IplImage * img_t);


private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
