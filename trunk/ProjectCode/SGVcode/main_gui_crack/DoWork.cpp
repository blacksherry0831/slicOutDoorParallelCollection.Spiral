#include "DoWork.hpp"


#include <assert.h> 
 

DoWorkThread::DoWorkThread(QObject *parent)
{
	m_progress = 0;
	m_parent = parent;

	m_Step_Motor_ptr = BE_1105_Driver::getInstance();
	m_hw_nvr_ptr.reset(new hw_nvr("192.168.9.200", 0));
}

DoWorkThread::~DoWorkThread()
{

}

void DoWorkThread::run()
{
	
	do{
		
		this->WorkStart();

		QThread::sleep(1);
	}while (0);
}

void DoWorkThread::ResetSlot()
{

	
}

/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void DoWorkThread::WorkStart()
{
	QString TITLE = "START";
#if TRUE

	if (this->m_Step_Motor_ptr->open(m_motor_port_name.toStdString())==false) {
		emit WorkProgress(QStringLiteral("步进电机：串口无法打开"));
		return;
	}

	//一切就绪
	if (this->m_Step_Motor_ptr->Wait4CmdDone() == false) {
			emit WorkProgress(QStringLiteral("步进电机：等待步进电机就绪"));
		return;
	}

	if (m_hw_nvr_ptr->hw_login() == false) {
		emit WorkProgress(QStringLiteral("NVR设备：无法登录"));
		return;
	}

	if (m_hw_nvr_ptr->Check7Link4Start() == false) {
		emit WorkProgress(QStringLiteral("NVR设备：0-7通道存在连接故障"));
		return;
	}

	if (m_hw_nvr_ptr->Wait4StopAllRecord() == false) {
		emit WorkProgress(QStringLiteral("NVR设备：停止录像异常"));
		return;
	}

	if (m_hw_nvr_ptr->Check7Status4Start() == false) {
		emit WorkProgress( QStringLiteral("NVR设备：通道存在开启"));
		return;
	}
#endif

#if TRUE
	//启动摄像
	if (m_hw_nvr_ptr->Wait4StartAllRecord() == false) {
		emit WorkProgress(QStringLiteral("NVR设备：通道不能全部开启"));
		return;
	}
	//启动步进电机
	this->m_Step_Motor_ptr->SendCmd(BE_1105_RUN_NEG,m_motor_speed);
	if (this->m_Step_Motor_ptr->Wait4CmdDone() == false) {
		emit WorkProgress( QStringLiteral("步进电机：无法执行"));
	}
	//关闭录像
	if (m_hw_nvr_ptr->Wait4StopAllRecord() == false) {
		emit WorkProgress(QStringLiteral("NVR设备：停止录像异常"));
		return;
	}
#endif

	emit WorkProgress(QStringLiteral("success"));
}