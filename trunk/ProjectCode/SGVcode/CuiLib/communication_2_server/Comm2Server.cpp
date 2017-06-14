#include "Comm2Server.h"
#include<iostream>
#include<sstream>
#include<string>
using namespace std; 
/*----------------------------------*/
/**
*
*/
/*----------------------------------*/
Comm2Server::Comm2Server(void)
{

}
/*----------------------------------*/
/**
*
*/
/*----------------------------------*/
Comm2Server::~Comm2Server(void)
{

}
/*----------------------------------*/
/**
*
*/
/*----------------------------------*/
int RegisterCar(void)
{
	string url_t="http://58.240.85.222";
	int port_t=10000;
	string url_path_t="/car/car/addCar";

	string param0="car_id=123456";
	string param1="userid=123456789";

	stringstream ss; //定义一个string流（使用s实例化） 

	string url_p;
	string str_p;
	string response_p;

	CHttpClient httpc_t;
	
	int code_r=httpc_t.Post(url_p,str_p,response_p);

	return code_r;
}
/*----------------------------------*/
/**
*
*/
/*----------------------------------*/


