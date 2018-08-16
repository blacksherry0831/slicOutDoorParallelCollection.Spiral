#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

#include "module_my.h"

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
	void SetChannel(int _channel);
	void ResizeWindowSize();
private:
	int m_WidthImg;
	int m_HeightImg;
	float m_Scale;
	 int mCurrentChannel;
	 QSharedPointer<CMD_CTRL> cmd_ctrl_image[8];
	 QSharedPointer<QtThread8VideoProcess> mVideoProcessData;
public slots:
	void img_stat_show(int _p_stat, int _channel, int _frames);

	void mouseMoveEventSigImg(QMouseEvent *event);
	void mousePressEventSigImg(QMouseEvent *event);
	void mouseReleaseEventSigImg(QMouseEvent *event);

	void keyPressEventSigImg(QKeyEvent *ev);
	void keyReleaseEventSigImg(QKeyEvent *ev);


private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
