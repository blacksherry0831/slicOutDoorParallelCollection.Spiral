#pragma once

#include <pthread.h>
#include <iostream>
using namespace std;

class ThreadMutex
{
public:
	ThreadMutex()
	{
		pthread_mutex_init(&mtx, NULL);
	}

	~ThreadMutex()
	{
		pthread_mutex_destroy( &mtx );
	}

	inline void Lock()
	{
		pthread_mutex_lock( &mtx );
	}

	inline void UnLock()
	{
		pthread_mutex_unlock( &mtx );
	}

	void Log(void);
private:
	pthread_mutex_t mtx;
}; 