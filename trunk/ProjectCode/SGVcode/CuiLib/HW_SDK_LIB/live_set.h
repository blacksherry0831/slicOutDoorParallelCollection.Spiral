#ifndef live_include_h
#define live_include_h

typedef struct  
{
	int snap_mode;
	int continus_snap_interval;
	int continus_snap_cnt;
	char snap_dir_path[255];
	int snap_jpg_quality;
}ipc_set_t;

bool get_ipc_set(const char* file,ipc_set_t* ipc_set);

#endif