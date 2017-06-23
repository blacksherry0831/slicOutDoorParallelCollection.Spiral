#include "StdAfx.h"
#include "Compass_HCM365.h"
//#include "opencv_stl.h"
#include "pt_mutex.h"
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
	
	buffer_result_idx=0;
	memset(buffer_result,0,sizeof(buffer_result));
//	this->open();
	this->m_heading=0;
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
void Compass_HCM365::open(int com_num)
{
	if(m_sp.IsOpen()==false){
		m_sp.Open(com_num,9600);
		{
			COMMTIMEOUTS Timeouts;//unit ms 
			Timeouts.ReadIntervalTimeout = 100;//ms
			Timeouts.ReadTotalTimeoutConstant=100;
			Timeouts.ReadTotalTimeoutMultiplier=100;
			Timeouts.WriteTotalTimeoutConstant=100;
			Timeouts.WriteTotalTimeoutMultiplier=100;
			m_sp.SetTimeouts(Timeouts);
		}
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
void Compass_HCM365::init()
{
	if (m_sp.IsOpen()==TRUE){
		return;
	}

	int  com_num=4;
	int  com_baud=9600;

	printf("Input Compass Com:\n");
	scanf("%d",&com_num);


	this->open(com_num);
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
			Sleep(1000);
			Timeout_t++;
			TimeCountStop("COMPASS>>>Read Serial Port Timeout!");
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
	
	m_MUTEX.Lock();
	
	this->m_pitch=ConvertBCD2Float(pitch_t);
	this->m_roll=ConvertBCD2Float(roll_t);
	this->m_heading=ConvertBCD2Float(heading_t);

	m_MUTEX.Unlock();

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
std::vector<std::string>  Compass_HCM365::split  (const std::string &s, char delim) {
		
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

	m_MUTEX.Lock();

	
	
	{
		char buffer_z[1024];
		sprintf_s(buffer_z,"(%0.3f,%0.3f,%0.3f)",m_pitch,m_roll,m_heading);
		LatLon_t=buffer_z;
	}
	


	m_MUTEX.Unlock();

	return LatLon_t;
} 
/*----------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------*/
string Compass_HCM365::GetHeadingStr()
{
	String head_t;

	m_MUTEX.Lock();
	
	{
		char buffer_z[1024];
		sprintf_s(buffer_z,"%0.3f",m_heading);
		head_t=buffer_z;
	}

	m_MUTEX.Unlock();

	return head_t;
} 
/*----------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------*/
//string Compass_HCM365::GetLatStr()
//{
//	String Lat_t;
//
//	m_MUTEX.Lock();
//
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
//
//	m_MUTEX.Unlock();
//
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

//	m_MUTEX.Lock();

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

//	m_MUTEX.Unlock();

//	return Lon_t;
//}
/*----------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------*/