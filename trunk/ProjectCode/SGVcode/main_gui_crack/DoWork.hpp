#pragma once

#include <QMainWindow>
#include <QThread>

#if _MSC_VER
#ifndef _X86_
#define _X86_
#endif // !_X86_
#include <minwindef.h>
#include <minwinbase.h>
#include <WinBase.h>
#endif

#if TRUE
#include "cpp_stl.h"
#include "SerialPort/BE_1105_Dirver.hpp"
#include "HW_SDK_LIB/hw_nvr.h"
#endif

class DoWorkThread : public QThread
{
Q_OBJECT
public:
BE_1105_Driver*		m_Step_Motor_ptr;
std::auto_ptr<hw_nvr> m_hw_nvr_ptr;
QString m_motor_port_name;
bool m_motor_run_direct;
private:

	int m_progress;
	QObject *m_parent;
protected:
	void run();
	void WorkStart();
public:
	DoWorkThread(QObject *parent = 0);

	~DoWorkThread();
signals:
	void WorkProgress(QString str);
public slots:
	void ResetSlot();
};