#include "StdAfx.h"
#include "GPS_WG_8020.h"
//#include "opencv_stl.h"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
GPS_WG_8020 *GPS_WG_8020::_instance =new GPS_WG_8020();
//CGarbo CGarbo::Garbo();
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/

GPS_WG_8020::GPS_WG_8020(void)
{
	buffer_result_idx=0;
	memset(buffer_result,0,sizeof(buffer_result));
	this->open();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
GPS_WG_8020::~GPS_WG_8020(void)
{

	this->close();

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void GPS_WG_8020::open()
{
	if(m_sp.IsOpen()==false){
		m_sp.Open(4,115200);
		this->SendCmdEnterAT();
	}

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void GPS_WG_8020::close()
{
	m_sp.Close();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/

/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void GPS_WG_8020::SendCmdEnterAT()
{
	char CMD_PPP[]="+++\n";
	char CMD_ATE0[]="ate0\n";

	m_sp.Write(CMD_PPP,sizeof(CMD_PPP));

	m_sp.Write(CMD_ATE0,sizeof(CMD_ATE0));

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void GPS_WG_8020::SendCmdVERS()
{
	char CMD_VERS[]="AT^VERS\n";
	m_sp.Write(CMD_VERS,sizeof(CMD_VERS));

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void GPS_WG_8020::SendCmdGPRMC()
{
	char CMD_GPRMC[]="AT^GPRMC\n";
	m_sp.Write(CMD_GPRMC,sizeof(CMD_GPRMC));
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void GPS_WG_8020::ReadGpsData()
{
	DWORD read_count=0;
	int  Timeout_t=0;
	TimeCountStart();
	do{
		read_count=m_sp.Read(&buffer_result[buffer_result_idx],1);

		if(read_count>0){

			if (buffer_result[0]=='$')
			{


				if (buffer_result_idx>=1
					&&buffer_result[buffer_result_idx-1]==0x0D
					&&buffer_result[buffer_result_idx]==0x0A)
				{
					printf("Result>>%s",buffer_result);

					this->process_gps_data();

					buffer_result_idx=0;
					memset(buffer_result,0,sizeof(buffer_result));

					break;
				}else{
					buffer_result_idx++;
				}



			}else{
				printf("%s",buffer_result);
				buffer_result_idx=0;
				memset(buffer_result,0,sizeof(buffer_result));
			}



		}else if (read_count==0){
			/*	memset(buffer_cmd,0,sizeof(buffer_cmd));
				scanf("%s",buffer_cmd);*/
			Sleep(100);
			Timeout_t++;
			TimeCountStop("Read Serial Port Timeout!");

		}else{

			ASSERT(0);
		}

	}while(Timeout_t<1*10);
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void GPS_WG_8020::process_gps_data()
{
	string data_t=buffer_result;
	int idx=0;
	vector<std::string> gps_datas_t=this->split(data_t,',');

	if (gps_datas_t.at(idx++).compare("$GPRMC")==0)
	{
		this->g_UtcTime_hhmmss=gps_datas_t.at(idx++);
		this->g_Location_status=gps_datas_t.at(idx++);
		
		if (this->g_Location_status.compare("A")==0){

			g_Lat=gps_datas_t.at(idx++);
			g_NS=gps_datas_t.at(idx++);
			g_Lon=gps_datas_t.at(idx++);
			g_EW=gps_datas_t.at(idx++);
			g_Speed=gps_datas_t.at(idx++);
			g_Direction=gps_datas_t.at(idx++);
			g_UtcTime_ddmmyy=gps_datas_t.at(idx++);
			g_CiPianJiao=gps_datas_t.at(idx++);
			g_CiPianJiaoFangXiang=gps_datas_t.at(idx++);
			g_Mode=gps_datas_t.at(idx++);

			this->Convert2Degree();

		}
		
		

	}

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
std::vector<std::string>  GPS_WG_8020::split(const std::string &s, char delim) {
		
	std::stringstream ss(s);
	std::string item;
	std::vector<std::string> elems;
	
	while (std::getline(ss, item, delim)) {
		
		elems.push_back(item);
			// elems.push_back(std::move(item)); // if C++11 (based on comment from @mchiasson)
		}
		return elems;
}
/*-------------------------------------*/
/**
*ddmm.mmmm
*dddmm.mmmm
*/
/*-------------------------------------*/
void GPS_WG_8020::Convert2Degree()
{
	double lat_t=atof(this->g_Lat.c_str());
	double lon_t=atof(this->g_Lon.c_str());
	//½ØÈ¡¸ßÎ»
	int lat_z_t=(int)(lat_t/100);
	int lon_z_t=(int)(lon_t/100);
	//

	double lat_f_t=lat_t-lat_z_t;

	double lon_f_t=lon_t-lon_z_t;

	this->g_Lat_float=lat_z_t+lat_f_t/60;
	this->g_Lon_float=lon_z_t+lon_f_t/60;

}
/*----------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------*/
GPS_WG_8020* GPS_WG_8020::getInstance()
{
	/*if (_instance==NULL){
		_instance=new GPS_WG_8020();
	}*/

	return _instance;

}
/*----------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------*/
unsigned GPS_WG_8020::readGpsThread(LPVOID lpParam)
{
	return 0;
}
/*----------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------*/