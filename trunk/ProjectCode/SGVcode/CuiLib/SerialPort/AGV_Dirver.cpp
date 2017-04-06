#include "StdAfx.h"
#include "AGV_Dirver.h"
//#include "opencv_stl.h"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
AGV_Dirver* AGV_Dirver::_instance =new AGV_Dirver();
//CGarbo CGarbo::Garbo();
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/

AGV_Dirver::AGV_Dirver(void)
{
	//this->g_Lat_float=-1;
	//this->g_Lon_float=-1;

	buffer_result_idx=0;
	memset(buffer_result,0,sizeof(buffer_result));
//	this->open();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
AGV_Dirver::~AGV_Dirver(void)
{

	this->close();

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void AGV_Dirver::open(int com_num)
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
		HANDLE handle_t=::CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)readResultThread,this,0,NULL);
#endif
	}

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void AGV_Dirver::init()
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
void AGV_Dirver::close()
{
	m_sp.Close();
}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
//void AGV_Dirver::SendCmdPitchRollHeading()
//{
//	char cmd_t[]={0x68,0x04,0x00,0x04,0x08};
//	
//	if (m_sp.IsOpen())
//	{
//		m_sp.Write(cmd_t,sizeof(cmd_t));
//	}
//
//	
//
//}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
//void AGV_Dirver::SendCmdEnterAT()
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
//void AGV_Dirver::SendCmdVERS()
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
//void AGV_Dirver::SendCmdGPRMC()
//{
//	char CMD_GPRMC[]="AT^GPRMC\n";
//	m_sp.Write(CMD_GPRMC,strlen(CMD_GPRMC));
//}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
void AGV_Dirver::ReadResultData()
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
					read_len_t=buffer_result[1]; //读取长度
				}

				if (buffer_result_idx==read_len_t){
					this->process_result_data();
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
void AGV_Dirver::process_result_data()
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

	/*this->m_pitch=ConvertBCD2Float(pitch_t);
	this->m_roll=ConvertBCD2Float(roll_t);
	this->m_heading=ConvertBCD2Float(heading_t);*/

#ifdef _MSC_VER
	m_MUTEX.Unlock();
#endif

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/
std::vector<std::string>  AGV_Dirver::split(const std::string &s, char delim) {
		
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
//float AGV_Dirver::ConvertBCD2Float(unsigned char *data_t)
//{
//	float zhengshu=0;
//	float xiaoshu=0;
//	float data_float_t=0;
//	xiaoshu=((data_t[2]>>4)*10+(data_t[2]&0x0f))/100.0;
//	zhengshu=(data_t[0]&0x0f)*100+(data_t[1]>>4)*10+(data_t[1]&0x0f);
//
//	if ((data_t[0]&0xf0)==0x10){
//		data_float_t=0-zhengshu-xiaoshu;
//	}else if ((data_t[0]&0xf0)==0x00){
//		data_float_t=zhengshu+xiaoshu;
//	}else{
//		ASSERT(0);
//	}
//	return data_float_t;
//}
/*----------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------*/
AGV_Dirver* AGV_Dirver::getInstance()
{
	/*if (_instance==NULL){
		_instance=new AGV_Dirver();
	}*/

	return _instance;

}

/*----------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------*/
DWORD AGV_Dirver::readResultThread(LPVOID lpParam)
{
	AGV_Dirver*gps_t=AGV_Dirver::getInstance();
		
	/*gps_t->SendCmdEnterAT();*/

	while(AGV_Dirver::_instance!=NULL){
		
	//	gps_t->SendCmdPitchRollHeading();
	///*	gps_t->SendCmdGPRMC();*/

	//	gps_t->ReadCompassData();

	//	Sleep(1000*1);
	
	}
	return 0;
}
/*----------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------*/
//string AGV_Dirver::GetPitchRollHeadingStr()
//{
//	String LatLon_t;
//#ifdef _MSC_VER
//	m_MUTEX.Lock();
//#endif
//	
//	
//	{
//		char buffer_z[1024];
//		sprintf_s(buffer_z,"(%0.3f,%0.3f,%0.3f)",m_pitch,m_roll,m_heading);
//		LatLon_t=buffer_z;
//	}
//	
//
//#ifdef _MSC_VER
//	m_MUTEX.Unlock();
//#endif
//	return LatLon_t;
//} 
/*----------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------*/
void AGV_Dirver::StartRun()
{
	
	
	this->GetCmd(cmd_current,TRUE,NO_TURN,TRUE,FALSE,40);

	m_sp.Write(cmd_current,sizeof(cmd_current));

}
/*----------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------*/
void AGV_Dirver::StopRun()
{
	
	
	this->GetCmd(cmd_current,FALSE,NO_TURN,TRUE,FALSE,40);

	m_sp.Write(cmd_current,sizeof(cmd_current));

}
/*----------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------*/
void AGV_Dirver::GetCmd(
	unsigned char* cmd,
	bool IsStart,
	int Direct,
	bool IsForward,
	bool IsRoadBlock,
	unsigned int Speed)
{
	cmd[0]=0x55;
	//启动停止
	if(IsStart){
		cmd[1]=0x01;
	}else{
		cmd[1]=0x00;
	}
	//方向
	cmd[2]=Direct;
	//正走倒走
	if(IsForward){
		cmd[3]=0x00;
	}else{
		cmd[3]=0x01;
	}
	//障碍物
	if(IsRoadBlock){
		cmd[4]=0x01;
	}else{
		cmd[4]=0x00;
	}


	
	cmd[5]=Speed/256;
	cmd[6]=Speed%256;


	//FLAG
	cmd[7]=0x55;
	//LRC
	cmd[8]=GetLRC(cmd);
	//END
	cmd[9]=0xaa;
}
/*----------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------*/
unsigned char AGV_Dirver::GetLRC(unsigned char* cmd)
{
	unsigned	char cmd_lrc=0;

	for(int i=1;i<8;i++){

		cmd_lrc+=cmd[i];
	
	}
	
	cmd[8]=cmd_lrc;

	return cmd_lrc;
	
}
/*----------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------*/
void AGV_Dirver::RunBack()
{
	this->cmd_current[3]=0x01;
	this->GetLRC(this->cmd_current);
}
/*----------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------*/
void AGV_Dirver::RunForward()
{
	this->cmd_current[3]=0x00;
	this->GetLRC(this->cmd_current);

}
/*----------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------*/
void AGV_Dirver::RunLeft()
{
	cmd_current[0]=LEFT_TURN;
}
/*----------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------*/
void AGV_Dirver::RunRight()
{
	cmd_current[0]=RIGHT_TURN;
}
/*----------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------*/	
void AGV_Dirver::RunStraight()
{
	cmd_current[0]=NO_TURN;
}
/*----------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------*/
