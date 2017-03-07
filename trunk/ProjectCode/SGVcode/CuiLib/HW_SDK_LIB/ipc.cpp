#include "stdafx.h"
#include "ipc.h"

#include "tinyxml2/tinyxml2.h"

bool get_ipcs_from_file(const char* file,std::vector<ipc_ptr>* ipcs)
{
	ipcs->clear();
#if 0
	TiXmlDocument doc(file);
	if(!doc.LoadFile())
	{
		return false;
	}

	TiXmlNode* node = 0;	
	TiXmlElement*	element;
	for(element = doc.RootElement();
		element;
		element = element->NextSiblingElement())
	{
		ipc_ptr ipc(new ipc_t());

		sprintf(ipc->ip,"%s",element->Attribute("ip"));
		sprintf(ipc->nvr_ip,"%s",element->Attribute("nvr_ip"));
		ipc->nvr_slot = atoi(element->Attribute("nvr_slot"));
		ipc->connect_by_nvr = atoi(element->Attribute("connect_by_nvr"));
		sprintf(ipc->name,"%s",element->GetText());

		ipcs->push_back(ipc);
	}
#else
	ASSERT(0);
#endif


	return true;
}

bool get_ipcs_from_name(const char *ip,std::vector<ipc_ptr>*ipcs){

	//ipcs->clear();
	ipc_ptr ipc(new ipc_t());
	sprintf(ipc->ip,"%s",ip);
	sprintf(ipc->nvr_ip,"%s","0.0.0.0");
	ipc->nvr_slot = 0;
	ipc->connect_by_nvr = 0;
	sprintf(ipc->name,"%s",ip);
	ipcs->push_back(ipc);
	return true;
}