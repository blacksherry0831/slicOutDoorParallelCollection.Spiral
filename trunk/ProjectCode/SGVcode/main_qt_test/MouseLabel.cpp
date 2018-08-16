#include "mouselabel.h"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
MouseLabel::MouseLabel(QWidget *parent) :
    QLabel(parent)
{
	this->setMouseTracking(true);
	setFocusPolicy(Qt::StrongFocus);
}
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
void MouseLabel::mouseMoveEvent(QMouseEvent *event) 
{
    this->x = event->x();
    this->y = event->y();
    emit Mouse_Position();

	emit mouseMoveEventSig(event);
	
}
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
void MouseLabel::mousePressEvent(QMouseEvent *event)
{
    emit Mouse_Pressed();

	emit mousePressEventSig(event);
}
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/

void MouseLabel::mouseDoubleClickEvent(QMouseEvent *event)
{
    emit Mouse_DoubleClick(this);
}
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
void MouseLabel::leaveEvent(QMouseEvent *)
{
    emit Mouse_Left();
}
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
void MouseLabel::keyPressEvent(QKeyEvent * ev)
{
	emit keyPressEventSig(ev);
}
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
void MouseLabel::keyReleaseEvent(QKeyEvent * ev)
{
	emit keyReleaseEventSig(ev);
}
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
void MouseLabel::mouseReleaseEvent(QMouseEvent *event) 
{
	if (timer.isActive()) {
		timer.stop();
		qDebug() << "double click";
		mouseDoubleClickEvent(event);
	}
	else {
		timer.start(300, this);
	}

	mouseReleaseEventSig(event);

}
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
void MouseLabel::timerEvent(QTimerEvent *) 
{
	timer.stop();
	qDebug() << "single click";
	emit clicked();
}
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/