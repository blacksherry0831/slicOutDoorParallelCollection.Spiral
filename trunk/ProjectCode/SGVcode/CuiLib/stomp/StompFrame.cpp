/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */



#include <string>
using namespace std;

#include "StompFrame.h"

#include "MY_SDK_LIB/Base.h"


////////////////////////////////////////////////////////////////////////////////
StompFrame::StompFrame() 
{

}

////////////////////////////////////////////////////////////////////////////////
StompFrame::~StompFrame()
{

}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
void StompFrame::setProperty(string key_p,string value_p)
{
	
	map<string,string>::iterator key_t =  this->properties.find(key_p); 
	
	if(key_t != this->properties.end())  
	{  
	  
	}else{
		this->properties[key_p]=value_p;
	} 


}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
void StompFrame::setCommand(const std::string cmd_p)
{
	this->command = cmd_p;
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
string StompFrame::GetConnectCmd()
{
	StompFrame sf_t;

	sf_t.setCommand(StompCommandConstants::CONNECT);
	
	sf_t.setProperty("accept-version","1.0,1.1,2.0");
	
	string  result_str=sf_t.str();

	char last_char=result_str.data()[result_str.size()-1];
	if (last_char!=0x00){
		result_str+='\0';
	}
	
	return result_str; 
}
/*-----------------------------------------*/
/**
*
*/
/*-----------------------------------------*/
StompFrame  StompFrame::GetSubscribeFrame(string id,string dest)
{
	StompFrame sf_t;

	sf_t.setCommand(StompCommandConstants::SUBSCRIBE);

	
	sf_t.setProperty(StompCommandConstants::HEADER_DESTINATION,dest);
	sf_t.setProperty(StompCommandConstants::HEADER_ID,id);
	return sf_t; 
}
/*-----------------------------------------*/
/**
*
*/
/*-----------------------------------------*/
string StompFrame::str()
{
	stringstream ss_result_t;
	const string E_O_L="\r\n";
	
	ss_result_t<<this->command;
	ss_result_t<<E_O_L;	  

	map <string,string>  ::iterator      iter;

	for(iter=this->properties.begin();iter!=this->properties.end();iter++)
	{
		ss_result_t<<iter->first<<":"<<iter->second;

		ss_result_t<<E_O_L;
	}

	//¿Õ°×ÐÐ
	ss_result_t<<E_O_L;
	//body

	if (body.size()!=0){
		string body_str=(char *)(body.data());
		ss_result_t<<body_str;
	}
	

	//½áÊø·û
	ss_result_t<<"\0";

	string result_str_t=ss_result_t.str();

	if (result_str_t.data()[result_str_t.size()-1]!='\0'){
			result_str_t+='\0';
	}


	return result_str_t;
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
StompFrame* StompFrame::Parse(string msg)
{	

	const vector<string> split_str_t0= Base::split(msg,"\n\n");	

	if (split_str_t0.size()==2){
		
		const string body_str_t=split_str_t0.at(1);
		this->SetBody(body_str_t);
		
		vector<string> split_str_t1=Base::split(split_str_t0.at(0),'\n');

		if (split_str_t1.size()!=0){

			this->command=split_str_t1.at(0);

			split_str_t1.erase(split_str_t1.begin());

			const vector<string> properties_str_t=split_str_t1;
						
			this->SetProperties(properties_str_t);

		}

	}


	return this;
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
void  StompFrame::SetBody(string body_str_t)
{
	const int LEN=body_str_t.length();
	const char* DATA=body_str_t.data();
	this->body.clear();
	for(int i=0;i<LEN;i++){
		this->body.push_back(DATA[i]);
	}
	this->body.push_back(0x00);
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
string  StompFrame::GetBody()
{
	char * body_c_t= (char *) this->body.data();

	string body_str_t=body_c_t;
	return body_str_t;
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
void  StompFrame::SetProperties(vector<string> p_str_t)
{
	for (int i=0;i<p_str_t.size();i++){

		const string k_v_p_t=p_str_t.at(i);

		vector<string> k_v_split=Base::split(k_v_p_t,":");

		if (k_v_split.size()==2){
			const string k_t=k_v_split.at(0);
			const string v_t=k_v_split.at(1);
			this->setProperty(k_t,v_t);

		}

	}
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
int StompFrame::IsConnected()
{
	return Base::IsEqual(command,StompCommandConstants::CONNECTED);
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
int StompFrame::IsERROR()
{
	return Base::IsEqual(command,StompCommandConstants::ERROR_CMD);
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
void StompFrame::setPropertyContentType(string value_p)
{
	this->setProperty("content-type",value_p);
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/
int StompFrame::IsMESSAGE()
{
	return Base::IsEqual(command,StompCommandConstants::MESSAGE);
}
/*-----------------------------------------*/
/**
*
*
*/
/*-----------------------------------------*/