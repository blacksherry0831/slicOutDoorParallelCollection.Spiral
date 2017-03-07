#include "stdafx.h"
#include "live_set.h"

#include "tinyxml2/tinyxml2.h"
using namespace tinyxml2;
//#include "tinyxml.h"

bool get_ipc_set(const char* file,ipc_set_t* ipc_set)
{
#if 0
TiXmlDocument doc(file);
	if(!doc.LoadFile())
	{
		return false;
	}

	TiXmlHandle doch(&doc);
	TiXmlElement* element_snap_mode = doch.FirstChildElement("set").FirstChildElement("snap_mode").Element(); 
	if(element_snap_mode)
	{
		ipc_set->snap_mode = atoi(element_snap_mode->GetText());
	}

	TiXmlElement* element_continus_snap_interval = doch.FirstChildElement("set").FirstChildElement("continus_snap_interval").Element(); 
	if(element_continus_snap_interval)
	{
		ipc_set->continus_snap_interval = atoi(element_continus_snap_interval->GetText());
	}

	TiXmlElement* element_continus_snap_cnt = doch.FirstChildElement("set").FirstChildElement("continus_snap_cnt").Element(); 
	if(element_continus_snap_cnt)
	{
		ipc_set->continus_snap_cnt = atoi(element_continus_snap_cnt->GetText());
	}

	TiXmlElement* element_snap_dir_path = doch.FirstChildElement("set").FirstChildElement("snap_dir_path").Element();
	if(element_snap_dir_path)
	{
		sprintf(ipc_set->snap_dir_path,"%s",element_snap_dir_path->GetText());
	}

	TiXmlElement* element_snap_jpg_quality = doch.FirstChildElement("set").FirstChildElement("snap_jpg_quality").Element();
	if(element_snap_jpg_quality)
	{
		ipc_set->snap_jpg_quality = atoi(element_snap_jpg_quality->GetText());	
	}
#else	
	assert(0);
#endif
	

	return true;
}