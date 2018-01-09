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
		emit WorkProgress(QStringLiteral("��������������޷���"));
		return;
	}

	//һ�о���
	if (this->m_Step_Motor_ptr->Wait4CmdDone() == false) {
			emit WorkProgress(QStringLiteral("����������ȴ������������"));
		return;
	}

	if (m_hw_nvr_ptr->hw_login() == false) {
		emit WorkProgress(QStringLiteral("NVR�豸���޷���¼"));
		return;
	}

	if (m_hw_nvr_ptr->Check7Link4Start() == false) {
		emit WorkProgress(QStringLiteral("NVR�豸��0-7ͨ���������ӹ���"));
		return;
	}

	if (m_hw_nvr_ptr->Wait4StopAllRecord() == false) {
		emit WorkProgress(QStringLiteral("NVR�豸��ֹͣ¼���쳣"));
		return;
	}

	if (m_hw_nvr_ptr->Check7Status4Start() == false) {
		emit WorkProgress( QStringLiteral("NVR�豸��ͨ�����ڿ���"));
		return;
	}
#endif

#if TRUE
	//��������
	if (m_hw_nvr_ptr->Wait4StartAllRecord() == false) {
		emit WorkProgress(QStringLiteral("NVR�豸��ͨ������ȫ������"));
		return;
	}
	//�����������
	this->m_Step_Motor_ptr->SendCmd(BE_1105_RUN_NEG,m_motor_speed);
	if (this->m_Step_Motor_ptr->Wait4CmdDone() == false) {
		emit WorkProgress( QStringLiteral("����������޷�ִ��"));
	}
	//�ر�¼��
	if (m_hw_nvr_ptr->Wait4StopAllRecord() == false) {
		emit WorkProgress(QStringLiteral("NVR�豸��ֹͣ¼���쳣"));
		return;
	}
#endif

	emit WorkProgress(QStringLiteral("success"));
}