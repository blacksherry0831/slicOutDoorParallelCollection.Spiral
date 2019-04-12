#pragma once
/*-------------------------------------*/
#include "cpp_stl.h"
/*-------------------------------------*/
#include "qt_all.h"
/*-------------------------------------*/

/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
class QtSigSjts :public QObject
{
	Q_OBJECT
public:
	QtSigSjts(QObject *_parent = nullptr);
	~QtSigSjts(void);
public:

signals:
	void status_sjts(int, QString);


};