#include "StdAfx.h"
#include "Compass_HCM365.h"
//#include "opencv_stl.h"
#include "pt_mutex.h"

#include "MY_SDK_LIB/Base.h"
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
	this->m_baudrate = 9600;
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
int Compass_HCM365::open(int com_num)
{

	if (SerialPortBase::open(com_num) == TRUE) {
	
#if _MSC_VER
		HANDLE handle_t=::CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)readCompassThread,this,0,NULL);
#endif

	}
	return init();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
int Compass_HCM365::init()
{

	if (this->IsOpen() == FALSE) {
	
		int  com_num=4;
		int  com_baud=9600;
		
		printf("Input Compass Com:\n");
		scanf("%d",&com_num);
		return this->open(com_num);
	}
	else
	{
		return TRUE;
	}
	



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
void Compass_HCM365::SendCmdPitchRollHeading()
{
	char cmd_t[]={0x68,0x04,0x00,0x04,0x08};
	
	if (init())
	{
		serial_write(cmd_t,sizeof(cmd_t));
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
		read_count= serial_read(&buffer_result[buffer_result_idx],1);

		if(read_count==1){

			if (buffer_result[0]==0x68)
			{		
				if (buffer_result[1]!=0x00){
					read_len_t=buffer_result[1]; //��ȡ����
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
	
	m_MUTEX.lock();
	
	this->m_pitch=ConvertBCD2Float(pitch_t);
	this->m_roll=ConvertBCD2Float(roll_t);
	this->m_heading=ConvertBCD2Float(heading_t);

	m_MUTEX.unlock();

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/

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
	std::string LatLon_t;

	m_MUTEX.lock();

	
	
	{
		char buffer_z[1024];
		sprintf_s(buffer_z,"(%0.3f,%0.3f,%0.3f)",m_pitch,m_roll,m_heading);
		LatLon_t=buffer_z;
	}
	


	m_MUTEX.unlock();

	return LatLon_t;
} 
/*----------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------*/
string Compass_HCM365::GetHeadingStr()
{
	std::string head_t;

	m_MUTEX.lock();
	
	{
		char buffer_z[1024];
		sprintf_s(buffer_z,"%0.3f",m_heading);
		head_t=buffer_z;
	}

	m_MUTEX.unlock();

	return head_t;
} 
/*----------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------*/
double  Compass_HCM365::GetHead()
{
	double head_t;

	m_MUTEX.lock();

	
		
		head_t=m_heading;
		

	m_MUTEX.unlock();

	return head_t;
}

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