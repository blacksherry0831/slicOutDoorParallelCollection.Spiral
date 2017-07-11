#pragma once

#include <pthread.h>
#include <iostream>
using namespace std;
#include "pt_mutex.H"

class ThreadSwitch
{
public:
	ThreadSwitch();

	~ThreadSwitch();	

	void Log(void);
public:
	int SetAtom(int p_t);
	int GetAtom();


private:
	ThreadMutex m_mutex;
	int         m_value_t;
}; 