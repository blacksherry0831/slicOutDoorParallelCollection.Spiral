

#include "communication_2_server/Comm2Server.h"

#include "SerialPort/AGV_Dirver.h"


void TestAGVDriver()
{
	int  com_num_agv=4;	
	AGV_Dirver*  agv_driver_t=AGV_Dirver::getInstance();
	printf("Input AGV Com:\n");
	scanf("%d",&com_num_agv);

	if (com_num_agv>=0){
		agv_driver_t->open(com_num_agv);
	}else{
		printf("Do not use Comm !\n");
	}

	while (TRUE){

		char cmd_t[1024];

		printf("please input cmd: \n");

		scanf("%s",cmd_t);

		string cmd_s_t=cmd_t;

		if(cmd_s_t.compare("start")==0){

			agv_driver_t->StartRun();

		}else if(cmd_s_t.compare("stop")==0){

			agv_driver_t->StopRun();

		}else if(cmd_s_t.compare("forward")==0 || cmd_s_t.compare("f")==0){

			agv_driver_t->RunForward();

		}else if(cmd_s_t.compare("back")==0 || cmd_s_t.compare("b")==0){

			agv_driver_t->RunBack();

		}else if(cmd_s_t.compare("left")==0 || cmd_s_t.compare("l")==0){

			agv_driver_t->RunLeft();

		}else if(cmd_s_t.compare("right")==0 || cmd_s_t.compare("r")==0){

			agv_driver_t->RunRight();

		}else if(cmd_s_t.compare("quit")==0 || cmd_s_t.compare("q")==0){
			break;
		}else{
			printf("Error Cmd \n");
		}


	}

}

void TestAGVComm()
{

	Comm2Server comm2server;
	comm2server.InputCarUserId();
	comm2server.RegisterCar();

#if 0
	comm2server.GetTaskLongitudeLatitude();
#endif


	comm2server.Start();

	comm2server.Join();
}

int main()
{

	int test_num=0;

	printf("Please Input Test Item:\n");

	printf("1 AGV Driver Test! \n");
	printf("2 Communication Test! \n");

	cin>>test_num;

	if (test_num==1){

		TestAGVDriver();

	}else if (test_num==2){
		
		TestAGVComm();
		
	}else{


	}

	cin.get();
	return 0;
}