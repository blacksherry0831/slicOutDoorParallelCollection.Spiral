#include "pt_mutex.h"
#include "pt_mutex_switch.h"

/*----------------------------------*/
/**
*
*/
/*----------------------------------*/
ThreadSwitch::ThreadSwitch()
{
	m_value_t=0;
}
/*----------------------------------*/
/**
*
*/
/*----------------------------------*/
ThreadSwitch::~ThreadSwitch()
{

}
/*----------------------------------*/
/**
*
*/
/*----------------------------------*/
 void ThreadSwitch::Log(void)
{
	printf("use pt threads mutex!");

}
 /*----------------------------------*/
 /**
 *
 */
 /*----------------------------------*/
 int ThreadSwitch::SetAtom(int p_t)
 {
	 
	 m_mutex.lock();
	 m_value_t=p_t;
	 m_mutex.Unlock();
	 return this->GetAtom();
 }
  /*----------------------------------*/
 /**
 *
 */
 /*----------------------------------*/
 int ThreadSwitch::GetAtom()
 {
	 int p_t;
		m_mutex.lock();
			p_t=m_value_t;
		m_mutex.Unlock();
	 return p_t;
 }
  /*----------------------------------*/
 /**
 *
 */
 /*----------------------------------*/