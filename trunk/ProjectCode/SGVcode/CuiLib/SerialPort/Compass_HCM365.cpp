#include "StdAfx.h"
#include "Compass_HCM365.h"
//#include "opencv_stl.h"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
Compass_HCM365* Compass_HCM365::_instance =new Compass_HCM365();
//CGarbo CGarbo::Garbo();
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/

Compass_HCM365::Compass_HCM365(void)
{
	//this->g_Lat_float=-1;
	//this->g_Lon_float=-1;

	buffer_result_idx=0;
	memset(buffer_result,0,sizeof(buffer_result));
	this->open();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
Compass_HCM365::~Compass_HCM365(void)
{

	this->close();

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void Compass_HCM365::open()
{
	if(m_sp.IsOpen()==false){
		m_sp.Open(7,9600);
#if _MSC_VER
		HANDLE handle_t=::CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)readCompassThread,this,0,NULL);
#endif
	}

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void Compass_HCM365::close()
{
	m_sp.Close();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void Compass_HCM365::SendCmdPitchRollHeading()
{
	char cmd_t[]={0x68,0x04,0x00,0x04,0x08};
	
	if (m_sp.IsOpen())
	{
		m_sp.Write(cmd_t,sizeof(cmd_t));
	}

	

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
//void Compass_HCM365::SendCmdEnterAT()
//{
//	char CMD_PPP[]="+++\n";
//	char CMD_ATE0[]="ate0\n";
//
//	int size=strlen(CMD_PPP);
//
//	m_sp.Write(CMD_PPP,strlen(CMD_PPP));
//	
//	this->ReadGpsData();
//
//	m_sp.Write(CMD_ATE0,strlen(CMD_ATE0));
//	
//	this->ReadGpsData();
//}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
//void Compass_HCM365::SendCmdVERS()
//{
//	char CMD_VERS[]="AT^VERS\n";
//	m_sp.Write(CMD_VERS,strlen(CMD_VERS));
//
//}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
//void Compass_HCM365::SendCmdGPRMC()
//{
//	char CMD_GPRMC[]="AT^GPRMC\n";
//	m_sp.Write(CMD_GPRMC,strlen(CMD_GPRMC));
//}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void Compass_HCM365::ReadCompassData()
{
	DWORD read_count=0;
	int  Timeout_t=0;
	int read_len_t=14;
	buffer_result_idx=0;
	memset(buffer_result,0,sizeof(buffer_result));

	TimeCountStart();

	do{
		read_count=m_sp.Read(&buffer_result[buffer_result_idx],1);

		if(read_count==1){

			if (buffer_result[0]==0x68)
			{		
				if (buffer_result[1]!=0x00){
					read_len_t=buffer_result[1]; //¶ÁÈ¡³¤¶È
				}

				if (buffer_result_idx==read_len_t){
					this->process_compass_data();
					break;
				}
				buffer_result_idx++;
			}else{
				printf("%s",buffer_result);
				buffer_result_idx=0;
				memset(buffer_result,0,sizeof(buffer_result));
			}



		}else if (read_count==0){
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
void Compass_HCM365::process_compass_data()
{
	unsigned char cmd_t=buffer_result[3];
	if(cmd_t==0x84){
		//return;
	}else{
		return;
	}
	unsigned	char *pitch_t=&this->buffer_result[4];
	unsigned	char *roll_t=&this->buffer_result[7];
	unsigned	char *heading_t=&this->buffer_result[10];

#ifdef _MSC_VER
	m_MUTEX.Lock();
#endif

	this->m_pitch=ConvertBCD2Float(pitch_t);
	this->m_roll=ConvertBCD2Float(roll_t);
	this->m_heading=ConvertBCD2Float(heading_t);

#ifdef _MSC_VER
	m_MUTEX.Unlock();
#endif

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
std::vector<std::string>  Compass_HCM365::split(const std::string &s, char delim) {
		
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
float Compass_HCM365::ConvertBCD2Float(unsigned char *data_t)
{
	float zhengshu=0;
	float xiaoshu=0;
	float data_float_t=0;
	xiaoshu=((data_t[2]>>4)*10+(data_t[2]&0x0f))/100.0;
	zhengshu=(data_t[0]&0x0f)*100+(data_t[1]>>4)*10+(data_t[1]&0x0f);

	if ((data_t[0]&0xf0)==0x10){
		data_float_t=0-zhengshu-xiaoshu;
	}else if ((data_t[0]&0xf0)==0x00){
		data_float_t=zhengshu+xiaoshu;
	}else{
		ASSERT(0);
	}
	return data_float_t;
}
/*----------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------*/
Compass_HCM365* Compass_HCM365::getInstance()
{
	/*if (_instance==NULL){
		_instance=new Compass_HCM365();
	}*/

	return _instance;

}

/*----------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------*/
DWORD Compass_HCM365::readCompassThread(LPVOID lpParam)
{
	Compass_HCM365*gps_t=Compass_HCM365::getInstance();
		
	/*gps_t->SendCmdEnterAT();*/

	while(Compass_HCM365::_instance!=NULL){
		
		gps_t->SendCmdPitchRollHeading();
	/*	gps_t->SendCmdGPRMC();*/

		gps_t->ReadCompassData();

		Sleep(1000*1);
	
	}
	return 0;
}
/*----------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------*/
string Compass_HCM365::GetPitchRollHeadingStr()
{
	String LatLon_t;
#ifdef _MSC_VER
	m_MUTEX.Lock();
#endif
	
	
	{
		char buffer_z[1024];
		sprintf_s(buffer_z,"(%0.3f,%0.3f,%0.3)",m_pitch,m_roll,m_heading);
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
//string Compass_HCM365::GetLatStr()
//{
//	String Lat_t;
//#ifdef _MSC_VER
//	m_MUTEX.Lock();
//#endif
//
//	/*if (this->g_Lat_float>0
//	&&this->g_Lon_float>0)
//	{
//	char buffer_z[1024];
//	sprintf_s(buffer_z,"%0.8f",this->g_Lat_float);
//	Lat_t=buffer_z;
//	}*/
//
//
//#ifdef _MSC_VER
//	m_MUTEX.Unlock();
//#endif
//	return Lat_t;
//}

/*----------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------*/
//string Compass_HCM365::GetLonStr()
//{
//	String Lon_t;
//#ifdef _MSC_VER
//	m_MUTEX.Lock();
//#endif
//
//	/*if (this->g_Lat_float>0
//		&&this->g_Lon_float>0)
//	{
//		char buffer_z[1024];
//		sprintf_s(buffer_z,"%0.8f",this->g_Lon_float);
//		Lon_t=buffer_z;
//	}*/
//
//
//#ifdef _MSC_VER
//	m_MUTEX.Unlock();
//#endif
//	return Lon_t;
//}
/*----------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------*/