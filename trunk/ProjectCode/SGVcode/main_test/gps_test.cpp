#include "stdafx.h"
#include <stdio.h>
#include "opencv_stl.h"


#if _MSC_VER


#include <SerialPort/Gps_WG_8020.h>
#include <SerialPort/Compass_HCM365.h>

#endif
/*-------------------------------------------------------------------------*/
/**
*
*/
/*-------------------------------------------------------------------------*/
int Test_GPS()
{

	char CMD_VERS[]="AT^VERS\n";
	char CMD_GPRMC[]="AT^GPRMC\n";
	char CMD_ATE1[]="ate1\n";
	char CMD_ATE0[]="ate0\n";
	char	buffer_result[1024]={0x00};
	int		buffer_result_idx=0;
	char buffer_cmd[1024]={0x00};

	int n_t=strlen(CMD_VERS);
	TimeCountStart();

#if 0
	{

		CSerialPort sp_t;

		sp_t.Open(4,
			115200);
		
		COMMTIMEOUTS Timeouts;
		memset(&Timeouts, 0, strlen(CMD_VERS));
		Timeouts.ReadIntervalTimeout = 100;
		Timeouts.ReadTotalTimeoutConstant=100;
		Timeouts.ReadTotalTimeoutMultiplier=100;
		Timeouts.WriteTotalTimeoutConstant=100;
		Timeouts.WriteTotalTimeoutMultiplier=100;

		
		sp_t.SetTimeouts(Timeouts);

		sp_t.Write(CMD_ATE0,strlen(CMD_ATE0));

		sp_t.Write(CMD_VERS,sizeof(CMD_VERS));

		

		DWORD read_count=0;

		do{

			

			read_count=sp_t.Read(&buffer_result[buffer_result_idx],1);
			if(read_count>0){
				
				if (buffer_result[0]=='$')
				{
					

					if (buffer_result_idx>=1
						&&buffer_result[buffer_result_idx-1]==0x0D
						&&buffer_result[buffer_result_idx]==0x0A)
					{
						printf("Result>>%s",buffer_result);
						
						buffer_result_idx=0;
						memset(buffer_result,0,sizeof(buffer_result));

					}else{
						buffer_result_idx++;
					}
				


				}else{
					printf("%s",buffer_result);
					buffer_result_idx=0;
					memset(buffer_result,0,sizeof(buffer_result));
				}



			}else{

				Sleep(100);

			}		

			if(read_count==0){
				printf("#");

				memset(buffer_cmd,0,sizeof(buffer_cmd));
				scanf("%s",buffer_cmd);

				buffer_cmd[strlen(buffer_cmd)]='\n';
				buffer_cmd[strlen(buffer_cmd)]='\0';
				sp_t.Write(buffer_cmd,strlen(buffer_cmd));

			}

		}while(TRUE);
	



		
	
	}
#endif // 0

	TimeCountStop("Threads Done : ");

	return 0;

}
/*-------------------------------------------------------------------------*/
/**
*
*/
/*-------------------------------------------------------------------------*/
void test_gps_class()
{
		GPS_WG_8020 *gps_t=GPS_WG_8020::getInstance();

	/*gps_t->open();	*/
	int i=0;
	while(i++<10){

		/*gps_t->SendCmdGPRMC();

		gps_t->ReadGpsData();*/
		gps_t->GetLatLonStr();

		Sleep(5*1000);
	}

}
/*-------------------------------------------------------------------------*/
/**
*
*/
/*-------------------------------------------------------------------------*/

int main()
{
#if 0
	return Test_GPS();
#else
		
		Compass_HCM365 *gps_t=Compass_HCM365::getInstance();

	
	int i=0;
	while(i++<10){

		

		Sleep(5*1000);
	}



#endif



	return 0;
}
