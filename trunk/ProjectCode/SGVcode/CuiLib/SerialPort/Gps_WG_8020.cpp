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
	this->g_Lat_float=-1;
	this->g_Lon_float=-1;

	buffer_result_idx=0;
	memset(buffer_result,0,sizeof(buffer_result));
//	this->open();
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
void GPS_WG_8020::open(int com_num)
{
	try{

		if(m_sp.IsOpen()==false){
		m_sp.Open(com_num,115200);
		{
			COMMTIMEOUTS Timeouts;
			Timeouts.ReadIntervalTimeout = 100;
			Timeouts.ReadTotalTimeoutConstant=100;
			Timeouts.ReadTotalTimeoutMultiplier=100;
			Timeouts.WriteTotalTimeoutConstant=100;
			Timeouts.WriteTotalTimeoutMultiplier=100;
			m_sp.SetTimeouts(Timeouts);
		}
#if _MSC_VER
		HANDLE handle_t=::CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)readGpsThread,this,0,NULL);
#endif
		}
	
	}catch(CSerialException e){
		printf("%s",e.what());
		ASSERT(0);
	}


}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void GPS_WG_8020::init()
{

	if (m_sp.IsOpen()==TRUE){
		return;
	}

	int  com_num=4;
	int  com_baud=115200;

	printf("Input Gps Com:\n");
	scanf("%d",&com_num);

	
	this->open(com_num);


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
string GPS_WG_8020::ReadString()
{
	DWORD read_count=0;
	int  Timeout_t=0;
	buffer_result_idx=0;
	memset(buffer_result,0,sizeof(buffer_result));
	string gps_str_t;
	TimeCountStart();
	do{
		read_count=m_sp.Read(&buffer_result[buffer_result_idx],1);

		if(read_count==1){
				
			buffer_result_idx++;

		}else if (read_count==0){
		
			Sleep(0);
			Timeout_t++;
			TimeCountStop("Read Serial Port Timeout!");

		}else{

			ASSERT(0);
		}

	}while(Timeout_t<1*10);
	
	gps_str_t=buffer_result;
	

	return gps_str_t;
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void GPS_WG_8020::SendCmdEnterAT()
{
	char CMD_PPP[]="+++";
	//char CMD_ATE0[]="ate0\n";


	int size=strlen(CMD_PPP);


	string result_t;

	do 
	{
		Sleep(2000);
		
		m_sp.Write(CMD_PPP,strlen(CMD_PPP));

		result_t=this->ReadString();

		printf("GPS>>> +++ : %s \n",result_t.c_str());
	} while (result_t=="OK"||result_t=="+++");


	//m_sp.Write(CMD_ATE0,strlen(CMD_ATE0));

	//this->ReadGpsData();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void GPS_WG_8020::SendCmdVERS()
{
	char CMD_VERS[]="AT^VERS\n";
	m_sp.Write(CMD_VERS,strlen(CMD_VERS));

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void GPS_WG_8020::SendCmdGPRMC()
{
	char CMD_GPRMC[]="AT^GPRMC\n";
	m_sp.Write(CMD_GPRMC,strlen(CMD_GPRMC));
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
	buffer_result_idx=0;
	memset(buffer_result,0,sizeof(buffer_result));

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
			Sleep(0);
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

#ifdef _MSC_VER
	m_MUTEX.Lock();
#endif

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
		}else if (this->g_Location_status.compare("V")==0){
			printf("无效定位\n");
		}else{

		}

	}

#ifdef _MSC_VER
	m_MUTEX.Unlock();
#endif

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
	//截取高位
	int lat_z_t=(int)(lat_t/100);
	int lon_z_t=(int)(lon_t/100);
	//

	double lat_f_t=lat_t-lat_z_t*100;

	double lon_f_t=lon_t-lon_z_t*100;

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
DWORD GPS_WG_8020::readGpsThread(LPVOID lpParam)
{
	GPS_WG_8020 *gps_t=GPS_WG_8020::getInstance();
		
		gps_t->SendCmdEnterAT();
	
	while(GPS_WG_8020::_instance!=NULL){
		
		
		gps_t->SendCmdGPRMC();

		gps_t->ReadGpsData();

		Sleep(1000*5);
	
	}
	return 0;
}
/*----------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------*/
string GPS_WG_8020::GetLatLonStr()
{
	String LatLon_t;
#ifdef _MSC_VER
	m_MUTEX.Lock();
#endif
	
	if (this->g_Lat_float>0
		&&this->g_Lon_float>0)
	{
		char buffer_z[1024];
			sprintf_s(buffer_z,"(%0.8f,%0.8f)",this->g_Lat_float,this->g_Lon_float);
		LatLon_t=buffer_z;
	}
	

#ifdef _MSC_VER
	m_MUTEX.Unlock();
#endif
	return LatLon_t;
} 
/*----------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------*/
string GPS_WG_8020::GetLatStr()
{
	String Lat_t;
#ifdef _MSC_VER
	m_MUTEX.Lock();
#endif

	if (this->g_Lat_float>0
		&&this->g_Lon_float>0)
	{
		char buffer_z[1024];
		sprintf_s(buffer_z,"%0.8f",this->g_Lat_float);
		Lat_t=buffer_z;
	}


#ifdef _MSC_VER
	m_MUTEX.Unlock();
#endif
	return Lat_t;
}

/*----------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------*/
string GPS_WG_8020::GetLonStr()
{
	String Lon_t;
#ifdef _MSC_VER
	m_MUTEX.Lock();
#endif

	if (this->g_Lat_float>0
		&&this->g_Lon_float>0)
	{
		char buffer_z[1024];
		sprintf_s(buffer_z,"%0.8f",this->g_Lon_float);
		Lon_t=buffer_z;
	}


#ifdef _MSC_VER
	m_MUTEX.Unlock();
#endif
	return Lon_t;
}
/*----------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------*/