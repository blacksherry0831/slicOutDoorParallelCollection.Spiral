#include "mem_pool_fixed.h"

#define BLK_SIZE (1920*1080)

#define BLK_NUM  (32)

unsigned char    BUFF[BLK_NUM][BLK_SIZE] = {0};

OS_MEM  m_os_mem;
OS_ERR  m_os_err;


void init_mem_pool() 
{

	OSMemCreate(&m_os_mem, "memory", BUFF, BLK_NUM,BLK_SIZE, &m_os_err);

	if (m_os_err == OS_ERR_NONE) {
		//printf("");
	}else{
		printf("mem init error");
		exit(-1);
	}
		
	return;
}



void* mem_malloc() 
{
	void *data = (unsigned char *)OSMemGet(&m_os_mem, &m_os_err);

	if (m_os_err == OS_ERR_NONE) {
		//printf("");
	}
	else {
		printf("mem malloc error");
		exit(-1);
	}

	return data;
}


void mem_free(void* _mem)
{

	OSMemPut(&m_os_mem, _mem, &m_os_err);
		
	if (m_os_err == OS_ERR_NONE) {
		//printf("");
	}
	else {
		printf("mem free error");
		exit(-1);
	}

}


void mem_destory()
{

	CPU_IntDestory();

}