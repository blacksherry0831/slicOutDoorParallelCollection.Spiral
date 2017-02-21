#ifndef   MALLOC_CUI
#define   MALLOC_CUI
#if 0
void * operator new (size_t size);
void * operator new [](size_t size);
void * operator new (size_t size, const char* file, const size_t line);
void * operator new [](size_t size, const char* file, const size_t line);


void operator delete (void * pointer);
void operator delete[](void * pointer);
void operator delete (void * pointer, const char* file, const size_t line);
void operator delete[](void * pointer, const char* file, const size_t line);


void * cui_malloc(size_t size);
void cui_free(void* ptr);
void writeAlloc2Disk(void);
void writeAlloc2Disk(int size);
#endif



#endif
