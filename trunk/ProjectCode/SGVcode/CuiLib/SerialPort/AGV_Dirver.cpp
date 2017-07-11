#include "StdAfx.h"
#include "AGV_Dirver.h"
#include "MY_SDK_LIB/Base.h"
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/
AGV_Dirver* AGV_Dirver::_instance =new AGV_Dirver();
/*-------------------------------------*/
/**
*
*
*/
/*-------------------------------------*/

AGV_Dirver::AGV_Dirver(void)
{
	buffer_result_idx=0;
	memset(buffer_result,0,sizeof(buffer_result));

	status_car_ready=0;
	status_car_battery_low=0;
	status_car_roadblock=0;
	status_car_internal_error=0;
	status_car_communication_error=0;
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
		 pthread_t handle_t; 
		 int ret=0; 
		 ret=pthread_create(&handle_t,NULL,readResultThread,NULL);  
		 if(ret!=0)  
		 {  
			 printf("Create pthread error!\n");  
			 ASSERT(ret!=0);
		 }  
#if _MSC_VER && 0
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

	this->GetCmd(cmd_current,TRUE,NO_TURN,TRUE,FALSE,40);
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

void AGV_Dirver::ReadResultData()
{
	DWORD read_count=0;
	int  Timeout_t=0;
	const int read_len_t=10;
	buffer_result_idx=0;
	memset(buffer_result,0,sizeof(buffer_result));

	TimeCountStart();

	do{
		read_count=m_sp.Read(&buffer_result[buffer_result_idx],1);

		if(read_count==1){

			if (buffer_result[0]==0x55)
			{				
				if (buffer_result_idx==read_len_t&&buffer_result[9]==0xaa){
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
	//处理 下位机上传上来的故障信息

	//Step 1 接受下位机命令，读取标志位0x00
	buffer_result[3];

	unsigned char start_b_55=buffer_result[0];//开始标志
	unsigned char cmd_type_b=buffer_result[7];//反馈命令
	unsigned char lrc_b_car=buffer_result[8];//校验,下位机
	unsigned char end_b_aa=buffer_result[9];//结束标志

	unsigned lrc_b_control=this->GetLRC(buffer_result);//校验，上位机
	



	//Step 2, 
	if (lrc_b_control==lrc_b_car){
		//校验成功
		buffer_result[8]=0x01;
	}else{
		//校验失败
		buffer_result[8]=0x02;
	}
	
	status_car_ready=buffer_result[1];
	status_car_battery_low=buffer_result[2];
	status_car_roadblock=buffer_result[3];
	status_car_internal_error=buffer_result[4];
	status_car_communication_error=buffer_result[5];
	
	this->Send2CarFeedBack();

}
/*-------------------------------------*/
/**
*
*/
/*-------------------------------------*/

/*----------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------*/
AGV_Dirver* AGV_Dirver::getInstance()
{
	return _instance;
}

/*----------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------*/
void* AGV_Dirver::readResultThread(void* lpParam)
{
	AGV_Dirver*gps_t=AGV_Dirver::getInstance();
		
	while(AGV_Dirver::_instance!=NULL){
			
	}

	return (void*)NULL;
}
/*----------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------*/
void AGV_Dirver::StartRun()
{	
	this->cmd_current[1]=0x01;
	if(this->Send2Car()){

	}else{
		std::cout<<"[local][car]Run START"<<std::endl;
	}
}
/*----------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------*/
void AGV_Dirver::StopRun()
{	
	this->cmd_current[1]=0x00;
	if(this->Send2Car()){

	}else{
		std::cout<<"[local][car]STOP "<<std::endl;
	}

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

	ASSERT(cmd[0]==0x55);
	ASSERT(cmd[9]==0xaa);

	for(int i=1;i<8;i++){

		cmd_lrc+=cmd[i];
	
	}
	
	cmd[8]=~cmd_lrc+1;//求和，取反加1

	return cmd[8];
	
}
/*----------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------*/
void AGV_Dirver::RunBack()
{
	this->cmd_current[3]=0x01;
	
	if(this->Send2Car()){

	}else{
		std::cout<<"[local][car]Run Back"<<std::endl;
	}
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

	if(this->Send2Car()){

	}else{
		std::cout<<"[local][car]Run Forward"<<std::endl;
	}

}
/*----------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------*/
void AGV_Dirver::RunLeft()
{
	cmd_current[0]=LEFT_TURN;
	if(this->Send2Car()){

	}else{
		std::cout<<"[local][car]Run Left"<<std::endl;
	}

}
/*----------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------*/
void AGV_Dirver::RunRight()
{
	cmd_current[0]=RIGHT_TURN;
	if(this->Send2Car()){

	}else{
		std::cout<<"[local][car]Run Right"<<std::endl;
	}
}
/*----------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------*/	
void AGV_Dirver::RunStraight()
{
	cmd_current[0]=NO_TURN;
	if(this->Send2Car()){

	}else{
		std::cout<<"[local][car]Run Straight"<<std::endl;
	}
}
/*----------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------*/
int AGV_Dirver::Send2Car(void)
{
	this->GetLRC(this->cmd_current);

	const int BUFF_SIZE=sizeof(cmd_current);
	int  buf_send_t=-1;
	if (m_sp.IsOpen()){
			buf_send_t=m_sp.Write(cmd_current,BUFF_SIZE);
	}else{
		return FALSE;
	}

	return buf_send_t==BUFF_SIZE;


}
/*----------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------*/
void AGV_Dirver::Send2CarFeedBack(void)
{
	this->GetLRC(this->buffer_result);
	if (m_sp.IsOpen()){
		m_sp.Write(cmd_current,sizeof(this->buffer_result));	
	}

}
/*----------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------*/
int AGV_Dirver::IsCarReady(void)
{
	return status_car_ready;
}
/*----------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------*/
int AGV_Dirver::IsCarLowPower(void)
{
	return status_car_battery_low;
}
/*----------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------*/
int AGV_Dirver::IsRoadblock(void)
{
	return status_car_roadblock;
}
/*----------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------*/
int AGV_Dirver::IsCarInternalError(void)
{
	return status_car_internal_error;
}
/*----------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------*/
int AGV_Dirver::IsCarCommunicationError(void)
{
	return status_car_communication_error;
}
/*----------------------------------------------------*/
/**
*
*/
/*----------------------------------------------------*/
