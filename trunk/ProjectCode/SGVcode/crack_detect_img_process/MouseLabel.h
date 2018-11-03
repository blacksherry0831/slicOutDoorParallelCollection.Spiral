#ifndef MOUSELABEL_H
#define MOUSELABEL_H

#include <QLabel>
#include <QMouseEvent>
#include <QEvent>
#include <QDebug>
#include <QBasicTimer>

class MouseLabel : public QLabel
{
    Q_OBJECT
public:
    explicit MouseLabel(QWidget *parent = 0);
private:
	void timerEvent(QTimerEvent *);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void leaveEvent(QMouseEvent *);
private:
    int x, y;
	QBasicTimer timer;

protected:
	virtual void keyPressEvent(QKeyEvent *ev);
	virtual void keyReleaseEvent(QKeyEvent *ev);

	void mouseMoveEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
signals:
    void Mouse_Pressed();
    void Mouse_Position();
    void Mouse_Left();
    void Mouse_Release();
    void Mouse_DoubleClick(QLabel* _qlabel);
	void clicked();
signals:
	void mouseMoveEventSig(QMouseEvent *event);
	void mousePressEventSig(QMouseEvent *event);
	void mouseReleaseEventSig(QMouseEvent *event);

	void keyPressEventSig(QKeyEvent *ev);
	void keyReleaseEventSig(QKeyEvent *ev);
public:
	int  mChannel;
public slots:

};

#endif // MOUSELABEL_H