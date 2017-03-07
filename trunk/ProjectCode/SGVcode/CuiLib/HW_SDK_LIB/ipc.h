#ifndef ipc_include_h
#define ipc_include_h

//#include <boost/shared_ptr.hpp>
#include <vector>
typedef struct  
{
	char ip[32];
	char name[128];
	char nvr_ip[32];
	int nvr_slot;
	int connect_by_nvr;
}ipc_t;



//typedef boost::shared_ptr<ipc_t> ipc_ptr;
typedef ipc_t* ipc_ptr;


bool get_ipcs_from_file(const char* file,std::vector<ipc_ptr>* ipcs);
bool get_ipcs_from_name(const char *ip,std::vector<ipc_ptr>*ipcs);


#endif