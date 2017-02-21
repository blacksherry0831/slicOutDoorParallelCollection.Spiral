#include "StdAfx.h"
//#include "module_all_cui.h"
//#include <stdio.h>
#include "rtthread.h"
//#include <stdlib.h>
//#include <fstream>
#if 0
/*----------------------------------------------------------*/
/**
*
*
*/
/*----------------------------------------------------------*/
#define MEMUSE 
#define GetAllocSize  1
/*----------------------------------------------------------*/
/**
*
*
*/
/*----------------------------------------------------------*/
#if 0
#define  MemSize  1024*1024*500
unsigned char mem[MemSize];
static bool IsInit=false;
void * cui_malloc(size_t size)
{
	if (IsInit==false){
		rt_system_heap_init(&mem[0],&mem[MemSize-1]);
		IsInit=true;
	}
	
	void *ptr=rt_malloc(size);
	return ptr;

}
#endif
#if 1

#define  MemSize  1024*1024*500
class Mem
{
public:
	unsigned char *data;
public:
	Mem()
		{
			data=(unsigned char *)malloc(MemSize);
		}
		~Mem()
		{
			free(data);
		}
};
///////////////////////////////////////////////////////////////
static bool IsInit=false;
#ifdef GetAllocSize
#if GetAllocSize
vector<int> allocSize;
#endif
#endif

void * cui_malloc(size_t size)
{
	if (IsInit==false){
		static Mem mem;
		rt_system_heap_init(&mem.data[0],&mem.data[MemSize-1]);
		IsInit=true;
	}

	void *ptr=rt_malloc(size);
	return ptr;

}

#endif
/*----------------------------------------------------------*/
/**
*
*
*/
/*----------------------------------------------------------*/
void cui_free(void* ptr)
{
	rt_free(ptr);
}
/*----------------------------------------------------------*/
/**
*
*
*/
/*----------------------------------------------------------*/
//void __free__(void* ptr)
//{
//	cui_free(ptr);
//}
/*----------------------------------------------------------*/
/**
*
*
*/
/*----------------------------------------------------------*/
//void* __malloc__(size_t size_t)
//{
//	return cui_malloc(size_t);
//}
/*----------------------------------------------------------*/
/**
*
*
*/
/*----------------------------------------------------------*/
void * operator new (size_t size){

	if(0 == size){

		return 0;

	}
	void *p = cui_malloc(size);
#ifdef GetAllocSize
#if GetAllocSize
	writeAlloc2Disk(size);
#endif
#endif
	return p;

}
/*----------------------------------------------------------*/
/**
*
*
*/
/*----------------------------------------------------------*/
void * operator new (size_t size, const char* file, const size_t line)
{
	return operator new(size);
}
/*----------------------------------------------------------*/
/**
*
*
*/
/*----------------------------------------------------------*/


void * operator new [](size_t size){

	return operator new(size);

}
/*----------------------------------------------------------*/
/**
*
*
*/
/*----------------------------------------------------------*/
void * operator new [](size_t size, const char* file, const size_t line)
{
	return operator new(size);
}
/*----------------------------------------------------------*/
/**
*
*
*/
/*----------------------------------------------------------*/
void operator delete (void * pointer){

	if(0 != pointer){

		cui_free(pointer);

	}

}
/*----------------------------------------------------------*/
/**
*
*
*/
/*----------------------------------------------------------*/
void operator delete (void * pointer, const char* file, const size_t line)
{
	operator delete(pointer);
}
/*----------------------------------------------------------*/
/**
*
*
*/
/*----------------------------------------------------------*/
void operator delete[](void * pointer){

	operator delete(pointer);

}
/*----------------------------------------------------------*/
/**
*
*
*/
/*----------------------------------------------------------*/
void operator delete[](void * pointer, const char* file, const size_t line)
{
	operator delete(pointer);
}
/*----------------------------------------------------------*/
/**
*
*
*/
/*----------------------------------------------------------*/
void writeAlloc2Disk(void)
{

#ifdef GetAllocSize
#if GetAllocSize
	ofstream examplefile ("example.txt");

	if (examplefile.is_open()) {

		for (int i=0;i<allocSize.size();i++){
			examplefile <<i;
			examplefile<<"   ";
			examplefile<<allocSize[i];
			examplefile<<"\n";
		}
		examplefile.close();
	}
#endif
#endif
}
/*----------------------------------------------------------*/
/**
*
*
*/
/*----------------------------------------------------------*/
void writeAlloc2Disk(int size)
{
	static int i=0;
	ofstream examplefile ("allocSize.txt",ios::app);
	if (examplefile.is_open()) {
		{
			examplefile <<i++;
			examplefile<<"   ";
			examplefile<<size;
			examplefile<<"\n";
		}
		examplefile.close();
	}
}
/*----------------------------------------------------------*/
/**
*
*
*/
/*----------------------------------------------------------*/
#endif
